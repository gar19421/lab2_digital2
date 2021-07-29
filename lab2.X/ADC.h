

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define ADC_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

void initADC(uint8_t channel);

#endif	/* ADC_H */

