#include "header.h"

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

    for (int i = 0; i < ordre; ++i) {
        fscanf(ifs, "%d", &s);
    }

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

    printf("ordre = %d\n",graphe->ordre);

    //printf("listes d'adjacence :\n");


    for (int i=0; i<graphe->ordre; i++)
    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }

}


void dijkstra(Graphe* graphe, int depart, int arrivee){
    int dist[graphe->ordre]; // Tableau pour stocker les distances minimales
    int explore[graphe->ordre]; // Tableau pour suivre les sommets visités
    int preced[graphe->ordre]; // Tableau pour stocker les parents des sommets sur le chemin le plus court

    // Initialisation des distances et des sommets parents
    for (int i = 0; i < graphe->ordre; ++i) {
        dist[i] = 9999; // Initialiser les distances à l'infini
        explore[i] = 0; // Aucun sommet n'est visité au début
        preced[i] = -1; // Aucun parent n'est défini au début
    }

    // La distance du départ à lui-même est 0
    dist[depart] = 0;

    // Algorithme de Dijkstra
    for (int j = 0; j < graphe->ordre - 1; ++j) {
        // Trouver le sommet avec la distance minimale non visité
        int minDist = 9999;
        int indice = -1;

        for (int i = 0; i < graphe->ordre; ++i) {
            if (!explore[i] && dist[i] < minDist) {
                minDist = dist[i];
                indice = i;
            }
        }

        // Marquer le sommet sélectionné comme visité
        explore[indice] = 1;

        // Mettre à jour les distances des sommets adjacents au sommet sélectionné
        for (pArc arc = graphe->pSommet[indice]->arc; arc != NULL; arc = arc->arc_suivant) {
            int adjacent = arc->sommet;
            int poids = arc->poids;

            if (!explore[adjacent] && dist[indice] + poids < dist[adjacent]) {
                dist[adjacent] = dist[indice] + poids;
                preced[adjacent] = indice;
            }
        }
    }

    // Afficher le chemin le plus court et la distance
    printf("Chemin le plus court de %d a %d : ", depart, arrivee);
    int actuel = arrivee;
    while (actuel != depart) {
        printf("%d <- ", actuel);
        actuel = preced[actuel];
    }
    printf("%d\n", depart);

    printf("Distance minimale : %d\n", dist[arrivee]);
}

int main()
{
    Graphe * g;
    int depart;
    char nom_fichier[50];
    int arrive;
    printf("entrer le nom du fichier du labyrinthe:");
    gets(nom_fichier);

    g = lire_graphe(nom_fichier);
    ///saisie du numéro du sommet initial pour lancer un BFS puis un DSF
   // printf("numero du sommet initial : \n");

    /// afficher le graphe
    graphe_afficher(g);
    printf("sommet de depart\n");
    scanf("%d", &depart);
    printf("sommet d'arriver\n");
    scanf("%d", &arrive);
    dijkstra(g,depart,arrive);
    free(g);
    return 0;
}



 