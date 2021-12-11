#if !defined(MACRO)
#define MACRO

#include "../scanner/scanner.h"

// les constants:

#define NOMBRE_ERREURS sizeof(erreurs) / sizeof(erreurs[0])
#define MAX_SYMBOLES 50

// enums et structs:

typedef enum _ERREUR_LEX{
    // mots clés
    ID_ERREUR, NUM_ERREUR, PROGRAM_ERREUR, CONST_ERREUR, VAR_ERREUR, BEGIN_ERREUR, END_ERREUR,
    IF_ERREUR, THEN_ERREUR, DO_ERREUR, READ_ERREUR, WRITE_ERREUR,
    // symboles speciaux
    PV_ERREUR, PT_ERREUR, PLUS_ERREUR, MOINS_ERREUR, MULT_ERREUR, DIV_ERREUR,
    VIR_ERREUR, AFF_ERREUR, INF_ERREUR, INFEG_ERREUR, SUP_ERREUR, SUPEG_ERREUR,
    DIFF_ERREUR, PO_ERREUR, PF_ERREUR, EG_ERREUR, FIN_ERREUR, WHILE_ERREUR,
    // autres
    CONST_VAR_BEGIN_ERREUR, INST_ERREUR, COND_ERREUR, FACT_ERREUR, REDECLARATION_ERREUR
    , PROGRAM_AFFECT_ERREUR
} ERREUR_LEX;

typedef enum _TSYM{
    TPROG, TVAR, TCONST
} TSYM;

typedef struct _IDF{
    symbole sym;
    TSYM tsym;
    double vnum;
} IDF;

// identificateur courant:
extern IDF idf_cour;

// tableau des erreurs:
extern char* erreurs[];

// table des symboles
extern IDF table_symboles[MAX_SYMBOLES];
extern int nbr_symboles;

// declaration des fonctions:

void test_symbole(CODES_LEX code);
void erreur(ERREUR_LEX erreur);
void program();
void block();
void consts();
void vars();
void insts();
void inst();
void affect();
void si();
void tanque();
void ecrire();
void lire();
void cond();
void expr();
void term();
void fact();



#endif // MACRO
