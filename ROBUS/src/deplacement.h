/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void Deplacement_Init(int robus);

bool Deplacement_Ligne(int distanceCM, float vitesseModifier = 0.5);
void Deplacement_Stop();

void Deplacement_Virage(int angle);

bool Deplacement_Fini();
