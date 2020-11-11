/****************************************************************************/
/* Includes --------------------------------------------------------------- */
#include "pidMaths.h"


/****************************************************************************/
/* Définitions de fonctions ----------------------------------------------- */

float PID_Calculate(float objectif, float valeur, pidPacket pid, float deltaT)
{
    // Calcul de l'erreur par rapport à la valeur désirée
    float erreur = objectif - valeur;

    // Calcul et mise à jour de l'intégrale
    float integ = 0;
    if(pid.integ != NULL)
    {
        integ = *pid.integ += (erreur * deltaT);
    }

    // Calcul de la dérivée et mise à jour de la dernière erreur
    float deriv = 0;
    if(pid.integ != NULL)
    {
        deriv      = (erreur - *pid.deriv) / deltaT;
        *pid.deriv = erreur;
    }

    // Calcul du multiplicateur de vitesse
    return (pid.KP * erreur) + (pid.KI * integ) + (pid.KD * deriv);
}