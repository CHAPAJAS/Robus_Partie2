/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup() {
  BoardInit();
  
  // Initialisations
  Deplacement_Init();

  print("\n Début de programme : %d --------------------------------- \n", millis());

  // Conditions initiales
  Deplacement_Ligne(100);
}

void loop() {
  extern int32_t commandeG;
  extern int32_t commandeD;

  if (Deplacement_Fini() == false)
  {

    
  }
  else
  {
    print("Déplacement terminé à %d, cmdG: %ld, cmdD: %ld", millis(), commandeG, commandeD);
    while(true){}
  }
  
}