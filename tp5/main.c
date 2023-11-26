#include <stdio.h>
#include <stdlib.h>


/* Structure d'un arc*/
struct Arc{
    int sommet; // numero de sommet d'un arc adjacent au sommet initial
    int valeur;
    int poids;
    struct Arc* arc_suivant;
};

/* Alias de pointeur sur un Arc */
typedef struct Arc* pArc;

typedef struct Arc2{
    int s1;
    int s2;
    int poids;
    int visite;
}Arc2;

/* Structure d'un sommet*/
struct Sommet{
    struct Arc* arc;
    int valeur;
    int couleur; // valeur de 0 a beaucoup qui correspond au graphe qui lui est rattachee(permet de connaitre le graphe le + leger)
};

/* Alias de pointeur sur un Sommet */
typedef struct Sommet* pSommet;

/* Alias d'un Graphe */
typedef struct Graphe{
    int taille;
    int orientation;
    int ordre;
    Arc2* ListeArc;
    pSommet* pSommet;
} Graphe;

typedef struct Element{
    int nombre;
    struct Element *suivant;
} Element;





// On recupere les fonctions de l'ancien TP
// creer le graphe
Graphe* CreerGraphe(int ordre){
    Graphe * Newgraphe=(Graphe*)malloc(sizeof(Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));
    Newgraphe->ordre = ordre;

    for(int i=0; i<ordre; i++){
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->valeur=i;
        Newgraphe->pSommet[i]->arc=NULL;
    }
    return Newgraphe;
}

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2, int poids){
    /*La fonction prend en argument un tableau de sommet ainsi que 2 sommets s1 et s2.
     * Elle renvoie le sommet pris en argument en l'ayant modifie.
     */

    if(sommet[s1]->arc==NULL){
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->poids=poids;
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1]->arc=Newarc;
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
        Newarc->poids=poids;
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;

        if(temp->sommet>s2)
        {
            Newarc->arc_suivant=temp->arc_suivant;
            Newarc->sommet=temp->sommet;
            temp->sommet=s2;
            temp->arc_suivant=Newarc;
            return sommet;
        }

        temp->arc_suivant=Newarc;
        return sommet;
    }
}

/* La construction du reseau peut se faire à partir d'un fichier dont le nom est passe en paramètre
Le fichier contient : ordre, taille, orientation (0 ou 1)et liste des arcs. */
Graphe * lire_graphe(char * nomFichier){
    Graphe* graphe;
    FILE * ifs = fopen(nomFichier,"r");
    int taille, orientation, ordre, s1, s2, poids;

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    fscanf(ifs,"%d",&ordre);

    graphe = CreerGraphe(ordre); // creer le graphe d'ordre sommets

    fscanf(ifs,"%d",&taille);
    fscanf(ifs,"%d",&orientation);

    Arc2* ListeArc = malloc(sizeof(Arc2)*taille);
    graphe->orientation = orientation;
    graphe->ordre = ordre;
    graphe->taille = taille;

    // creer les arêtes du graphe
    for (int i=0; i<taille; ++i){
        fscanf(ifs,"%d%d%d",&s1,&s2,&poids);
        graphe->pSommet=CreerArete(graphe->pSommet, s1, s2,poids);

        if(!orientation){
            graphe->pSommet=CreerArete(graphe->pSommet, s2, s1,poids);
        }
        Arc2 temp = {s1,s2,poids};
        ListeArc[i] = temp;
    }
    graphe->ListeArc = ListeArc;
    return graphe;
}

Graphe* Kruskals(Graphe* graphe){
    Graphe* Gfin = CreerGraphe(graphe->ordre);
    Arc2* ListeArc = graphe->ListeArc;

    int* Sommet = malloc(sizeof(int)*graphe->ordre);
    for(int i=0; i<graphe->ordre;i++){
        Sommet[i] = i;
    }

    for(int i=0;i<graphe->taille-1;i++){
        for (int j = i + 1; j < graphe->taille; j++){
            if (ListeArc[i].poids > ListeArc[j].poids){
                Arc2 c = ListeArc[i];
                ListeArc[i] = ListeArc[j];
                ListeArc[j] = c;
            }
        }
    }
    int counter = 0;
    for(int i = 0; i<graphe->taille;i++){
        int s1 = ListeArc[i].s1;
        int s2 = ListeArc[i].s2;

        if(Sommet[s1] != Sommet[s2]){
            Gfin->pSommet = CreerArete(Gfin->pSommet,s1,s2,ListeArc[i].poids);
            int moins, plus;
            if(Sommet[s1] > Sommet[s2]){
                moins = Sommet[s1];
                plus = Sommet[s2];
            }
            if(Sommet[s1] < Sommet[s2]){
                moins = Sommet[s1];
                plus = Sommet[s2];
            }
            for(int j=0;j<graphe->ordre;j++){
                if(plus == Sommet[j]){
                    Sommet[j] = moins;
                }
            }
            counter++;
        }
        if(counter == graphe->ordre-1){
            break;
        }
    }
    free(Sommet);
    return Gfin;
}

Graphe* Prim(Graphe* graphe, int depart) {
    Graphe* Gfin = CreerGraphe(graphe->ordre);
    Arc2* ListeArc = malloc(sizeof(Arc2) * (graphe->ordre - 1));

    int* Sommet = malloc(sizeof(int) * graphe->ordre);
    for (int i = 0; i < graphe->ordre; i++) {
        Sommet[i] = 0;
    }

    Sommet[depart] = 1;
    int som_min1, som_min2, poids;
    int counter = 1, poids_total = 0;


    printf("listes des arcs :\n");

    while (counter < graphe->ordre){
        poids = INT_MAX; // Initialisation du poids à l'infini
        for (int i = 0; i < graphe->ordre; i++) {
            if (Sommet[i] == 1) {
                pArc temp = graphe->pSommet[i]->arc;
                while (temp != NULL) {
                    if (Sommet[temp->sommet] == 0 && temp->poids < poids) {
                        poids = temp->poids;
                        som_min1 = i;
                        som_min2 = temp->sommet;
                    }
                    temp = temp->arc_suivant;
                }
            }
        }

        printf("(%d, %d) -- Poids : %d\n",som_min1,som_min2,poids);
        poids_total += poids;
        Gfin->pSommet = CreerArete(Gfin->pSommet, som_min1, som_min2, poids);
        Arc2 tempo = {som_min1, som_min2, poids};
        ListeArc[counter - 1] = tempo;
        Sommet[som_min2] = 1;
        counter++;
    }

    Gfin->ListeArc = ListeArc;
    printf("Le poids de ce graphe est %d\n",poids_total);

    free(Sommet); // Liberez la memoire allouee pour Sommet

    return Gfin;
}


int afficher_successeurs(pSommet * sommet, int num){
    /*La fonction prend en argument un tableau de Sommet ainsi qu'un numero de sommet.
     * Elle ne renvoie rien. Mais affiche tous les sommets relie au sommet num.
     */

    pArc arc = sommet[num]->arc;
    int poid = 0;

    while(arc!=NULL){
        //On affiche le sommet actuel avant de regarder le sommet suivant

        printf("(%d, %d) -- Poids : %d\n",num ,arc->sommet,arc->poids);
        poid += arc->poids;
        arc=arc->arc_suivant;
    }
    return poid;
}

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe){
    int poids = 0;



    printf("listes des arcs :\n");

    for (int i=0; i<graphe->ordre; i++){

        poids += afficher_successeurs(graphe->pSommet, i);
    }
    printf("Le poids de ce graphe est %d.\n",poids);
}

int main() {
    char* fichier = "graphe.txt";
    Graphe* g;

    g = lire_graphe(fichier);

    int choix;
    do {
        printf("\n");
        printf("Choisissez une option:\n");
        printf("1. Algorithme de Kruskal\n");
        printf("2. Algorithme de Prim\n");
        printf("0. Quitter\n");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Algorithme de Kruskal:\n");
                Graphe* GKruskal = Kruskals(g);
                graphe_afficher(GKruskal);
                free(GKruskal);
                break;
            case 2:

                printf("Saisir le sommet de depart pour Prim: \n");
                int sommet_depart;
                scanf("%d", &sommet_depart);
                printf("Algorithme de Prim:\n");
                Graphe* Gfin_Prim = Prim(g, sommet_depart);
                free(Gfin_Prim);
                break;
            case 0:
                printf("Au revoir!\n");
                break;
            default:
                printf("Option non valide. Veuillez choisir à nouveau.\n");
        }
    } while (choix != 0);

    return 0;
}

