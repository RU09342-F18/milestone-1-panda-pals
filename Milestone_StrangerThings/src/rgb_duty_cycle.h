/*
 * rgb_duty_cycle.h
 *
 */
#ifndef RGB_DUTY_CYCLE_H
#define RGB_DUTY_CYCLE_H

#define RED_LED     BIT3 // P1.3
#define GREEN_LED   BIT4 // P1.6
#define BLUE_LED    BIT5 // P1.5

// sets rgb led pins to output
void setup_leds();

// sets up TimerA0 to flash color at specified duty cycles
void setup_timers();

// sets TACCR1 to corresponding duty cycle, 0=off, 255=100% duty cycle
void set_r_duty_cycle(char brightness);

// sets TACCR2 to corresponding duty cycle, 0=off, 255=100% duty cycle
void set_g_duty_cycle(char brightness);

// sets TACCR3 to corresponding duty cycle, 0=off, 255=100% duty cycle
void set_b_duty_cycle(char brightness);

#endif
