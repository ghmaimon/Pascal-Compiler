#if !defined(_SCANNER)
#define _SCANNER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// les constants

#define MAX_CARS_IN_NOM 20
#define NOMBRE_MOTS_CLES sizeof(mots_cles) / sizeof(mots_cles[0])
#define NOMBRE_CARS_SPEC sizeof(cars_specs) / sizeof(cars_specs[0])

// definition du boolean:
typedef enum {
    TRUE, FALSE
} boolean;

// codes des symboles:

typedef enum _CODES_LEX{
    // mots clés
    ID_TOKEN, NUM_TOKEN, PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN, BEGIN_TOKEN, END_TOKEN,
    IF_TOKEN, THEN_TOKEN, DO_TOKEN, READ_TOKEN, WRITE_TOKEN,
    // symboles speciaux
    PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN, MULT_TOKEN, DIV_TOKEN,
    VIR_TOKEN, AFF_TOKEN, INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN,
    DIFF_TOKEN, PO_TOKEN, PF_TOKEN, EG_TOKEN, FIN_TOKEN, WHILE_TOKEN,
    // symbole erronée
    ERREUR_TOKEN
} CODES_LEX;

// structure d'une symbole sortant de l'analyseur lexical:
typedef struct _SYMBOLE{
    CODES_LEX code;
    char nom[MAX_CARS_IN_NOM];
} symbole;

// le tableau des mots clés:
extern char* mots_cles[];
// le tableau des caracters spéciaux
extern char* cars_specs[]; 
// le tableau de toutes les tokens en chaînes de caractéres.
extern char* tokens[];

// les fonctions:
void lire_car();
void token_suiv();
void ignor_whitespaces();
boolean lire_mot();
boolean lire_nombre();
boolean lire_special();
boolean lire_errones();


// les variables globales:
extern char car_cour;
extern char nom_cour[MAX_CARS_IN_NOM];
extern int index_token;
extern char* nom_fichier;
extern FILE* fichier;
extern symbole sym_cour;


#endif // _SCANNER
