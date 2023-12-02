//
// Created by paulo on 22/10/2023.
//

#ifndef TP6_HEADER_H
#define TP6_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#define TAILLE_MAX 100

// Structure pour représenter une arête
typedef struct {
    int sommet1;
    int sommet2;
    int poids;
} Arete;

/* Structure d'un arc*/
struct Arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial
    int poids;
    struct Arc* arc_suivant;
};

/* Alias de pointeur sur un Arc */
typedef struct Arc* pArc;

/* Structure d'un sommet*/
struct Sommet
{
    struct Arc* arc;
    char couleur;
    int valeur;
};

/* Alias de pointeur sur un Sommet */
typedef struct Sommet* pSommet;

/* Alias d'un Graphe */
typedef struct Graphe
{
    int capacites[TAILLE_MAX][TAILLE_MAX];
    int ordre;
    pSommet* pSommet;
} Graphe;


// créer le graphe
Graphe* CreerGraphe(int ordre);

/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
Graphe* lire_graphe(char * nomFichier);

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2,int poids);

/* affichage des successeurs du sommet num*/
void afficher_successeurs(pSommet * sommet, int num);

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe);

int comparer(const void* a, const void* b);

Arete creerArete(int s1, int s2, int poids);

void kruskal(Graphe* graphe);

void prim(Graphe* graphe, int sommetDepart);

#endif //TP6_HEADER_H
