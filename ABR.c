#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp4.h"

Patient* CreerPatient(char* nm, char* pr){
    Patient* p = malloc(sizeof(Patient));
    p->nom = nm;
    p->prenom = pr;
    p->ListeConsult = NULL;
    p->nbrconsult = 0;
    p->fils_gauche = NULL;
    p->fils_droit = NULL;
    return p;
}

void inserer_patient(Parbre * abr, char * nm, char * pr){
    Patient* p = CreerPatient(nm, pr);
    if((*abr) == NULL){
        // printf("inside \"abr==NULL\"\n");
        *abr = p;
        // printf("abr null ? : %d\n", *abr==NULL);
        // printf("nom = %s\n",(*abr)->nom);
        return;
    }
    // printf("after \"abr==NULL\"\n");
    Patient* a = malloc(sizeof(Patient));
    a = *abr;
    Patient* precedent;
    int i=0;
    do{
        precedent = a;
        if(a->nom[i]=='\0' || p->nom[i]=='\0' || a->nom[i] > p->nom[i]){
            a = a->fils_gauche;
            if(a==NULL){
                precedent->fils_gauche = p;
                break;
            }
            i=0;
        }
        else if (a->nom[i]=='\0' || p->nom[i]=='\0' || a->nom[i] < p->nom[i]){
            a = a->fils_droit;
            if(a==NULL){
                precedent->fils_droit = p;
                break;
            }
            i=0;
        }
        else i++;
    }while(a != NULL);

    //transformation pour garder un ABR
    free(a);
}

Patient * rechercher_patient(Parbre * abr, char* nm){
    if(*abr==NULL){ printf("null"); return NULL;}
    if((*abr)->nom == nm) return *abr;
    
    int i=0;
    do{
        if((*abr)->nom[i] > nm[i]){
            return rechercher_patient(&(*abr)->fils_gauche, nm);
            i=0;
        }
        else if ((*abr)->nom[i] < nm[i]){
            return rechercher_patient(&(*abr)->fils_droit, nm);
            i=0;
        }
        else i++;
    }while((*abr)->nom[i]!='\0' || nm[i]!='\0');
    if((*abr)->nom == nm) return *abr;
}

void afficher_fiche(Parbre * abr, char* nm){
    Patient* p = rechercher_patient(abr, nm);

    printf("Nom : %s", p->nom);

    printf("\nPrenom : %s", p->prenom);

    printf("\nNombre de consultations : %d\n", p->nbrconsult);

    Consultation* consult = malloc(sizeof(Consultation));
    consult = p->ListeConsult;
    for(int i = 1; i <= p->nbrconsult; i++){
        printf("Consultation n°%d : date=%s ; motif=%s ; niveau d'urgence=%d\n", i, consult->date, consult->motif, consult->niveauUrg);
    }
}

void afficher_patients(Parbre * abr){
    if((*abr) != NULL){
        printf("Nom : %s \t Prenom : %s\n", (*abr)->nom, (*abr)->prenom);
        
        afficher_patients(&((*abr)->fils_droit));
        afficher_patients(&((*abr)->fils_gauche));
        
        return;
    }
    else return;
}

Consultation * CreerConsult(char * date, char* motif, int nivu){
    Consultation* consult = malloc(sizeof(Consultation));
    consult->date = date;
    consult->motif = motif;
    consult->niveauUrg = nivu;
    consult->suivant = NULL;
    return consult;
}

void ajouter_consultation(Parbre * abr, char * nm, char * date, char* motif, int nivu){
    Patient* p = rechercher_patient(abr, nm);
    Consultation* consult = CreerConsult(date, motif, nivu);
    consult->suivant = p->ListeConsult;
    p->ListeConsult = consult;
    p->nbrconsult ++;
}


//------------------------------------------------------------
//------------- Fonction de service ajoutee -----------------
Patient * rechercher_patient_precedent(Parbre * abr, char* nm, Patient* precedent)
{
    if((*abr)==NULL) return NULL;
    else if((*abr)->nom == nm) return precedent;
    int i=0;
    do{
        if((*abr)->nom[i] > nm[i]){
            return rechercher_patient_precedent(&((*abr)->fils_gauche), nm, *abr);
            i=0;
        }
        else if ((*abr)->nom[i] < nm[i]){
            return rechercher_patient_precedent(&((*abr)->fils_droit), nm, *abr);
            i=0;
        }
        else i++;
    }while((*abr)->nom[i]!='\0' || nm[i]!='\0');
    
    if((*abr)->nom == nm) return precedent;
}

//------------------------------------------------------------
//------------- Fonction de service ajoutee -----------------
Patient* nearestPatient(Parbre* abr, Parbre* arbre_complet){   //abr=fils gauche sur noeud a supprimer
    /*retourne le patient le plus proche du noeud a supprimer et fait pointer son patient precedent sur NULL*/
    
    if((*abr)->fils_droit == NULL){
            Patient* precedent = malloc(sizeof(Patient));
            precedent = rechercher_patient_precedent(arbre_complet,(*abr)->nom, NULL);
            precedent->fils_droit = NULL;
            return *abr;
    }
    else nearestPatient(&((*abr)->fils_droit), arbre_complet);
}

//------------------------------------------------------------

void supprimer_patient(Parbre * abr, char* nm){
    printf("ligne 1 supprimer patient");
    if((*abr) == NULL){ printf("*abr null ?", *abr==NULL); return;}

    Patient* p = malloc(sizeof(Patient));
    p = NULL;
    p = rechercher_patient(abr,nm);
    if(p == NULL) return;
    
    Patient* precedent = malloc(sizeof(Patient));
    precedent = NULL;
    precedent = rechercher_patient_precedent(abr,nm,NULL);
    Patient* remplacent = malloc(sizeof(Patient));
    remplacent = NULL;

    if(p->fils_gauche->fils_droit == NULL){
        remplacent = p->fils_gauche;
        p->fils_gauche = NULL;
    }
    else if(p->fils_gauche->fils_droit != NULL) remplacent = nearestPatient(&(p->fils_gauche),abr);
    else if(precedent != NULL) remplacent = precedent;
    else {
        (*abr) = p->fils_droit;
        return;
    }

    if(precedent == NULL) *abr = remplacent;
    else if(precedent->fils_droit->nom == p->nom) precedent->fils_droit = remplacent;
    else if(precedent->fils_gauche->nom == p->nom) precedent->fils_gauche = remplacent;
    else printf("Erreur dans la recherche du patient precedent");

    remplacent->fils_droit = p->fils_droit;
    remplacent->fils_gauche = p->fils_gauche;
        
    for(int k=0; k<p->nbrconsult; k++) free(&(p->ListeConsult[k]));
    free(p);
}

//-----------------------------------------------------------
//------------- Fonction de service ajoutee -----------------
void inserer_patient_MAJ(Parbre* abr, Patient* p){
    if((*abr) == NULL){
        *abr = p;
        return;
    }
    Patient* a = malloc(sizeof(Patient));
    a= *abr;
    Patient* precedent;
    int i=0;
    do{
        precedent = a;
        if(a->nom[i] > p->nom[i]){
            a = a->fils_gauche;
            if(a==NULL){
                precedent->fils_gauche = p;
                break;
            }
            i=0;
        }
        else if (a->nom[i] < p->nom[i]){
            a = a->fils_droit;
            if(a==NULL){
                precedent->fils_droit = p;
                break;
            }
            i=0;
        }
        else i++;
    }while(a != NULL);

    //transformation pour garder un ABR
}

void maj(Parbre * abr, Parbre * abr2){
    if(abr==NULL) return;

    else if(rechercher_patient(abr2,(*abr)->nom) == NULL){
        Patient* p = CreerPatient((*abr)->nom,(*abr)->prenom);
        p->nbrconsult = (*abr)->nbrconsult;
        p->ListeConsult = (*abr)->ListeConsult;
        inserer_patient_MAJ(abr2, p);
    }
    maj(&((*abr)->fils_droit), abr2);
    maj(&((*abr)->fils_gauche), abr2);
}

int main(){
    Parbre* abr;
    abr = malloc(sizeof(Parbre));
    *abr = NULL;
    
    char* nomB = "DupondB";
    char* prenomB = "JeanB";
    inserer_patient(abr,nomB, prenomB);
    //printf("abr null ? : %d\n", *abr==NULL);

    char* nomE = "DupondE";
    char* prenomE = "JeanE";
    inserer_patient(abr,nomE, prenomE);
    //printf("abr null ? : %d\n", *abr==NULL);

    char* nomC = "DupondC";
    char* prenomC = "JeanC";
    inserer_patient(abr,nomC, prenomC);
    //printf("abr null ? : %d\n", *abr==NULL);

    char* nomA = "DupondA";
    char* prenomA = "JeanA";
    inserer_patient(abr,nomA, prenomA);
    //printf("abr null ? : %d\n", *abr==NULL);

    Patient* pA = malloc(sizeof(Patient));
    pA = NULL;
    pA = rechercher_patient(abr, nomA);
    if(pA!=NULL) printf("****recherche réussi****\n");
    afficher_fiche(abr, nomA);
    printf("****afficher fiche résussi****\n");
    afficher_patients(abr);

    char* date = "2022-11-16";
    char* motif = "covid";
    ajouter_consultation(abr, nomA, date, motif, 1);
    afficher_fiche(abr, nomA);

    int i=0;
    printf("***supprimer patient main***\n");
    while((*abr) != NULL){
        printf("int %d\n",i);
        printf("(*abr)->nom = %s\n", (*abr)->nom);

        supprimer_patient(abr,(*abr)->nom);
        printf("int %d.1\n",i);
        printf("abr == null ? : %d\n", (*abr)==NULL);
        afficher_patients(abr);
        printf("---------%d----------\n",i);
        printf("--------------------\n");
        i++;
    }
    printf("? ? ?\n");

    return 1;
}
