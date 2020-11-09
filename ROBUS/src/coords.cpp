/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "coords.h"
#include "deplacement.h"

#include <math.h>


/******************************************************************************/
/* Définitions -------------------------------------------------------------- */
#define LONGEUR_PISTE 500    // cm
#define HAUTEUR_PISTE 100    // cm
#define LARGEUR_CARRE 30     // cm
#define MOITIE_CARRE  LARGEUR_CARRE / 2


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
const point Depart_A{MOITIE_CARRE, 75};    // Départ du robot A
const point Depart_B{MOITIE_CARRE, 25};    // Départ du robot B

const point Pastille{100 + MOITIE_CARRE, HAUTEUR_PISTE / 2};    // Pastille de couleur, 1m du début
const point Balle{200 + 50, HAUTEUR_PISTE / 2};                 // Balle à 2.5m du début

const point Jaune{300 + 1, MOITIE_CARRE};    // A ACTUALISER
const point Bleu{400 + MOITIE_CARRE, HAUTEUR_PISTE - MOITIE_CARRE};
const point Rose{500 - MOITIE_CARRE, MOITIE_CARRE};    // Accoté sur le mur du fond,


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
point positionActuelle = {0, 0};
float angleActuel      = 0;


/******************************************************************************/
/* Définition de fonctions -------------------------------------------------- */
void Coords_Init(int robus)
{
    if(robus == 0 || robus == 'A')
    {
        positionActuelle = Depart_A;
    }
    else if(robus == 1 | robus == 'B')
    {
        positionActuelle = Depart_B;
    }
    else
    {
        BIIIP();
    }
}


void Coords_Move(point destination)
{
    // Vérification du paramètre
    if(destination.x > LONGEUR_PISTE || destination.y > HAUTEUR_PISTE)
    {
        BIIIP();
        return;
    }

    float distance = GetDistanceToPoint(positionActuelle, destination);
    // float angle    = GetAngleToPoint(positionActuelle, destination);

    // Rotation
    // A IMPLEMENTER

    // Deplacement
    Deplacement_Ligne(distance);

    // Actualisation de la position et de l'angle
    positionActuelle = destination;
    // angleActuel += angle;
}

void Coords_Move(int32_t x, int32_t y)
{
    // Appelle la vraie fonction 'Move' avec les points
    Coords_Move({x, y});
}

void Coords_Move(cible destination)
{
    switch(destination)
    {
        case Cible_DepartA:
            Coords_Move(Depart_A);
            return;
        case Cible_DepartB:
            Coords_Move(Depart_B);
            return;
        case Cible_Pastille:
            Coords_Move(Pastille);
            return;
        case Cible_Balle:
            Coords_Move(Balle);
            return;
        case Cible_Jaune:
            Coords_Move(Jaune);
            return;
        case Cible_Bleue:
            Coords_Move(Bleu);
            return;
        case Cible_Rose:
            Coords_Move(Rose);
            return;

        default:
            BIIIP();
            return;
    }
}


point Coords_PositionActuelle()
{
    return positionActuelle;
}

float GetDistanceToPoint(point A, point B)
{
    // Distance entre les points sur les axes x et y
    // Pas besoin de faire la valeur absolue, car on les met au carré.
    int16_t dx = A.x - B.x;
    int16_t dy = A.y - B.y;

    // Pythagore
    dx = dx * dx;    // x^2
    dy = dy * dy;    // y^2

    float distance = sqrt(dx + dy);
    return distance;
}

float GetAngleToPoint(point A, point B)
{
    // Distance entre les points sur les axes x et y
    float dx = A.x - B.x;
    float dy = A.y - B.y;

    // dx est le côté adjacent, dy est le côté opposé. On fait une arctan(dy/dx)
    float rad = atan(dy / dx);
    float deg = rad * 180 / PI;

    return deg;
}