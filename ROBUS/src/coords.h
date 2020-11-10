/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"


/******************************************************************************/
/* Types -------------------------------------------------------------------- */
typedef struct
{
    int16_t x;
    int16_t y;
}point;

typedef enum
{
    Cible_DepartA,
    Cible_DepartB,
    Cible_Pastille,
    Cible_Balle,
    Cible_Jaune,
    Cible_Bleue,
    Cible_Rouge
}cible;


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void Coords_Init(int robus);    // 0 pour A, 1 pour B

void Coords_Move(point destination);
void Coords_Move(int32_t x, int32_t y);
void Coords_Move(cible destination);
point Coords_PositionActuelle();


float GetDistanceToPoint(point A, point B);
float GetAngleToPoint(point A, point B);