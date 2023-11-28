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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
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

*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int op1;
    int op2;
} Edge;

typedef struct {
    int operation;
    int station;
} Node;

int calculerStationsMinimales(Edge *graph, int numEdges, int numTasks, int **stations, int *numStations) {
    Node *nodes = (Node *)malloc(numTasks * sizeof(Node));

    for (int i = 0; i < numTasks; ++i) {
        nodes[i].operation = i + 1;
        nodes[i].station = 0;
    }

    for (int i = 0; i < numEdges; ++i) {
        int op1 = graph[i].op1;
        int op2 = graph[i].op2;

        if (nodes[op1 - 1].station >= nodes[op2 - 1].station) {
            nodes[op2 - 1].station = nodes[op1 - 1].station + 1;
        }
    }

    *numStations = 0;
    for (int i = 0; i < numTasks; ++i) {
        if (nodes[i].station > *numStations) {
            *numStations = nodes[i].station;
        }
    }

    *stations = (int *)malloc((*numStations) * sizeof(int));

    for (int i = 0; i < *numStations; ++i) {
        printf("Station %d: ", i + 1);
        for (int j = 0; j < numTasks; ++j) {
            if (nodes[j].station == i + 1) {
                printf("%d ", nodes[j].operation);
            }
        }
        printf("\n");
    }

    free(nodes);

    return *numStations;
}

int main() {
    FILE *file = fopen("precedences.txt", "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    int numEdges = 0;
    int op1, op2;
    while (fscanf(file, "%d %d", &op1, &op2) == 2) {
        numEdges++;
    }

    Edge *graph = (Edge *)malloc(numEdges * sizeof(Edge));

    rewind(file);

    for (int i = 0; i < numEdges; ++i) {
        fscanf(file, "%d %d", &graph[i].op1, &graph[i].op2);
    }

    fclose(file);

    int numTasks = numEdges + 1;
    int *stations;
    int numStations = calculerStationsMinimales(graph, numEdges, numTasks, &stations, &numStations);

    printf("Le nombre minimal de stations nécessaires est : %d\n", numStations);

    free(graph);
    free(stations);

    return 0;
}*/


#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

// Structure pour représenter un nœud du graphe
typedef struct Node {
    int id;
    struct Node* next;
} Node;

// Fonction pour créer un nouveau nœud
Node* createNode(int id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

// Fonction pour ajouter une relation de précédence
void addEdge(Node* graph[], int start, int end) {
    Node* newNode = createNode(end);
    newNode->next = graph[start];
    graph[start] = newNode;
}

// Fonction pour afficher le graphe
void printGraph(Node* graph[], int numNodes) {
    for (int i = 1; i <= numNodes; i++) {
        printf("%d -> ", i);
        Node* current = graph[i];
        while (current != NULL) {
            printf("%d ", current->id);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    FILE *file = fopen("precedences.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    int numNodes = 0, numRelations = 0;
    int start, end;

    // Tableau de listes d'adjacence pour représenter le graphe
    Node* graph[MAX_NODES + 1] = {NULL};  // Notez l'ajout de "+ 1" pour correspondre à l'index du tableau

    // Lecture des relations de précédence depuis le fichier
    while (fscanf(file, "%d %d", &start, &end) == 2) {
        // Mise à jour du nombre de nœuds
        if (start > numNodes) {
            numNodes = start;
        }
        if (end > numNodes) {
            numNodes = end;
        }

        addEdge(graph, start, end);
        numRelations++;
    }

    fclose(file);

    // Affichage du graphe
    printf("Graphe de precedent :\n");
    printGraph(graph, numNodes);

    return 0;
}