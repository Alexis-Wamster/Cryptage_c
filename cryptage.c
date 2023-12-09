#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {CODE,DECODE,CLEF} numOption;

/*cette fonction prends en arguments les arguments passé en ligne de commande
Si l'utilisateur ne passe pas les bons arguments, le programme s'arrêteras et décriras les arguments qu'il faut passer
Sinon, la fonction va renvoyer l'option qui à été saisis (soit CODE, soit DECODE, soit CLEF)*/
numOption optionSaisis(int argc, char** argv){
    int option;
    if (argc>=4){
        if (argv[1][0]=='-'){
            if (argv[1][1]=='c' && argv[1][2]=='\0' && argc>4){
            return CODE;
            }
            if (argv[1][1]=='d' && argv[1][2]=='\0' && argc>4){
                return DECODE;
            }
            if (argv[1][1]=='k' && argv[1][2]=='\0'){
                return CLEF;
            }
        }
    }
    printf("Utilisation de %s :\n\n",argv[0]);
    printf("%s -k <fichier_message> <fichier_clef>\n",argv[0]);
    printf("=> creer <fichier_clef> à partir de taille <fichier_message>\n\n");
    printf("%s -c <fichier_message> <fichier_clef> <fichier_code>\n",argv[0]);
    printf("=> creer <fichier_code> à partir de <fichier_clef> et <fichier_message>\n\n");
    printf("%s -d <fichier_message> <fichier_clef> <fichier_code>\n",argv[0]);
    printf("=> creer <fichier_message> à partir de <fichier_clef> et <fichier_code>\n");
    exit(1);
}

/*Cette fonction essaye d'ouvrir un fichier dans un certain mode
Si c'est réussi, la fonction renvoie le FILE* qui correspond
Sinon, le programme s'arrête avec un message d'erreur*/
FILE* ouvrirFichier(char* chemin, char* mode){
    FILE* fichier = fopen(chemin,mode);
    if (fichier==NULL){
        fprintf(stderr,"Erreur: impossible d'ouvrir le fichier \"%s\" en mode \"%s\"\n",chemin,mode);
        exit(1);
    }
    return fichier;
}

/*Cette fonction met un caractere dans un fichier(dont on donne sa valeur ASCII sous forme d'entier compris entre 0 et 255)
Si l'écriture échoue, le programme s'arrête avec un message d'erreur*/
void ecrire(int valeur, FILE* fichier, char* chemin){
    size_t reussite;
    reussite = fputc(valeur,fichier);
    if (reussite==EOF){
        fprintf(stderr,"Echec lors d'une tentative d'écriture dans \"%s\"\n",chemin);
        exit(1);
    }
}

/*Cette fonction remplace la valeur de valeurClef et valeur2 par la valeur des caracteres lu respectivement dans fichierClef et fichier2
Si on arrive à lire un caractere dans fichier2 mais pas dans fichierClef alors le programme s'arrête avec un message d'erreur (car le contenue de fichierClef est trop petit)
Si il n'y a plus de caractere à lire dans fichier2, la fonction renvoie 0 sinon elle renvoie 1*/
int lire2Fichiers(FILE* fichierClef, char* cheminClef, int* valeurClef, FILE* fichier2, char* chemin2, int* valeur2){
    *valeur2 = fgetc(fichier2);
    if (*valeur2==EOF){
        return 0;
    }
    *valeurClef = fgetc(fichierClef);
    if (*valeurClef==EOF){
        fprintf(stderr,"Erreur: la cle \"%s\" est plus petite que le contenue de \"%s\"\n",cheminClef,chemin2);
        exit(1);
    }
    return 1;
}
    
/*cette fonction créer ou modifie un fichier contenant un message codé (comme vu en cours)*/
int code(char* cheminMessage, char* cheminClef, char* cheminCode){
    FILE* fichierMessage = ouvrirFichier(cheminMessage,"r");
    FILE* fichierClef = ouvrirFichier(cheminClef,"r");
    FILE* fichierCode = ouvrirFichier(cheminCode,"w");
    int intMessage;
    int intClef;
    int intCode;

    while (lire2Fichiers(fichierClef, cheminClef, &intClef, fichierMessage, cheminMessage, &intMessage)){
        intCode = (intMessage + intClef)%256;
        ecrire(intCode,fichierCode,cheminCode);
    }
    fclose(fichierMessage);
    fclose(fichierClef);
    fclose(fichierCode);
    return 0;
}

/*cette fonction créer ou modifie un fichier contenant un message décodé (comme vu en cours)*/
int decode(char* cheminMessage, char* cheminClef, char* cheminCode){
    FILE* fichierMessage = ouvrirFichier(cheminMessage,"w");
    FILE* fichierClef = ouvrirFichier(cheminClef,"r");
    FILE* fichierCode = ouvrirFichier(cheminCode,"r");
    int intMessage;
    int intClef;
    int intCode;

    while (lire2Fichiers(fichierClef, cheminClef, &intClef, fichierCode, cheminCode, &intCode)){
        intMessage = (intCode-intClef)%256; //j'ai remarqué que cette opération marchais également avec des nombres négatifs
        ecrire(intMessage, fichierMessage, cheminMessage);
    }

    fclose(fichierMessage);
    fclose(fichierClef);
    fclose(fichierCode);
    return 0;
}

/*cette fonction place des caractere ASCII (entre 0 et 255) choisis aléatoirement dans un fichier clef.
elle en place autant que de caractere contenue dans le message*/
int fabriqueClef(char* cheminMessage, char* cheminClef){
    FILE* fichierMessage = ouvrirFichier(cheminMessage,"r");
    FILE* fichierClef = ouvrirFichier(cheminClef,"w");
    int nbRandom;

    srand(time(NULL));
    while (fgetc(fichierMessage) != EOF){
        nbRandom = rand()%256;
        ecrire(nbRandom,fichierClef,cheminClef);
    }

    fclose(fichierMessage);
    fclose(fichierClef);
    return 0;
}

/*la fonction principale appelle optionSaisis pour savoir quelle option à saisis l'utilisateur.
En fonction de celle ci il execute la fonction aproprié (codage, décodage ou fabrication de clef)*/
int main(int argc, char** argv)
{
    int option = optionSaisis(argc, argv);
    if (option==CODE){
        if (code(argv[2], argv[3], argv[4])==1){
            return 1;
        }
    }
    if (option==DECODE){
        if (decode(argv[2], argv[3], argv[4])==1){
            return 1;
        }
    }
    if (option==CLEF){
        if (fabriqueClef(argv[2], argv[3])==1){
            return 1;
        }
    }
    return EXIT_SUCCESS;
}