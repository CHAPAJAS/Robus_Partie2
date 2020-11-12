/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "analog.h"
#include "capteurs.h"
#include "coords.h"
#include "deplacement.h"
#include "servo.h"
#include "sonar.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define PIN_ROBUS 13
#define ROBUS_A   0
#define ROBUS_B   1

#define DELAY_LECTURE 20
#define DISTANCE_B    200

#define ANGLE_FILET_UP   120
#define ANGLE_FILET_DOWN 240


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
    // Servo_Init();

    print("\n Début de programme %c : %d --------------------------------- \n",
          (Robus == ROBUS_A) ? 'A' : (Robus == ROBUS_B) ? 'B' : 'x',
          millis());

    // attendre le coup de sifflet
    // analogWait();

    // Appelle la fonction principale correspondante
    if(Robus == ROBUS_A)
    {
        Deplacement_Ligne(100);
        // RoutineA();
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
    // Attend que le ROBUS B soit passé
    // delay(4000);

    // Déplace vers la couleur
    Coords_Move(CIBLE_PASTILLE);

    // Lecture de la couleur
    int couleur = RoutineCouleur();

    // Déplacement vers la balle
    Coords_Move(CIBLE_BALLE);

    // Ramasser la balle
    SERVO_SetAngle(LEFT, ANGLE_FILET_DOWN);

    // Déplacement vers la cible de couleur
    switch(couleur)
    {
        case ROUGE:
            Coords_Move(CIBLE_ROUGE);
            break;
        case JAUNE:
            Coords_Move(CIBLE_JAUNE);
            break;
        case BLEU:
            Coords_Move(CIBLE_BLEUE);
            break;

        default:
            // On se prend une chance, on a 1/3 de chance d'avoir raison!
            Coords_Move(CIBLE_JAUNE);
            break;
    }
}

void RoutineB()
{
    // Commence un déplacement de 2m
    Deplacement_Ligne(DISTANCE_B);

    delay(2000);

    int cptDistance = DISTANCE_B;
    int distanceQuille;
    do
    {
        if(ROBUS_IsBumper(FRONT) == true)
        {
            print("bumper");
            break;
        }

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

        delay(DELAY_LECTURE);
    } while(distanceQuille > 45);

    delay(500);
    Deplacement_Stop();

    delay(1000);

    // virage a gauche
    Deplacement_Virage(-90);
    delay(500);

    Deplacement_Ligne(75);
}

int RobusVersionDetection()
{
    pinMode(PIN_ROBUS, INPUT);
    return digitalRead(PIN_ROBUS);
}
