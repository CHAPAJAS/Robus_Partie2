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
  Deplacement_Ligne(50);
}

void loop() {
  extern float commandeG;
  extern float commandeD;

  if (Deplacement_Fini() == false)
  {
    print("Commande: %ld\n", (int32_t)commandeG);
    delay(50);
  }
  else
  {
    print("\nDéplacement terminé à %d, cmdG: %ld, cmdD: %ld\n", millis(), commandeG, commandeD);
    while(true){}
  }
  
}