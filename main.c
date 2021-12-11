#include "parser/parser.h"

char* nom_fichier = "files/test1.pascal";
symbole sym_cour;
FILE* fichier;
IDF table_symboles[MAX_SYMBOLES];
int nbr_symboles;

int main(int argc, char const *argv[])
{
    fichier = fopen(nom_fichier, "r");
    lire_car();
    token_suiv();
    do
    {
        program();
    } while (sym_cour.code != FIN_TOKEN 
        && sym_cour.code != ERREUR_TOKEN);
    printf("programm success\n");
    for (int i = 0; i < nbr_symboles; i++)
    {
        printf("%s = %f\n", table_symboles[i].sym.nom, table_symboles[i].vnum);
    }
    
    return 0;
}
