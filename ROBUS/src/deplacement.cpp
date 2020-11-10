/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "deplacement.h"

#include "../Timer/TimerOne.h"


/******************************************************************************/
/* Types -------------------------------------------------------------------- */
struct pidPacket
{
    float  KP;
    float  KI;
    float  KD;
    float* integ;
    float* deriv;
};


/******************************************************************************/
/* Constantes --------------------------------------------------------------- */
#define NEW_PID    // Protect this line at all costs

#define ENCODEUR_GAUCHE_360_A (int32_t)8169
#define ENCODEUR_DROIT_360_A  (int32_t)7667
#define ENCODEUR_GAUCHE_360_B (int32_t)7700
#define ENCODEUR_DROIT_360_B  (int32_t)7840

#define ANGULOD_A 0.98
#define ANGULOG_A 0.955
#define ANGULOD_B 0.961
#define ANGULOG_B 0.95

#define TIMER_DELAY_MS 50    // Delai entre les mesures et ajustements
#define DELTA_T        ((float)TIMER_DELAY_MS / 1000.0)

#define KP_POSITION 0.005f
#define KI_POSITION 0.00005f

#ifdef NEW_PID
#define KP_VITESSE_OBJECTIF 0.001f
#define KI_VITESSE_OBJECTIF 0.0000125f
#define KD_VITESSE_OBJECTIF 0.0002f

#define KP_VITESSE_CONSTANCE 0.288f
#define KI_VITESSE_CONSTANCE 0.000125f
#define KD_VITESSE_CONSTANCE 0.000125f
#else
#define KP_VITESSE_G 0.005f
#define KI_VITESSE_G 0.00005f
#define KD_VITESSE_G 0.02f
#define KP_VITESSE_D 0.005f
#define KI_VITESSE_D 0.00005f
#define KD_VITESSE_D 0.02f
#endif

#define MARGE_VALEUR 50    // Vérification de position en nombre de coches

#define COCHES_DANS_TOUR 3200
#define DIAMETRE_ROUE    (3 * 2.54)
#define DIAMETRE_TOUR    18.5

#define TEMPS_POUR_METRE 3000     // ms pour traverser 1m à 0.5
#define COCHES_PAR_MS    4.456    // Coches par ms

#define PUISSANCE_DEFAULT 0.5

#define DELAY_VIRAGE 1000


/******************************************************************************/
/* Variables ---------------------------------------------------------------- */
int32_t tempsRequis      = 0;
int32_t consigneInitiale = 0;

float speedModifier = 1;    // Multiplie la vitesse

float commandeG          = 0;
float commandeD          = 0;
float integralePositionG = 0.0;
float integralePositionD = 0.0;

float commandeVitesse = COCHES_PAR_MS;

#ifdef NEW_PID
float integraleG      = 0;
float integraleD      = 0;
float derniereErreurG = 0;
float derniereErreurD = 0;
#else
float           erreurVitessePrev = 0;
float           integraleVitesseG = 0;
float           integraleVitesseD = 0;
#endif

bool fini = true;

int32_t constanteEncodeurG = 0;
int32_t constanteEncodeurD = 0;
float   angulo_g           = 0;
float   angulo_d           = 0;


#ifdef NEW_PID
const pidPacket PID_SPEED_OBJECTIF  = {KP_VITESSE_OBJECTIF,
                                      KI_VITESSE_OBJECTIF,
                                      KD_VITESSE_OBJECTIF,
                                      &integraleG,
                                      &derniereErreurG};
const pidPacket PID_SPEED_CONSTANCE = {KP_VITESSE_CONSTANCE,
                                       KI_VITESSE_CONSTANCE,
                                       KD_VITESSE_CONSTANCE,
                                       &integraleD,
                                       &derniereErreurD};
#else
const pidPacket PID_SPEED_G       = {KP_VITESSE_G, KI_VITESSE_G, KD_VITESSE_G, &integraleVitesseG};
const pidPacket PID_SPEED_D       = {KP_VITESSE_D, KI_VITESSE_D, KD_VITESSE_D, &integraleVitesseD};
#endif
const pidPacket PID_POSITION_G = {KP_POSITION, KI_POSITION, 0, &integralePositionG, nullptr};
const pidPacket PID_POSITION_D = {KP_POSITION, KI_POSITION, 0, &integralePositionD, nullptr};


/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
int32_t CMtoCoche(int32_t valeurCM);

void  PID();
float cheeky_pid(float objectif, float valeur, pidPacket pid);

void Deplacement_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD);
void Vitesse_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD);

float Deplacement_PID_Calculate(uint32_t valeur, float cmd, pidPacket pid);
float Vitesse_PID_Calculate(float vitesseActuelle, float cmd, pidPacket pid);

bool Deplacement_Check(int32_t valeurVoulue, int32_t valeurEncodeur);

float Accel(int32_t distanceTotale, int32_t distanceRestante);

void Virage_Gauche(int angle);
void Virage_Droit(int angle);


/******************************************************************************/
/* Définitions de fonctions ------------------------------------------------- */
void Deplacement_Init(int robus)
{
    if(robus == 0 || robus == 'A')
    {
        constanteEncodeurG = ENCODEUR_GAUCHE_360_A;
        constanteEncodeurD = ENCODEUR_DROIT_360_A;
        angulo_d           = ANGULOD_A;
        angulo_g           = ANGULOG_A;
    }
    else if(robus == 1 | robus == 'B')
    {
        constanteEncodeurG = ENCODEUR_GAUCHE_360_A;
        constanteEncodeurD = ENCODEUR_DROIT_360_A;
        angulo_d           = ANGULOD_B;
        angulo_g           = ANGULOG_B;
    }
    else
    {
        BIIIP();
    }

    // Réinitiatialisation de l'encodeur
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    print("\n%ld", (int32_t)(KP_VITESSE_CONSTANCE * 1000));
    print("\n%ld", (int32_t)(KI_VITESSE_CONSTANCE * 1000));
    print("\n%ld\n", (int32_t)(KD_VITESSE_CONSTANCE * 1000));
    Timer1.initialize((unsigned long)TIMER_DELAY_MS * 1000L);
    Timer1.attachInterrupt(&PID);
}

bool Deplacement_Fini()
{
    return fini;
}

void Deplacement_Stop()
{
#ifdef NEW_PID
    integraleG      = 0;
    integraleD      = 0;
    derniereErreurG = 0;
    derniereErreurD = 0;
#else
    integraleVitesseG     = 0;
    integraleVitesseD     = 0;
#endif
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
float multiplicateurG;
float multiplicateurD;
void  Vitesse_PID(int32_t valeurEncodeurG, int32_t valeurEncodeurD)
{
    // Calcul de la vitesse actuelle du ROBUS   (position2 - position1) / dt
    vitesseG = (float)valeurEncodeurG / TIMER_DELAY_MS;
    vitesseD = (float)valeurEncodeurD / TIMER_DELAY_MS;

    // Calcul de la vitesse théorique
    commandeVitesse = COCHES_PAR_MS;    // Accel(consigneInitiale, commandeG);

// Calcul du multiplicateur de vitesse
#ifdef NEW_PID
    multiplicateurG = cheeky_pid(commandeVitesse, vitesseG, PID_SPEED_OBJECTIF);
    multiplicateurD = cheeky_pid(vitesseG, vitesseD, PID_SPEED_CONSTANCE);
#else
    float multiplicateurG = Vitesse_PID_Calculate(vitesseG, commandeVitesse, PID_SPEED_G);
    float multiplicateurD = Vitesse_PID_Calculate(vitesseD, commandeVitesse, PID_SPEED_D);
#endif

// Ajustement des vitesses des deux roues
#ifdef NEW_PID
    MOTOR_SetSpeed(LEFT, PUISSANCE_DEFAULT + multiplicateurG);
    MOTOR_SetSpeed(RIGHT, PUISSANCE_DEFAULT + multiplicateurD);
#else
    MOTOR_SetSpeed(LEFT, PUISSANCE_DEFAULT * multiplicateurG);
    MOTOR_SetSpeed(RIGHT, PUISSANCE_DEFAULT * multiplicateurD);
#endif
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

#ifndef NEW_PID
float Vitesse_PID_Calculate(float vitesseActuelle, float cmd, pidPacket pid)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    // Une valeur de `erreur` négative indique qu'on va trop vite
    // Une valeur de `erreur` positive indique qu'on va pas assez vite
    float erreur = cmd - vitesseActuelle * (1 + pid.KP);

    // Calcul de la dérivée
    float deriv = (erreur - erreurVitessePrev) * (pid.KD / DELTA_T);

    // Calcul de l'intégrale
    // On multiplie l'erreur par dt (en s), et on l'ajoute au total
    *pid.integ += erreur * ((TIMER_DELAY_MS / 1000) * pid.KI);

    // Mise à jour de l'ancienne erreur
    erreurVitessePrev = erreur;

    // Calcul du multiplicateur de vitesse
    return 1 + *pid.integ + deriv + erreur;
}
#endif

#ifdef NEW_PID
float cheeky_pid(float objectif, float valeur, pidPacket pid)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    float erreur = objectif - valeur;

    // Calcul et mise à jour de l'intégrale
    float integ = *pid.integ += (erreur * DELTA_T);

    // Calcul de la dérivée et mise à jour de la dernière erreur
    float deriv = (erreur - *pid.deriv) / DELTA_T;
    *pid.deriv  = erreur;

    // Calcul du multiplicateur de vitesse
    return (pid.KP * erreur) + (pid.KI * integ) + (pid.KD * deriv);
}
#endif

bool Deplacement_Check(int32_t valeurVoulue, int32_t valeurEncodeur)
{
    if(valeurEncodeur >= valeurVoulue - MARGE_VALEUR)
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
     * L-----------------------------> x
     */

    // Sous 10% de la distance ou au-dessus de 90%
    if((distance < 0.1 * distanceTotale) || (distance > 0.9 * distanceTotale))
    {
        return COCHES_PAR_MS * 0.2;
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

void Deplacement_Virage(int angle)
{
#if VIRAGES == false
    return;
#endif

    for(; abs(angle) > 100; angle = (angle >= 0) ? angle - 90 : angle + 90)
    {
        Deplacement_Virage((angle % 90 == 0) ? ((angle >= 0) ? 90 : -90) : angle % 90);
        delay(DELAY_VIRAGE);
    }

    print("Virage de %d°\n", angle);
    if(angle < 0)
    {
        angle = angle * -1;
        Virage_Gauche(angle);
    }
    else
    {
        Virage_Droit(angle);
    }
}

void Virage_Droit(int angle)
{
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    long valeurEncodeurGauche = ENCODER_Read(0);

    while(valeurEncodeurGauche <= constanteEncodeurG / (360 / (angle * angulo_d)))
    {
        MOTOR_SetSpeed(0, 0.3);
        MOTOR_SetSpeed(1, -0.3);
        valeurEncodeurGauche = ENCODER_Read(0);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}


void Virage_Gauche(int angle)
{
    ENCODER_ReadReset(0);
    ENCODER_ReadReset(1);

    long valeurEncodeurDroit = ENCODER_Read(1);

    while(valeurEncodeurDroit <= constanteEncodeurD / (360 / (angle * angulo_g)))
    {
        MOTOR_SetSpeed(0, -0.3);
        MOTOR_SetSpeed(1, 0.3);
        valeurEncodeurDroit = ENCODER_Read(1);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

