/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "suiveur.h"


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
Adafruit_TCS34725* p_tcs;


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Suiveur_Init()
{
    p_tcs = getTCS();
}

bool Suiveur_IsOnLine()
{
    struct RGB  couleurSaisie;

    saisirRGB(p_tcs, &couleurSaisie);
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
