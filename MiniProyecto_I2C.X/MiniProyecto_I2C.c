//Universidad del Valle de Guatemala
//Digital 2
//Diego Mencoss 18300
//Slave 1-ADC



#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "MPU6050.h"
#include "I2C.h"
#include "USART.h"



//Definimos variables
char Ay, Az;
char Ax;
uint8_t contador;
uint8_t valorRX;


// BEGIN CONFIG
#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enabl e bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG




//Prototipos de funciones
void setup(void);
void __interrupt() isr(void);



//Configuramos interrupciones
//Interrupciones

void __interrupt() isr(void) {
    if (PIR1bits.RCIF == 1) {
        valorRX = UART_get_char(); //Aqui recibimos el dato de la recepcion
        PIR1bits.RCIF = 0;
        // PORTD = valorRX;
    }
}

void main() {
    setup();
    MPU6050_init();
    UART_config();

    while (1) {

        if (valorRX == 0b11110010) {//LED ROJO
            PORTCbits.RC2 = 0;
            RCREG = 0; //Ponemos en 0 para que no vuelva a entrar al if
            valorRX = 0;
        } else if (valorRX == 0b11111110) { //LED ROJO
            PORTCbits.RC2 = 1;
            RCREG = 0; //Ponemos en 0 para que no vuelva a entrar al if
            valorRX = 0;
        } else if (valorRX == 0b11110111) {//LED VERDE
            PORTCbits.RC1 = 0;
            RCREG = 0; //Ponemos en 0 para que no vuelva a entrar al if
            valorRX = 0;
        } else if (valorRX == 0b11111011) { //LUZ VERDE
            PORTCbits.RC1 = 1;
            RCREG = 0; //Ponemos en 0 para que no vuelva a entrar al if
            valorRX = 0;
        }

        UART_send_char(0);
        __delay_ms(200);

        Ax = MPU6050_get_Ax(); // Acelerometro eje x
        PORTD=Ax;
        UART_send_char('a');
        __delay_ms(200);
        UART_send_char(Ax);
        __delay_ms(200);

        Ay = MPU6050_get_Ay(); // Acelerometro eje y
        UART_send_char('b');
        __delay_ms(200);
        UART_send_char(Ay);
        __delay_ms(200);

        Az = MPU6050_get_Az(); // Acelerometro eje z
        UART_send_char('c');
        __delay_ms(200);
        UART_send_char(Az);
        __delay_ms(200);




        //  Gx = MPU6050_get_Gx(); // Giroscopio eje x
        //  Gy = MPU6050_get_Gy(); // Giroscopio eje y
        //  Gz = MPU6050_get_Gz(); // Giroscopio eje z


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
    INTCONbits.GIE = 1; //Habilitamos las interrupciones
    INTCONbits.PEIE = 1; //Habilitamos las interrupciones perifericas
    PIR1bits.RCIF = 0; //Apagamos la bandera del RX
}
