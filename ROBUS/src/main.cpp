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
#define PIN_ROBUS 13

#define ROBUS_A 0
#define ROBUS_B 1


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void RoutineA();
void RoutineB();
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
    int Robus = -1;
    pinMode(PIN_ROBUS, INPUT);
    Robus = digitalRead(PIN_ROBUS);

    // Initialisations
    BoardInit();
    Coords_Init(Robus);
    Deplacement_Init(Robus);
    capteurCouleur_Init(tcs);

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
    /*int cible = 0;
    cible  = RoutineCouleur();*/

}


void RoutineA()
{
}

void RoutineB()
{
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
}