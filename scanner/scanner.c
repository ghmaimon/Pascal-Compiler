#include "scanner.h"
#include "../parser/parser.h"

// définition des variables globales (deja définis dans parser.h):
char* mots_cles[] = {
    "id", "num", "program", "const", "var", "begin", "end",
    "if", "then", "do", "read", "write"
};
char* cars_specs[] = {
    ";", ".", "+", "-", "*", "/", ",",
    ":=", "<", "<=", ">", ">=", "<>", "(",
    ")", "=", "EOF"
};
char* tokens[] = {
    // mots clés
    "ID_TOKEN", "NUM_TOKEN", "PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN",
    "IF_TOKEN", "THEN_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN",
    // symboles speciaux
    "PV_TOKEN", "PT_TOKEN", "PLUS_TOKEN", "MOINS_TOKEN", "MULT_TOKEN", "DIV_TOKEN",
    "VIR_TOKEN", "AFF_TOKEN", "INF_TOKEN", "INFEG_TOKEN", "SUP_TOKEN", "SUPEG_TOKEN",
    "DIFF_TOKEN", "PO_TOKEN", "PF_TOKEN", "EG_TOKEN", "FIN_TOKEN", "WHILE_TOKEN",
    "ERREUR_TOKEN"
};

int index_token = 0;
char car_cour;
char nom_cour[MAX_CARS_IN_NOM];
FILE* fichier;
symbole sym_cour;
int nbr_symboles;

// lire un caracter de la fichier
void lire_car() {
    car_cour = fgetc(fichier);
}

boolean lire_mot(){
    // si le premier token n'est pas un caractére on arrête.
    if (!((car_cour <= 'z' && car_cour >= 'a')
    || (car_cour <= 'Z' && car_cour >= 'A')))
    return FALSE;
    while ((car_cour <= 'z' && car_cour >= 'a')
        || (car_cour <= 'Z' && car_cour >= 'A')
        || (car_cour <= '9' && car_cour >= '0')
        || car_cour == '_')
    {   // on lit le token.
        nom_cour[index_token] = tolower(car_cour);
        index_token ++;
        lire_car();
    }

    sym_cour.code = ID_TOKEN;
    strcpy(sym_cour.nom, nom_cour);

    for (int i = 0; i < NOMBRE_MOTS_CLES; i++)
    if (!strcmp(mots_cles[i], nom_cour))
        // si le token est un mot clee on change le code.
        sym_cour.code = (CODES_LEX)i;
    index_token = 0;
    return TRUE; // le token s'agit bien d'une mot.
}

boolean lire_nombre(){
    if (!(car_cour <= '9' && car_cour >= '0'))
    return FALSE;

    while (car_cour <= '9' && car_cour >= '0')
    {   // on lit le token.
        nom_cour[index_token] = car_cour;
        index_token ++;
        lire_car();
    }

    if (car_cour == '.') // c'est le cas des nombre décimales
    {
        nom_cour[index_token] = car_cour;
        index_token ++;
        lire_car();
        
        // doit lire les chiffres après la vérgule.
        if (!(car_cour <= '9' && car_cour >= '0'))
        return FALSE;

        while (car_cour <= '9' && car_cour >= '0')
        {   // on lit le token.
            nom_cour[index_token] = car_cour;
            index_token ++;
            lire_car();
        }
    }
    sym_cour.code = NUM_TOKEN;
    strcpy(sym_cour.nom, nom_cour);
    table_symboles[nbr_symboles - 1].vnum = atof(sym_cour.nom);
    index_token = 0;
    return TRUE;
}

boolean lire_special(){
    if (car_cour == EOF){
        memset(nom_cour, 0, sizeof(nom_cour));
        sym_cour.code = FIN_TOKEN;
        lire_car();
        return TRUE;
    }
    for (int i = 0; i < NOMBRE_CARS_SPEC; i++)
    if (cars_specs[i][0] == car_cour) {
        if (strlen(cars_specs[i]) == 1){
            strcpy(sym_cour.nom, cars_specs[i]);
            sym_cour.code = (CODES_LEX)(i + NOMBRE_MOTS_CLES);
            lire_car();
            return TRUE;
        }
        else {
            // le cas des symboles speciales de 2 caracteres.
            lire_car();
            if (cars_specs[i][1] == car_cour){
                strcpy(sym_cour.nom, cars_specs[i]);
                sym_cour.code = (CODES_LEX)(i + NOMBRE_MOTS_CLES);
                lire_car();
                return TRUE;
            }
        }
    }
    return FALSE;
}

boolean lire_errones () {
    sym_cour.code = ERREUR_TOKEN;
    strcpy(sym_cour.nom, nom_cour);
    lire_car();
    return TRUE;
}

void ignor_whitespaces() {
    while (car_cour == ' ' || car_cour == '\n' || car_cour == '\t')
    {
        lire_car();
    }
}

void token_suiv() {
    memset(nom_cour, 0, sizeof(nom_cour));
    ignor_whitespaces();
    if (lire_special() == FALSE)
    if (lire_nombre() == FALSE)
    if (lire_mot() == FALSE)
    lire_errones();
}
