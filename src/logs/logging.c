#include <stdio.h>
#include <stdarg.h>
#include "logging.h"

void logMessage(const char* format, ...) {
    // Tentative d'ouverture du fichier de log
    FILE* logFile = fopen("./src/logs/game.log", "a");  

    // Gestion des erreurs d'ouverture de fichier
    if (!logFile) {
        printf("Erreur: impossible d'ouvrir le fichier de log.\n");
        perror("Erreur lors de l'ouverture du fichier");  
        return;
    }

    // Ecriture dans le fichier de log
    va_list args;
    va_start(args, format);  
    vfprintf(logFile, format, args);  
    fprintf(logFile, "\n");
    va_end(args);
    fclose(logFile);  

    // Affichage en console
    va_list argsConsole;
    va_start(argsConsole, format);
    vprintf(format, argsConsole);
    printf("\n");
    va_end(argsConsole);
}
