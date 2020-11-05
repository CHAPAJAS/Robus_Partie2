/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "deplacement.h"

#include "../Timer/TimerOne.h"


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
#define TIMER_DELAY_MS 30.0        // Delai entre les mesures et ajustements

#define KP 0.005
#define KI 0.00005

#define MARGE_VALEUR 50        // Vérification de position en nombre de coches

#define COCHES_DANS_TOUR 3200
#define DIAMETRE_ROUE    (3 * 2.54)
#define DIAMETRE_TOUR    18.5

#define TEMPS_POUR_METRE 3000         // ms pour traverser 1m à 0.5
#define COCHES_PAR_MS    4.456        // Coches par ms

#define PUISSANCE_DEFAULT 0.2


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
int32_t tempsRequis     = 0;

float commandeG          = 0;
float commandeD          = 0;
float integralePositionG = 0.0;
float integralePositionD = 0.0;

float commandeVitesse  = COCHES_PAR_MS;
float integraleVitesse = 0;

bool fini = true;


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
int32_t CMtoCoche(int32_t valeurCM);

void  Deplacement_PID();
float Deplacement_PID_Calculate(uint32_t valeur, float* cmd, float* integ);
float Vitesse_PID_Calculate(uint32_t vitesseActuelle, float cmd, float* integ);
bool  Deplacement_Check(int32_t valeurVoulue, int32_t valeurEncodeur);


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Deplacement_Init()
{
    // Réinitiatialisation de l'encodeur
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    Timer1.initialize(TIMER_DELAY_MS * 1000);
    Timer1.attachInterrupt(&Deplacement_PID);
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
    commandeG = CMtoCoche(distanceCM);
    commandeD = CMtoCoche(distanceCM);
    fini      = false;

    // Calcul du temps total requis pour le déplacement demandé.
    tempsRequis = CMtoCoche(distanceCM) / COCHES_PAR_MS;

    print("Départ de déplacement de %d cm, G: %ld, D: %ld\n", distanceCM, commandeG, commandeD);

    return true;
}

void Deplacement_PID()
{
    if(fini == true)
    {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        return;
    }

    // Lecture de la valeur de l'encodeur gauche
    int32_t valeurEncodeurG = ENCODER_ReadReset(LEFT);
    int32_t valeurEncodeurD = ENCODER_Read(RIGHT);

    // Vérifie si on a fini notre déplacement
    if(Deplacement_Check(commandeG, valeurEncodeurG) == true
       || Deplacement_Check(commandeD, valeurEncodeurD))
    {
        integralePositionG = 0.0;
        integralePositionD = 0.0;
        commandeG          = 0;
        commandeD          = 0;
        fini               = true;
        return;
    }

    // Calcul de la vitesse actuelle du ROBUS
    // (position2 - position1) / dt
    float vitesseG = (valeurEncodeurG * 1000.0) / TIMER_DELAY_MS;
    // float vitesseD = (valeurEncodeurD) / (TIMER_DELAY_MS / 1000);

    // float tempsTheorique = commandeG / COCHES_PAR_MS;
    // float tempsReal      = commandeG / vitesseG;

    // // Si le dt est positif, il faut accélérer, s'il est négatif, il faut décélerer
    // float deltaTemps = tempsTheorique - tempsReal;


    Deplacement_PID_Calculate(valeurEncodeurG, &commandeG, &integralePositionG);
    Deplacement_PID_Calculate(valeurEncodeurD, &commandeD, &integralePositionD);

    float multiplicateur = Deplacement_PID_Calculate(vitesseG, &commandeVitesse, &integraleVitesse);

    print("encodeur = %ld, vitesse = %d   commande = %d, multiplicateur = %d\n", valeurEncodeurG, vitesseG, commandeG, multiplicateur);

    // Ajustement des vitesses des deux roues
    MOTOR_SetSpeed(LEFT, PUISSANCE_DEFAULT);
    MOTOR_SetSpeed(RIGHT, PUISSANCE_DEFAULT);

    // Actualisation du temps
    tempsRequis -= TIMER_DELAY_MS;
}

float Deplacement_PID_Calculate(uint32_t valeur, float* cmd, float* integ)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    // Une valeur de `erreur` négative indique qu'on a trop déplacé
    // Une valeur de `erreur` positive indique qu'on a pas assez déplacé
    float erreur = (*cmd - valeur) * (1 + KP);

    // Calcul de l'intégrale
    // On multiplie l'erreur par dt (en s), et on l'ajoute au total
    //*integ += erreur * (TIMER_DELAY_MS / 1000) * KI;

    // Calcul du multiplicateur de vitesse
    *cmd = *integ + erreur;
    return 1.0 + *cmd;
}

float Vitesse_PID_Calculate(uint32_t vitesseActuelle, float cmd, float* integ)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    // Une valeur de `erreur` négative indique qu'on a trop déplacé
    // Une valeur de `erreur` positive indique qu'on a pas assez déplacé
    float erreur = cmd - vitesseActuelle * (1 + KP);

    // Calcul de l'intégrale
    // On multiplie l'erreur par dt (en s), et on l'ajoute au total
    *integ += erreur * (TIMER_DELAY_MS / 1000) * KI;

    // Calcul du multiplicateur de vitesse
    return 1 + *integ + erreur;
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
