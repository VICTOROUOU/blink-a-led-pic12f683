#include <xc.h>

/**
 * Blink a led with varying power.
 *
 * PIC: 12F683
 * LED Connected to GPIO 2 (pin 5)
 */

// CONFIG
#pragma config FOSC = INTOSCIO
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF

#define LED GPIObits.GP2

volatile char counter1 = 0;
char dim = 0;
int on = 1;

void main(void)
{
    OSCCON=0b00010111;

    // Prescaler setup
    OPTION_REGbits.PSA=0;   // Assign to TIMER0
    OPTION_REGbits.PS2=0;   // 1:16 (011)
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.T0CS=0;  // Internal Clock
    OPTION_REGbits.T0SE=0;  // Incr low -> high

    CCP1CONbits.CCP1M3=1;
    CCP1CONbits.CCP1M2=1;
    CCP1CONbits.CCP1M1=0;
    CCP1CONbits.CCP1M0=0;
    PR2=0x0F;
    T2CON=0b00000100;
    CCPR1L=0x00;
    CCPR1H=0x00;
    TMR2IF=0;               // Timer2 to PR2 match has not occurred (must be cleared)

    INTCONbits.T0IE=1;      // Enables the Timer0 interrupt
    INTCONbits.T0IF=0;      // Timer0 Overflow Interrupt Flag bit (must be cleared)
    INTCONbits.GIE=1;       // Enable interrupts (global)
    
    TRISIO=0x00;

    LED=1;

    while(1){
        CCPR1L=dim;
        if (dim== 0x01) { on = 1; }
        if (dim==0x10){on = 0;}
        if (counter1==1){LED=0;}
        if (counter1==2){LED=1;}
    }
}

void interrupt timeroverflow () {
    if (INTCONbits.T0IF==1){
        INTCONbits.T0IF=0;
        counter1++;
        if ( on == 1 ) dim++;
        else dim--;

        if (counter1==3){counter1=1;}
    }
}