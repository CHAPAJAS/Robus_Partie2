/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "suiveur.h"


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
Adafruit_TCS34725* tcs;


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Suiveur_Init()
{
    tcs = getTCS();
}

bool Suiveur_IsOnLine()
{
    struct RGB  couleurSaisie;
    static char nomCouleur[100] = "";

    saisirRGB(tcs, &couleurSaisie);
    int couleur = detecterCouleur(couleurSaisie);

    if(couleur == VERT)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}
