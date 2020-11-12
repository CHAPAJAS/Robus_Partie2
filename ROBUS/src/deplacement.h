/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
void Deplacement_Init(int robus);

bool Deplacement_Ligne(int distanceCM);
void Deplacement_Stop();
void Deplacement_Continue();


void Deplacement_Virage(int angle);

bool Deplacement_Fini();
void Deplacement_Wait();

void Deplacement_Debug();