/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "coords.h"
#include "deplacement.h"

#include <math.h>


/*****************************s*************************************************/
/* Définitions -------------------------------------------------------------- */
#define LONGEUR_PISTE 500    // cm
#define HAUTEUR_PISTE 100    // cm
#define LARGEUR_CARRE 30     // cm
#define MOITIE_CARRE  LARGEUR_CARRE / 2

#define OFFSET_A 0
#define OFFSET_B 0

#define DELAY_DEPLACEMENT 250


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
const point Depart_A{MOITIE_CARRE, 75};    // Départ du robot A
const point Depart_B{MOITIE_CARRE, 25};    // Départ du robot B

const point Pastille{100, 50};                     // Pastille de couleur, 1m du début
const point Balle{200 + 50, HAUTEUR_PISTE / 2};    // Balle à 2.5m du début

const point Jaune{300 + 20, MOITIE_CARRE};    // Pas accoté sur la ligne de 3m
const point Bleu{400 + MOITIE_CARRE, HAUTEUR_PISTE - MOITIE_CARRE};
const point Rose{500 - MOITIE_CARRE, MOITIE_CARRE};    // Accoté sur le mur du fond,


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
point positionActuelle = {0, 0};
float angleActuel      = 0;
float angleOffset      = 0;


/******************************************************************************/
/* Définition de fonctions -------------------------------------------------- */
void Coords_Init(int robus)
{
    if(robus == 0 || robus == 'A')
    {
        positionActuelle = Depart_A;
        angleOffset      = OFFSET_A;
    }
    else if(robus == 1 || robus == 'B')
    {
        positionActuelle = Depart_B;
        angleOffset      = OFFSET_B;
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

    // float distance = GetDistanceToPoint(positionActuelle, destination);
    // Si le y de la destination est plus élevé que le y actuel, il faut avoir un angle négatif
    // (tourner à gauche).
    float angle =
      (destination.y == positionActuelle.y)
        ? 0
        : (destination.y > positionActuelle.y) ? -90 : 90;    // Choisi un angle de 90° ou -90°
    // GetAngleToPoint(positionActuelle, destination);
    angle += angleOffset;

    // debugging avancé
    print("Moving to: (%d, %d) à %ld\n", destination.x, destination.y, (int32_t)angle);

    // Deplacement sur l'axe des x
    Deplacement_Ligne(abs(destination.x - positionActuelle.x));
    Deplacement_Wait();
    delay(DELAY_DEPLACEMENT);


    // Rotation
    Deplacement_Virage(angle);
    delay(DELAY_DEPLACEMENT);

    // Deplacement sur l'axe des y
    Deplacement_Ligne(abs(destination.y - positionActuelle.y));
    Deplacement_Wait();
    delay(DELAY_DEPLACEMENT);
    // Deplacement_Ligne(distance);

    // Contre-rotation
    Deplacement_Virage(-angle);
    delay(DELAY_DEPLACEMENT);

    // Actualisation de la position et de l'angle
    positionActuelle.x = destination.x;
    positionActuelle.y = destination.y;
    // angleActuel += angle;

    print("Déplacement à x:%d, y:%d terminé", destination.x, destination.y);
}

void Coords_Move(int16_t x, int16_t y)
{
    // Appelle la vraie fonction 'Move' avec les points
    Coords_Move({x, y});
}

void Coords_Move(int destination)
{
    switch(destination)
    {
        case CIBLE_DEPART_A:
            Coords_Move(Depart_A);
            return;
        case CIBLE_DEPART_B:
            Coords_Move(Depart_B);
            return;
        case CIBLE_PASTILLE:
            Coords_Move(Pastille);
            return;
        case CIBLE_BALLE:
            Coords_Move(Balle);
            return;
        case CIBLE_JAUNE:
            Coords_Move(Jaune);
            return;
        case CIBLE_BLEUE:
            Coords_Move(Bleu);
            return;
        case CIBLE_ROUGE:
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

void Coords_MoveOffset(int16_t x, int16_t y)
{
    x += positionActuelle.x;
    y += positionActuelle.y;

    Coords_Move({x, y});
}

void Coords_AjusterOffsetAngle(int16_t angleAjustement)
{
    angleOffset += angleAjustement;
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
