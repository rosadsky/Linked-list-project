#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define NSTR 100
#define SUBOR "filmy.txt"
#define N 25

typedef struct meno{
    char meno[NSTR];
    char priezvisko[NSTR];
} MENO;

typedef struct herec {
    MENO herec;
    int rok;
    struct herec *dalsi;
} HEREC;

typedef struct film{
    char nazovFilmu[NSTR];
    int rokVyroby;
    MENO reziser;
    HEREC *herci;
    struct film *dalsif;
}FILM;

/* - NACITAJ - táto funkcia načítaj filmy z definovaného súboru, vracia spájaný zoznam filmy */

FILM *nacitaj(FILM *filmy){
    FILE *f;

    char c = 0;
    char str[20];
    int n = 0;
    int podmienka=0;


    FILM *pomocnaFilm, *start, *filmy_vymaz,*temp;

    HEREC *pomocnaHerec,*nasledujuciHerec,*tempHerec;


    if((f = fopen(SUBOR, "r")) == NULL) {
        printf("Nepodarilo sa otvorit subor\n");
        return 0;
    }

    filmy_vymaz = filmy;

    if (filmy_vymaz != NULL) {      //uvolnenie pamate ak už bol zoznam raz načítaný


        while (filmy_vymaz->dalsif != NULL){ //uvolnenie pamati pre filmy
            while (filmy_vymaz->herci!= NULL) { //uvolnenie pamati pre hercov

                tempHerec = filmy_vymaz->herci->dalsi;
                filmy_vymaz->herci = tempHerec;
                free(filmy_vymaz->herci);

            }

            temp = filmy_vymaz->dalsif;
            filmy_vymaz->dalsif = temp->dalsif;
            free(temp);
        }


    }



    while((c = fgetc(f)) != EOF) {
        ungetc(c,f);


        /* načítanie pre prvý začiatočný film */

        if (c!='*'){

            if (n==1){ //podmienka pre zastavenie na hviezdičkách pre prvých hercov
                podmienka = 1;
            } else{


                filmy = (FILM*) malloc(sizeof(FILM));

                fscanf(f, "%[^\n]%*c", filmy->nazovFilmu); //NAZOV FILMU
                fscanf(f, "%d", &filmy->rokVyroby); //ROK FILMU
                fscanf(f, "%s", filmy->reziser.meno); //MENO REZISER
                fscanf(f, "%s", filmy->reziser.priezvisko); //PRIEZVISKO REZISER

                fseek(f,1,SEEK_CUR); //UPRACEM POINTER

                filmy->herci = NULL;
            }
        }

        if(c=='*'){ //NAČÍTAVANIE HERCOV

            n = 1;

            pomocnaHerec = (HEREC*) malloc(sizeof(HEREC));

            fscanf(f, "%s", str); //IGNORE HVIEZDA
            fscanf(f, "%s", pomocnaHerec->herec.meno); //MENO
            fscanf(f, "%s", pomocnaHerec->herec.priezvisko); //PRIEZVISKO
            fscanf(f, "%d", &pomocnaHerec->rok); //ROK

            fseek(f,1,SEEK_CUR); //UPRATAT POINTER


            //vytváranie spájaného zoznamu
            if(filmy->herci==NULL){
                filmy->herci = pomocnaHerec;

            } else {

                nasledujuciHerec = filmy->herci;

                while(nasledujuciHerec->dalsi != NULL){
                    nasledujuciHerec = nasledujuciHerec->dalsi;

                }
                nasledujuciHerec->dalsi = pomocnaHerec;

            }
        }



        filmy->dalsif = NULL;
        start = filmy; //START - ZAČIATOČNÁ PREMENNá

        if(podmienka==1) { //načítame 2 film a vyššie
            while ((c = fgetc(f)) != EOF) {
                ungetc(c,f);
                if (c!='*'){

                    pomocnaFilm = (FILM*) malloc(sizeof(FILM));

                    fscanf(f, "%[^\n]%*c", pomocnaFilm->nazovFilmu); //NAZOV FILMU
                    fscanf(f, "%d", &pomocnaFilm->rokVyroby); //ROK FILMU
                    fscanf(f, "%s", pomocnaFilm->reziser.meno); //MENO REZISER
                    fscanf(f, "%s", pomocnaFilm->reziser.priezvisko); //PRIEZVISKO REZISER

                    fseek(f,1,SEEK_CUR); //UPRACEM POINTER

                }

                if(c=='*'){ //NAČÍTAVANIE HERCOV

                    pomocnaHerec = (HEREC*) malloc(sizeof(HEREC)); //malloc pre hercov

                    fscanf(f, "%s", str); //IGNORE HVIEZDA

                    fscanf(f, "%s", pomocnaHerec->herec.meno); //MENO
                    fscanf(f, "%s", pomocnaHerec->herec.priezvisko); //PRIEZVISKO
                    fscanf(f, "%d", &pomocnaHerec->rok); //ROK

                    fseek(f,1,SEEK_CUR); //UPRATAT POINTER


                    //vytváranie spájaného zoznamu
                    if(filmy->herci==NULL){
                        filmy->herci = pomocnaHerec; //

                    } else {
                        nasledujuciHerec = filmy->herci;
                        while(nasledujuciHerec->dalsi != NULL){
                            nasledujuciHerec = nasledujuciHerec->dalsi;
                        }
                        nasledujuciHerec->dalsi = pomocnaHerec;

                    }
                }
                //vytváranie spájaného zoznamu
                pomocnaFilm->dalsif = NULL;
                filmy->dalsif = pomocnaFilm;
                filmy = filmy->dalsif;

                pomocnaFilm->dalsif = NULL; // nakoniec dať NULL nech sa prohgram zastaví pri while loope

            }
        }

    }

    filmy = start;

    return filmy; //vraciame prvý záznam v spájanom zozname

}

/* - VYPIS - vypis spájaného zoznamu */

void vypis(FILM *filmy){

    FILM *zaciatokFilmy;

    HEREC *zaciatokHerci;

    int pomocnaVypis=0;


    zaciatokFilmy = filmy;

    while(filmy != NULL){ //vypis filmov

        printf("%s ",filmy->nazovFilmu);
        printf("(%d) ",filmy->rokVyroby);
        printf("%s ",filmy->reziser.meno);
        printf("%s \n",filmy->reziser.priezvisko);
        printf("\t Hraju: ");
        while(filmy->herci != NULL) { //vypis hercov
            if(pomocnaVypis == 0){
                zaciatokHerci = filmy->herci; //ulozíme si prvé miesto v spájanom zozname
            }
            pomocnaVypis++;
            printf("%s ",filmy->herci->herec.meno);
            printf("%s ",filmy->herci->herec.priezvisko);
            printf("(%d)",filmy->herci->rok);

            if(filmy->herci->dalsi == NULL){
                printf(" ");
            } else {
                printf(", ");
            }


            filmy->herci = filmy->herci->dalsi; //posúvam sa v zozname




        }
        printf("\n");
        filmy->herci = zaciatokHerci;
        pomocnaVypis = 0;
        filmy = filmy->dalsif;
    }

    filmy = zaciatokFilmy;

}

/* - PRIDAJ - pridáme film do spájaného zoznamu
 *
 * VSTUP:
 *
 *  NAZOV FILMU
 *  ROK
 *  REZISER.M REZISER.P
 *  HEREC.M HEREC.P ROK
 *
 *  UKONČENIE *
 *
 **/

void pridaj(FILM *filmy){
    FILM *dalsiFilm, *pomocna;
    HEREC *dalsiHerec, *pomocnaHerec;

    char str;
    char ch;

    dalsiFilm = (FILM*)malloc(sizeof(FILM));

    if(dalsiFilm == NULL) {
        printf("Nemožem pridať film.\n");
    }
    else {

        scanf("%c",&str);

        scanf("%[^\n]",dalsiFilm->nazovFilmu);

        scanf("%d",&dalsiFilm->rokVyroby);
        scanf("%s",dalsiFilm->reziser.meno);
        scanf("%s",dalsiFilm->reziser.priezvisko);


        while ((ch = getchar()) != '*') { //nacitavam vstupy

            dalsiHerec = (HEREC*) malloc(sizeof(HEREC));

            scanf("%s", dalsiHerec->herec.meno);

            if(strcmp(dalsiHerec->herec.meno,"*")==0) //ukončenie programu pri hviezdičke
                break;

            scanf("%s", dalsiHerec->herec.priezvisko);
            scanf("%d", &dalsiHerec->rok); //ROK

            //pridanie do spájaného zoznamu
            if(dalsiFilm->herci == NULL){
                dalsiFilm->herci = dalsiHerec;
            } else {
                pomocnaHerec = dalsiFilm->herci;
                while(pomocnaHerec->dalsi != NULL){
                    pomocnaHerec = pomocnaHerec->dalsi;
                }
                pomocnaHerec->dalsi = dalsiHerec;
            }
        }

        dalsiFilm->dalsif = NULL;

        pomocna = filmy;

        while(pomocna->dalsif != NULL)
            pomocna = pomocna->dalsif;

        pomocna->dalsif = dalsiFilm;

    }
}

/* - VYMAZ - vymazanie filmu zo zoznamu hercov
 *
 * VSTUP
 *
 * NAZOV FILMU
 *
 * */

void vymaz (FILM *filmy){
    FILM *temp;
    char c;
    char vymaz[50];

    scanf("%c",&c);
    scanf("%[^\n]",vymaz); //nacitavam celý riadok


    while(filmy != NULL){ //najdenie herca v spájanom zozname ktorého chceme vymazať

        if(strcmp(filmy->dalsif->nazovFilmu,vymaz)==0)
            break;

        filmy = filmy->dalsif;
    }


    temp = filmy->dalsif;
    filmy->dalsif = temp->dalsif; //vymazanie herca


    while(temp->herci != NULL){ //uvolnenie pamate pre hercov
        free(temp->herci);
        temp->herci = temp->herci->dalsi;
    }


    free(temp); //uvolnenie filmu

}

/* - FILMY - nájdenie filmou v ktorých hral daný herec
 *
 * VSTUP:
 *
 * HEREC.M HEREC.P
 *
 * */

void najdiFilmy (FILM *filmy){


    HEREC *zaciatokHerci;

    int pomocnaVypis=0;
    char vypisMeno[20];
    char vypisPriezvisko[20];
    char c;

    scanf("%c",&c);
    scanf("%s",vypisMeno);
    scanf("%s",vypisPriezvisko);

    while(filmy != NULL){ //pohyb v spajanom zozname pre filmy
        while(filmy->herci != NULL) { //pohyb v spajanom zozname pre hercov
            if(pomocnaVypis == 0)
                zaciatokHerci = filmy->herci;

            pomocnaVypis++;

            if(strcmp(filmy->herci->herec.meno,vypisMeno)==0 &&
               strcmp(filmy->herci->herec.priezvisko,vypisPriezvisko)==0) { //porovnávame hercove meno,priezvisko ak je to
                printf("%s",filmy->nazovFilmu);                          // true vypíše sa film + rok
                printf(" (%d)\n",filmy->rokVyroby);
            }


            filmy->herci = filmy->herci->dalsi;
        }

        filmy->herci = zaciatokHerci;
        pomocnaVypis = 0;
        filmy = filmy->dalsif;
    }

}

/* - HERCI - výpis hercov, ktorí hrali v dvoch vybraných filmoch
 *
 * VSTUP:
 *
 * NAZOV FILMU
 * NAZOV FILMU
 *
 * */

void najdiHercov (FILM *filmy) {

    HEREC *zaciatokHerci,*zaciatokHerciDruhy;

    FILM *pomocnaprvyFilm,*pomocnadruhyFilm;

    int pomocnaVypis=0;
    int pomocnaVypisdruha=0;
    char prvyFilm[20];
    char druhyFilm[20];
    char c;


    scanf("%c",&c);
    scanf("%[^\n]\n",prvyFilm);
    scanf("%[^\n]",druhyFilm);

    pomocnaprvyFilm = filmy;
    pomocnadruhyFilm = filmy;


    while(pomocnaprvyFilm != NULL){ // idem do štruktúry kde je prvý film
        if(strcmp(pomocnaprvyFilm->nazovFilmu,prvyFilm)==0){
            break;
        }
        pomocnaprvyFilm = pomocnaprvyFilm->dalsif;
    }

    while(pomocnadruhyFilm != NULL){ // idem do štruktúry kde je druhý film
        if(strcmp(pomocnadruhyFilm->nazovFilmu,druhyFilm)==0){
            break;
        }
        pomocnadruhyFilm = pomocnadruhyFilm->dalsif;
    }


    while(pomocnaprvyFilm->herci != NULL) { //porovnavam prvý film s druhým
        if(pomocnaVypisdruha == 0){
            zaciatokHerciDruhy = pomocnaprvyFilm->herci;
        }
        pomocnaVypisdruha++;
        while(pomocnadruhyFilm->herci != NULL) { //porovnávam druhý film s prvým
            if(pomocnaVypis == 0){
                zaciatokHerci = pomocnadruhyFilm->herci;
            }
            pomocnaVypis++;

            //podmienka ak sa nájde herec ktorý hra v oboch filmoch vypíše sa jeho meno priezvisko rok
            if(strcmp(pomocnadruhyFilm->herci->herec.meno,pomocnaprvyFilm->herci->herec.meno)==0 &&
               strcmp(pomocnadruhyFilm->herci->herec.priezvisko,pomocnaprvyFilm->herci->herec.priezvisko)==0){

                printf("%s ",pomocnadruhyFilm->herci->herec.meno);
                printf("%s ",pomocnadruhyFilm->herci->herec.priezvisko);
                printf("(%d)\n",pomocnadruhyFilm->herci->rok);

            }

            pomocnadruhyFilm->herci = pomocnadruhyFilm->herci->dalsi;
        }

        pomocnadruhyFilm->herci = zaciatokHerci;
        pomocnaprvyFilm->herci = pomocnaprvyFilm->herci->dalsi;
    }

    pomocnaprvyFilm->herci = zaciatokHerciDruhy;

}

/* - ROK - výpis hercov, ktorí hrali vo filmoch v daných rokoch a následné zoradenie
 *
 *  zoradenie podla priezviska -> mena -> roka
 *
 *  VSTUP
 *
 *  ROK FILMOV
 *
 * */

void rok (FILM *filmy) {

    HEREC *zaciatokHerci;

    HEREC sortPole[25];

    FILM *zaciatok;

    int i = 0;
    int sortRok=0;
    char tempSort[25];

    int pomocnaVypis=0;

    zaciatok = filmy;

    scanf("%d",&sortRok);

    while(filmy != NULL){ // pohyb po filmoch
        if(filmy->rokVyroby == sortRok) { // ak sa rok filmu rok zadaného
            while (filmy->herci != NULL) {
                if (pomocnaVypis == 0) {
                    zaciatokHerci = filmy->herci;
                }
                pomocnaVypis++;

                // prepíšeme si všetkých hercov do našeho pomocného poľa
                strcpy(sortPole[i].herec.meno, filmy->herci->herec.meno);
                strcpy(sortPole[i].herec.priezvisko, filmy->herci->herec.priezvisko);
                sortPole[i].rok = filmy->herci->rok;

                filmy->herci = filmy->herci->dalsi;

                i++;

            }
            filmy->herci = zaciatokHerci;
        }

        pomocnaVypis = 0;
        filmy = filmy->dalsif;
    }



    int max;

    max = i;

    //Vymazávame daného herca ktorý sa opakuje dva krát
    for (int j = 0; j < max ; j++) {
        for (int v = j+1; v < max ; v++) {
            if (strcmp(sortPole[j].herec.priezvisko, sortPole[v].herec.priezvisko) == 0 &&
                strcmp(sortPole[j].herec.meno, sortPole[v].herec.meno) == 0 &&
                (sortPole[j].rok == sortPole[v].rok )
                    ) {

                for(int vymaz = v; vymaz<max ; vymaz++){
                    strcpy(sortPole[vymaz].herec.priezvisko,sortPole[vymaz+1].herec.priezvisko);
                    strcpy(sortPole[vymaz].herec.meno,sortPole[vymaz+1].herec.meno);
                    sortPole[vymaz].rok = sortPole[vymaz+1].rok;
                }
                max--;
            }

        }

    }


    //Zoradujeme hercou len podla priezviska od najmenšieho po najmenší
    for(i=0;i<=max;i++) {
        for (int j = i + 1; j <= max-1 ; j++) {
            if (strcmp(sortPole[i].herec.priezvisko, sortPole[j].herec.priezvisko) > 0) {
                //vymiename hodnoty medzi sebou
                int temp = sortPole[i].rok;
                sortPole[i].rok = sortPole[j].rok;
                sortPole[j].rok = temp;

                strcpy(tempSort, sortPole[i].herec.priezvisko);
                strcpy(sortPole[i].herec.priezvisko, sortPole[j].herec.priezvisko);
                strcpy(sortPole[j].herec.priezvisko, tempSort);

                strcpy(tempSort, sortPole[i].herec.meno);
                strcpy(sortPole[i].herec.meno, sortPole[j].herec.meno);
                strcpy(sortPole[j].herec.meno, tempSort);

            }

        }
    }




    // skontrolujeme či sa herec náhodou s rovnakým priezviskom nenachádza v poli ak áno zmeníme poradie

    // taktiež skontrolujeme či herec s rovnakým menom a priezviskom sa nachádza v poli ak áno zmenime poradie


    for (int i = 0; i < max ; i++) {
        for (int j = i + 1; j <= max-1 ; j++) {
            if (strcmp(sortPole[i].herec.priezvisko, sortPole[j].herec.priezvisko) == 0) {
                if (strcmp(sortPole[i].herec.meno, sortPole[j].herec.meno) > 0) {
                    //vymiename hodnoty medzi sebou
                    int temp = sortPole[i].rok;
                    sortPole[i].rok = sortPole[j].rok;
                    sortPole[j].rok = temp;

                    strcpy(tempSort, sortPole[i].herec.priezvisko);
                    strcpy(sortPole[i].herec.priezvisko, sortPole[j].herec.priezvisko);
                    strcpy(sortPole[j].herec.priezvisko, tempSort);

                    strcpy(tempSort, sortPole[i].herec.meno);
                    strcpy(sortPole[i].herec.meno, sortPole[j].herec.meno);
                    strcpy(sortPole[j].herec.meno, tempSort);




                }

                if (strcmp(sortPole[i].herec.meno, sortPole[j].herec.meno) == 0) {
                    if(sortPole[i].rok > sortPole[j].rok ) {
                        //vymiename hodnoty medzi sebou
                        int temp = sortPole[i].rok;
                        sortPole[i].rok = sortPole[j].rok;
                        sortPole[j].rok = temp;

                        strcpy(tempSort, sortPole[i].herec.priezvisko);
                        strcpy(sortPole[i].herec.priezvisko, sortPole[j].herec.priezvisko);
                        strcpy(sortPole[j].herec.priezvisko, tempSort);

                        strcpy(tempSort, sortPole[i].herec.meno);
                        strcpy(sortPole[i].herec.meno, sortPole[j].herec.meno);
                        strcpy(sortPole[j].herec.meno, tempSort);

                    }
                }
            }
        }
    }


    for (int i = 0; i < max ; i++) {
        printf("%s ",sortPole[i].herec.meno);
        printf("%s ",sortPole[i].herec.priezvisko);
        printf("(%d)",sortPole[i].rok);
        if (i < max-1){
            printf(", ");
        } else {
            printf(" ");
        }


    }
    printf("\n");


    filmy = zaciatok;
}

/* - KONIEC - ukončenie program a následné uvolnenie pamate */

void koniec(FILM *filmy_vymaz){

    FILM *temp;

    HEREC *tempHerec;

    while (filmy_vymaz->dalsif != NULL){ //uvolnenie pamati pre filmy
        while (filmy_vymaz->herci!= NULL) { //uvolnenie pamati pre hercov

            tempHerec = filmy_vymaz->herci->dalsi;
            free(filmy_vymaz->herci);
            filmy_vymaz->herci = tempHerec;

        }

        temp = filmy_vymaz->dalsif;
        filmy_vymaz->dalsif = temp->dalsif;
        free(temp);
    }

}

int main() {
    FILM *filmy = NULL;

    char menu[10];

    while (scanf("%s",menu) == 1  ) {

        if(strcmp(menu,"nacitaj") == 0)
            filmy = nacitaj(filmy);

        if(strcmp(menu,"vypis") == 0)
            vypis(filmy);

        if(strcmp(menu,"pridaj") == 0)
            pridaj(filmy);

        if(strcmp(menu,"vymaz") == 0)
            vymaz(filmy);

        if(strcmp(menu,"filmy") == 0)
            najdiFilmy(filmy);

        if(strcmp(menu,"herci") == 0)
            najdiHercov(filmy);

        if(strcmp(menu,"rok") == 0)
            rok(filmy);

        if(strcmp(menu,"koniec") == 0){
            koniec(filmy);
            return 0;
        }
    }

    return 0;

}