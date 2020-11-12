/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"

#include "../lib/Timer/TimerThree.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define PIN_SERVO 11    // Pin 11 en mode PWM

#define FREQ_INTERRUPT_US 500L
#define FREQ_INTERRUPT_MS (FREQ_INTERRUPT_US / 1000.0)

#define COUNTER_PULSE_DELAY (20.0 / FREQ_INTERRUPT_MS)

#define CALCULATE_PULSE(angle) (((((float)angle) / 120.0) + 0.5) / (FREQ_INTERRUPT_US / 1000.0))


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
volatile int pulseDelayCounter = 0;
volatile int pulseSizeCounter  = 0;
int          desiredPulse      = CALCULATE_PULSE(90);


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void Servo_Handle();


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Servo_Init()
{
    pinMode(PIN_SERVO, OUTPUT);

    Timer3.initialize(FREQ_INTERRUPT_US);
    Timer3.attachInterrupt(&Servo_Handle, FREQ_INTERRUPT_US);
}

void Servo_SetAngle(int angle)
{
    desiredPulse = CALCULATE_PULSE(angle);
    print("Servo pulses: %d\n", desiredPulse);
}

void Servo_Handle()
{
    if(++pulseSizeCounter >= desiredPulse)
    {
        digitalWrite(PIN_SERVO, LOW);
    }

    if(++pulseDelayCounter >= COUNTER_PULSE_DELAY)
    {
        pulseDelayCounter = 0;
        pulseSizeCounter  = 0;
        digitalWrite(PIN_SERVO, HIGH);
    }
}
