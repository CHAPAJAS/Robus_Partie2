#pragma once
/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"


/******************************************************************************/
/* Types -------------------------------------------------------------------- */
typedef struct
{
    int16_t x;
    int16_t y;
} point;


#define CIBLE_DEPART_A 0
#define CIBLE_DEPART_B 1
#define CIBLE_PASTILLE 2
#define CIBLE_BALLE    3
#define CIBLE_JAUNE    4
#define CIBLE_BLEUE    5
#define CIBLE_ROUGE    6


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void Coords_Init(int robus);    // 0 pour A, 1 pour B

void  Coords_Move(point destination);
void  Coords_Move(int16_t x, int16_t y);
void  Coords_Move(int destination);
void  Coords_MoveOffset(int16_t x, int16_t y);

void Coords_AjusterOffsetAngle(int16_t angleAjustement);

point Coords_PositionActuelle();

float GetDistanceToPoint(point A, point B);
float GetAngleToPoint(point A, point B);