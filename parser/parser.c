#include "parser.h"


// redeclaration des variables globales:
char* erreurs[] = {
    // mots clés
    "ID_TOKEN", "NUM_TOKEN", "PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN",
    "IF_TOKEN", "THEN_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN",
    // symboles speciaux
    "PV_TOKEN", "PT_TOKEN", "PLUS_TOKEN", "MOINS_TOKEN", "MULT_TOKEN", "DIV_TOKEN",
    "VIR_TOKEN", "AFF_TOKEN", "INF_TOKEN", "INFEG_TOKEN", "SUP_TOKEN", "SUPEG_TOKEN",
    "DIFF_TOKEN", "PO_TOKEN", "PF_TOKEN", "EG_TOKEN", "FIN_TOKEN", "WHILE_TOKEN",
    // autres
    "CONST_VAR_BEGIN_ERR", "INST_ERREUR", "COND_ERREUR", "FACT_ERREUR", "REDECLARATION_ERREUR",
    "PROGRAM_AFFECT_ERREUR"
}; 
int nbr_symboles;
IDF table_symboles[MAX_SYMBOLES];


// implémentation des fonctions:
void test_symbole(CODES_LEX code) {
    if (sym_cour.code == code) {
        token_suiv();
    }
    else {
        erreur((ERREUR_LEX) code);}
}

void erreur(ERREUR_LEX erreur) {
    printf("erreur trouvé, code: %s\n", erreurs[erreur]);
    exit(-1);
}

void program() {
    test_symbole(PROGRAM_TOKEN);
    // partie sémantique **
    if (sym_cour.code == ID_TOKEN)
    {        
        table_symboles[nbr_symboles].sym.code = ID_TOKEN;
        strcpy(table_symboles[nbr_symboles].sym.nom, sym_cour.nom);
        table_symboles[nbr_symboles].tsym = TPROG;
        nbr_symboles ++;
    }
    // *********************
    test_symbole(ID_TOKEN);
    test_symbole(PV_TOKEN);
    block();
    test_symbole(PT_TOKEN);
}

void block() {
    consts();
    vars();
    insts();
}

boolean trouver_token(char* nom){
    for (int i = 0; i < nbr_symboles + 1; i++)
        if (!strcmp(table_symboles[i].sym.nom, nom)) return TRUE;
    return FALSE;
}

void const_declaration() {
    // partie sémantique ***
    if (sym_cour.code == ID_TOKEN)
    {      
        if (trouver_token(sym_cour.nom) == TRUE)
        erreur(REDECLARATION_ERREUR);  
        table_symboles[nbr_symboles].sym.code = ID_TOKEN;
        strcpy(table_symboles[nbr_symboles].sym.nom, sym_cour.nom);
        table_symboles[nbr_symboles].tsym = TCONST;
        nbr_symboles ++;
    }
    // *********************
    test_symbole(ID_TOKEN);
    test_symbole(EG_TOKEN);
    test_symbole(NUM_TOKEN);
    test_symbole(PV_TOKEN);
}


void consts() {
    switch (sym_cour.code) {
        case CONST_TOKEN:
            token_suiv();
            const_declaration();
            while (sym_cour.code == ID_TOKEN)
                const_declaration();
            break;
        case VAR_TOKEN: break;
        case BEGIN_TOKEN: break;
        default: erreur(CONST_VAR_BEGIN_ERREUR);
    }
}

void variable_declaration(){
    // partie sémantique *******
    if (sym_cour.code == ID_TOKEN)
    {
        if (trouver_token(sym_cour.nom) == TRUE)
            erreur(REDECLARATION_ERREUR);
        table_symboles[nbr_symboles].sym.code = ID_TOKEN;
        strcpy(table_symboles[nbr_symboles].sym.nom, sym_cour.nom);
        table_symboles[nbr_symboles].tsym = TVAR;
        nbr_symboles++;
    }
    // *************************
    test_symbole(ID_TOKEN);
}

void vars() {
    switch (sym_cour.code)
    {
        case VAR_TOKEN:
            token_suiv();
            variable_declaration();
            while (sym_cour.code == VIR_TOKEN) {
                token_suiv();
                variable_declaration();
            }
            test_symbole(PV_TOKEN);
            break;
        case BEGIN_TOKEN: break;
        default: erreur(CONST_VAR_BEGIN_ERREUR);
    }
}

void insts() {

    test_symbole(BEGIN_TOKEN);
    inst();
    while (sym_cour.code == PV_TOKEN) {
        token_suiv();
        inst();
    }
    test_symbole(END_TOKEN);
}

void inst() {
    switch (sym_cour.code)
    {
    case BEGIN_TOKEN:
        insts();
        break;
    case ID_TOKEN:
        affect();
        break;
    case IF_TOKEN:
        si();
        break;
    case WHILE_TOKEN:
        tanque();
        break;
    case WRITE_TOKEN:
        ecrire();
        break;
    case READ_TOKEN:
        lire();
        break;
    case PV_TOKEN: break;
    case END_TOKEN: break;
    default: erreur(INST_ERREUR);
    }
}

void affect() {
    test_symbole(ID_TOKEN);
    test_symbole(AFF_TOKEN);
    if (sym_cour.code == ID_TOKEN)
    for (int i = 0; i < nbr_symboles; i++)
    {
        if(table_symboles[i].tsym == TPROG 
        && !strcmp(sym_cour.nom, table_symboles[i].sym.nom))
        erreur(PROGRAM_AFFECT_ERREUR);
    }
    
    expr();
}

void si() {
    test_symbole(IF_TOKEN);
    cond();
    test_symbole(THEN_TOKEN);
    inst();
}

void tanque() {
    test_symbole(WHILE_TOKEN);
    cond();
    test_symbole(DO_TOKEN);
    inst();
}

void ecrire() {
    test_symbole(WRITE_TOKEN);
    test_symbole(PO_TOKEN);
    expr();
    while (sym_cour.code == VIR_TOKEN)
    {
        token_suiv();
        expr();
    }
    test_symbole(PF_TOKEN);
}

void lire() {
    test_symbole(READ_TOKEN);
    test_symbole(PO_TOKEN);
    test_symbole(ID_TOKEN);
    while (sym_cour.code == VIR_TOKEN)
    {
        token_suiv();
        test_symbole(ID_TOKEN);
    }
    test_symbole(PF_TOKEN);
}

void cond() {
    expr();
    if (sym_cour.code == EG_TOKEN ||
        sym_cour.code == DIFF_TOKEN ||
        sym_cour.code == SUP_TOKEN ||
        sym_cour.code == SUPEG_TOKEN ||
        sym_cour.code == INF_TOKEN ||
        sym_cour.code == INFEG_TOKEN)
        token_suiv();
    else erreur(COND_ERREUR);
    expr();
}

void expr() {
    term();
    if (sym_cour.code == PLUS_TOKEN ||
        sym_cour.code == MOINS_TOKEN) {
        token_suiv();
        term();
    }
}

void term() {
    fact();
    if (sym_cour.code == MULT_TOKEN ||
        sym_cour.code == DIV_TOKEN) {
        token_suiv();
        fact();
    }
}

void fact(){
    switch (sym_cour.code)
    {
    case ID_TOKEN:
        token_suiv();
        break;
    case NUM_TOKEN:
        token_suiv();
        break;
    case PO_TOKEN:
        token_suiv();
        expr();
        test_symbole(PF_TOKEN);
    default:
        erreur(FACT_ERREUR);
        break;
    }
}