/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "coords.h"
#include "deplacement.h"
#include "sonar.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define PIN_ROBUS 13

#define DELAY_LECTURE   20
#define ROBUS_A 0
#define ROBUS_B 1


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void RoutineA();
void RoutineB();


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup()
{
    // Pour déterminer quel ROBUS on est, on lit la pin 13 en mode digital.
    // Si la pin est à LOW (au GND), on est le ROBUS A, si elle est à HIGH (Vcc),
    // on est le ROBUS B.
    int Robus = -1;
    pinMode(PIN_ROBUS, INPUT);
    Robus = digitalRead(PIN_ROBUS);

    // Initialisations
    BoardInit();
    Coords_Init(Robus);
    Deplacement_Init(Robus);

    print("\n Début de programme : %d --------------------------------- \n", millis());

    // Conditions initiales
    // Deplacement_Ligne(100);
    // Coords_Move(Cible_Pastille);


    // Appelle la fonction principale correspondante
    if(Robus == ROBUS_A)
    {
        RoutineA();
    }
    else if(Robus == ROBUS_B)
    {
        RoutineB();
    }
    else
    {
        // Erreur! Le robus n'est pas configuré à une version correcte.
        BIIIP();
    }
}

void loop()
{
    Deplacement_Debug();
}


void RoutineA()
{
}

void RoutineB()
{
    // Deplacement_Ligne(200);
    int distanceQuille;
    int cptDistance = 0;
    /*distanceQuille= distanceSonar();
    Serial.println(distanceQuille);
    delay(1000);
    distanceQuille= distanceSonar();
    Serial.println(distanceQuille);
    delay(1000);
    distanceQuille= distanceSonar();
    Serial.println(distanceQuille);
    delay(1000);
    distanceQuille= distanceSonar();
    Serial.println(distanceQuille);
    delay(1000);
    */

    
    Deplacement_Ligne(200);
    cptDistance+=200;
    distanceQuille = distanceSonar();
    print("distance quille %d\n", distanceQuille);
    while(distanceQuille > 50)
    {
        delay(DELAY_LECTURE);
        
        if(Deplacement_Fini() && cptDistance<400)
        {
            Deplacement_Ligne(200);
            cptDistance+=200;
        }
        else if(Deplacement_Fini() && cptDistance>300)
        {
            Deplacement_Ligne(75);
        }
        distanceQuille = distanceSonar();
        print("dist: %d\n", distanceQuille);
    }
    delay(500);
    Deplacement_Stop();

    // virage a droite
    if(distanceQuille < 85)        // le fait avancer 10cm de plus pour compenser l'incertitude
                                   // distance si pas trop proche de 1m(largeur de la piste)
    {
        distanceQuille += 10;
    }

    // Deplacement_Ligne(distanceQuille);
    while(!ROBUS_IsBumper(2))
    {
    }
    Deplacement_Stop();
}