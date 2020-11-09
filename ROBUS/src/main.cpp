/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include "capteurs.h"
#include <string.h>



/******************************************************************************/
/* main --------------------------------------------------------------------- */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);

//Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup() {
  BoardInit();
  
  //Deplacement_Init();
  //Deplacement_Ligne(50);
  Serial.begin(9600);
  capteurCouleur_Init(tcs);

}

void loop() {

  MOTOR_SetSpeed(LEFT, 0.25);
  MOTOR_SetSpeed(RIGHT, 0.25);

  struct RGB couleur;
  char nomCouleur[100] = "";
  saisirRGB(tcs, &couleur);
  detecterCouleur(couleur, nomCouleur);
  Serial.println(nomCouleur);

  if (strcmp(nomCouleur, "Rouge") == 0)
  {
      MOTOR_SetSpeed(LEFT, 0);
      while (1)
      {
        /* code */
      }
      
  }else if (strcmp(nomCouleur, "Jaune") == 0)
  {
    MOTOR_SetSpeed(RIGHT, 0);
    while (1)
      {
        /* code */
      }
  }else if (strcmp(nomCouleur, "Blue") == 0)
  {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    while (1)
      {
        /* code */
      }
  }
}