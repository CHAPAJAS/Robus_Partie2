/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "LibCHAPAJAS.h"
#include "coords.h"
#include "deplacement.h"


/******************************************************************************/
/* Defines ------------------------------------------------------------------ */
#define ROBUS 'A'


/******************************************************************************/
/* main --------------------------------------------------------------------- */
void setup()
{
    // Initialisations
    BoardInit();
    Coords_Init(ROBUS);
    Deplacement_Init();

    print("\n Début de programme : %d --------------------------------- \n", millis());

    // Conditions initiales
    Deplacement_Ligne(100);
}

void loop()
{
    Deplacement_Debug();
}