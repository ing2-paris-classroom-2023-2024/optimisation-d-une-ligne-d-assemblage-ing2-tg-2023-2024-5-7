#include <stdio.h>
#include <stdlib.h>

#define MAX_OPERATIONS 100

int precedences[MAX_OPERATIONS][MAX_OPERATIONS];
int nbOperations = 0;

int operationsTriees[MAX_OPERATIONS];
int nbPredecesseurs[MAX_OPERATIONS];
float temps[MAX_OPERATIONS];

void initialiserMatrice() {
    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        for (int j = 0; j < MAX_OPERATIONS; ++j) {
            precedences[i][j] = 0;
        }
        nbPredecesseurs[i] = 0;
    }
}

void lireContraintePrecedence() {
    FILE *fichier = fopen("precedences.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
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

void lireTempsOperations() {
    FILE *fichier = fopen("operations.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
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
    int degre[MAX_OPERATIONS] = {0};
    int queue[MAX_OPERATIONS];
    int front = 0, rear = -1;

    // Initialiser la file avec les opérations sans prédécesseurs
    for (int i = 0; i < nbOperations; ++i) {
        int current = operationsTriees[i];
        for (int j = 0; j < nbOperations; ++j) {
            if (precedences[j][current]) {
                degre[current]++;
            }
        }
        if (degre[current] == 0) {
            queue[++rear] = current;
        }
    }

    // Effectuer le tri topologique
    while (front <= rear) {
        int current = queue[front++];
        operationsTriees[front - 1] = current;

        for (int i = 0; i < nbOperations; ++i) {
            if (precedences[current][i]) {
                if (--degre[i] == 0) {
                    queue[++rear] = i;
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

void repartirEnStations(float tempsCycle) {
    float tempsStation = 0;
    int station = 1;

    printf("\nRépartition en stations :\n");
    for (int i = 0; i < nbOperations; ++i) {
        int currentOperation = operationsTriees[i];
        if (tempsStation + temps[currentOperation] <= tempsCycle) {
            tempsStation += temps[currentOperation];
            printf("Station %d : Opération %d (%.2f)\n", station, currentOperation + 1, temps[currentOperation]);
        } else {
            tempsStation = temps[currentOperation];
            printf("\nStation %d : Opération %d (%.2f)\n", ++station, currentOperation + 1, temps[currentOperation]);
        }
    }
}

int main() {
    initialiserMatrice();
    lireContraintePrecedence();
    afficherGraphePrecedenceConsole();

    // Initialiser les opérations triées
    for (int i = 0; i < nbOperations; ++i) {
        operationsTriees[i] = i;
    }

    // Trier les opérations en fonction du nombre de prédécesseurs
    trierOperations();

    triTopologique();

    printf("\nOrdre trié des opérations : ");
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

    // Répartir les opérations en stations en respectant le temps de cycle
    repartirEnStations(tempsCycle);

    return 0;
}
