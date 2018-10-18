/*
 * rgb_duty_cycle.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "rgb_duty_cycle.h"

void setup_leds(){
    P1DIR |= (RED_LED + BLUE_LED + GREEN_LED);

    // start leds as turned off
    P1OUT &= ~(RED_LED + BLUE_LED + GREEN_LED);
}

void setup_timers(){
    BCSCTL3 = LFXT1S_2; // makes aclk use an internal 12kHz oscillator instead of external

    TA0CCTL0 = CCIE; //  CCR0 interrupt enabled
    TA0CCTL1 = CCIE; //  CCR1 interrupt enabled
    TA0CCTL2 = CCIE; //  CCR2 interrupt enabled

    TA1CCTL0 = CCIE; //  "CCR3" TA1CCR0 interrupt enabled

    // Add ID_1 to both timers (divide clk by 2) if brightness of 0 doesn't work
    // because there is not enough time between Timer A0 and Timer A1

    TA0CTL = TASSEL_1 + MC_1 + TAIE /*+ ID_1 */; // Set the timer A to SMCLCK, Up mode
    TA1CTL = TASSEL_1 + MC_1/*+ ID_1 */; // Set the timer A to SMCLCK, Up mode

    TA0CCR0 = 256;
    // TA0CCR0 will set the overall RGB led period
    //              currently 1:1
    // if TA0CCRO must be changed, try to increase to binary power
    // multiply brightness accordingly
    //    EX: TA0CCR0 = 512;
    //        TA0CCR1 = brightness*2

    set_r_duty_cycle(100);
    set_g_duty_cycle(10);
    set_b_duty_cycle(100);
}

void set_r_duty_cycle(char brightness){
    TA0CCR1 = brightness;
}

void set_g_duty_cycle(char brightness){
    TA0CCR2 = brightness;
}

void set_b_duty_cycle(char brightness){
    TA1CCR0 = brightness;
}

