# linux_OS: a small x86 operating system

A 32-bit x86 kernel written for ECE 391 (UIUC, Fall 2023, MP3). It boots with GRUB,
runs user programs in ring 3, and has:

- **Paging**: 4 KB pages for video memory, 4 MB pages for the kernel and for each program.
- **Interrupts**: IDT, 8259 PIC, keyboard, RTC, PIT timer, and exception handlers.
- **File system**: a read-only file system loaded as a boot module.
- **10 system calls**: `halt`, `execute`, `read`, `write`, `open`, `close`, `getargs`,
  `vidmap`, `set_handler`, `sigreturn` (the last two are stubs).
- **3 terminals**: switch with Alt+F1 / F2 / F3.
- **Round-robin scheduler**: programs on all three terminals run at the same time,
  each getting a 10 ms time slice from the PIT.

> **Branches:** `working_os` has the fixed kernel described here. `broken_os` is the
> kernel as it was submitted, which crashed when switching between processes (see
> [What was fixed](#what-was-fixed)).

---

## Run it on a MacBook

### 1. Just boot it (no build needed)

The repo includes a ready-to-boot disk image, so you only need QEMU:

```sh
brew install qemu
git clone https://github.com/alirc0320/linux_OS.git
cd linux_OS/student-distrib
qemu-system-i386 -drive file=mp3.img,format=raw -m 256
```

A QEMU window opens with `391OS>` on terminal 1. Click into the window to type.
To get your mouse back, press **Ctrl+Option+G**.

This works on both Apple Silicon and Intel Macs. On Apple Silicon, QEMU emulates the x86 CPU in software.

### 2. Build from source

You need a cross-compiler that targets 32-bit x86 (the Mac's own `clang` can't build this kernel):

```sh
brew install i686-elf-gcc qemu
cd linux_OS/student-distrib
make dep      # generate header dependencies (once, or after adding files)
make          # compile, link bootimg, and write it into mp3.img
make run      # boot mp3.img in QEMU
```

`make` writes the new kernel into the disk image with `update_image.py`, a small
Python script that edits the ext3 partition directly. It replaces the course's
`debug.sh`, which needs Linux loop mounts. You need no `sudo` and no Docker.

The Makefile still works on Linux too (`make dep && sudo make`). It needs `gcc-multilib`.

### 3. Debug with gdb

```sh
make run DEBUG=1                  # QEMU waits for a debugger on localhost:1234
# in another terminal:
brew install i686-elf-gdb
i686-elf-gdb bootimg -ex "target remote localhost:1234"
```

Then use `break scheduler`, `break execute`, `continue` and so on. `bootimg` has debug symbols.

---

## Using the OS

### Keys

| Keys | What it does |
|---|---|
| **Alt+F1 / Alt+F2 / Alt+F3** | Show terminal 1 / 2 / 3. Each terminal has its own text color (cyan, red, purple). |
| Ctrl+L | Clear the current terminal |
| Tab | Insert 4 spaces |
| Shift, Caps Lock, Backspace | Work as usual. Lines are up to 127 characters. |

**On a Mac keyboard, Alt is Option.** If F1-F3 control brightness or other system
features, hold **fn** as well: **fn+Option+F1**. If macOS still grabs the keys, open
the QEMU monitor (View → compatmonitor0, or Ctrl+Option+2), type `sendkey alt-f2`, then
go back to the screen with Ctrl+Option+1.

### Programs

Type these at the `391OS>` prompt:

| Command | What it does |
|---|---|
| `ls` | List every file in the file system |
| `cat <file>` | Print a file, e.g. `cat frame0.txt` or `cat verylargetextwithverylongname.tx` |
| `grep <text>` | Print lines in any file that contain `<text>` |
| `hello` | Asks for your name and greets you (tests keyboard `read`) |
| `counter` | Prompts for a test number; `0`, `1` or `2` counts to 100, 10,000 or 100,000 |
| `pingpong` | Draws a bouncing pattern forever, timed by the RTC |
| `fish` | Animated fish (uses `vidmap` and the RTC); exits by itself after about 12 seconds |
| `testprint` | Prints one line, the simplest possible program |
| `syserr` | System call error tests; type `0` to run all 8 |
| `sigtest 0` | Writes to a NULL pointer on purpose to test exception handling |
| `shell` | Starts a nested shell; `exit` returns to the one below it |
| `exit` | Quits the current shell. A terminal's base shell restarts automatically. |

The kernel allows **6 processes at once**. The three base shells take three of them,
so three more programs can run.

---

## Things to try

### Scheduling: three programs at once

1. On terminal 1, run `counter`, then type `2`.
2. Press Alt+F2. Run `pingpong`.
3. Press Alt+F3. Run `fish`.
4. Switch between the terminals. All three keep running while hidden:
   - The counter's number keeps jumping ahead.
   - `pingpong` keeps moving.
   - `fish` keeps animating into its own screen.

   Nothing from one terminal shows up on another.

### Terminals keep their own state

- Type half a command on terminal 1, switch to terminal 2, and come back.
  The half-typed line, cursor and screen are all still there.
- `hello` on terminal 2 waits for input only from terminal 2. Terminals 1 and 3 keep working meanwhile.

### Process create and exit across terminals

These used to crash the kernel (see below), and now work:

- Terminal 1: `exit`, then Alt+F3 and `ls`, then Alt+F1. Terminal 1 has a fresh shell.
- Terminal 1: `shell`, then Alt+F2 and `shell`, then Alt+F1 and `exit`, then `ls`.
  Then Alt+F2, and `ls` still works there.
- Run `shell` four times on one terminal. The fourth prints `Max number of processes reached`.
  `exit` frees the slots again.

### Files and error handling

- `cat frame0.txt`, `cat frame1.txt`, and `cat verylargetextwithverylongname.tx` (32 characters) all print.
- `cat verylargetextwithverylongname.txt` (33 characters) is rejected, because file names are at most 32 characters.
- `syserr`, then `0` prints `Overall Tests: PASS`.
- `sigtest 0` prints `Page Fault Exception Caught!` and `program terminated by exception`,
  and only that program dies.

### Kernel unit tests

`student-distrib/tests.c` holds the in-kernel tests from the early checkpoints
(IDT, paging, file system). They are commented out inside `launch_tests()`, and the
`launch_tests();` call in `kernel.c` is commented out too. To use one, uncomment
both and run `make && make run`. The user programs above (`syserr`, `ls`, `cat`,
`fish`, and so on) are the main tests now.

---

## Project layout

```
linux_OS/
├── student-distrib/        # the kernel (the code we wrote)
│   ├── boot.S              # multiboot header, sets up the boot stack, calls entry()
│   ├── kernel.c            # entry(): GDT/TSS/LDT, PIC, paging, devices, then starts the terminals
│   ├── x86_desc.S/.h       # GDT, IDT, TSS definitions (course-provided)
│   ├── idt_init.c          # fills the IDT: exceptions, PIT/keyboard/RTC, int 0x80
│   ├── idt_assembly.S      # assembly entry points for every interrupt and the syscall dispatcher
│   ├── idt_handlers.c      # exception handlers (print, then kill the faulting program)
│   ├── i8259.c             # PIC init, mask/unmask, EOI
│   ├── paging_init.c       # page directory and tables, per-process 4 MB pages, vidmap mapping
│   ├── paging_asm.S        # loads CR3 and turns on paging and 4 MB pages
│   ├── Systems_Calls.c     # halt, execute, read, write, open, close, getargs, vidmap
│   ├── file_system.c       # read-only file system: dentries, inodes, data blocks, file/dir ops
│   ├── keyboard.c          # scancode handling, line editing, terminal read/write
│   ├── terminal.c          # 3 terminals: setup, Alt+F switching, saved screens
│   ├── pit.c               # PIT timer at 100 Hz and the round-robin scheduler
│   ├── context_switch.S    # saves one kernel stack and resumes another
│   ├── rtc.c               # RTC at 1024 Hz, with a separate rate for each open RTC file
│   ├── lib.c               # printf/putc (each terminal has its own cursor), string helpers
│   ├── cursor.c            # VGA text cursor
│   ├── tests.c             # checkpoint unit tests
│   ├── update_image.py     # writes bootimg into mp3.img on macOS
│   ├── Makefile
│   ├── bootimg             # the built kernel (ELF)
│   ├── mp3.img             # bootable disk: GRUB + bootimg + filesys_img
│   ├── filesys_img         # the file system image GRUB loads as a module
│   └── BUG_LOG.txt         # bugs we hit and fixed during the semester
├── syscalls/               # user programs (shell, ls, cat, grep, ...) and their syscall stubs (course-provided)
├── fish/                   # the fish animation program (course-provided)
├── fsdir/                  # files packed into filesys_img
├── createfs, elfconvert    # Linux tools that build filesys_img from fsdir (course-provided)
```

### Memory layout

| Address | Contents |
|---|---|
| 0xB8000 | VGA text memory (the visible screen) |
| 0xB9000 / 0xBA000 / 0xBB000 | Saved screens for terminals 1 / 2 / 3 while they are hidden |
| 4 MB to 8 MB | Kernel (one 4 MB page) |
| Just below 8 MB | Six 8 KB blocks, one per process: PCB at the bottom, kernel stack on top |
| 8 MB + 4 MB × slot | Physical memory for each process's program |
| 128 MB (virtual) | The running program's 4 MB page. Programs load at 0x08048000. |
| 132 MB (virtual) | `vidmap` page: the real screen, or the saved screen if that terminal is hidden |

### How scheduling works

- Every PIT tick (10 ms), `scheduler()` in `pit.c` moves the CPU to the next terminal. For that terminal it swaps in:
  - its process list,
  - its program's 4 MB page,
  - its kernel stack in the TSS,
  - its `vidmap` page.
- `context_switch` then saves the current kernel stack pointer and resumes the next one.
- Terminals 2 and 3 start on a small pre-built stack that runs `execute("shell")` the first time they're scheduled.
- Switching terminals with Alt+F*n* only changes what's on screen and which terminal gets the keyboard. It doesn't touch the scheduler.

---

## What was fixed

The `broken_os` branch is the kernel as submitted. It crashed with
`Page Fault Exception Caught!` when processes exited while other terminals had
programs running. The `working_os` branch fixes the following:

**Scheduler and process bugs (the crashes)**
- `halt` freed whichever process slot was handed out last, not the exiting
  process's own slot. The next program could then overwrite another terminal's
  running program and stack.
- When a terminal's base shell exited and restarted, it used another process's kernel stack.
- There was no real scheduler: the PIT only started the three shells, so programs
  on hidden terminals were frozen. The new scheduler is round-robin, using `context_switch`.
- The kernel turned interrupts on before the terminals were set up.
- System calls could be interrupted halfway through changing the process tables.
- Each process's kernel stack was placed one 8 KB block too low.
- `halt` set the TSS kernel stack to the middle of the parent's stack instead of its top.

**Other bugs**
- `execute` copied 4 bytes into a 1-byte stack buffer when reading the program's entry point.
- `read_dir` wrote past the end of its name buffer. It also shared one position across every process and assumed 17 files.
- `open` succeeded on files that don't exist.
- Reading past the end of a file returned garbage, and could crash.
- Exceptions reported success (`halt((uint8_t)256)` is `halt(0)`). Most exceptions also froze the whole OS instead of killing the program.
- `rtc_write` divided by zero for frequency 0. Programs on different terminals also changed each other's RTC rate.
- Command arguments longer than 128 characters overflowed a buffer.
- File names longer than 32 characters matched the first 32 characters.

**Build**
- Builds on macOS with `i686-elf-gcc`.
- Modern-GCC flags: `-std=gnu89 -fcommon -fpermissive`.
- `update_image.py` replaces the Linux-only `debug.sh`.

## Known limitations

- Signals (`set_handler`, `sigreturn`) are not implemented, so `sigtest` is only an exception test.
- The file system is read-only, and only 6 processes can exist at once.
- `filesys_img` can only be rebuilt on Linux (`createfs` is a Linux binary). The prebuilt image already has every program.

---

## Course notice

ECE 391 MP3 starter code © University of Illinois. Please don't copy this code
for a current offering of the course; see the
[University of Illinois Student Code](http://studentcode.illinois.edu/article1_part4_1-401.html)
on academic integrity.

**LEGAL**

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose, without fee, and without written agreement is
hereby granted, provided that the above copyright notice and the following
two paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO
ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE,
SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
