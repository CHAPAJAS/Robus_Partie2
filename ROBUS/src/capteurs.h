#pragma once
/******************************************************************************/
/* Inclusions -------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "Adafruit_TCS34725.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define ROUGE 0
#define JAUNE 1
#define BLEU  2
#define VERT  3


/******************************************************************************/
/* Types -------------------------------------------------------------------- */
struct RGB
{
    double red;
    double green;
    double blue;
};


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void capteurCouleur_Init();
void detecterCouleur(struct RGB couleur, char couleurDetecte[]);
int detecterCouleur(struct RGB couleur);
void RoutineCouleur();
void saisirRGB(Adafruit_TCS34725* tcs, struct RGB* rawCouleur);

Adafruit_TCS34725* getTCS();



struct RGB color_converter(int hexValue);
