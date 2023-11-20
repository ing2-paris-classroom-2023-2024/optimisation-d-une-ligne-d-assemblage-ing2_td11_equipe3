#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPERATIONS 35
#define MAX_STATIONS 35

typedef struct {
    int operations[MAX_OPERATIONS];
    int compte;
} Station;

typedef struct {
    int op;
    int contraintes;
} OpContrainte;

int comparerContraintes(const void *a, const void *b) {
    OpContrainte *opA = (OpContrainte *)a;
    OpContrainte *opB = (OpContrainte *)b;
    return opB->contraintes - opA->contraintes; // Tri décroissant
}

void lirePairesExclusion(char *nomFichier, int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1], OpContrainte ops[MAX_OPERATIONS]) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture de fichier");
        exit(1);
    }

    int op1, op2;
    while (fscanf(fichier, "%d %d", &op1, &op2) == 2) {
        exclusion[op1][op2] = 1;
        exclusion[op2][op1] = 1;
        ops[op1 - 1].contraintes++;
        ops[op2 - 1].contraintes++;
    }

    for (int i = 0; i < MAX_OPERATIONS; i++) {
        ops[i].op = i + 1;
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

void assignerOperations(Station stations[MAX_STATIONS], int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1], OpContrainte ops[MAX_OPERATIONS]) {
    int nombreStations = 0;

    for (int i = 0; i < MAX_OPERATIONS; i++) {
        int op = ops[i].op;
        int assignee = 0;

        for (int j = 0; j < nombreStations; j++) {
            if (peutAjouterOperation(op, stations[j], exclusion)) {
                stations[j].operations[stations[j].compte++] = op;
                assignee = 1;
                break;
            }
        }

        if (!assignee) {
            stations[nombreStations].operations[stations[nombreStations].compte++] = op;
            nombreStations++;
        }
    }
}

void afficherStations(Station stations[MAX_STATIONS], int nombreStations) {
    for (int i = 0; i < nombreStations && i < 2; i++) { // Limite l'affichage aux deux premières stations
        printf("Station %d: ", i + 1);
        for (int j = 0; j < stations[i].compte; j++) {
            printf("%d ", stations[i].operations[j]);
        }
        printf("\n");
    }
}

int main() {
    Station stations[MAX_STATIONS] = {0};
    int exclusion[MAX_OPERATIONS + 1][MAX_OPERATIONS + 1] = {0};
    OpContrainte ops[MAX_OPERATIONS] = {0};

    char *nomFichier = "exclusions.txt";
    lirePairesExclusion(nomFichier, exclusion, ops);

    qsort(ops, MAX_OPERATIONS, sizeof(OpContrainte), comparerContraintes);

    assignerOperations(stations, exclusion, ops);

    afficherStations(stations, MAX_STATIONS);

    return 0;
}
