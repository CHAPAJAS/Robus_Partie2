#include "LibCHAPAJAS.h"


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



struct RGB color_converter(int hexValue);
