#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPS 100  // Nombre maximal d'opérations

// Structure pour stocker les informations d'une opération
typedef struct {
    int numero;
    double temps;
    int visite;
    int nb_precedences;
    int precedences[MAX_OPS];
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
    int visited[MAX_OPS] = {0};
    int index = nb_operations;
    int stack[MAX_OPS];

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

    printf("Station %d: ", num_station);
    for (i = 0; i < nb_operations; i++) {
        int op_index = operations_triees[i] - 1;  // Corriger l'index
        double temps_operation = operations[op_index].temps;

        // Vérifier si l'ajout de cette opération dépasse le temps de cycle max
        if (temps_actuel + temps_operation > temps_cycle_max) {
            // Passer à la station suivante et réinitialiser le temps actuel
            num_station++;
            temps_actuel = 0.0;
            printf("\nStation %d: ", num_station);
        }

        // Ajouter l'opération à la station actuelle
        printf("Operation %d ", operations[op_index].numero);
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

int main() {
    Operation operations[MAX_OPS];
    int operations_triees[MAX_OPS];
    int nb_operations = 0;

    if (!lire_operations("temps.txt", operations, &nb_operations)) {
        fprintf(stderr, "Erreur lors de la lecture des temps d'opération.\n");
        return 1;
    }

    if (!lire_precedences("precedence.txt", operations, nb_operations)) {
        fprintf(stderr, "Erreur lors de la lecture des préférences.\n");
        return 1;
    }

    tri_topologique(operations, nb_operations, operations_triees);
    equilibrer_stations(operations, operations_triees, nb_operations, 10); // Temps de cycle max de 10

   // afficher_ordre_topologique(operations_triees, nb_operations);
    return 0;
}
