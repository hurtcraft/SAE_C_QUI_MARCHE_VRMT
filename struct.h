#ifndef __STRUCT_H
#define __STRUCT_H


enum
{
    NB_SEMESTRES = 2,
    MIN_UE = 3,
    MAX_UE = 6,
    MAX_MATIERES = 10,
    MAX_EPREUVES = 5,
    MAX_ETUDIANTS = 100,
    MAX_CHAR = 30
};
const float MIN_NOTE = 0.f, MAX_NOTE = 20.f;
typedef enum BOOL
{
    False,
    True,
} BOOL;

typedef struct Epreuve
{
    char nom[MAX_CHAR];
    float tab_coeff_UE[MAX_UE];
    float listes_notes[MAX_ETUDIANTS];
} Epreuve;

typedef struct Matiere
{
    char nom[MAX_CHAR];
    unsigned int nb_epreuve;
    Epreuve liste_epreuve[MAX_EPREUVES];
} Matiere;
typedef struct Semestre
{
    unsigned int nb_matieres;
    Matiere liste_matieres[MAX_MATIERES];
} Semestre;
typedef struct Formation
{
    unsigned int nb_UE;
    BOOL nb_UE_is_def;
    Semestre liste_semestres[NB_SEMESTRES];

    int nb_etudiants;
} Formation;
#endif 