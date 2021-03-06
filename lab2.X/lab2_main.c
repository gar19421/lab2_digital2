/*
 * File:   lab3.c
 * Author: Brandon Garrido
 *
 * Created on July 26, 2021, 9:39 PM
 */


#include <xc.h>
#define __XTAL_FREQ 8000000
#include <stdint.h>


#include "USART.h"
#include "ADC.h"
#include "LCD.h"

//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//                                  Variables
//******************************************************************************
  unsigned int a;
  uint8_t ADC1;
  uint8_t ADC2;
  
  uint8_t bandera;
  uint8_t RC_temp;
  
  uint8_t contador;
  uint8_t cont;
  
  uint8_t C11;
  uint8_t C12;
  uint8_t C13;
  uint8_t C21;
  uint8_t C22;
  uint8_t C23;
  uint8_t C31;
  uint8_t C32;
  uint8_t C33;
  uint8_t bandera1;
  
  
//******************************************************************************
                        //  Prototipos
//******************************************************************************
void setup (void);
void displayLCD(void);

void __interrupt() isr(void){
  
    
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        cont++;
        INTCONbits.T0IF = 0;        // TERMINAR INTERRUPCION DE TMR0
    }
    
    if(ADIF == 1){
        // bandera para cambiar de canal
        // hacer 1 canal por interrupcion
        if (bandera == 1){
            ADC1 = ADRESH;
            ADCON0bits.CHS0 = 1;
            bandera = 0;
        } else{
            ADC2 = ADRESH;
            ADCON0bits.CHS0 = 0;
            bandera = 1;
        }
        
        ADIF = 0;
        __delay_us(60);
        ADCON0bits.GO = 1;
    } 
    if(PIR1bits.RCIF == 1){
        //verificaci??n + - de contador usart
        RA7 = 1;//bandera
        if (RCREG ==  0x0D){
        RA7 = 0;
            if (RC_temp == 0x2B){
                contador++;
                if (contador > 255){
                    contador = 0;
                }
            } else if (RC_temp == 0x2D){
                contador--;
                
                if (contador > 255){
                    contador = 0;
                }

            }
        } else {
        RC_temp = RCREG;
        }
    }
    
    if (TXIF == 1){
        if (bandera1 == 0){
            // enviar chars de los 2 adc, usar banderas para enviar 1 por 1
            TXREG = C11 + 48;
            bandera1 = 1;
        } else if (bandera1 == 1){
            TXREG = 0x2E;
            bandera1 = 2;
        } else if (bandera1 == 2){
            TXREG = C12 + 48;
            bandera1 = 3;
        } else if (bandera1 == 3){
            TXREG = C13 + 48;
            bandera1 = 4;
        } else if (bandera1 == 4){
            TXREG = 0x2D;
            bandera1 = 5;
        }
        else if (bandera1 == 5){
            TXREG = C21 + 48;
            bandera1 = 6;
        } else if (bandera1 == 6){
            TXREG = 0x2E;
            bandera1 = 7;
        } else if (bandera1 == 7){
            TXREG = C22 + 48;
            bandera1 = 8;
        } else if (bandera1 == 8){
            TXREG = C23 + 48;
            bandera1 = 9;
        } else if (bandera1 == 9){
            TXREG = 0x0D;
            bandera1 = 0;
        }
        
    TXIF = 0; 
    }   

}

void main(void) {
    setup();
    while(1){  
        if(cont > 15){
            cont = 0;
            TXIE = 1;
        }
        
  //conversiones valores adc y sensor 3 a 0.00 a 5.00V
    C11 = ADC1 / 51;
    C12 = ((ADC1 * 100 / 51) - (C11*100))/10;
    C13 = ((ADC1 * 100 / 51) - (C11*100) - (C12*10));  
    
    C21 = ADC2 / 51;
    C22 = (((ADC2 * 100) / 51) - (C21*100))/10;
    C23 = (((ADC2 * 100) / 51) - (C21*100) - (C22*10));
      
    
    C31 = contador / 100;
    C32 = ( contador - C31*100)/10;
    C33 = ( contador - C31*100) - C32*10;
    
    //arpoximaci??n de los decimales
    if (C12 > 9){
        C12 = 9;
    }
    if (C13 > 9){
        C13 = 9;
    }
    if (C22 > 9){
        C22 = 9;
    }
    if (C23 > 9){
        C23 = 9;
    }
    
    if (C11 > 5){
        C11 = 5;
    }
    if (C21 > 5){
        C11 = 5;
    }
    if (C31 > 5){
        C11 = 5;
    }
    
    // colocar chars en posicion en la lcd
    // sensor 1
    Lcd_Set_Cursor(2,1);
    Lcd_Write_Char(C11 +48);
    Lcd_Set_Cursor(2,3);
    Lcd_Write_Char(C12 + 48);
    Lcd_Set_Cursor(2,4);
    Lcd_Write_Char(C13 + 48);
    
    //sensor 2
    Lcd_Set_Cursor(2,7);
    Lcd_Write_Char(C21 +48);
    Lcd_Set_Cursor(2,9);
    Lcd_Write_Char(C22 + 48);
    Lcd_Set_Cursor(2,10);
    Lcd_Write_Char(C23 + 48);
     
    //sensor 3
    Lcd_Set_Cursor(2,13);
    Lcd_Write_Char(C31 +48);
    Lcd_Set_Cursor(2,14);
    Lcd_Write_Char(C32 + 48);
    Lcd_Set_Cursor(2,15);
    Lcd_Write_Char(C33 + 48);
    
  }
}

void setup(void){
    
    //Configuraci??n de reloj
    OSCCONbits.IRCF2 =1 ; // IRCF = 111 (8MHz) 
    OSCCONbits.IRCF1 =1 ;
    OSCCONbits.IRCF0 =1 ;
    OSCCONbits.SCS = 1; // Habilitar reloj interno
    
    
    //Configuraci??n puertos I/O
    TRISA = 0x03;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
        
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    ANSEL = 0x03;
    ANSELH = 0;
    
    //Habilitar interrupciones globales
    INTCONbits.GIE = 1;      
    
    // Inicializar LCD
    Lcd_Init();

    
    // Inicializar canales anal??gicos
    initADC(0);
    initADC(1);
       
    
    //Configuraci??n TMR0
    OPTION_REGbits.T0CS = 0;        // TMR0 Clock source
    OPTION_REGbits.PSA = 0;         // Prescaler a tmr0
    OPTION_REGbits.PS = 0b111;        // prescaler 1:256
    TMR0 = 10;
    
    
    // Interrupcion y bandera TMR0
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0;            
    
    // Inicializar usart
    initUSART();
    
    //desplegar texto en pantalla
    displayLCD();
    
}


void displayLCD(){
    //mostrar en pantalla lcd
    Lcd_Set_Cursor(1,3);
    Lcd_Write_String("S1");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("0.00");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2");
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String("0.00");
    Lcd_Set_Cursor(1,14);
    Lcd_Write_String("S3");
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String("000");

}
