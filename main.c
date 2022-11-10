#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "struct.h"

#pragma warning(disable:4996)
/*--------------------------------------prototypes des commandes--------------------------------------*/
BOOL create_formation(Formation *ma_formation);
BOOL create_epreuve(Formation *ma_formation);
void coefficients(const Formation * ma_formation);
void add_note(Formation *ma_formation, char liste_etudiant[][MAX_CHAR]);
void notes(const Formation *ma_formation,const char liste_etudiant[][MAX_CHAR]);
void commande_releve(const Formation *ma_formation, const char liste_etudiant[][MAX_CHAR],BOOL do_affichage);
void decision(const Formation *ma_formation, const char liste_etudiant[][MAX_CHAR]);

/*--------------------------------------prototypes des fonctions annexes--------------------------------------*/
BOOL semestre_is_valid(int num_semestre);
BOOL UE_is_def(const Formation *ma_formation);
int get_matiere_indice(char nom_matiere[], const Matiere liste_mat[], int nb_matiere);
int get_epreuve_indice(char nom_epreuve[], const Matiere liste_mat[] , int nb_matiere, char nom_matiere[]);
BOOL epreuve_already_exist(char nom_epreuve[], const Matiere liste_mat[], int nb_matiere,char nom_matiere[]);
int verif_coeff(const Formation *ma_formation,int num_semestre);
int verif_note(const Formation * ma_formation ,char nom_etudiant[],const char liste_etudiant[][MAX_CHAR],int num_semestre);
void affiche_erreur_coeff(int num_erreur);
void affiche_erreur_note(int num_erreur);
int get_etudiant_indice(char nom_etudiant[], const char liste_etudiant[][MAX_CHAR], int nb_etudiant);
void print_entete_UE(int nb_UE);
int get_max_space(const Formation *ma_formation,int num_semestre);
char *add_space(char ma_chaine[], int nb_space);
float *get_releve(const Formation *ma_formation, const char liste_etudiant[][MAX_CHAR],BOOL do_affichage,int num_semestre,char nom_etudiant[]);

int main()
{
    Formation ma_formation;
    ma_formation.nb_UE_is_def=False;
    char liste_etudiant[MAX_ETUDIANTS][MAX_CHAR];
    char commande[MAX_CHAR];
    Semestre s;

    for (size_t i = 0; i < NB_SEMESTRES; i++)
    {
        s.nb_matieres=0;
        for (size_t j = 0; j < MAX_MATIERES; j++)
        {
            for (size_t k = 0; k < MAX_EPREUVES; k++)
            {
                for (size_t z = 0; z < MAX_ETUDIANTS; z++)
                {
                    ma_formation.liste_semestres[i].liste_matieres[j].liste_epreuve[k].listes_notes[z]=-1;
                }
                
            }
            
        }
        ma_formation.liste_semestres[i]=s;
    }
    
    ma_formation.nb_etudiants=0;
    

    do
    {
        scanf("%s",commande);

        if (strcmp(commande,"formation")==0){
            create_formation(&ma_formation);
        }
        else if (strcmp(commande,"epreuve")==0){
            create_epreuve(&ma_formation);
        }
        else if(strcmp(commande,"coefficients")==0){
            coefficients(&ma_formation);
        }
        else if(strcmp(commande,"note")==0){
            add_note(&ma_formation,liste_etudiant);
        }
        else if(strcmp(commande,"notes")==0){
            notes(&ma_formation,liste_etudiant);
        }
        else if(strcmp(commande,"releve")==0){
            commande_releve(&ma_formation,liste_etudiant,True);
        }
        else if(strcmp(commande,"decision")==0){
            decision(&ma_formation,liste_etudiant);
        }


    } while (strcmp(commande,"exit")!=0);
    
    return 0;
}

BOOL create_formation(Formation *ma_formation)
{
    /*
        *ma_formation -> pointeur vers la formation qu'on va crée
        cree une formation et return True si la formation à pu se créer False sinon
    */

    unsigned int nb_UE;
    scanf("%d", &nb_UE);
    

    if (ma_formation->nb_UE_is_def == True)
    {
        printf("Le nombre d'UE est deja defini\n");
        return False;
    }

    else if (nb_UE >= 3 && nb_UE <= 6)
    {
        printf("Le nombre d'UE est defini\n");
        ma_formation->nb_UE_is_def = True;
        ma_formation->nb_UE = nb_UE;
        return True;
    }
    else
    {
        printf("Le nombre d'UE est incorrect\n");
        ma_formation->nb_UE_is_def = False;
        return False;
    }
}
BOOL create_epreuve(Formation *ma_formation)
{
    /*
        *ma_formation -> notre formation
        return True si l'epreuve a pu se cree False sinon
    */
    if (UE_is_def(ma_formation)==False)
    {
        return False;
    }
    
    int num_semestre;
    scanf("%d",&num_semestre);
    if (semestre_is_valid(num_semestre) == False)
    {
        return False;
    }

    int nb_UE = ma_formation->nb_UE;
    int nb_coeff_equal_zero = 0;
    float mon_coeff = 0.;


    char nom_matiere[MAX_CHAR];
    Matiere *liste_mat = ma_formation->liste_semestres[num_semestre-1].liste_matieres;
    scanf("%s",nom_matiere);

    Epreuve mon_epreuve;
    scanf("%s",mon_epreuve.nom);

    // liste_mat contient la liste de toutes les matieres de la formation pour un semestre donnée
    int nb_matiere = ma_formation->liste_semestres[num_semestre - 1].nb_matieres;
    
    for (size_t i = 0; i < nb_UE; i++)
    {
        scanf("%f",&mon_coeff);
        if (mon_coeff == 0)
        {
            nb_coeff_equal_zero++;
        }
        else if (mon_coeff < 0)
        {
            printf("Au moins un des coefficients est incorrect\n");
            return False;
        }
        mon_epreuve.tab_coeff_UE[i] = mon_coeff;
    }

    int matiere_indice = get_matiere_indice(nom_matiere, liste_mat, nb_matiere); // recuperation de l'indice de la matiere rechercher
    Matiere ma_matiere;
    if (epreuve_already_exist(mon_epreuve.nom, liste_mat, nb_matiere ,nom_matiere) == True)
    {
        printf("Une meme epreuve existe deja\n");
        return False;
    }
    if (nb_coeff_equal_zero == nb_UE)
    {
        printf("Au moins un des coefficients est incorrect\n");
        return False;
    }
    for (size_t i = 0; i < MAX_ETUDIANTS; i++)
    {
        mon_epreuve.listes_notes[i]=-1;
    }

    if (matiere_indice == -1)
    {
        // creation matiere

        ma_matiere.nb_epreuve = 0;
        strcpy(ma_matiere.nom, nom_matiere);
        ma_matiere.liste_epreuve[ma_matiere.nb_epreuve] = mon_epreuve;
        ma_matiere.nb_epreuve += 1;
        liste_mat[nb_matiere] = ma_matiere;
        nb_matiere += 1;
        printf("Matiere ajoutee a la formation\n");

        ma_formation->liste_semestres[num_semestre - 1].nb_matieres++;
    }
    else
    {
        liste_mat[matiere_indice].liste_epreuve[liste_mat[matiere_indice].nb_epreuve] = mon_epreuve;
        liste_mat[matiere_indice].nb_epreuve += 1;
    }

    
    printf("Epreuve ajoutee a la formation\n");
    return True;
}

void coefficients(const Formation *ma_formation)
{
    /*
        num_semestre -> numero du semestre dont on souhaite verifier les coeff
        *ma_formation -> notre formation
        retourne un nombre et en fonction du nombre que verif_coeff retourne
        on pourra gérer l'affichage avec "void affiche_erreur_coeff(int num_erreur)"
    */
    if (UE_is_def(ma_formation)==False)
    {
        return;
    }
    int num_semestre;
    scanf("%d",&num_semestre);
    if (semestre_is_valid(num_semestre) == False)
    {
        return;
    }
    affiche_erreur_coeff(verif_coeff(ma_formation,num_semestre));
    


}

void add_note(Formation *ma_formation, char liste_etudiant[][MAX_CHAR]){
    if (UE_is_def(ma_formation)==False)
    {
        return ;
    }
    int num_semestre;
    scanf("%d",&num_semestre);
    if (semestre_is_valid(num_semestre)==False)
    {
        return ;
    }
    char nom_etudiant[MAX_CHAR];
    scanf("%s",nom_etudiant);
    char nom_matiere[MAX_CHAR];
    scanf("%s",nom_matiere);
    char nom_epreuve[MAX_CHAR];
    scanf("%s",nom_epreuve);
    float note_etudiant;
    scanf("%f",&note_etudiant);


    int nb_etudiants=ma_formation->nb_etudiants;
    Matiere *liste_mat=ma_formation->liste_semestres[num_semestre-1].liste_matieres;
    int nb_matiere=ma_formation->liste_semestres[num_semestre-1].nb_matieres;

    int matiere_indice = get_matiere_indice(nom_matiere, liste_mat, nb_matiere); // recuperation de l'indice de la matiere rechercher
    int epreuve_indice = get_epreuve_indice(nom_epreuve,liste_mat,nb_matiere,nom_matiere);

    if (matiere_indice==-1)
    {
        printf("Matiere inconnue\n");
        return ;
    }
    if(epreuve_already_exist(nom_epreuve,liste_mat,nb_matiere,nom_matiere)==False){
        printf("Epreuve inconnue\n");
        return ;
    }
    if (note_etudiant<MIN_NOTE || note_etudiant>MAX_NOTE)
    {
        printf("Note incorrecte\n");
        return ;

    }
    int indice_etudiant=get_etudiant_indice(nom_etudiant,liste_etudiant,nb_etudiants);
    if (indice_etudiant==-1)
    {
        strcpy(liste_etudiant[nb_etudiants],nom_etudiant);
        ma_formation->nb_etudiants+=1;
        printf("Etudiant ajoute a la formation\n");
    }
    indice_etudiant = get_etudiant_indice(nom_etudiant,liste_etudiant,MAX_ETUDIANTS); // on remet a jour l'indice de l'etudiant au cas ou l'etudiant n'etait pas dans la formation
    int note_actuel_etudiant=ma_formation->liste_semestres[num_semestre-1].liste_matieres[matiere_indice].liste_epreuve[epreuve_indice].listes_notes[indice_etudiant];
    if (note_actuel_etudiant!=-1)//equivalent a dire qu'une note a déja été attribue a cet etudiant car les note des toutes les epreuves sont a -1 par defaut
    {
        printf("Une note est deja definie pour cet etudiant\n");
        return;
    }
    
    ma_formation->liste_semestres[num_semestre-1].liste_matieres[matiere_indice].liste_epreuve[epreuve_indice].listes_notes[indice_etudiant]=note_etudiant;
    printf("Note ajoutee a l'etudiant\n");
    
    //printf("semestre: %d nom : %s matiere : %s epreuve :%s note :%f",num_semestre,liste_etudiant[indice_etudiant],ma_formation->liste_semestres[num_semestre-1].liste_matieres[matiere_indice].nom,ma_formation->liste_semestres[num_semestre-1].liste_matieres[matiere_indice].liste_epreuve[epreuve_indice].nom,ma_formation->liste_semestres[num_semestre-1].liste_matieres[matiere_indice].liste_epreuve[epreuve_indice].listes_notes[indice_etudiant]);

    

}
void notes(const Formation *ma_formation ,const char liste_etudiant[][MAX_CHAR]){
    if (UE_is_def(ma_formation)==False)
    {
        return;
    }
    int num_semestre;
    scanf("%d",&num_semestre);
    if (semestre_is_valid(num_semestre)==False)
    {
        return;
    }
    char nom_etudiant[MAX_CHAR];
    scanf("%s",nom_etudiant);
    affiche_erreur_note(verif_note(ma_formation,nom_etudiant,liste_etudiant,num_semestre));
    
}

void commande_releve(const Formation *ma_formation, const char liste_etudiant[][MAX_CHAR],BOOL do_affichage){
    int num_semestre;
    char nom_etudiant[MAX_CHAR];
    scanf("%d",&num_semestre);
    scanf("%s",nom_etudiant);
    
    get_releve(ma_formation,liste_etudiant,True,num_semestre,nom_etudiant);
    
}
float *get_releve(const Formation *ma_formation, const char liste_etudiant[][MAX_CHAR],BOOL do_affichage,int num_semestre,char nom_etudiant[]){
    static float releve_etudiant[MAX_UE]={-1,-1,-1,-1,-1,-1};
    if (UE_is_def(ma_formation)==False)
    {
        return releve_etudiant;
    }

    if (semestre_is_valid(num_semestre)==False)
    {

        return releve_etudiant;
    }

    int nb_etudiant=ma_formation->nb_etudiants;
    int indice_etudiant=get_etudiant_indice(nom_etudiant,liste_etudiant,nb_etudiant);
    if(indice_etudiant==-1 ){
        printf("Eudiant inconnu\n");
        return releve_etudiant;
    }
    if (verif_coeff(ma_formation,num_semestre)==2 )
    {
        printf("Les coefficients de ce semestre sont incorrects\n");
        return releve_etudiant;
    }

    if (verif_note(ma_formation,nom_etudiant,liste_etudiant,num_semestre)==2)
    {
        printf("Il manque au moins une note pour cet etudiant\n");
        return releve_etudiant;
    }
    int nb_matiere=ma_formation->liste_semestres[num_semestre-1].nb_matieres;
  
    int nb_space=get_max_space(ma_formation,num_semestre);

    if (do_affichage)
    {
        printf("%s",add_space(" ",nb_space));
        
        print_entete_UE(ma_formation->nb_UE);

    }
    
    char nom_matiere[MAX_CHAR];
    float somme_coeff=0;
    float somme_notes=0;
    int nb_epreuve;
    float coeff;
    float note;
    int res;


    float tab_coeff_somme_UE[MAX_UE]={0};
    float tab_note_somme_UE[MAX_UE]={0};
    for (size_t i = 0; i < nb_matiere; i++)
    {
        strcpy(nom_matiere,ma_formation->liste_semestres[num_semestre-1].liste_matieres[i].nom);
        if (do_affichage)
        {
            printf("%s",add_space(nom_matiere,nb_space));
        }
        
        
        nb_epreuve=ma_formation->liste_semestres[num_semestre-1].liste_matieres[i].nb_epreuve;
        
        for (size_t k = 0; k < ma_formation->nb_UE; k++)
        {
            for (size_t j = 0; j < nb_epreuve ; j++)
            {
                coeff=ma_formation->liste_semestres[num_semestre-1].liste_matieres[i].liste_epreuve[j].tab_coeff_UE[k];
                note=ma_formation->liste_semestres[num_semestre-1].liste_matieres[i].liste_epreuve[j].listes_notes[indice_etudiant];
                
                somme_coeff+=coeff;
                somme_notes+=note*coeff;
            }
            if (somme_coeff==0 && do_affichage)
            {
                printf("%5s","ND");
            }
            else if (somme_coeff > 0 && do_affichage)
            {
                printf("%5.1f", floorf((somme_notes / somme_coeff) * 10) / 10);
            }
            tab_coeff_somme_UE[k]+=somme_coeff;
            tab_note_somme_UE[k]+=somme_notes;

            somme_coeff=0;
            somme_notes=0;
            
        }
        if (do_affichage)
        {
            printf("\n");
        }
        
    }
    if (do_affichage)
    {
        printf("--\n");
        printf("%s", add_space("Moyennes", nb_space));
    }
    float moyenne;
    for (size_t i = 0; i < ma_formation->nb_UE; i++)
    {
        
        moyenne=tab_note_somme_UE[i]/tab_coeff_somme_UE[i];
        releve_etudiant[i]=moyenne;
        if (do_affichage)
        {
            printf("%5.1f", floorf(moyenne * 10) / 10);
        }
    }
    if (do_affichage)
    {
        printf("\n");
    }
    return releve_etudiant;
}
void decision(const Formation *ma_formation, const char liste_etudiant[][MAX_CHAR]){
    
    char nom_etudiant[MAX_CHAR];
    scanf("%s",nom_etudiant);

    float * releve;
    float tab_moyenne_anuelles[MAX_UE]={0};
    for (size_t i = 1; i < NB_SEMESTRES+1; i++)
    {
        releve=get_releve(ma_formation,liste_etudiant,False,i,nom_etudiant);
        if(releve[0]==-1){
            return;
        }
    }
    printf("%18s"," ");
    print_entete_UE(ma_formation->nb_UE);

    for (size_t i = 1; i < NB_SEMESTRES+1; i++)
    {
        printf("S%d                ",i);
        releve=get_releve(ma_formation,liste_etudiant,False,i,nom_etudiant);

        for (size_t j = 0; j < ma_formation->nb_UE; j++)
        {
            printf("%5.1f",floorf(releve[j]*10)/10);
            tab_moyenne_anuelles[j]+=releve[j]*0.5;// on multiplie par 0.5 comme sa on optient directement la moyenne anuelles pour chaque UE
        }                                          // car on sait que dans une formation il n'y a que 2 semestres.
        printf("\n");
        
    }
    printf("--\n");
    printf("%s", add_space("Moyennes annuelles", 18));

    for (size_t i = 0; i < ma_formation->nb_UE; i++)
    {
        printf("%5.1f", floorf(tab_moyenne_anuelles[i] * 10) / 10);
    }

    printf("\n%s", add_space("Acquisition", 18));

    int nb_UE_valide = 0;
    for (size_t i = 0; i < ma_formation->nb_UE; i++)
    {
        if (tab_moyenne_anuelles[i] >= 10)
        {
            printf(" UE%d", i + 1);
            if (i < ma_formation->nb_UE-1)
            {
                printf(",");
            }
            nb_UE_valide += 1;
        }
        if (i == ma_formation->nb_UE - 1)
        {
            if (nb_UE_valide == 0)
            {
                printf(" Aucune");
            }

            printf("\n%s", add_space("Devenir", 18));
            if (nb_UE_valide > (ma_formation->nb_UE / 2))
            {
                printf(" Passage");
            }
            else
            {
                printf(" Redoublement");
            }
        }
    }

    printf("\n");
    
}



/*-------------------------------------Fonctions annexe------------------------------------------*/
BOOL semestre_is_valid(int num_semestre)
{
    /*
        renvoie False si le numero de semestre est incorrect
    */
    if (num_semestre > 2 || num_semestre < 1)
    {
        printf("Le numero de semestre est incorrect\n");
        return False;
    }
    return True;
}
BOOL UE_is_def(const Formation *ma_formation){
    /*
        si le nombre d'UE n'est pas définie , cette fonction affiche
        "Le nombre d'UE n'est pas defini" et return False sinon return True
    */
    if (ma_formation->nb_UE_is_def==False)
    {
        printf("Le nombre d'UE n'est pas defini\n");
        return False;
    }
    return True;
    
}
int get_matiere_indice(char nom_matiere[], const Matiere liste_mat[], int nb_matiere)
{
    /*
        return l'indice qui a été attribuer à la matiere dans liste_mat[]
        sinon renvoie -1 si la matiere n'existe pas
    */
    for (size_t i = 0; i < nb_matiere; i++)
    {
        if (strcmp(nom_matiere, liste_mat[i].nom) == 0)
        {
            return i;
        }
    }

    return -1;
}
BOOL epreuve_already_exist(char nom_epreuve[], const Matiere liste_mat[], int nb_matiere,char nom_matiere[])
{
    /*
        renvoie True si l'epreuve existe deja False sinon
    */


   if (get_epreuve_indice( nom_epreuve,  liste_mat,  nb_matiere, nom_matiere)!=-1)
   {
        return True;
   }
   return False;
   
}

int get_epreuve_indice(char nom_epreuve[], const Matiere liste_mat[] , int nb_matiere, char nom_matiere[]){
    int indice = get_matiere_indice(nom_matiere, liste_mat, nb_matiere);
    if (indice != -1)
    {
        for (size_t i = 0; i < liste_mat[indice].nb_epreuve; i++)
        {
            if (strcmp(liste_mat[indice].liste_epreuve[i].nom, nom_epreuve) == 0)
            {
                return i;
            }
        }
    }

    return -1;
}
void affiche_erreur_coeff(int num_erreur)
{
    /*
        num_erreur sera le numero de l'erreur retourner
        par notre fonction verif_coeff . De ce fait on pourra
        traité et afficher l'erreur en question.
    */
    switch (num_erreur)
    {
    case 0:
        printf("Coefficients corrects\n");
        break;
    case 1:
        printf("Le semestre ne contient aucune epreuve\n");
        break;
    case 2:
        printf("Les coefficients d'au moins une UE de ce semestre sont tous nuls\n");
        break;
    default:
        break;
    }
}
void affiche_erreur_note(int num_erreur)
{
    /*
        num_erreur sera le numero de l'erreur retourner
        par notre fonction verif_note . De ce fait on pourra
        traité et afficher l'erreur en question.
    */
    switch (num_erreur)
    {
    case 0:
        printf("Notes correctes\n");
        break;
    case 1:
        printf("Etudiant inconnu\n");
        break;
    case 2:
        printf("Il manque au moins une note pour cet etudiant\n");
        break;
    default:
        break;
    }
}
int verif_coeff(const Formation *ma_formation,int num_semestre){
    int nb_UE = ma_formation->nb_UE;
    int nb_matiere = ma_formation->liste_semestres[num_semestre - 1].nb_matieres;
    const Matiere *liste_mat = ma_formation->liste_semestres[num_semestre - 1].liste_matieres;
    int *tab_UE = NULL; // tab_UE[x] va etre incrementer de 1 a chaque fois que le coefficient d'une UE
                        // d'un semestre donner sera > 0 donc si tab_UE[x]==0
                        // sa voudra dire que tout les coefficients de cet UE de ce semestre sont nul
    tab_UE = calloc(nb_UE, sizeof(int));
    if (tab_UE == NULL)
    {
        printf("allocation echouer");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    for (size_t i = 0; i < nb_matiere; i++)
    { // parcours matieres

        for (size_t j = 0; j < liste_mat[i].nb_epreuve; j++)
        { // parcours epreuves

            for (size_t z = 0; z < nb_UE; z++)
            {
                // parcours UE
                if (liste_mat[i].liste_epreuve[j].tab_coeff_UE[z] > 0.)
                {
                    tab_UE[z] += 1;
                }
            }
        }
    }
    if (nb_matiere == 0)
    {
        return 1; //"Le semestre ne contient aucune epreuve\n"
    }

    for (size_t i = 0; i < nb_UE; i++)
    {
        if (tab_UE[i] == 0)
        {
            return 2; //"Les coefficients d'au moins une UE de ce semestre sont tous nuls\n"
        }
    }
    free(tab_UE);

    return 0; //"Coefficients corrects\n"
}
int verif_note(const Formation * ma_formation ,char nom_etudiant[],const char liste_etudiant[][MAX_CHAR],int num_semestre){
    
    int nb_etudiant=ma_formation->nb_etudiants;
    int indice_etudiant=get_etudiant_indice(nom_etudiant,liste_etudiant,nb_etudiant);
    if (indice_etudiant==-1)
    {
        
        return 1; //"Etudiant inconnu\n"
    }
    int notes_total_semestre=0;
    int notes_total_etudiant=0;
    int nb_matiere=ma_formation->liste_semestres[num_semestre-1].nb_matieres;
    int nb_epreuve;
    for (size_t i = 0; i < nb_matiere; i++)
    {
        nb_epreuve=ma_formation->liste_semestres[num_semestre-1].liste_matieres[i].nb_epreuve;
        notes_total_semestre+=nb_epreuve;
        for (size_t j = 0; j < nb_epreuve; j++)
        {
            if (ma_formation->liste_semestres[num_semestre-1].liste_matieres[i].liste_epreuve[j].listes_notes[indice_etudiant]!=-1)
            {
                notes_total_etudiant+=1;
            }
            
        }
        
    }
    if (notes_total_etudiant!=notes_total_semestre)
    {
        return 2 ; //"Il manque au moins une note pour cet etudiant\n"
    }
    return 0;//"Notes correctes\n"
}
int get_etudiant_indice(char nom_etudiant[],const char liste_etudiant[][MAX_CHAR], int nb_etudiant)
{
    // return l'indice de l'etudiant dans la liste d'etudiant
    // si l'etudiant n'existe pas on renvoie -1
    int indice_etudiant = -1; // si l'etudiant n'est pas dans la liste

    for (size_t i = 0; i < nb_etudiant; i++)
    {
        if (strcmp(liste_etudiant[i], nom_etudiant) == 0)
        {
            indice_etudiant = i;
            break;
        }
    }
    return indice_etudiant;
}
void print_entete_UE(int nb_UE)
{
    /*
        affiche les entetes d'UE dans les commandes 7 et 8
    */

    for (size_t i = 1; i < nb_UE + 1; i++)
    {
        if (i == 1)
        {
            printf("%4s%d", "UE",i);
        }
        else if (i < nb_UE)
        {
            printf("%4s%d", "UE",i);
        }
        else
        {
            printf("%4s%d \n","UE", i);
        }
    }
}
int get_max_space(const Formation *ma_formation,int num_semestre){
    /* renvoie la taille maximale du plus long nom de la matiere de toute la formation*/
    int max=strlen("Moyennes");
    int temp;
    for (size_t j = 0; j < NB_SEMESTRES; j++)
    {
        for (size_t i = 0; i < ma_formation->liste_semestres[j].nb_matieres; i++)
        {
            temp=strlen(ma_formation->liste_semestres[j].liste_matieres[i].nom);
            if (temp>max)
            {
                max=temp;
            }

        
        }
    }
    

    return max;
    
}
char *add_space(char ma_chaine[], int nb_space)
{
    /*
        permet d'ajouté des espaces en fin
        d'une chaine de charactere , cela facilitera
        les affichages dans les commandes 7 et 8
    */
    static char buffer[MAX_CHAR];
    strcpy(buffer, ma_chaine);
    while (strlen(buffer) < nb_space)
    {
        strncat(buffer, " ", 1);
    }
    return buffer;
}


