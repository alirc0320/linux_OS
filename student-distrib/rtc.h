#ifndef _RTC_H
#define _RTC_H

#include "types.h"


void init_RTC(); // initialize RTC 

void rtc_interrupt(void);
int rtc_write(int32_t freq);
int rtc_open(void);
int rtc_read();
int rtc_close();


#endif /* _RTC_H */
