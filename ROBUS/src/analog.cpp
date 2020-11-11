/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "analog.h"
#include "LibCHAPAJAS.h"
#include "deplacement.h"
int  readPin  = A4;
int  readPin2 = A5;
int  bruitfiltre = 0;
int  bruitambiant = 0;
void analogsetup()
{
    pinMode(readPin, INPUT);
    pinMode(readPin2, INPUT);
    Serial.begin(9600);
}

void analogWait()
{
    bruitfiltre  = analogRead(readPin);
    bruitambiant = analogRead(readPin2);
    // soustraction 5kHz - bruit ambiant
    int deltabruit = bruitfiltre - bruitambiant;
    // Conversion de 0 à 1023 en (0-5V)
    float deltabruitvolt = deltabruit * (5.0 / 1023.0);
    // impression de la valeur
    int i = 0;
    while(true)
    {
        delay(5);
        if(deltabruitvolt > 0.30)
        {
            i++;
        }
        else
        {
            i = 0;
        }
        if(i == 5)
        {
            return;
        }
    }
}
