/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "analog.h"
#include "capteurs.h"
#include "coords.h"
#include "deplacement.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define PIN_ROBUS        13
#define PIN_HIGH_VERSION 11

#define COULEUR_RED    0
#define COULEUR_BLUE   1
#define COULEUR_YELLOW 2

#define ROBUS_A 0
#define ROBUS_B 1


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void RoutineA();
void RoutineB();
int  RobusVersionDetection();


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup()
{
    // Pour déterminer quel ROBUS on est, on lit la pin 13 en mode digital.
    // Si la pin est à LOW (au GND), on est le ROBUS A, si elle est à HIGH (Vcc),
    // on est le ROBUS B.
    int Robus = RobusVersionDetection();

    // Initialisations
    BoardInit();
    Coords_Init(Robus);
    Deplacement_Init(Robus);
    capteurCouleur_Init();
    analogsetup();    // sifflet

    print("\n Début de programme %c : %d --------------------------------- \n",
          (Robus == ROBUS_A) ? 'A' : (Robus == ROBUS_B) ? 'B' : 'x',
          millis());

    // attendre le coup de sifflet
    // analogWait();

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
    
}


void RoutineA()
{
}

void RoutineB()
{
    Deplacement_Ligne(100);
    while(ROBUS_IsBumper(FRONT) == false)
    {
        Deplacement_Debug();
    }
    Deplacement_Stop();
    BIIIP();
}

int RobusVersionDetection()
{
    // La pin 11 est mise à HIGH sur les deux robots
    // Le B est branché dessus,
    pinMode(PIN_HIGH_VERSION, OUTPUT);
    pinMode(PIN_ROBUS, INPUT);
    digitalWrite(PIN_HIGH_VERSION, HIGH);
    delay(10);

    return digitalRead(PIN_ROBUS);
}
