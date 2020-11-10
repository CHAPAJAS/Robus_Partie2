/******************************************************************************/
/* Inclusions --------------------------------------------------------------- */
#include "../lib/LibRobus/src/LibRobus.h"

#include <stdio.h>



/******************************************************************************/
/* Macros ------------------------------------------------------------------- */
/*
 * @def     sizeof_array
 * @brief   Retourne le nombre de cases dans un tableau
 *
 * L'opérateur `sizeof` retourne normalement le nombre d'octets contenus dans
 * un tableau.
 * On peut par contre diviser cette valeur par la taille d'un seul élément du
 * tableau, ce qui peut être fait avec `sizeof(typeDeMonTableau)` ou simplement
 * en avec `sizeof(monTableau[0])`, qui est le premier élément du tableau.
 * Cette macro retourne donc le nombre d'octet du tableau divisé par la taille
 * en octets d'un élément du tableau, et donc le nombre de cases du tableau.
 *
 * @param   tableau: Le tableau dont on souhaite avoir le nombre de cases
 * @return  Nombre de cases dans le tableau
 *
 * @note    Fonctionne aussi avec des tableaux vides, car il cherche juste à
 *          obtenir la taille d'un élément, et il interprète librement la valeur
 *          obtenue à l'index 0 comme un élément du bon type.
 *          Le calcul est probablement entièrement fait à la compilation.
 *
 * @example
 *      float monTableau[8] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
 *      int a = sizeof(monTableau);       // a vaut 8 * 4 = 32
 *      int b = sizeof_array(monTableau); // b vaut 8
 *
 * @example
 *      // Peut être utilisé dans une boucle:
 *      for(int i = 0; i < sizeof_array(monTableau); i++)
 *      {
 *           print("%d", monTableau[i]);
 *      }
 *
 * Référence de l'opérateur `sizeof`:
 * https://en.cppreference.com/w/cpp/language/sizeof
 */
#define sizeof_array(tableau) (int)(sizeof(tableau) / sizeof(tableau[0]))


#define BIIIP() AX_BuzzerON(500, 2000)



/******************************************************************************/
/* Déclarations de fonctions ------------------------------------------------ */
// Fonction de communication avec le port série
void print(const char* format, ...);
void print(const String& string);