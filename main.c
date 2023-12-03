
#include "Graphe.h"


// Fonction recursive pour trouver et afficher tous les chemins de l'entree a la sortie
void trouver_tous_les_chemins(Graphe* graphe, int sommet_courant, int sommet_sortie, int* visite, int* chemin, int index) {
    visite[sommet_courant] = 1;
    chemin[index] = sommet_courant;
    index++;

    if (sommet_courant == sommet_sortie) {
        // Nous avons trouve un chemin de l'entree a la sortie, l'afficher
        printf("Chemin trouve : ");
        for (int i = 0; i < index; i++) {
            printf("%d", chemin[i]);
            if (i < index - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
    } else {
        pArc arc = graphe->pSommet[sommet_courant]->arc;
        while (arc != NULL) {
            int voisin = arc->sommet;
            if (!visite[voisin]) {
                trouver_tous_les_chemins(graphe, voisin, sommet_sortie, visite, chemin, index);
            }
            arc = arc->arc_suivant;
        }
    }

    // Marquer le sommet courant comme non visite pour permettre d'explorer d'autres chemins
    visite[sommet_courant] = 0;
}

// Fonction principale pour trouver et afficher tous les chemins de l'entree a la sortie
void trouver_tous_les_chemins_principale(Graphe* graphe, int sommet_initial, int sommet_sortie) {
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));
    int* chemin = (int*)malloc(graphe->ordre * sizeof(int));

    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    printf("Tous les chemins possibles de l'entree (%d) a la sortie (%d):\n", sommet_initial, sommet_sortie);
    trouver_tous_les_chemins(graphe, sommet_initial, sommet_sortie, visite, chemin, 0);

    free(visite);
    free(chemin);
}

void DetruireFile(File file) {
    free(file->tableau);
    free(file);
}


int FileVide(File file) {
    return (file->taille == 0);
}


int Defiler(File file) {
    if (FileVide(file)) {
        printf("La file est vide. Impossible de defiler.\n");
        return -1; // Valeur d'erreur, a adapter en fonction de vos besoins.
    }

    int valeur = file->tableau[file->front];
    file->front = (file->front + 1) % file->capacite;
    file->taille--;
    return valeur;
}


int FilePleine(File file) {
    return (file->taille == file->capacite);
}

void Enfiler(File file, int item) {
    if (FilePleine(file)) {
        printf("La file est pleine. Impossible d'enfiler.\n");
        return;
    }

    file->rear = (file->rear + 1) % file->capacite;
    file->tableau[file->rear] = item;
    file->taille++;
}


File CreerFile() {
    File file = (File)malloc(sizeof(struct File));
    if (!file) {
        // Gestion de l'echec de l'allocation de memoire
        exit(1);
    }
    file->capacite = 100; // Remplacez par la capacite souhaitee
    file->front = file->taille = 0;
    file->rear = -1;
    file->tableau = (int*)malloc(file->capacite * sizeof(int));
    if (!file->tableau) {
        // Gestion de l'echec de l'allocation de memoire
        exit(1);
    }
    return file;
}

/* affichage des successeurs du sommet num*/
void afficher_successeurs(pSommet * sommet, int num)
{

    printf(" sommet %d :\n",num+1);

    pArc arc=sommet[num]->arc;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc=arc->arc_suivant;
    }

}

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2)
{
    if(sommet[s1]->arc==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
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

// creer le graphe
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


/* La construction du reseau peut se faire a partir d'un fichier dont le nom est passe en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
Graphe * lire_graphe(char * nomFichier)
{
    Graphe* graphe;
    FILE * ifs = fopen(nomFichier,"r");
    int taille, orientation, ordre, s1, s2;

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    fscanf(ifs,"%d",&ordre);

    graphe=CreerGraphe(ordre); // creer le graphe d'ordre sommets

    fscanf(ifs,"%d",&taille);
    fscanf(ifs,"%d",&orientation);

    graphe->orientation=orientation;
    graphe->ordre=ordre;

    // creer les arêtes du graphe
    for (int i=0; i<taille; ++i)
    {
        fscanf(ifs,"%d%d",&s1,&s2);
        graphe->pSommet=CreerArete(graphe->pSommet, s1, s2);

        if(!orientation)
            graphe->pSommet=CreerArete(graphe->pSommet, s2, s1);
    }

    return graphe;
}

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe)
{
    printf("graphe\n");

    if(graphe->orientation)
        printf("oriente\n");
    else
        printf("non oriente\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'adjacence :\n");

    for (int i=0; i<graphe->ordre; i++)

    {
        afficher_successeurs(graphe->pSommet, i);
        printf("\n");
    }

}


/* Fonction pour le parcours en largeur BFS*/
void BFS(Graphe* graphe, int sommet_depart) {
    // Assurez-vous que sommet_depart est valide
    if (sommet_depart < 0 || sommet_depart >= graphe->ordre) {
        printf("Sommet de depart invalide\n");
        return;
    }

    // Tableau pour marquer les sommets visites
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));
    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    // File pour effectuer le BFS
    File* file = CreerFile();
    visite[sommet_depart] = 1;
    Enfiler(file, sommet_depart);

    printf("Parcours en largeur (BFS) a partir du sommet %d :\n", sommet_depart);

    while (!FileVide(file)) {
        int sommet_courant = Defiler(file);
        printf("%d ", sommet_courant);

        // Parcourez les voisins du sommet courant
        pArc arc = graphe->pSommet[sommet_courant]->arc;
        while (arc != NULL) {
            int voisin = arc->sommet;
            if (!visite[voisin]) {
                visite[voisin] = 1;
                Enfiler(file, voisin);
            }
            arc = arc->arc_suivant;
        }
    }

    printf("\n");

    free(visite);
    DetruireFile(file);
}

/* Fonction pour le parcours en profondeur DFS rescursif */
void DFSRecursif(Graphe* graphe, int sommet, int* visite) {
    visite[sommet] = 1;
    printf("%d ", sommet);

    // Parcourez les voisins du sommet
    pArc arc = graphe->pSommet[sommet]->arc;
    while (arc != NULL) {
        int voisin = arc->sommet;
        if (!visite[voisin]) {
            DFSRecursif(graphe, voisin, visite);
        }
        arc = arc->arc_suivant;
    }
}

/* Fonction pour le parcours en profondeur DFS */
void DFS(Graphe* graphe, int sommet_depart) {
    // Assurez-vous que sommet_depart est valide
    if (sommet_depart < 0 || sommet_depart >= graphe->ordre) {
        printf("Sommet de depart invalide\n");
        return;
    }

    int* visite = (int*)malloc(graphe->ordre * sizeof(int));
    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    printf("Parcours en profondeur (DFS) a partir du sommet %d :\n", sommet_depart);
    DFSRecursif(graphe, sommet_depart, visite);

    printf("\n");

    free(visite);
}





// Fonction récursive pour explorer les sommets d'une composante connexe
void explorer_composante_connexe(Graphe* graphe, int sommet, int* visite, int* sommets_composante, int* taille_composante) {
    visite[sommet] = 1;
    sommets_composante[(*taille_composante)++] = sommet;

    pArc arc = graphe->pSommet[sommet]->arc;
    while (arc != NULL) {
        int voisin = arc->sommet;
        if (!visite[voisin]) {
            explorer_composante_connexe(graphe, voisin, visite, sommets_composante, taille_composante);
        }
        arc = arc->arc_suivant;
    }
}

// Fonction pour rechercher et afficher les composantes connexes d'un graphe non orienté
void rechercher_et_afficher_composantes_connexes(Graphe* graphe) {
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));

    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    int composante_connexe = 0;

    printf("Composantes connexes du graphe  :\n");

    for (int sommet = 0; sommet < graphe->ordre; sommet++) {
        if (!visite[sommet] && sommet != 0 && sommet != 1) {
            composante_connexe++;
            int* sommets_composante = (int*)malloc(graphe->ordre * sizeof(int));
            int taille_composante = 0;

            printf("Composante connexe %d : ", composante_connexe);

            // Appel à la fonction explorer_composante_connexe pour explorer la composante
            explorer_composante_connexe(graphe, sommet, visite, sommets_composante, &taille_composante);

            // Trier les sommets de la composante connexe
            qsort(sommets_composante, taille_composante, sizeof(int), comparaison_entiers);

            // Afficher les sommets triés
            for (int i = 0; i < taille_composante; i++) {
                printf("%d ", sommets_composante[i]);
            }

            printf("\n");

            free(sommets_composante);
        }
    }

    free(visite);
}

// Fonction pour trier
int comparaison_entiers(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    Graphe * g;
    char nom_fichier[50];

    printf("Entrer le nom du fichier du labyrinthe: ");
    fgets(nom_fichier, sizeof(nom_fichier), stdin);
    // Supprime le saut de ligne (\n) de la fin de la chaîne de caractères.
    nom_fichier[strcspn(nom_fichier, "\n")] = '\0';

    g = lire_graphe(nom_fichier);

    int sommet_initial;
    printf("Numero du sommet initial : ");
    scanf("%d", &sommet_initial);

    // Afficher le graphe
    graphe_afficher(g);

    // Effectuer le BFS et DFS avec le sommet initial
    BFS(g, sommet_initial);
    DFS(g, sommet_initial);




    int sommet_sortie;


    printf("Numero du sommet de sortie : ");
    scanf("%d", &sommet_sortie);

    // Afficher le graphe
    graphe_afficher(g);

    // Appeler la fonction pour trouver et afficher tous les chemins possibles
    trouver_tous_les_chemins_principale(g, sommet_initial, sommet_sortie);

    // Rechercher et afficher les composantes connexes
    rechercher_et_afficher_composantes_connexes(g);

    return 0;


}
