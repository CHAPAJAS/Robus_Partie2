#include<Adafruit_TCS34725.h>
#include<Wire.h>

#define ROUGE "Rouge"
#define JAUNE "Jaune"
#define BLUE "Blue"

/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void capteurCouleur_Init(Adafruit_TCS34725 tcs);
void saisirRGB(Adafruit_TCS34725 tcs, struct RGB *rawCouleur);
void detecterCouleur(struct RGB couleur, char couleurDetecte[]);

struct RGB
{
    double red;
    double green;
    double blue;
};


struct RGB color_converter(int hexValue);
