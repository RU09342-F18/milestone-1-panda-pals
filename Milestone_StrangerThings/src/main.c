#include <msp430.h>
#include <stdint.h>
#include "rgb_duty_cycle.h"
#include "uart.h"

/**
 * main.c
 */

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    setup_leds();

    setup_timers();

    setup_uart();

    __enable_interrupt();
}

// Interrupt vector for TA0CCTL0 (when TAR == TACCRO)
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if(TA0CCR1 >= 1) // 1 can be increased if nec.
    {
        P1OUT |= RED_LED; // turn red led on
    }
    else
    {
        P1OUT &= ~RED_LED; // turn red led off
    }

    if(TA0CCR2 >= 1) // 1 can be increased if nec.
    {
        P1OUT |= GREEN_LED; // turn green led on
    }
    else
    {
        P1OUT &= ~GREEN_LED; // turn green led off
    }
    if(TA1CCR0 >= 1) // 1 can be increased if nec.
    {
        P1OUT |= BLUE_LED; // turn blue led on
    }
    else
    {
        P1OUT &= ~BLUE_LED; // turn blue led off
    }

}

// Interrupt vector for TA0CCTL1..3
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer0_A1 (void)
#else
#error Compiler not supported!
#endif
{
    uint8_t _TA0IV = TA0IV; // we need to save this b/c it will get cleared on read

    if (_TA0IV == TA0IV_TACCR1)
    {
        // red off
        P1OUT &= ~RED_LED;
    }
    else if (_TA0IV == TA0IV_TACCR2)
    {
        // green off
        P1OUT &= ~GREEN_LED;
    }
}

// Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0 (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT |= BLUE_LED; // blue LED state ON
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer1_A1 (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT &= ~BLUE_LED; // blue LED state OFF
}

unsigned char in_count = 0;
unsigned char in_message_len = 0;
unsigned char out_message_len = 0;
unsigned char out_message[] = {};
unsigned char err_flag = 0;
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    unsigned char data = UCA0RXBUF;
    if(in_message_len == 0) {
        in_message_len = data;
        out_message_len = in_message_len - 3;
        out_message[0] = out_message_len;
        in_count = 0;
    }
    else if(in_count >= in_message_len - 1) {
        if(data == 0x0d) {
            out_message[out_message_len - 1] = 0x0d;
            if(err_flag == 1)
                err_flag = 0;
            else
                send_bytes(out_message, out_message_len);
            in_message_len = 0;
            out_message_len = 0;
        }
        else {
            err_flag = 1;
        }
    }
    else if(in_count == 1) {
        set_r_duty_cycle(data);
    }
    else if(in_count == 2) {
        set_g_duty_cycle(data);
    }
    else if(in_count == 3) {
        set_b_duty_cycle(data);
    }
    else {
        out_message[in_count - 3] = data;
    }

    //send_bytes(&out_message_len, 1);
    //if(err_flag != 0)
    in_count++;
}

