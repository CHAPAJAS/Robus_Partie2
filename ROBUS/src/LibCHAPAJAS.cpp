#include "LibCHAPAJAS.h"


/* 
 * @brief   Communication avec le port série
 *          Cette fonction prend des arguments comme un printf prendrait,
 *          mais les redirige vers le port série.
 * 
 * @param   format: Une string de formatting du style de printf, dans laquelle
 *                  sera insérée les autres informations qui suivent.
 * @param   ...: Une liste de paramètres dits 'variadiques', donc un nombre
 *               variable de paramètres. 
 *               Lus selon ce qui est inscrit dans la string de format
 * 
 * @note    Ne prend pas en charge des strings de plus de 64 charactères!
 * 
 * @example
 *      int variable = 12;
 *      print("La valeur de ma variable est %d.", variable);
 * 
 * Référence de format de printf:
 * http://www.cplusplus.com/reference/cstdio/printf/
 *
 * Les keywords `inline static` permettent ici d'avoir la définition de cette 
 * fonction dans un fichier "header" (.h) sans causer de problèmes d'inclusions.
 */
void print(const char* format, ...)
{
    // Allocation d'un buffer de 64 octets, initialisé avec des 0 partout
    // Le keyword `static` permet à cet espace mémoire de ne se faire 
    // allouer qu'une seule fois, puis jamais désalloué; le même espace
    // mémoire est donc réutilisé.
    // https://www.geeksforgeeks.org/static-keyword-cpp/
    static char buffer[128] = {0};
    
    // Voici des types bizarres spécifique au C, qui permettent de contenir les
    // paramètres variadiques de la fonction. 
    // va_list est un contenant pour ces types, et va_start l'initialise avec 
    // les valeurs passées en paramètres
    // http://www.cplusplus.com/reference/cstdarg/va_list/
    // http://www.cplusplus.com/reference/cstdarg/va_start/
    va_list args;
    va_start(args, format);

    // La fonction `sprintf` agit comme un `printf`, mais envoie le contenu de
    // son travail non pas dans la console (innaccessible sur Arduino), mais 
    // dans un buffer qui est son premier paramètre.
    // `vsprintf` est une version de `sprintf` qui prend en argument une va_list,
    // ce qui permet de lui réacheminer les arguments variadiques reçus.
    // http://www.cplusplus.com/reference/cstdio/vsprintf/
    // `vsnprintf` est une version plus safe de `vsprintf`, dans laquelle on
    // passe la taille maximale du tableau (elle est techniquement aussi rapide,
    // car `vsprintf` appelle `vsnprintf`, avec comme taille `INT_MAX`).
    int size = vsnprintf(buffer, sizeof(buffer), format, args);

    Serial.write((uint8_t*)buffer, size);
}

/* 
 * @brief   Communication avec le port série
 *          Cette fonction prend une instance de la classe String d'Arduino, et
 *          envoie sont contenu sur le port série.
 * 
 * @param   String: Une instance de la classe string d'Arduino.
 * 
 * @example
 *      String myString = "Hello World!";
 *      print(myString);
 */
void print(const String& string)
{
    // La classe String a son propre buffer de mémoire déjà alloué, on a donc
    // qu'à récupérer l'addresse de ce buffer à l'aide de la fonction `c_str`.
    Serial.print(string.c_str());
}
