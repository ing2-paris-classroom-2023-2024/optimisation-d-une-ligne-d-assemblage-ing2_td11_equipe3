#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPERATIONS 35
#define MAX_STATIONS 35

typedef struct {
    int operations[MAX_OPERATIONS];
    int compte;
} Station;

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

int peutAjouterOperation(int operation, Station station, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1]) {
    for (int i = 0; i < station.compte; i++) {
        if (exclusion[operation][station.operations[i]]) {
            return 0;
        }
    }
    return 1;
}

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

void afficherStations(Station stations[MAX_STATIONS], int nombreStations, int contrainte, int nombreStationsAffichage) {
    printf("Contrainte %d - Repartition des operations par station:\n", contrainte);

    // Limiter l'affichage à deux stations
    for (int i = 0; i < nombreStationsAffichage && i < nombreStations; i++) {
        if (stations[i].compte > 0) {
            printf("Station %d: ", i + 1);
            for (int j = 0; j < stations[i].compte; j++) {
                printf("%d ", stations[i].operations[j]);
            }
            printf("\n");
        }
    }
}

int main() {
    Station stations[MAX_STATIONS] = {0};
    int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1] = {0};
    int operations[MAX_OPERATIONS] = {0};
    int nombreOperations = 0;

    // Lire les exclusions à partir d'un fichier (peut être le même pour chaque contrainte ou différent)
    char *nomFichierExclusions = "exclusions.txt";
    lireExclusions(nomFichierExclusions, exclusion);

    // Lire les opérations à partir d'un fichier spécifique à la contrainte
    char nomFichierOperations[50];
    sprintf(nomFichierOperations, "operations_contrainte%d.txt", 1);
    lireOperations(nomFichierOperations, operations, &nombreOperations);

    // Réinitialiser les stations pour chaque contrainte
    memset(stations, 0, sizeof(Station) * MAX_STATIONS);

    assignerOperations(stations, operations, nombreOperations, exclusion);

    // Afficher les résultats sur seulement deux stations
    afficherStations(stations, MAX_STATIONS, 1, 2);

    return 0;
}
