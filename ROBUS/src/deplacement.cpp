/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "deplacement.h"

#include "../Timer/TimerOne.h"


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
#define TIMER_DELAY_MS 10       // Delai entre les mesures et ajustements

#define KP 0.05
#define KI 0.00005

#define MARGE_VALEUR     50     // Vérification de position en nombre de coches

#define COCHES_DANS_TOUR 3200
#define DIAMETRE_ROUE    (3 * 2.54)
#define DIAMETRE_TOUR    18.5


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
int32_t consigne  = 0;
float   integrale = 0.0;

bool fini = true;


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
int32_t CMtoCoche(int32_t valeurCM);

void  Deplacement_PID();
float Deplacement_PID_Calculate(int32_t valeurVoulue, int32_t valeurEncodeur);
bool  Deplacement_Check(int32_t valeurVoulue, int32_t valeurEncodeur);


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Deplacement_Init()
{
    Timer1.initialize(TIMER_DELAY_MS * 1000);
    Timer1.attachInterrupt(&Deplacement_PID);

    pinMode(LED_BUILTIN, OUTPUT);
}

bool Deplacement_Fini()
{
    return fini;
}

bool Deplacement_Ligne(int distanceCM)
{
    // Vérification que le mouvement actuel n'est pas fini
    if(Deplacement_Fini() == false)
    {
        return false;
    }

    // Réinitiatialisation de l'encodeur
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    // Mise à jour de la nouvelle consigne
    consigne = CMtoCoche(distanceCM);
    fini     = false;
}

void Deplacement_PID()
{
    if (fini == true)
    {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        return;
    }

    // Lecture de la valeur de l'encodeur gauche
    int32_t valeurEncodeur = ENCODER_Read(LEFT);

    // Vérifie si on a fini notre déplacement
    if (Deplacement_Check(consigne, valeurEncodeur) == true)
    {
        fini = true;
        return;
    }

    // Calcul du multiplicateur de vitesse de la roue gauche à l'aide de
    // l'algorithme de PID.
    float multiplicateur = Deplacement_PID_Calculate(consigne, valeurEncodeur);

    // Ajustement des vitesses des deux roues
    MOTOR_SetSpeed(LEFT, 0.5 * multiplicateur);
    MOTOR_SetSpeed(RIGHT, 0.5);
}

float Deplacement_PID_Calculate(int32_t valeurVoulue, int32_t valeurEncodeur)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    // Une valeur de `erreur` négative indique qu'on a trop déplacé
    // Une valeur de `erreur` positive indique qu'on a pas assez déplacé
    int32_t erreur = valeurVoulue - valeurEncodeur;

    // Calcul de l'intégrale
    // On multiplie l'erreur par dt (en s), et on l'ajoute au total
    integrale += erreur * (TIMER_DELAY_MS / 1000);

    // Calcul du multiplicateur de vitesse
    return 1 + (KP * erreur + KI * integrale);
}

bool Deplacement_Check(int32_t valeurVoulue, int32_t valeurEncodeur)
{
    if((valeurEncodeur >= valeurVoulue - MARGE_VALEUR)
       && (valeurEncodeur <= valeurVoulue + MARGE_VALEUR))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int32_t CMtoCoche(int32_t valeurCM)
{
    int32_t valeurCoche = (valeurCM / (DIAMETRE_ROUE * PI)) * COCHES_DANS_TOUR;
    return valeurCoche;
}
