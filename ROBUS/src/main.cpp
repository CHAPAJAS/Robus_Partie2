/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"
#include "coords.h"


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup() {
  BoardInit();
  
  Deplacement_Init();
  Deplacement_Ligne(50);
}

void loop() {
  // put your main code here, to run repeatedly:
}