///VERSION 1
/*#include <stdio.h>
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
}*/


///VERSIONS2
#include <stdio.h>
#include <stdlib.h>

#define MAX_OPERATIONS 100  // Augmenté la taille maximale des opérations
///#define MAX_STATIONS 10

int exclusions[MAX_OPERATIONS][MAX_OPERATIONS] = {0};

int nombreOperations = 0;  // Nouvelle variable pour stocker le nombre d'opérations

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
        // Mettre à jour le nombre d'opérations
        if (op1 > nombreOperations) {
            nombreOperations = op1;
        }
        if (op2 > nombreOperations) {
            nombreOperations = op2;
        }
    }

    fclose(fichier);
}

int trouverNombreStations(int stations[MAX_OPERATIONS], int nbStations) {
    int maxStation = 0;
    for (int i = 0; i < nombreOperations; i++) {
        if (stations[i] > maxStation) {
            maxStation = stations[i];
        }
    }
    return maxStation;
}

void colorierGraphe(int stations[MAX_OPERATIONS]) {
    for (int i = 0; i < nombreOperations; i++) {
        for (int couleur = 1; ; couleur++) {
            int valide = 1;
            for (int j = 0; j < nombreOperations; j++) {
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
        for (int j = 0; j < nombreOperations; j++) {
            if (stations[j] == i + 1) {
                printf(" %d", j + 1);
            }
        }
        printf("\n");
    }

    return 0;
}

///VERSION 3 DE EXCLUSIONS trop compliquée
/*

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

struct Graph {
    int V;
    int **adjMatrix;
};

struct Graph *createGraph(int V) {
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    graph->adjMatrix = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        graph->adjMatrix[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }

    return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
}

void printColors(int colors[], int V) {
    printf("Coloration du graphe : \n");
    for (int i = 0; i < V; i++) {
        printf("Sommet %d --> Couleur %d\n", i + 1, colors[i]);
    }
}

int isSafe(struct Graph *graph, int v, int c, int colors[]) {
    for (int i = 0; i < graph->V; i++) {
        if (graph->adjMatrix[v][i] && colors[i] == c) {
            return 0;
        }
    }
    return 1;
}

int graphColoringUtil(struct Graph *graph, int m, int v, int colors[], int exclusions[][2], int numExclusions) {
    if (v == graph->V) {
        return 1;
    }

    for (int c = 1; c <= m; c++) {
        int isSafeColor = 1;

        // Vérifier si la couleur est exclue pour le sommet actuel
        for (int i = 0; i < numExclusions; i++) {
            if ((v + 1 == exclusions[i][0] && colors[exclusions[i][1] - 1] == c) ||
                (v + 1 == exclusions[i][1] && colors[exclusions[i][0] - 1] == c)) {
                isSafeColor = 0;
                break;
            }
        }

        if (isSafe(graph, v, c, colors) && isSafeColor) {
            colors[v] = c;

            if (graphColoringUtil(graph, m, v + 1, colors, exclusions, numExclusions)) {
                return 1;
            }

            colors[v] = 0;
        }
    }

    return 0;
}

void graphColoring(struct Graph *graph, int m, int exclusions[][2], int numExclusions) {
    int *colors = (int *)malloc(graph->V * sizeof(int));
    for (int i = 0; i < graph->V; i++) {
        colors[i] = 0;
    }

    if (graphColoringUtil(graph, m, 0, colors, exclusions, numExclusions)) {
        printColors(colors, graph->V);
    } else {
        printf("Aucune coloration possible avec %d couleurs.\n", m);
    }

    free(colors);
}

int main() {
    FILE *file = fopen("exclusions.txt", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    int V = 0;
    int src, dest;

    while (fscanf(file, "%d %d", &src, &dest) == 2) {
        if (src > V) V = src;
        if (dest > V) V = dest;
    }

    fclose(file);

    struct Graph *graph = createGraph(V);

    file = fopen("exclusions.txt", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    while (fscanf(file, "%d %d", &src, &dest) == 2) {
        addEdge(graph, src - 1, dest - 1);
    }

    fclose(file);

    int numExclusions = 0;
    file = fopen("exclusions.txt", "r");
    while (fscanf(file, "%d %d", &src, &dest) == 2) {
        numExclusions++;
    }

    fclose(file);

    int exclusions[numExclusions][2];
    file = fopen("exclusions.txt", "r");
    for (int i = 0; i < numExclusions; i++) {
        fscanf(file, "%d %d", &exclusions[i][0], &exclusions[i][1]);
    }

    fclose(file);

    int m = V;
    graphColoring(graph, m, exclusions, numExclusions);

    for (int i = 0; i < V; i++) {
        free(graph->adjMatrix[i]);
    }
    free(graph->adjMatrix);
    free(graph);

    return 0;
}
 */