#include "header.h"

#include <stdbool.h> // Inclure pour le type bool

// Définition de la fonction rechercheChemin
bool rechercheChemin(int graphe_residuel[TAILLE_MAX][TAILLE_MAX], int source, int puit, int parent[], int ordre) {
    bool visited[ordre];
    for (int i = 0; i < ordre; i++) {
        visited[i] = false;
    }

    visited[source] = true;
    parent[source] = -1;

    int file[TAILLE_MAX];
    int front = 0;
    int rear = 0;
    file[rear] = source;
    rear++;

    while (front != rear) {
        int sommet_actuel = file[front];
        front++;

        for (int puit = 0; puit < ordre; puit++) {
            if (!visited[puit] && graphe_residuel[sommet_actuel][puit] > 0) {
                file[rear] = puit;
                rear++;
                parent[puit] = sommet_actuel;
                visited[puit] = true;
            }
        }
    }

    return visited[puit]; // Retourne true si un chemin est trouvé jusqu'au puits
}


/* affichage des successeurs du sommet num*/
void afficher_successeurs(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num);

    pArc arc=sommet[num]->arc;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}


// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2,int poids)
{
    if(sommet[s1]->arc==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc=Newarc;
        Newarc->poids=poids;
        return sommet;
    }

    else
    {
        pArc temp=sommet[s1]->arc;
        while( !(temp->arc_suivant==NULL))
        {
            temp=temp->arc_suivant;
        }
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->poids=poids;
        Newarc->arc_suivant=NULL;

        if(temp->sommet>s2)
        {
            Newarc->arc_suivant=temp->arc_suivant;
            Newarc->sommet=temp->sommet;
            temp->sommet=s2;
            Newarc->poids=poids;
            temp->arc_suivant=Newarc;
            return sommet;
        }

        temp->arc_suivant=Newarc;
        return sommet;
    }
}

// créer le graphe
Graphe* CreerGraphe(int ordre)
{
    Graphe * Newgraphe=(Graphe*)malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));

    for(int i=0; i<ordre; i++)
    {
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->valeur=i;
        Newgraphe->pSommet[i]->arc=NULL;
    }
    return Newgraphe;
}


/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
Graphe * lire_graphe(char * nomFichier)
{
    Graphe* graphe;
    FILE * ifs = fopen(nomFichier, "r");
    int taille, orientation, ordre, s1, s2, poids, s;

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    fscanf(ifs, "%d", &ordre);

    graphe = CreerGraphe(ordre); // créer le graphe d'ordre sommets

    fscanf(ifs, "%d", &taille);
    // fscanf(ifs, "%d", &orientation);

    //   graphe->orientation = orientation;
    graphe->ordre = ordre;

    // créer les arêtes du graphe
    for (int i = 0; i < taille; ++i)
    {
        fscanf(ifs, "%d%d%d\n", &s1, &s2, &poids);
        graphe->pSommet = CreerArete(graphe->pSommet, s1, s2, poids);

        graphe->pSommet = CreerArete(graphe->pSommet, s2, s1, poids);
    }

    fclose(ifs); // Fermez le fichier après avoir terminé la lecture.

    return graphe;
}

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe)
{
    printf("graphe\n");

    printf("non oriente\n");

    //  printf("ordre = %d\n",graphe->ordre);

    //printf("listes d'adjacence :\n");


    for (int i=0; i<graphe->ordre; i++)
    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }

}

int capacite(const Graphe* graphe, int sommet1, int sommet2) {
    return graphe->capacites[sommet1][sommet2];
}

Graphe felkurson(Graphe* graphe) {
    int sommet_actuel;
    int puit;
    int graphe_residuel[TAILLE_MAX][TAILLE_MAX];
    int parent[TAILLE_MAX];
    int flotMaximal = 0;

    for (sommet_actuel = 0; sommet_actuel < graphe->ordre; sommet_actuel++) {
        for (puit = 0; puit < graphe->ordre; puit++) {
            graphe_residuel[sommet_actuel][puit] = graphe->capacites[sommet_actuel][puit];
        }
    }

    while (rechercheChemin(graphe_residuel, 0, graphe->ordre - 1, parent, graphe->ordre)) {
        int cheminFlot = INT_MAX;
        for (puit = graphe->ordre - 1; puit != 0; puit = parent[puit]) {
            sommet_actuel = parent[puit];
            cheminFlot = (cheminFlot > graphe_residuel[sommet_actuel][puit]) ? graphe_residuel[sommet_actuel][puit] : cheminFlot;
        }

        for (puit = graphe->ordre - 1; puit != 0; puit = parent[puit]) {
            sommet_actuel = parent[puit];
            graphe_residuel[sommet_actuel][puit] -= cheminFlot;
            graphe_residuel[puit][sommet_actuel] += cheminFlot;
        }

        flotMaximal += cheminFlot;
    }

    Graphe grapheFlot;
    grapheFlot.ordre = graphe->ordre;
    grapheFlot.pSommet = graphe->pSommet;

    for (sommet_actuel = 0; sommet_actuel < graphe->ordre; sommet_actuel++) {
        grapheFlot.pSommet[sommet_actuel] = graphe->pSommet[sommet_actuel];
        for (puit = 0; puit < graphe->ordre; puit++) {
            grapheFlot.capacites[sommet_actuel][puit] = graphe->capacites[sommet_actuel][puit] - graphe_residuel[sommet_actuel][puit];
        }
    }

    printf("Flot maximal: %d\n", flotMaximal);

    return grapheFlot;
}

// Fonction pour calculer le graphe d'écart
Graphe getEcart(const Graphe* grapheFlot, const Graphe* grapheOriginal) {
    Graphe grapheEcart;
    grapheEcart.ordre = grapheFlot->ordre;
    int sommet_actuel;
    int puit;

    for (sommet_actuel = 0; sommet_actuel < grapheFlot->ordre; sommet_actuel++) {
        grapheEcart.pSommet[sommet_actuel] = grapheFlot->pSommet[sommet_actuel];
        for (puit = 0; puit < grapheFlot->ordre; puit++) {
            grapheEcart.capacites[sommet_actuel][puit] = grapheOriginal->capacites[sommet_actuel][puit] - grapheFlot->capacites[sommet_actuel][puit];
        }
    }

    return grapheEcart;
}

int main() {
    Graphe *g;
    int depart;
    char nom_fichier[50];
    printf("Entrer le nom du fichier du graphe: ");
    fgets(nom_fichier, sizeof(nom_fichier), stdin);
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0'; // Supprimer le saut de ligne.

    g = lire_graphe(nom_fichier);
    // Saisie du numéro du sommet initial pour lancer un BFS puis un DSF.
    // printf("Numéro du sommet initial : \n");

    // Afficher le graphe
    graphe_afficher(g);

    // Appeler la fonction felkurson
    Graphe grapheFlot = felkurson(g);

    // Afficher le graphe de flot
    graphe_afficher(&grapheFlot);

    return 0;
}

