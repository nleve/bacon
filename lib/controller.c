#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "xbee.h"
#include "bmp.h"
#include "status.h"

#include "controller.h"

#define PEERING_LED_DELAY 500
#define THRESHOLD_DIST 100
#define MAX_SOLENOID_TIME 14000
#define CONTROLLER_P 25
#define CONTROLLER_I 0
#define CONTROLLER_D 0

volatile uint16_t timer_1 = 0;
volatile uint16_t timer_2 = 0;
volatile uint16_t peer_timer = 0;
volatile uint16_t sim_timer = 0;
volatile uint16_t solenoid_on_time = 0;
volatile uint8_t solenoid_on = 0;
volatile uint8_t currently_peering = 0;

// timeout fix

void tim_init()
{
    // 1ms resolution
    // See datasheet if you don't know why this is 1ms.
    TCCR1B |= (1<<WGM12);
    TCCR1B |= (1<<CS11)|(1<<CS10);
    OCR1A = 125;

    TIMSK1 |= (1<<OCIE1A);
}

// Set solenoid pin to 0 and make it an output.
void solenoid_init()
{
    PORTB &= ~(1 << PB1);
    DDRB |= (1 << PB1);
}

// Activate solenoid for on_time ms
void activate_solenoid(uint16_t on_time)
{
    PORTB |= (1 << PB1);
    timer_2 = 0;
    solenoid_on = 1;
    solenoid_on_time = on_time;
}

void deactivate_solenoid()
{
    solenoid_on = 0;
    PORTB &= ~(1 << PB1);
}

uint16_t control(int32_t alt, int32_t peer_alt)
{
    uint16_t release_time = 0;
    int32_t distance;

    /* distance = alt - peer_alt; */
    distance = peer_alt - alt;

    // Greater than 100 decimeters
    if (distance > THRESHOLD_DIST)
    {
        /* release_time = distance / PROP_SCALER; */
        /* release_time /= 1.5;    // gain 1.5 decimeters per second per */
        /* release_time *= 100;    // 100 grams of weight lost */
        /* release_time /= POUR_RATE; */
        release_time = distance * CONTROLLER_P;
    }

    if (release_time > MAX_SOLENOID_TIME)
        release_time = MAX_SOLENOID_TIME;

    return release_time;
}

ISR(TIMER1_COMPA_vect)
{
    // timer is a ms counter
    timer_1++;
    timer_2++;
    sim_timer++;
    peer_timer++;
    if (solenoid_on && (timer_2 >= solenoid_on_time))
    {
        deactivate_solenoid();
    }
    else if (currently_peering && (timer_2 >= PEERING_LED_DELAY))
    {
        // Blink green LED
        status_toggle(STATUS0);
        timer_2 = 0;
    }
}