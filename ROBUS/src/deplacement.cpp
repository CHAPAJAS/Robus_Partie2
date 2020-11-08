/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "deplacement.h"

#include "../Timer/TimerOne.h"


/******************************************************************************/
/* Types -------------------------------------------------------------------- */
struct pidPacket
{
    float KP;
    float KI;
    float KD;
    float* integ;
};


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
#define TIMER_DELAY_MS 50        // Delai entre les mesures et ajustements

#define KP_POSITION 0.005f
#define KI_POSITION 0.00005f

#define KP_VITESSE_G 0.005f
#define KI_VITESSE_G 0.00005f
#define KD_VITESSE_G 0.02f
#define KP_VITESSE_D 0.005f
#define KI_VITESSE_D 0.00005f
#define KD_VITESSE_D 0.02f

#define MARGE_VALEUR 50        // Vérification de position en nombre de coches

#define COCHES_DANS_TOUR 3200
#define DIAMETRE_ROUE    (3 * 2.54)
#define DIAMETRE_TOUR    18.5

#define TEMPS_POUR_METRE 3000         // ms pour traverser 1m à 0.5
#define COCHES_PAR_MS    4.456        // Coches par ms

#define PUISSANCE_DEFAULT 0.1


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
int32_t tempsRequis      = 0;
int32_t consigneInitiale = 0;

float speedModifier = 1;        // Multiplies the speed

float commandeG          = 0;
float commandeD          = 0;
float integralePositionG = 0.0;
float integralePositionD = 0.0;

float erreurVitessePrev = 0;
float commandeVitesse  = COCHES_PAR_MS;
float integraleVitesseG = 0;
float integraleVitesseD = 0;

bool fini = true;

const pidPacket PID_SPEED_G = {KP_VITESSE_G, KI_VITESSE_G, KD_VITESSE_G, &integraleVitesseG};
const pidPacket PID_SPEED_D = {KP_VITESSE_D, KI_VITESSE_D, KD_VITESSE_D, &integraleVitesseD};
const pidPacket PID_POSITION_G = {KP_POSITION, KI_POSITION, 0, &integralePositionG};
const pidPacket PID_POSITION_D = {KP_POSITION, KI_POSITION, 0, &integralePositionD};


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
int32_t CMtoCoche(int32_t valeurCM);

void  PID();

void  Deplacement_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD);
void  Vitesse_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD);

float Deplacement_PID_Calculate(uint32_t valeur, float cmd, pidPacket pid);
float Vitesse_PID_Calculate(float vitesseActuelle, float cmd, pidPacket pid);

bool  Deplacement_Check(int32_t valeurVoulue, int32_t valeurEncodeur);

float Accel(int32_t distanceTotale, int32_t distanceRestante);


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Deplacement_Init()
{
    // Réinitiatialisation de l'encodeur
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    Timer1.initialize((unsigned long)TIMER_DELAY_MS * 1000L);
    Timer1.attachInterrupt(&PID);
}

bool Deplacement_Fini()
{
    return fini;
}

void Deplacement_Stop()
{
    integraleVitesseG   = 0;
    integraleVitesseD   = 0;
    commandeVitesse    = COCHES_PAR_MS;
    integralePositionG = 0.0;
    integralePositionD = 0.0;
    commandeG          = 0;
    commandeD          = 0;
    fini               = true;
}

/**
 * @brief   Commence un déplacement non-bloquant.
 * 
 * @param   distanceCM: La distance à faire en centimètres
 * @param   vitesseModifier: Un modificateur de la vitesse.
 *                           À 0, 50% de la vitesse
 *                           À 1, 200% de la vitesse
 * 
 * @retval  true:  Si le déplacement peut commencer
 * @retval  false: S'il y a déjà un déplacement en cours et qu'on ne peut pas
 *                 en commencer un nouveau.
 */
bool Deplacement_Ligne(int distanceCM, float vitesseModifier)
{
    // Vérification que le mouvement actuel n'est pas fini
    if(Deplacement_Fini() == false)
    {
        return false;
    }

    // Multiplicateur de vitesse
    speedModifier = ((3.0 / 2.0) * vitesseModifier) + 0.5;

    // Réinitiatialisation de l'encodeur
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    // Mise à jour de la nouvelle consigne
    consigneInitiale = CMtoCoche(distanceCM);
    commandeG        = consigneInitiale;
    commandeD        = consigneInitiale;
    fini             = false;

    // Calcul du temps total requis pour le déplacement demandé.
    tempsRequis = consigneInitiale / COCHES_PAR_MS;

    print("Départ de déplacement de %d cm, G: %ld, D: %ld\n",
          distanceCM,
          (int32_t)commandeG,
          (int32_t)commandeD);

    return true;
}

int32_t valEncodeurG;
int32_t valEncodeurD;

void PID()
{
    // Arrête tout si on a fini le déplacement
    if(fini == true)
    {
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, 0);
        return;
    }

    // Lecture de la valeur de l'encodeur
    int32_t valeurEncodeurG = ENCODER_ReadReset(LEFT);
    int32_t valeurEncodeurD = ENCODER_ReadReset(RIGHT);

    valEncodeurG = valeurEncodeurG;
    valEncodeurD = valeurEncodeurD;

    Deplacement_PID(valeurEncodeurG, valeurEncodeurD);
    Vitesse_PID(valeurEncodeurG, valeurEncodeurD);

    // Actualisation du temps
    tempsRequis -= TIMER_DELAY_MS;
}

void Deplacement_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD)
{
    // Vérifie si on a fini notre déplacement
    if(Deplacement_Check(commandeG, valeurEncodeurG) == true
       || Deplacement_Check(commandeD, valeurEncodeurD))
    {
        Deplacement_Stop();
        return;
    }

    commandeG = Deplacement_PID_Calculate(valeurEncodeurG, commandeG, PID_POSITION_G);
    commandeD = Deplacement_PID_Calculate(valeurEncodeurD, commandeD, PID_POSITION_D);
}

float vitesseG;
float vitesseD;
void Vitesse_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD)
{
    // Calcul de la vitesse actuelle du ROBUS   (position2 - position1) / dt
    vitesseG = (float)valeurEncodeurG / TIMER_DELAY_MS;
    vitesseD = (float)valeurEncodeurD / TIMER_DELAY_MS;

    // Calcul de la vitesse théorique
    commandeVitesse = Accel(consigneInitiale, commandeG);

    // Calcul du multiplicateur de vitesse
    float multiplicateurG = Vitesse_PID_Calculate(vitesseG, commandeVitesse, PID_SPEED_G);
    float multiplicateurD = Vitesse_PID_Calculate(vitesseD, commandeVitesse, PID_SPEED_D);

    // Ajustement des vitesses des deux roues
    MOTOR_SetSpeed(LEFT, PUISSANCE_DEFAULT * multiplicateurG * speedModifier);
    MOTOR_SetSpeed(RIGHT, PUISSANCE_DEFAULT * multiplicateurD * speedModifier);
}

float Deplacement_PID_Calculate(uint32_t valeur, float cmd, pidPacket PID)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    // Une valeur de `erreur` négative indique qu'on a trop déplacé
    // Une valeur de `erreur` positive indique qu'on a pas assez déplacé
    float erreur = (cmd - valeur) * (1 + PID.KP);

    // Calcul de l'intégrale
    // On multiplie l'erreur par dt (en s), et on l'ajoute au total
    //*integ += erreur * (TIMER_DELAY_MS / 1000) * KI;

    // Calcul du multiplicateur de vitesse
    return *PID.integ + erreur;
}

float Vitesse_PID_Calculate(float vitesseActuelle, float cmd, pidPacket pid)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    // Une valeur de `erreur` négative indique qu'on va trop vite
    // Une valeur de `erreur` positive indique qu'on va pas assez vite
    float erreur = cmd - vitesseActuelle * (1 + pid.KP);

    // Calcul de la dérivée
    float deriv = (erreur - erreurVitessePrev) * (pid.KD / ((float)TIMER_DELAY_MS / 1000.0));

    // Calcul de l'intégrale
    // On multiplie l'erreur par dt (en s), et on l'ajoute au total
    *pid.integ += erreur * ((TIMER_DELAY_MS / 1000) * pid.KI);

    // Mise à jour de l'ancienne erreur
    erreurVitessePrev = erreur;

    // Calcul du multiplicateur de vitesse
    return 1 + *pid.integ + deriv + erreur;
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
    int32_t valeurCoche = ((float)valeurCM / (DIAMETRE_ROUE * PI)) * COCHES_DANS_TOUR;
    return valeurCoche;
}


float Accel(int32_t distanceTotale, int32_t distanceRestante)
{
    int32_t distance = distanceTotale - distanceRestante;

    /*
     * ^ P   _________________
     * |    /                 \ 
     * | __/                   \__
     * |                        
     * L-----------------------------> x . <
     * 
     */

    // Sous 10% de la distance ou au-dessus de 90%
    if((distance < 0.1 * distanceTotale) || (distance > 0.9 * distanceTotale))
    {
        return COCHES_PAR_MS * 0.3;
    }
    // Sous 25% de la distance
    if(distance < 0.25 * distanceTotale)
    {
        // Les multiplications par 128 s'annulent, c'est un comportement désiré.
        int32_t multiplicateur = ((distance * 128) / distanceTotale) * 2.8;
        return (COCHES_PAR_MS / 128) * multiplicateur;
    }
    // Au delà de 75% de la distance
    if(distance > 0.75 * distanceTotale)
    {
        int32_t multiplicateur = (((distance * 128) / distanceTotale) * -2.8) + (2.8 * 128);
        return (COCHES_PAR_MS / 128) * multiplicateur;
    }

    // Reste des valeurs
    return COCHES_PAR_MS * 0.7;
}