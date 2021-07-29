
#include "ADC.h"
#include <xc.h>


void initADC(uint8_t channel) {
    
    // configuracion del adc
    ADCON1bits.ADFM = 0; // justificacion a la izquierda
    ADCON1bits.VCFG0 = 0; //configuracion vdd y ground en vss
    ADCON1bits.VCFG1 = 0; 
    
    ADCON0bits.ADCS = 0b10; // conversion del clock a fosc/32
    ADCON0bits.CHS = 0;
    
    ADCON0bits.ADON = 1;
    __delay_us(50);
    ADCON0bits.GO = 1;
    
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;

    
    switch(channel){
        case 0:
            TRISAbits.TRISA0 = 1;           // entrada
            ANSELbits.ANS0 = 1;             // analogica
            break;
        case 1:
            TRISAbits.TRISA1 = 1;           // entrada
            ANSELbits.ANS1 = 1;             // analogica
            break;
        case 2:
            TRISAbits.TRISA2 = 1;           // entrada
            ANSELbits.ANS2 = 1;             // analogica
            break;
        case 3:
            TRISAbits.TRISA3 = 1;           // entrada
            ANSELbits.ANS3 = 1;             // analogica
            break;
        case 4:
            TRISAbits.TRISA5 = 1;           // entrada
            ANSELbits.ANS4 = 1;             // analogica
            break;
        case 5:
            TRISEbits.TRISE0 = 1;           // entrada
            ANSELbits.ANS5 = 1;             // analogica
            break;
        case 6:
            TRISEbits.TRISE1 = 1;           // entrada
            ANSELbits.ANS6 = 1;             // analogica
            break;
        case 7:
            TRISEbits.TRISE2 = 1;           // entrada
            ANSELbits.ANS7 = 1;             // analogica
            break;
        default:
            TRISAbits.TRISA0 = 1;           // entrada
            ANSELbits.ANS0 = 1;             // analogica
            break;
    }
}