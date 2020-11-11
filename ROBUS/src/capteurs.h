#pragma once
/******************************************************************************/
/* Inclusions -------------------------------------------------------------- */
#include "Adafruit_TCS34725.h"
#include "LibCHAPAJAS.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define ROUGE            0
#define JAUNE            1
#define BLEU             2
#define VERT             3
#define COULEUR_INCONNUE -1


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
int  detecterCouleur(struct RGB couleur);
int  RoutineCouleur();
void saisirRGB(Adafruit_TCS34725* tcs, struct RGB* rawCouleur);


struct RGB color_converter(int hexValue);
