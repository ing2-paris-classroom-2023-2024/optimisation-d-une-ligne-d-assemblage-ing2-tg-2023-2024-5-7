#include <stdio.h>
#include <stdlib.h>

#define MAX_OPERATIONS 35

int exclusions[MAX_OPERATIONS][MAX_OPERATIONS] = {0};

void lireExclusions(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de la lecture du fichier d'exclusions");
        exit(EXIT_FAILURE);
    }

    int op1, op2;
    while (fscanf(fichier, "%d %d", &op1, &op2) == 2) {
        exclusions[op1 - 1][op2 - 1] = 1;
        exclusions[op2 - 1][op1 - 1] = 1;  // Les exclusions sont bidirectionnelles
    }

    fclose(fichier);
}

int trouverNombreStations(int stations[MAX_OPERATIONS], int nbStations) {
    int maxStation = 0;
    for (int i = 0; i < MAX_OPERATIONS; i++) {
        if (stations[i] > maxStation) {
            maxStation = stations[i];
        }
    }
    return maxStation;
}

void colorierGraphe(int stations[MAX_OPERATIONS]) {
    for (int i = 0; i < MAX_OPERATIONS; i++) {
        for (int couleur = 1; ; couleur++) {
            int valide = 1;
            for (int j = 0; j < MAX_OPERATIONS; j++) {
                if (exclusions[i][j] && stations[j] == couleur) {
                    valide = 0;
                    break;
                }
            }
            if (valide) {
                stations[i] = couleur;
                break;
            }
        }
    }
}

int main() {
    const char *nomFichierExclusions = "exclusions.txt";
    lireExclusions(nomFichierExclusions);

    int stations[MAX_OPERATIONS] = {0};

    colorierGraphe(stations);

    int nbStations = trouverNombreStations(stations, MAX_OPERATIONS);

    printf("Repartition des operations par station (nombre de stations = %d) :\n", nbStations);
    for (int i = 0; i < nbStations; i++) {
        printf("Station %d :", i + 1);
        for (int j = 0; j < MAX_OPERATIONS; j++) {
            if (stations[j] == i + 1) {
                printf(" %d", j + 1);
            }
        }
        printf("\n");
    }

    return 0;
}