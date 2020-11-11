/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "analog.h"
#include "LibCHAPAJAS.h"
#include "deplacement.h"


/*****************************************************************************/
/* Définitions ------------------------------------------------------------- */
#define NUMBER_OF_SAMPLES 50
#define SAMPLE_DELAY_MS   1

#define SETPOINT 0.3


int  readPin      = A4;
int  readPin2     = A5;
int  bruitfiltre  = 0;
int  bruitambiant = 0;
void analogsetup()
{
    pinMode(readPin, INPUT);
    pinMode(readPin2, INPUT);
}

void analogWait()
{
    int i = 0;
    while(true)
    {
        delay(SAMPLE_DELAY_MS);

        // Lecture des valeurs analogiques du bruit
        bruitfiltre  = analogRead(readPin);
        bruitambiant = analogRead(readPin2);
        // soustraction 5kHz - bruit ambiant
        int deltabruit = bruitfiltre - bruitambiant;
        // Conversion de 0 à 1023 en (0-5V)
        float deltabruitvolt = deltabruit * (5.0 / 1023.0);

        print("%ldmv\n", (int32_t)(deltabruitvolt * 1000));

        if(deltabruitvolt > SETPOINT)
        {
            if(i < NUMBER_OF_SAMPLES)
            {
                i++;
            }
            else
            {
                return;
            }
        }
        else
        {
            if(i > 0)
            {
                i--;
            }
        }
    }
}
