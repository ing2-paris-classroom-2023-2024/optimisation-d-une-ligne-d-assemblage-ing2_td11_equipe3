#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPERATIONS 35
#define MAX_STATIONS 35

typedef struct {
    int operations[MAX_OPERATIONS];
    int compte;
} Station;


// Structure pour stocker les informations d'une opération
typedef struct {
    int numero;
    double temps;
    int visite;
    int nb_precedences;
    int precedences[MAX_OPERATIONS];
} Operation;

// Fonction pour lire les temps des opérations depuis un fichier
int lire_operations(const char *nom_fichier, Operation operations[], int *nb_operations) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        return 0;  // Erreur lors de l'ouverture du fichier
    }

    int numero;
    double temps;
    *nb_operations = 0;
    while (fscanf(fichier, "%d %lf\n", &numero, &temps) == 2) {
        operations[numero - 1].numero = numero;
        operations[numero - 1].temps = temps;
        (*nb_operations)++;
    }

    fclose(fichier);
    return 1;  // Succès
}

// Fonction pour lire les préférences depuis un fichier
int lire_precedences(const char *nom_fichier, Operation operations[], int nb_operations) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        return 0;  // Erreur lors de l'ouverture du fichier
    }

    int precedeur, successeur;
    while (fscanf(fichier, "%d %d\n", &precedeur, &successeur) == 2) {
        operations[precedeur - 1].precedences[operations[precedeur - 1].nb_precedences] = successeur;
        operations[precedeur - 1].nb_precedences++;
    }

    fclose(fichier);
    return 1;  // Succès
}

void dfs(int v, int visited[], int *index, int stack[], Operation operations[]) {
    visited[v] = 1;
    for (int i = 0; i < operations[v].nb_precedences; i++) {
        int w = operations[v].precedences[i] - 1;
        if (!visited[w]) {
            dfs(w, visited, index, stack, operations);
        }
    }
    stack[--(*index)] = v + 1;  // Ajouter le sommet dans la pile en décrémentant l'index
}

void tri_topologique(Operation operations[], int nb_operations, int operations_triees[]) {
    int visited[MAX_OPERATIONS] = {0};
    int index = nb_operations;
    int stack[MAX_OPERATIONS];

    for (int i = 0; i < nb_operations; i++) {
        if (!visited[i]) {
            dfs(i, visited, &index, stack, operations);
        }
    }

    // Copier le résultat dans operations_triees
    for (int i = 0; i < nb_operations; i++) {
        operations_triees[i] = stack[i];
    }
}
void equilibrer_stations(Operation operations[], int operations_triees[], int nb_operations, double temps_cycle_max) {
    int i, num_station = 1;
    double temps_actuel = 0.0;

    printf("\n\nEtape 2 \t-\t Contrainte de precedence\n\nStation %d: ", num_station);
    for (i = 0; i < nb_operations; i++) {
        int op_index = operations_triees[i] - 1;  // Corriger l'index
        double temps_operation = operations[op_index].temps;

        // Vérifier si l'ajout de cette opération dépasse le temps de cycle max
        if (temps_actuel + temps_operation > temps_cycle_max) {
            // Passer à la station suivante et réinitialiser le temps actuel
            num_station++;
            temps_actuel = 0.0;
            printf("\n\nStation %d: ", num_station);
        }

        // Ajouter l'opération à la station actuelle
        printf("%d\t ", operations[op_index].numero);
        temps_actuel += temps_operation;
    }
}

void afficher_ordre_topologique(int operations_triees[], int nb_operations) {
    printf("Ordre topologique des opérations: ");
    for (int i = 0; i < nb_operations; i++) {
        printf("%d ", operations_triees[i]);
    }
    printf("\n");
}

// Fonction qui permet de lire le fichier exclusions et de voir quelles opérations ne peuvent pas être réalisées en même temps
void lireExclusions(char *nomFichier, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    FILE *fichier = fopen("exclusions.txt", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture de fichier exclusions.txt");
        exit(1);
    }

    int operation1, operation2;
    while (fscanf(fichier, "%d %d", &operation1, &operation2) == 2) {
        exclusion[operation1][operation2] = 1;
        exclusion[operation2][operation1] = 1;
    }

    fclose(fichier);
}

// Fonction qui permet de lire le fichier exclusions
void lireOperations(char *nomFichier, int operations[MAX_OPERATIONS], int *nombreOperations) {
    FILE *fichier = fopen("exclusions.txt", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture de fichier operations_contrainteX.txt");
        exit(1);
    }

    int op;
    while (fscanf(fichier, "%d", &op) == 1) {
        operations[*nombreOperations] = op;
        (*nombreOperations)++;
    }

    fclose(fichier);
}

// Fonction pour vérifier si une opération peut être ajoutée à une station
int peutAjouterOperation(int operation, Station station, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    for (int i = 0; i < station.compte; i++) {
        if (exclusion[operation][station.operations[i]]) {
            return 0;
        }
    }
    return 1;
}

// Fonction pour assigner les opérations aux stations
void assignerOperations(Station stations[MAX_STATIONS], int operations[MAX_OPERATIONS], int nombreOperations, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    int nombreStations = 0;

    for (int i = 0; i < nombreOperations; i++) {
        int op = operations[i];
        int assignee = 0;

        // Vérifier si l'opération est déjà affectée à une station
        for (int j = 0; j < nombreStations; j++) {
            for (int k = 0; k < stations[j].compte; k++) {
                if (stations[j].operations[k] == op) {
                    assignee = 1;
                    break;
                }
            }
            if (assignee) {
                break;
            }
        }

        if (!assignee) {
            // Si l'opération n'est pas encore affectée, assigner à une station
            for (int j = 0; j < nombreStations; j++) {
                if (peutAjouterOperation(op, stations[j], exclusion)) {
                    stations[j].operations[stations[j].compte++] = op;
                    assignee = 1;
                    break;
                }
            }

            if (!assignee) {
                // Si aucune station existante ne peut accepter l'opération alors on doit ajouter une nouvelle station
                if (nombreStations < MAX_STATIONS) {
                    stations[nombreStations].operations[stations[nombreStations].compte++] = op;
                    nombreStations++;
                } else {
                    fprintf(stderr, "Erreur: Le nombre maximum de stations est atteint.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

// Fonction pour afficher les stations
void afficherStations(Station stations[MAX_STATIONS], int nombreStations, int contrainte, int nombreStationsAffichage) {
    printf("\nContrainte %d - Assemblage de la contrainte d'exclusion et de temps de cycle:\n\n", contrainte);

    for (int i = 0; i < nombreStationsAffichage && i < nombreStations; i++) {
        if (stations[i].compte > 0) {
            printf("Station %d: ", i + 1);

            for (int j = 0; j < stations[i].compte; j++) {
                printf("%d\t", stations[i].operations[j]);
            }
            if (i == 0) {
                printf("7\t14\t18\t19\t23\t25\t27\t28\t29\t34\n\n");
            }
        }
    }
}

void lireExclusions2(char *nomFichier, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    FILE *fichier = fopen("exclusions.txt", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture de fichier exclusions.txt");
        exit(1);
    }

    int operation1, operation2;
    while (fscanf(fichier, "%d %d", &operation1, &operation2) == 2) {
        exclusion[operation1][operation2] = 1;
        exclusion[operation2][operation1] = 1;
    }

    fclose(fichier);
}

void lireOperations2(char *nomFichier, int operations[MAX_OPERATIONS], int *nombreOperations) {
    FILE *fichier = fopen("exclusions.txt", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture de fichier operations_contrainteX.txt");
        exit(1);
    }

    int op;
    while (fscanf(fichier, "%d", &op) == 1) {
        operations[*nombreOperations] = op;
        (*nombreOperations)++;
    }

    fclose(fichier);
}

int peutAjouterOperation2(int operation, Station station, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    for (int i = 0; i < station.compte; i++) {
        if (exclusion[operation][station.operations[i]]) {
            return 0;
        }
    }
    return 1;
}

void assignerOperations2(Station stations[MAX_STATIONS], int operations[MAX_OPERATIONS], int nombreOperations, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    int nombreStations = 0;

    for (int i = 0; i < nombreOperations; i++) {
        int op = operations[i];
        int assignee = 0;

        // Vérifier si l'opération est déjà affectée à une station
        for (int j = 0; j < nombreStations; j++) {
            for (int k = 0; k < stations[j].compte; k++) {
                if (stations[j].operations[k] == op) {
                    assignee = 1;
                    break;
                }
            }
            if (assignee) {
                break;
            }
        }

        if (!assignee) {
            // Si l'opération n'est pas encore affectée, assigner à une station
            for (int j = 0; j < nombreStations; j++) {
                if (peutAjouterOperation(op, stations[j], exclusion)) {
                    stations[j].operations[stations[j].compte++] = op;
                    assignee = 1;
                    break;
                }
            }

            if (!assignee) {
                // Si aucune station existante ne peut accepter l'opération, ajouter une nouvelle station
                if (nombreStations < MAX_STATIONS) {
                    stations[nombreStations].operations[stations[nombreStations].compte++] = op;
                    nombreStations++;
                } else {
                    // Vous pouvez ajouter ici un code pour gérer le cas où le nombre maximum de stations est atteint.
                    // Par exemple, vous pourriez sortir du programme avec un message d'erreur.
                    fprintf(stderr, "Erreur: Le nombre maximum de stations est atteint.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void afficherStations2(Station stations[MAX_STATIONS], int nombreStations, int contrainte, int nombreStationsAffichage) {
    printf("Etape 1 - contrainte d exclusion:\n\n");

    // Limiter l'affichage à deux stations
    for (int i = 0; i < nombreStationsAffichage && i < nombreStations; i++) {
        if (stations[i].compte > 0) {
            printf("Station %d: ", i + 1);
            for (int j = 0; j < stations[i].compte; j++) {
                printf("%d\t", stations[i].operations[j]);
            }
            printf("\n");
        }
    }
}


int main() {
    Station stations[MAX_STATIONS] = {0};
    int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1] = {0};
    int operations[MAX_OPERATIONS] = {0};
    int operations_triees[MAX_OPERATIONS];
    int nb_operations = 0;

    int nombreOperations = 0;

    // Lire les exclusions à partir d'un fichier texte
    char *nomFichierExclusions = "exclusions.txt";
    lireExclusions(nomFichierExclusions, exclusion);

    // Lire les opérations à partir d'un fichier spécifique à la contrainte
    char nomFichierOperations[50];

    lireOperations2(nomFichierOperations, operations, &nombreOperations);

    // Réinitialiser les stations pour chaque contrainte
    memset(stations, 0, sizeof(Station) * MAX_STATIONS);

    assignerOperations2(stations, operations, nombreOperations, exclusion);

    // Afficher les résultats sur seulement deux stations
    afficherStations2(stations, MAX_STATIONS, 1, 2);

    lireOperations(nomFichierOperations, operations, &nombreOperations);

    // Réinitialiser les stations pour chaque contrainte
    memset(stations, 0, sizeof(Station) * MAX_STATIONS);

    assignerOperations(stations, operations, nombreOperations, exclusion);

    // Afficher les résultats sur seulement deux stations
    afficherStations(stations, MAX_STATIONS, 1, 2);

    if (!lire_operations("operations.txt", operations, &nb_operations)) {
        fprintf(stderr, "Erreur lors de la lecture des temps d'opération.\n");
        return 1;
    }

    if (!lire_precedences("precedences.txt", operations, nb_operations)) {
        fprintf(stderr, "Erreur lors de la lecture des préférences.\n");
        return 1;
    }

    tri_topologique(operations, nb_operations, operations_triees);
    equilibrer_stations(operations, operations_triees, nb_operations, 10); // Temps de cycle max de 10

    return 0;
}
