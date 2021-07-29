#include "Oscilador.h"

/*
 INICIALIZACION DE OSCILADOR INTERNO: PARAMETRO FRESC DEL OSCILADOR INT: PAG 62
 */
void initOsc(uint8_t IRCF){
    
    switch(IRCF){
        case 0: //31kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 0;
            break;
        case 1: //125kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;
            break;
        case 2: //250kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;
            break;
        case 3: //500kHz
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 1;
            break;
        case 4: //1MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 0;
            break;
        case 5: //2MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;
            break;
        case 6: //4MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;
            break;
        case 7: //8MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 1;
            break;        
        default: //4MHz
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;
            break;
    }
    
    OSCCONbits.SCS = 1; //UTILIZO OSCILADOR INTERNO
    
}

