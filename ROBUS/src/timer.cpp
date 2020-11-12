#include "TimerOne.h"
#include "timer.h"

#define COMPTES_POUR_PID    TIMER_DELAY_MS / ((double)FREQ_INTERRUPT_US / 1000.0)

uint32_t compteur;

void timer_handler();

void timer_init()
{
    Timer1.initialize(FREQ_INTERRUPT_US);
    Timer1.attachInterrupt(&timer_handler);
}

void timer_handler()
{
    extern void Servo_Handle();
    extern void PID();

    Servo_Handle();

    if(++compteur >= COMPTES_POUR_PID)
    {
        compteur = 0;
        Timer1.stop();
        PID();

        Timer1.resume();
    }
}