#include <stdio.h>
#include <stdlib.h>

#define MAX_OPERATIONS 100

int precedences[MAX_OPERATIONS][MAX_OPERATIONS];
int exclusions[MAX_OPERATIONS][MAX_OPERATIONS];
int nbOperations = 0;

int operationsTriees[MAX_OPERATIONS];
int nbPredecesseurs[MAX_OPERATIONS];
float temps[MAX_OPERATIONS];

void initialiserMatrices() {
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        for (int j = 0; j < MAX_OPERATIONS; ++j) {
            precedences[i][j] = 0;
            exclusions[i][j] = 0;
        }
        nbPredecesseurs[i] = 0;
    }
}

void lireContraintePrecedence() {
    FILE *fichier = fopen("precedences.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier de précédences.\n");
        return;
    }

    int operationA, operationB;
    while (fscanf(fichier, "%d %d", &operationA, &operationB) == 2) {
        precedences[operationA - 1][operationB - 1] = 1;
        nbPredecesseurs[operationB - 1]++;
        nbOperations = (operationA > nbOperations) ? operationA : nbOperations;
        nbOperations = (operationB > nbOperations) ? operationB : nbOperations;
    }

    fclose(fichier);
}

void lireContrainteExclusion() {
    FILE *fichier = fopen("exclusion.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier d'exclusions.\n");
        return;
    }

    int operationA, operationB;
    while (fscanf(fichier, "%d %d", &operationA, &operationB) == 2) {
        exclusions[operationA - 1][operationB - 1] = 1;
        exclusions[operationB - 1][operationA - 1] = 1;
    }

    fclose(fichier);
}

void lireTempsOperations() {
    FILE *fichier = fopen("operations.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier des temps d'opérations.\n");
        return;
    }

    int operation;
    float tempsOperation;
    while (fscanf(fichier, "%d %f", &operation, &tempsOperation) == 2) {
        temps[operation - 1] = tempsOperation;
    }

    fclose(fichier);
}

void afficherGraphePrecedenceConsole() {
    printf("Graphe de précédence :\n");
    for (int i = 0; i < nbOperations; ++i) {
        printf("Opération %d : ", i + 1);
        for (int j = 0; j < nbOperations; ++j) {
            if (precedences[i][j]) {
                printf("%d -> %d   ", i + 1, j + 1);
            }
        }
        printf("\n");
    }
}

void afficherContrainteExclusionConsole() {
    printf("Contraintes d'exclusion :\n");
    for (int i = 0; i < nbOperations; ++i) {
        for (int j = 0; j < nbOperations; ++j) {
            if (exclusions[i][j]) {
                printf("Opération %d et Opération %d ne peuvent pas être dans la même station.\n", i + 1, j + 1);
            }
        }
    }
}

void trierOperations() {
    // Tri à bulles pour trier les opérations en fonction du nombre de prédécesseurs
    for (int i = 0; i < nbOperations - 1; ++i) {
        for (int j = 0; j < nbOperations - i - 1; ++j) {
            if (nbPredecesseurs[j] > nbPredecesseurs[j + 1]) {
                // Échanger les positions des opérations et de leurs nombres de prédécesseurs
                int temp = nbPredecesseurs[j];
                nbPredecesseurs[j] = nbPredecesseurs[j + 1];
                nbPredecesseurs[j + 1] = temp;

                temp = operationsTriees[j];
                operationsTriees[j] = operationsTriees[j + 1];
                operationsTriees[j + 1] = temp;
            }
        }
    }
}

void triTopologique() {
    int degres[MAX_OPERATIONS] = {0};
    int file[MAX_OPERATIONS];
    int avant = 0, arriere = -1;

    // Initialiser la file avec les opérations sans prédécesseurs
    for (int i = 0; i < nbOperations; ++i) {
        int actuelle = operationsTriees[i];
        for (int j = 0; j < nbOperations; ++j) {
            if (precedences[j][actuelle]) {
                degres[actuelle]++;
            }
        }
        if (degres[actuelle] == 0) {
            file[++arriere] = actuelle;
        }
    }

    // Effectuer le tri topologique
    while (avant <= arriere) {
        int actuelle = file[avant++];
        operationsTriees[avant - 1] = actuelle;

        for (int i = 0; i < nbOperations; ++i) {
            if (precedences[actuelle][i]) {
                if (--degres[i] == 0) {
                    file[++arriere] = i;
                }
            }
        }
    }
}


float calculerSommeTemps() {
    float somme = 0;
    for (int i = 0; i < nbOperations; ++i) {
        somme += temps[operationsTriees[i]];
    }
    return somme;
}

int estExclu(int operationA, int operationB) {
    return exclusions[operationA][operationB] || exclusions[operationB][operationA];
}

void repartirEnStationsAvecTempsNonNul(float tempsCycle) {
    float tempsStation = 0;
    int station = 1;

    printf("\nRépartition en stations :\n");
    for (int i = 0; i < nbOperations; ++i) {
        int currentOperation = operationsTriees[i];

        // Vérifier les contraintes d'exclusion
        int exclu = 0;
        for (int j = 0; j < i; ++j) {
            int prevOperation = operationsTriees[j];
            if (estExclu(currentOperation, prevOperation)) {
                exclu = 1;
                break;
            }
        }

        // Vérifier si le temps de l'opération est non nul
        if (!exclu && temps[currentOperation] > 0 && tempsStation + temps[currentOperation] <= tempsCycle) {
            tempsStation += temps[currentOperation];
            printf("Station %d : Operation %d (%.2f)\n", station, currentOperation + 1, temps[currentOperation]);
        } else {
            if (temps[currentOperation] > 0) {
                tempsStation = temps[currentOperation];
                printf("\nStation %d : Operation %d (%.2f)\n", ++station, currentOperation + 1, temps[currentOperation]);
            }
        }
    }
}

int main() {
    initialiserMatrices();
    lireContraintePrecedence();
    lireContrainteExclusion();
    afficherGraphePrecedenceConsole();
    afficherContrainteExclusionConsole();

    // Initialiser les opérations triées
    for (int i = 0; i < nbOperations; ++i) {
        operationsTriees[i] = i;
    }

    // Trier les opérations en fonction du nombre de prédécesseurs
    trierOperations();

    triTopologique();

    printf("\nOrdre trié des operations : ");
    for (int i = 0; i < nbOperations; ++i) {
        printf("%d ", operationsTriees[i] + 1);
    }
    printf("\n");

    lireTempsOperations();
    float sommeTemps = calculerSommeTemps();
    printf("Somme des temps : %.2f\n", sommeTemps);

    // Lire le temps de cycle depuis le fichier temps_cycle.txt (à ajuster selon le format du fichier)
    FILE *fichierCycle = fopen("temps_cycle.txt", "r");
    if (fichierCycle == NULL) {
        printf("Impossible d'ouvrir le fichier temps_cycle.txt.\n");
        return 1;
    }

    float tempsCycle;
    if (fscanf(fichierCycle, "%f", &tempsCycle) != 1) {
        printf("Erreur de lecture du temps de cycle.\n");
        fclose(fichierCycle);
        return 1;
    }

    fclose(fichierCycle);

    printf("\nTemps de cycle : %.2f\n", tempsCycle);

    // Répartir les opérations en stations en respectant le temps de cycle et les contraintes d'exclusion
    repartirEnStationsAvecTempsNonNul(tempsCycle);

    return 0;
}
