#!/usr/bin/env python3
"""Copy files into the ext2/ext3 partition of mp3.img without mounting it.

This replaces debug.sh (which needs Linux loop mounts) so the OS can be built
on macOS. Each file must already exist in the image's root directory; its data
is overwritten in place using the blocks it already owns, so the new file can
not be larger than the space allocated for the old one.

Usage: python3 update_image.py mp3.img bootimg [filesys_img ...]
"""
import os
import struct
import sys


class Ext2Image:
    def __init__(self, f):
        self.f = f
        mbr = self.read(0, 512)
        if mbr[510:512] != b"\x55\xaa":
            sys.exit("not a disk image with a partition table")
        start_lba = struct.unpack_from("<I", mbr, 446 + 8)[0]   # first partition
        self.base = start_lba * 512

        sb = self.read(self.base + 1024, 1024)
        if struct.unpack_from("<H", sb, 56)[0] != 0xEF53:
            sys.exit("partition 1 is not an ext2/ext3 file system")
        self.block_size = 1024 << struct.unpack_from("<I", sb, 24)[0]
        self.first_data_block = struct.unpack_from("<I", sb, 20)[0]
        self.inodes_per_group = struct.unpack_from("<I", sb, 40)[0]
        rev = struct.unpack_from("<I", sb, 76)[0]
        self.inode_size = struct.unpack_from("<H", sb, 88)[0] if rev >= 1 else 128
        incompat = struct.unpack_from("<I", sb, 96)[0]
        if incompat & 0x4:
            sys.exit("ext3 journal needs recovery; boot the image once in Linux first")

    def read(self, offset, length):
        self.f.seek(offset)
        return self.f.read(length)

    def write(self, offset, data):
        self.f.seek(offset)
        self.f.write(data)

    def block_offset(self, block):
        return self.base + block * self.block_size

    def inode_offset(self, ino):
        group, index = divmod(ino - 1, self.inodes_per_group)
        gdt = self.block_offset(self.first_data_block + 1) + group * 32
        inode_table = struct.unpack_from("<I", self.read(gdt, 32), 8)[0]
        return self.block_offset(inode_table) + index * self.inode_size

    def blocks_of(self, inode):
        ptrs = struct.unpack_from("<15I", inode, 40)
        per_block = self.block_size // 4
        blocks = [b for b in ptrs[:12] if b]

        def indirect(block, depth):
            if not block:
                return []
            entries = struct.unpack("<%dI" % per_block,
                                    self.read(self.block_offset(block), self.block_size))
            if depth == 1:
                return [b for b in entries if b]
            out = []
            for b in entries:
                out += indirect(b, depth - 1)
            return out

        blocks += indirect(ptrs[12], 1) + indirect(ptrs[13], 2) + indirect(ptrs[14], 3)
        return blocks

    def lookup_root(self, name):
        root = self.read(self.inode_offset(2), self.inode_size)
        for block in self.blocks_of(root):
            data = self.read(self.block_offset(block), self.block_size)
            pos = 0
            while pos < len(data):
                ino, rec_len, name_len = struct.unpack_from("<IHB", data, pos)
                if rec_len == 0:
                    break
                if ino and data[pos + 8:pos + 8 + name_len].decode() == name:
                    return ino
                pos += rec_len
        return None

    def replace(self, name, contents):
        ino = self.lookup_root(name)
        if ino is None:
            sys.exit("%s is not in the image's root directory" % name)
        offset = self.inode_offset(ino)
        inode = bytearray(self.read(offset, self.inode_size))
        blocks = self.blocks_of(inode)
        needed = -(-len(contents) // self.block_size)
        if needed > len(blocks):
            sys.exit("%s is %d bytes but only %d bytes are allocated for it in the image"
                     % (name, len(contents), len(blocks) * self.block_size))

        padded = contents + b"\0" * (needed * self.block_size - len(contents))
        for i in range(needed):
            chunk = padded[i * self.block_size:(i + 1) * self.block_size]
            self.write(self.block_offset(blocks[i]), chunk)

        struct.pack_into("<I", inode, 4, len(contents))          # i_size
        self.write(offset, bytes(inode))
        print("wrote %s (%d bytes, %d of %d blocks)" % (name, len(contents), needed, len(blocks)))


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    with open(sys.argv[1], "r+b") as f:
        image = Ext2Image(f)
        for path in sys.argv[2:]:
            with open(path, "rb") as src:
                image.replace(os.path.basename(path), src.read())


if __name__ == "__main__":
    main()
