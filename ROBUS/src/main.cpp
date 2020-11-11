/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "analog.h"
#include "capteurs.h"
#include "coords.h"
#include "deplacement.h"
#include "sonar.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define PIN_ROBUS 13
#define ROBUS_A   0
#define ROBUS_B   1

#define DELAY_LECTURE 20
#define DISTANCE_B    200


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void RoutineA();
void RoutineB();


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

void routine_couleur();
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
    // Attend que le ROBUS B soit passé
    delay(4000);

    // Déplace vers la couleur
    Coords_Move(CIBLE_PASTILLE);

   

}

void RoutineB()
{
    // Commence un déplacement de 2m
    Deplacement_Ligne(DISTANCE_B);

    int cptDistance    = DISTANCE_B;
    int distanceQuille = distanceSonar();

    while(distanceQuille > 50)
    {
        delay(DELAY_LECTURE);

        if(Deplacement_Fini() && cptDistance < 400)
        {
            Deplacement_Ligne(DISTANCE_B);
            cptDistance += DISTANCE_B;
        }
        else if(Deplacement_Fini() && cptDistance > 300)
        {
            Deplacement_Ligne(75);
        }
        distanceQuille = distanceSonar();
        print("dist: %d\n", distanceQuille);
    }
    delay(500);
    Deplacement_Stop();

    // virage a gauche
    Deplacement_Virage(-90);

    if(distanceQuille < 85)    // le fait avancer 10cm de plus pour compenser l'incertitude
                               // distance si pas trop proche de 1m(largeur de la piste)
    {
        distanceQuille += 10;
    }

    Deplacement_Ligne(distanceQuille);
    while(!ROBUS_IsBumper(FRONT))
    {
    }
    Deplacement_Stop();
}
void routine_couleur()
{
    
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
