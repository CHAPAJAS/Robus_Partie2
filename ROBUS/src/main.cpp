/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "deplacement.h"


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup()
{
    BoardInit();

    // Initialisations
    Deplacement_Init();

    print("\n Début de programme : %d --------------------------------- \n", millis());

    // Conditions initiales
    Deplacement_Ligne(100);
}

void loop()
{
    extern float commandeG;
    extern float commandeD;
    extern float commandeVitesse;

    extern float   vitesseG;
    extern float   vitesseD;
    extern int32_t valEncodeurG;
    extern int32_t valEncodeurD;

    extern float multiplicateurG;
    extern float multiplicateurD;

    if(Deplacement_Fini() == false)
    {
        print("Commande: %ld \t %ld\n", (int32_t)commandeG, (int32_t)(commandeVitesse * 1000));
        print("Vitesse: %ld, %ld\n", (int32_t)(vitesseG * 1000), (int32_t)(vitesseD * 1000));
        print(
          "mult: %ld, %ld\n", (int32_t)(multiplicateurG * 1000), (int32_t)(multiplicateurD * 1000));
        print(
          "Encodeur: %ld, %ld, %ld\n", valEncodeurG, valEncodeurD, (valEncodeurG - valEncodeurD));
        print("-----\n");
        delay(50);
    }
    else
    {
        print("\nDéplacement terminé à %d, cmdG: %ld, cmdD: %ld\n", millis(), commandeG, commandeD);
        while(true)
        {
        }
    }
}