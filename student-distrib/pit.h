#ifndef _PIT_H
#define _PIT_H

#include "types.h"

void init_pit();
void pit_interrupt();
void scheduler(void);
void context_switch(uint32_t* save_esp, uint32_t new_esp);

#endif /* _PIT_H */
