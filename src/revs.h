#pragma once

extern unsigned int revcount;
extern unsigned int revs;

void inline calculate_revs(unsigned long time_us) {
    if(time_us <= 0 || revcount <=0 ) revs = 0;
    else
    revs = (int)((((double)revcount) * 1000000.0 / (double)time_us) * 30.0);
    
    //(int)(((float)(timingrevcount * 1000000.0) / timing) * 30.0);
    revcount = 0;
}
