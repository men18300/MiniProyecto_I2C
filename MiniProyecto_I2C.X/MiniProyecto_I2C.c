//Universidad del Valle de Guatemala
//Digital 2
//Diego Mencoss 18300
//Slave 1-ADC



#define _XTAL_FREQ 8000000

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "MPU6050.h"
#include "I2C.h"
#include "USART.h"

float Ax,Gx ;

//Definimos variables
uint8_t contador;


// BEGIN CONFIG
#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG




//Prototipos de funciones
void setup(void);



//Configuramos interrupciones

void main() {
    setup();
    MPU6050_init();
    UART_config() ;
    while (1) {
//        PORTDbits.RD2 = 1;
//        __delay_ms(20);
//        PORTDbits.RD2 = 0;
//        __delay_ms(20);


     //  PORTD = MPU6050_get_Ax(); // Acelerometro eje x
//        Ay = MPU6050_get_Ay(); // Acelerometro eje y
//        Az = MPU6050_get_Az(); // Acelerometro eje z
//
        PORTD = MPU6050_get_Gx(); // Giroscopio eje x
        UART_send_string(PORTD);
     //   PORTD=;
//        Gy = MPU6050_get_Gy(); // Giroscopio eje y
//        Gz = MPU6050_get_Gz(); // Giroscopio eje z

    } 
}

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISA = 0;
    PORTA = 0;
    TRISB = 0;
    PORTB = 0;
    TRISC = 0;
    PORTC = 0;
    TRISD = 0;
    PORTD = 0;
    TRISE = 0;
    PORTE = 0;
    contador = 0;
    I2C_Master_Init(100000);

}
