/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "Adafruit_TCS34725.h"
#include "LibCHAPAJAS.h"
#include "capteurs.h"
#include "coords.h"
#include "deplacement.h"
#include <Wire.h>
#include <string.h>



/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define PIN_ROBUS        13
#define PIN_HIGH_VERSION 11
#define PIN_RED          8
#define PIN_BLUE         9
#define PIN_YELLOW       10

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

void AffichageCouleur(int couleur);
int  RoutineCouleur();


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup()
{
    // Pour déterminer quel ROBUS on est, on lit la pin 13 en mode digital.
    // Si la pin est à LOW (au GND), on est le ROBUS A, si elle est à HIGH (Vcc),
    // on est le ROBUS B.
    int Robus = RobusVersionDetection();

    // Initialisations
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    digitalWrite(PIN_RED, HIGH);    // La LED est éteinte à l'état HIGH
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_YELLOW, HIGH);

    BoardInit();
    /*Coords_Init(Robus);
    Deplacement_Init(Robus);*/

    capteurCouleur_Init(tcs);

    /*print("\n Début de programme %c : %d --------------------------------- \n",
          (Robus == ROBUS_A) ? 'A' : (Robus == ROBUS_B) ? 'B' : 'x',
          millis());

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
    }*/
}

void loop()
{
    //Deplacement_Debug();
    int cible = 0;
    cible  = RoutineCouleur();

    
    AffichageCouleur(cible);

    /*SERVO_Enable(0);
    SERVO_SetAngle(0, 180); 
    delay(3000);
    MOTOR_SetSpeed(0, 0.25);
    MOTOR_SetSpeed(1, 0.25);*/
    //SERVO_SetAngle(0, 90);
    //delay(2000);
    
}


void RoutineA()
{
    Coords_MoveOffset(50, 25);
}

void RoutineB()
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

/**
 * @brief   Allume la led correspondante à la couleur détectée
 */
void AffichageCouleur(int couleur)
{
    switch(couleur)
    {
        case Cible_Rouge:
            digitalWrite(PIN_RED, LOW);
            return;

        case Cible_Bleue:
            digitalWrite(PIN_BLUE, LOW);
            return;

        case Cible_Jaune:
            digitalWrite(PIN_YELLOW, LOW);
            return;

        default:
            //BIIIP();

            break;
    }
}

int RoutineCouleur()
{
    struct RGB couleur;
    char       nomCouleur[100] = "";
    saisirRGB(tcs, &couleur);
    detecterCouleur(couleur, nomCouleur);
    Serial.println(nomCouleur);

    if(strcmp(nomCouleur, ROUGE) == 0)
    {
        return Cible_Rouge;
    }
    else if(strcmp(nomCouleur, JAUNE) == 0)
    {
        return Cible_Jaune;
    }
    else if(strcmp(nomCouleur, BLUE) == 0)
    {
        return Cible_Bleue;
    }
    return 0;
}