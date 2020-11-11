#include "LibCHAPAJAS.h"


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void capteurCouleur_Init();
void saisirRGB(Adafruit_TCS34725 tcs, struct RGB *rawCouleur);
void detecterCouleur(struct RGB couleur, char couleurDetecte[]);

struct RGB
{
    double red;
    double green;
    double blue;
};


struct RGB color_converter(int hexValue);
