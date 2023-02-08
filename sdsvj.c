#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <sys/stat.h>
#include "sdsvj.h"

//nacita z suboru
void nacitaj(void)
{
    int cislo,e_cislo,d_c,d_m,d_r;
    t_surovina *zaznam=NULL;
    t_surovina *plus_surovina=NULL;
    struct stat fileStat;
    FILE *ptrfile;

    if (!(ptrfile=fopen("databaze.txt","r")))
    {
        printf("Nepodarilo se otevrit soubor pro cteni\n");
    }
    stat("databaze.txt", &fileStat);

    if(fileStat.st_size!=0)
    {
        while(!feof(ptrfile))
        {

            if(pocet_zaznamov==0)
            {
                zaznam=(t_surovina*)malloc(sizeof(t_surovina));
                fscanf (ptrfile,"%s %d %d %d %d %d ",zaznam->nazov,&cislo,&e_cislo,&d_c,&d_m,&d_r);
                zaznam->mnozstvo=cislo;
                zaznam->skratka=e_cislo;
                zaznam->den=d_c;
                zaznam->mesiac=d_m;
                zaznam->rok=d_r;
                prvy=zaznam;
                pocet_zaznamov++;
            }
            else
            {
                plus_surovina=(t_surovina*)malloc(sizeof(t_surovina));
                fscanf (ptrfile,"%s %d %d %d %d %d ",plus_surovina->nazov,&cislo,&e_cislo,&d_c,&d_m,&d_r);
                plus_surovina->mnozstvo=cislo;
                plus_surovina->skratka=e_cislo;
                plus_surovina->den=d_c;
                plus_surovina->mesiac=d_m;
                plus_surovina->rok=d_r;
                zaznam->dalsi=plus_surovina;
                zaznam=zaznam->dalsi;
                pocet_zaznamov++;
            }
        }
    }
    fclose (ptrfile);
}

//Pridanie zaznamu
void pridat_s (char *nazov, int *mnozstvo, t_jednotka *skratka,int *den,int *mesiac,int *rok, int *pozicia)
{
    int m=0;
    t_surovina *aktualna_surovina=prvy;
    t_surovina *plus_surovina;
    t_surovina *p_surovina;
    p_surovina=(t_surovina*)malloc(sizeof(t_surovina));

    strcpy(p_surovina->nazov,nazov);
    p_surovina->mnozstvo=*mnozstvo;
    p_surovina->skratka=*skratka;
    p_surovina->den=*den;
    p_surovina->mesiac=*mesiac;
    p_surovina->rok=*rok;
    pocet_zaznamov++;


    if(pocet_zaznamov==1)
    {
        prvy=p_surovina;
    }
    else
    {
        while(m!=*pozicia)
        {
            aktualna_surovina=aktualna_surovina->dalsi;
            m++;
        }

        if(1==(pocet_zaznamov-(*pozicia)-2))
        {
            plus_surovina=aktualna_surovina->dalsi;
            aktualna_surovina->dalsi=p_surovina;
            aktualna_surovina=aktualna_surovina->dalsi;
            aktualna_surovina->dalsi=plus_surovina;
        }
        else if(1<(pocet_zaznamov-(*pozicia)-2))
        {
            plus_surovina=aktualna_surovina->dalsi;
            aktualna_surovina->dalsi=p_surovina;
            aktualna_surovina=aktualna_surovina->dalsi;
            aktualna_surovina->dalsi=plus_surovina;
            aktualna_surovina=aktualna_surovina->dalsi;
            m=0;

            while(m!=(pocet_zaznamov-(*pozicia)-3))
            {
                plus_surovina=aktualna_surovina->dalsi;
                aktualna_surovina->dalsi=plus_surovina;
                aktualna_surovina=aktualna_surovina->dalsi;
                m++;
            }
        }
        else
        {
            plus_surovina=p_surovina;
            aktualna_surovina->dalsi=plus_surovina;
        }

        (*pozicia)++;
    }
}


//Odstranit
void odstran(int *pozicia)
{
    int m=0;
    t_surovina *o_surovina;
    t_surovina *aktualna_surovina=prvy;

    if (*pozicia==0)
    {
        o_surovina=aktualna_surovina->dalsi;
        free(aktualna_surovina);
        prvy=o_surovina;
        pocet_zaznamov--;
    }
    else if ((*pozicia!=0)&&(*pozicia!=pocet_zaznamov-1))
    {
        while(m!=*pozicia)
        {
            aktualna_surovina=aktualna_surovina->dalsi;
            m++;
        }
        o_surovina=aktualna_surovina->dalsi;
        free(aktualna_surovina);
        pocet_zaznamov--;
        aktualna_surovina=prvy;
        m=0;

        while(m!=((*pozicia-1)))
        {
            aktualna_surovina=aktualna_surovina->dalsi;
            m++;
        }
        aktualna_surovina->dalsi=o_surovina;
    }
    else
    {
        m=0;

        while(m!=*pozicia-1)
        {
            aktualna_surovina=aktualna_surovina->dalsi;
            m++;
        }

        o_surovina=aktualna_surovina;
        free(aktualna_surovina->dalsi);
        pocet_zaznamov--;
        (*pozicia)--;
    }
}


//Vypisanie
void vypis(int *pozicia)
{
    int jednotka;
    t_surovina *v_surovina;
    int pocet=pocet_zaznamov;
    int m=0;
    v_surovina=prvy;

    if(v_surovina!=0)
    {
        do
        {
            jednotka=v_surovina->skratka;
            printf("%s\t\t%5d ",v_surovina->nazov,v_surovina->mnozstvo);
            switch (jednotka)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\t%d.%d.%d",v_surovina->den,v_surovina->mesiac,v_surovina->rok);
            if(*pozicia==m)
            {
                printf("    <-\n");
            }
            else
            {
                printf("\n");
            }
            m++;
            v_surovina=v_surovina->dalsi;
        }
        while(--pocet>0);
    }
    else
    {
        printf("\tDatabaza je prazdna\n");
    }
}

//Precitanie polozky
void precitaj(char *nazov,int *mnozstvo,t_jednotka *skratka, int *den,int *mesiac,int *rok)
{
    int c=0;
    int pocet;
    int ch1=0, ch2 =0;

    printf("Pridanie polozky\n");
    printf("----------------------------\n");
    printf("Nazov:");
    scanf("%s",nazov);
    fflush(stdin);
    printf("Mnozstvo:");
    scanf("%d",&pocet);
    fflush(stdin);
    printf("Jednotka:");
    printf("kg\n");
    printf("----------------------------");

    do
    {
        fflush(stdin);
        ch1 = getch();
        ch2 = 0;
        if (ch1 == 0xE0)
        {
            ch2 = getch();
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch(ch2)
            {
            case 72:
            {
                c--;
                if(c<0)
                {
                    c=2;
                    printf("kus");
                    printf("\n----------------------------");
                }
                else if(c==0)
                {
                    printf("kg");
                    printf("\n----------------------------");
                }
                else
                {
                    printf("lit");
                    printf("\n----------------------------");
                }
                break;

            }

            case 80:
            {
                c++;
                if(c>2)
                {
                    c=0;
                    printf("kg");
                    printf("\n----------------------------");
                }
                else if(c==1)
                {
                    printf("lit");
                    printf("\n----------------------------");
                }
                else
                {
                    printf("kus");
                    printf("\n----------------------------");
                }
                break;

            }
            };
        }
    }
    while(ch1!=13);

    *mnozstvo=pocet;
    *skratka=c;
    char ch[8]= {'0','0','0','0','0','0','0','0'};
    int cislica=0;
    ch1=0;
    ch2=0;

    do
    {
        switch (cislica)
        {
        case 0:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("|%c|%c.%c%c.%c%c%c%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;
        }
        case 1:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c|%c|.%c%c.%c%c%c%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;

        }
        case 2:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c%c.|%c|%c.%c%c%c%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;

        }
        case 3:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c%c.%c|%c|.%c%c%c%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;

        }
        case 4:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c%c.%c%c.|%c|%c%c%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;
        }
        case 5:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c%c.%c%c.%c|%c|%c%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;

        }
        case 6:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c%c.%c%c.%c%c|%c|%c\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;

        }
        case 7:
        {
            system("cls");
            printf("Pridanie polozky\n");
            printf("----------------------------\n");
            printf("Nazov:%s\n",nazov);
            printf("Mnozstvo:%d\n",pocet);
            printf("Jednotka:");
            switch (c)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\nDatum expiracie:");
            printf("%c%c.%c%c.%c%c%c|%c|\n",ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
            printf("----------------------------");
            break;
        }
        }

        fflush(stdin);
        ch1 = getch();
        ch2 = 0;

        if (ch1 == 0xE0)
        {
            ch2 = getch();
            switch(ch2)
            {
            case 75:
                cislica--;
                break; //sipka dolava
            case 77:
                cislica++;
                break; //sipka doprava
            };
        }
        else if(ch1==13)
        {
            break;
        }
        else
        {
            ch[cislica]=ch1;
            cislica++;
        }

        if(cislica==-1)
        {
            cislica=7;
        }
        else if(cislica==8)
        {
            cislica=0;
        }
    }
    while(ch1!='13');

    *den=(ch[0]-48)*10+(ch[1]-48)*1;
    *mesiac=(ch[2]-48)*10+(ch[3]-48)*1;
    *rok=(ch[4]-48)*1000+(ch[5]-48)*100+(ch[6]-48)*10+(ch[7]-48);
    system("cls");
    printf("Datum expiracie:%d.%d.%d\n",*den,*mesiac,*rok);
}

//ovladanie
t_prikaz ovladanie(void)
{
    int ch1=0, ch2 =0;
    fflush(stdin);
    ch1 = getch();
    ch2 = 0;

    if (ch1 == 0xE0)
    {
        ch2 = getch();
        switch(ch2)
        {
        case 72:
            return hore;//sipka horr
        case 80:
            return dole;//sipka dole
        case 75:
            return dolava;//sipka dolava
        case 77:
            return doprava;//sipka doprava
        };
    }
    else
    {
        switch (ch1)
        {
        case 27 :return odist;
        case 13 :return enter;
        }
    }
}

//kurzor hore
void pozicia_hore(int *pozicia)
{
    (*pozicia)--;

    if (*pozicia>pocet_zaznamov-1)
    {
        (*pozicia)=0;
    }
    else if (*pozicia<0)
    {
        (*pozicia)=pocet_zaznamov-1;
    }
}


//kurzor dole
void pozicia_dole(int *pozicia)
{
    (*pozicia)++;

    if (*pozicia>pocet_zaznamov-1)
    {
        (*pozicia)=0;
    }
    else if (*pozicia<0)
    {
        (*pozicia)=pocet_zaznamov-1;
    }
}

//ulozenie do suboru
int uloz (void)
{
    int n=0;
    FILE *fptr;
    t_surovina *zaznam=prvy;
    fptr=fopen("databaze.txt","w");

    if (fptr)
    {
        while (n!= pocet_zaznamov)
        {
            fprintf(fptr,"%s %d %d %d %d %d ",zaznam->nazov,zaznam->mnozstvo,zaznam->skratka,zaznam->den,zaznam->mesiac,zaznam->rok);
            zaznam=zaznam->dalsi;
            n++;
        }
    }
    else
    {
        printf ("Nepodaøilo sa otvorit subor");
        return 1;
    }
    fclose (fptr);
}



//hlavicka
void tisk_hlavicka(void)
{
    printf("Produkt\t\tMnozstvo\tDatum expiracie\n");
    printf("-----------------------------------------------\n");
}


//Poznamka
void tisk_poznamky(int *pozicia_poznamky)
{
    if (*pozicia_poznamky<0)
    {
        *pozicia_poznamky=3;
    }
    else if(*pozicia_poznamky>3)
    {
        *pozicia_poznamky=0;
    }
    printf("-----------------------------------------------\n");

    switch (*pozicia_poznamky)
    {
    case 0:
    {
        printf("\033[1;31m");
        printf("Pridat     ");
        printf("\033[0m");
        printf("Odstranit     Hladat     Triedit     ");
    }
    break;
    case 1:
    {
        printf("Pridat     ");
        printf("\033[1;31m");
        printf("Odstranit     ");
        printf("\033[0m");
        printf("Hladat     Triedit     ");
    }
    break;
    case 2:
    {
        printf("Pridat     Odstranit     ");
        printf("\033[1;31m");
        printf("Hladat     ");
        printf("\033[0m");
        printf("Triedit     ");
    }
    break;
    case 3:
    {
        printf("Pridat     Odstranit     Hladat     ");
        printf("\033[1;31m");
        printf("Triedit     ");
        printf("\033[0m");
    }
    break;
    }
}

//hladanie
void najdi(char *vyraz)
{
    printf("\t\tHladat:");
    scanf("%s",vyraz);
    fflush(stdin);
}

//Nadpis hladania
void hladat_poznamka (char *vyraz)
{
    printf("\t\tHladat:%s\n",vyraz);
}



//vypis hladania
void vypis_hladania(char *u_vyraz,int *pozicia)
{
    t_surovina *zaznam=prvy;
    int count=pocet_zaznamov;
    char vyraz [10];
    char produkt[20];
    int x,n,priznak,jednotka,m=0;

    strcpy(vyraz,u_vyraz);
    x=strlen(vyraz);

    while(count!=0)
    {
        priznak=0;
        strcpy(produkt,zaznam->nazov);
        for(n=0; n<x; n++)
        {
            if(produkt[n]!=vyraz[n])
            {
                priznak=1;
                n=x;
            }
        }

        if(priznak==1)
        {
            printf("%s\t\t%5d ",zaznam->nazov,zaznam->mnozstvo);
            jednotka=zaznam->skratka;
            switch (jednotka)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\t%d.%d.%d",zaznam->den,zaznam->mesiac,zaznam->rok);
            if(*pozicia==m)
            {
                printf("    <-\n");
            }
            else
            {
                printf("\n");
            }
            m++;
            zaznam=zaznam->dalsi;
            count--;
        }
        else
        {
            printf("\033[1;31m");
            printf("%s\t\t%5d ",zaznam->nazov,zaznam->mnozstvo);
            jednotka=zaznam->skratka;

            switch (jednotka)
            {
            case 0:
                printf("kg");
                break;
            case 1:
                printf("lit");
                break;
            case 2:
                printf("kus");
                break;
            }
            printf("\t%d.%d.%d",zaznam->den,zaznam->mesiac,zaznam->rok);
            if(*pozicia==m)
            {
                printf("    <-\n");
            }
            else
            {
                printf("\n");
            }
            m++;
            zaznam=zaznam->dalsi;
            count--;
            printf("\033[0m");
        }

    }
}

//triedenie vzostupne podla datumu
void tried_vzostupne_datum(void)
{
    t_surovina *zaznam_1=prvy;
    t_surovina *zaznam_2=NULL;
    t_surovina *zaznam_pred=NULL;
    t_surovina *zaznam_za=NULL;

    int mnozstvo1;
    int mnozstvo2;
    int priznak=1;
    int c=pocet_zaznamov;

    if(pocet_zaznamov>1)
    {
        while(priznak!=0)
        {
            zaznam_1=prvy;
            zaznam_2=zaznam_1->dalsi;
            priznak=0;
            c=pocet_zaznamov;

            if(pocet_zaznamov>4)
            {
                while (c!=1)
                {
                    mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                    mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==2)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                        }
                        c--;
                    }
                    else
                    {
                        if(mnozstvo1>mnozstvo2)
                        {
                            if((c-3)>0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                        else
                        {
                            if((c-3)>0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                    }
                }
            }
            else if (pocet_zaznamov==4)
            {
                while (c!=1)
                {
                    mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                    mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==3)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_1=zaznam_1;
                            zaznam_2=zaznam_za;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_2;
                            zaznam_2=zaznam_za;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==3)
            {
                while (c!=1)
                {
                    mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                    mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==2)
            {
                mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;

                if (mnozstvo1>mnozstvo2)
                {
                    priznak=1;

                    prvy=zaznam_2;
                    zaznam_2->dalsi=zaznam_1;
                    zaznam_1->dalsi=NULL;

                    zaznam_1=zaznam_2;
                    zaznam_2=zaznam_1->dalsi;
                }
            }
        }
    }
}

//triedeni datumu expiracie zostupne
void tried_zostupne_datum (void)
{
    t_surovina *zaznam_1=prvy;
    t_surovina *zaznam_2=NULL;
    t_surovina *zaznam_pred=NULL;
    t_surovina *zaznam_za=NULL;

    int mnozstvo1;
    int mnozstvo2;
    int priznak=1;
    int c=pocet_zaznamov;

    if(pocet_zaznamov>1)
    {
        while(priznak!=0)
        {
            zaznam_1=prvy;
            zaznam_2=zaznam_1->dalsi;
            priznak=0;
            c=pocet_zaznamov;

            if(pocet_zaznamov>4)
            {
                while (c!=1)
                {
                    mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                    mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==2)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                        }
                        c--;
                    }
                    else
                    {
                        if(mnozstvo1<mnozstvo2)
                        {
                            if((c-3)>0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                        else
                        {
                            if((c-3)>0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                    }
                }
            }
            else if (pocet_zaznamov==4)
            {
                while (c!=1)
                {
                    mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                    mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==3)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_1=zaznam_1;
                            zaznam_2=zaznam_za;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_2;
                            zaznam_2=zaznam_za;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==3)
            {
                while (c!=1)
                {
                    mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                    mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==2)
            {
                mnozstvo1=(zaznam_1->rok)*10000+(zaznam_1->mesiac)*100+zaznam_1->den;
                mnozstvo2=(zaznam_2->rok)*10000+(zaznam_2->mesiac)*100+zaznam_2->den;

                if (mnozstvo1<mnozstvo2)
                {
                    priznak=1;

                    prvy=zaznam_2;
                    zaznam_2->dalsi=zaznam_1;
                    zaznam_1->dalsi=NULL;

                    zaznam_1=zaznam_2;
                    zaznam_2=zaznam_1->dalsi;
                }
            }
        }
    }
}

//triedenie zostupne podla mnozstva
void tried_zostupne_mnozstvo (void)
{
    t_surovina *zaznam_1=prvy;
    t_surovina *zaznam_2=NULL;
    t_surovina *zaznam_pred=NULL;
    t_surovina *zaznam_za=NULL;

    int mnozstvo1;
    int mnozstvo2;
    int priznak=1;
    int c=pocet_zaznamov;

    if(pocet_zaznamov>1)
    {
        while(priznak!=0)
        {
            zaznam_1=prvy;
            zaznam_2=zaznam_1->dalsi;
            priznak=0;
            c=pocet_zaznamov;

            if(pocet_zaznamov>4)
            {
                while (c!=1)
                {
                    mnozstvo1=zaznam_1->mnozstvo;
                    mnozstvo2=zaznam_2->mnozstvo;

                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==2)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                        }
                        c--;
                    }
                    else
                    {
                        if(mnozstvo1<mnozstvo2)
                        {
                            if((c-3)>0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                        else
                        {
                            if((c-3)>0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                    }
                }
            }
            else if (pocet_zaznamov==4)
            {
                while (c!=1)
                {
                    mnozstvo1=zaznam_1->mnozstvo;
                    mnozstvo2=zaznam_2->mnozstvo;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==3)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_1=zaznam_1;
                            zaznam_2=zaznam_za;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_2;
                            zaznam_2=zaznam_za;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==3)
            {
                while (c!=1)
                {
                    mnozstvo1=zaznam_1->mnozstvo;
                    mnozstvo2=zaznam_2->mnozstvo;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1<mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==2)
            {
                mnozstvo1=zaznam_1->mnozstvo;
                mnozstvo2=zaznam_2->mnozstvo;

                if (mnozstvo1<mnozstvo2)
                {
                    priznak=1;

                    prvy=zaznam_2;
                    zaznam_2->dalsi=zaznam_1;
                    zaznam_1->dalsi=NULL;

                    zaznam_1=zaznam_2;
                    zaznam_2=zaznam_1->dalsi;
                }
            }
        }
    }
}

//triedenie vzostupne podla mnozstva
void tried_vzostupne_mnozstvo (void)
{
    t_surovina *zaznam_1=prvy;
    t_surovina *zaznam_2=NULL;
    t_surovina *zaznam_pred=NULL;
    t_surovina *zaznam_za=NULL;

    int mnozstvo1;
    int mnozstvo2;
    int priznak=1;
    int c=pocet_zaznamov;

    if(pocet_zaznamov>1)
    {
        while(priznak!=0)
        {
            zaznam_1=prvy;
            zaznam_2=zaznam_1->dalsi;
            priznak=0;
            c=pocet_zaznamov;

            if(pocet_zaznamov>4)
            {
                while (c!=1)
                {
                    mnozstvo1=zaznam_1->mnozstvo;
                    mnozstvo2=zaznam_2->mnozstvo;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==2)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                        }
                        c--;
                    }
                    else
                    {
                        if(mnozstvo1>mnozstvo2)
                        {
                            if((c-3)>0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                        else
                        {
                            if((c-3)>0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                    }
                }
            }
            else if (pocet_zaznamov==4)
            {
                while (c!=1)
                {
                    mnozstvo1=zaznam_1->mnozstvo;
                    mnozstvo2=zaznam_2->mnozstvo;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==3)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_1=zaznam_1;
                            zaznam_2=zaznam_za;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_2;
                            zaznam_2=zaznam_za;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==3)
            {
                while (c!=1)
                {
                    mnozstvo1=zaznam_1->mnozstvo;
                    mnozstvo2=zaznam_2->mnozstvo;


                    if(c==pocet_zaznamov)
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        c--;
                    }
                    else
                    {
                        if (mnozstvo1>mnozstvo2)
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==2)
            {
                mnozstvo1=zaznam_1->mnozstvo;
                mnozstvo2=zaznam_2->mnozstvo;

                if (mnozstvo1>mnozstvo2)
                {
                    priznak=1;

                    prvy=zaznam_2;
                    zaznam_2->dalsi=zaznam_1;
                    zaznam_1->dalsi=NULL;

                    zaznam_1=zaznam_2;
                    zaznam_2=zaznam_1->dalsi;
                }
            }
        }
    }
}


//triedenie z->a
void tried_z (void)
{
    t_surovina *zaznam_1=prvy;
    t_surovina *zaznam_2=NULL;
    t_surovina *zaznam_pred=NULL;
    t_surovina *zaznam_za=NULL;

    char nazov_1[20];
    char nazov_2[20];
    int priznak=1;
    int c=pocet_zaznamov;

    if(pocet_zaznamov>1)
    {
        while(priznak!=0)
        {
            zaznam_1=prvy;
            zaznam_2=zaznam_1->dalsi;
            priznak=0;
            c=pocet_zaznamov;

            if(pocet_zaznamov>4)
            {
                while (c!=1)
                {
                    strcpy(nazov_1,zaznam_1->nazov);
                    strcpy(nazov_2,zaznam_2->nazov);

                    if(c==pocet_zaznamov)
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==2)
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;
                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                        }
                        c--;
                    }
                    else
                    {
                        if(nazov_1[0]<nazov_2[0])
                        {
                            if((c-3)>0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                        else
                        {
                            if((c-3)>0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                    }
                }
            }
            else if (pocet_zaznamov==4)
            {
                while (c!=1)
                {
                    strcpy(nazov_1,zaznam_1->nazov);
                    strcpy(nazov_2,zaznam_2->nazov);


                    if(c==pocet_zaznamov)
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==3)
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_1=zaznam_1;
                            zaznam_2=zaznam_za;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_2;
                            zaznam_2=zaznam_za;
                        }
                        c--;
                    }
                    else
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==3)
            {
                while (c!=1)
                {
                    strcpy(nazov_1,zaznam_1->nazov);
                    strcpy(nazov_2,zaznam_2->nazov);


                    if(c==pocet_zaznamov)
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        c--;
                    }
                    else
                    {
                        if (nazov_1[0]<nazov_2[0])
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==2)
            {
                strcpy(nazov_1,zaznam_1->nazov);
                strcpy(nazov_2,zaznam_2->nazov);

                if (nazov_1[0]<nazov_2[0])
                {
                    priznak=1;

                    prvy=zaznam_2;
                    zaznam_2->dalsi=zaznam_1;
                    zaznam_1->dalsi=NULL;

                    zaznam_1=zaznam_2;
                    zaznam_2=zaznam_1->dalsi;
                }
            }
        }
    }
}

//triedenie a->z
void tried_a (void)
{
    t_surovina *zaznam_1=prvy;
    t_surovina *zaznam_2=NULL;
    t_surovina *zaznam_pred=NULL;
    t_surovina *zaznam_za=NULL;

    char nazov_1[20];
    char nazov_2[20];
    int priznak=1;
    int c=pocet_zaznamov;

    if(pocet_zaznamov>1)
    {
        while(priznak!=0)
        {
            zaznam_1=prvy;
            zaznam_2=zaznam_1->dalsi;
            priznak=0;
            c=pocet_zaznamov;

            if(pocet_zaznamov>4)
            {
                while (c!=1)
                {
                    strcpy(nazov_1,zaznam_1->nazov);
                    strcpy(nazov_2,zaznam_2->nazov);

                    if(c==pocet_zaznamov)
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==2)
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;
                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                        }
                        c--;
                    }
                    else
                    {
                        if(nazov_1[0]>nazov_2[0])
                        {
                            if((c-3)>0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                priznak=1;
                                zaznam_pred->dalsi=zaznam_2;
                                zaznam_2->dalsi=zaznam_1;
                                zaznam_1->dalsi=zaznam_za;

                                zaznam_pred=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                        else
                        {
                            if((c-3)>0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                                zaznam_za=zaznam_za->dalsi;
                            }
                            else if((c-3)==0)
                            {
                                zaznam_pred=zaznam_1;
                                zaznam_1=zaznam_2;
                                zaznam_2=zaznam_za;
                            }
                            c--;
                        }
                    }
                }
            }
            else if (pocet_zaznamov==4)
            {
                while (c!=1)
                {
                    strcpy(nazov_1,zaznam_1->nazov);
                    strcpy(nazov_2,zaznam_2->nazov);


                    if(c==pocet_zaznamov)
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                            zaznam_za=zaznam_2->dalsi;
                        }
                        c--;
                    }
                    else if(c==3)
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_1=zaznam_1;
                            zaznam_2=zaznam_za;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_2;
                            zaznam_2=zaznam_za;
                        }
                        c--;
                    }
                    else
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==3)
            {
                while (c!=1)
                {
                    strcpy(nazov_1,zaznam_1->nazov);
                    strcpy(nazov_2,zaznam_2->nazov);


                    if(c==pocet_zaznamov)
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;
                            zaznam_za=zaznam_2->dalsi;

                            prvy=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=zaznam_za;

                            zaznam_pred=zaznam_2;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        else
                        {
                            zaznam_pred=zaznam_1;
                            zaznam_1=zaznam_1->dalsi;
                            zaznam_2=zaznam_1->dalsi;
                        }
                        c--;
                    }
                    else
                    {
                        if (nazov_1[0]>nazov_2[0])
                        {
                            priznak=1;

                            zaznam_pred->dalsi=zaznam_2;
                            zaznam_2->dalsi=zaznam_1;
                            zaznam_1->dalsi=NULL;
                        }
                        c--;
                    }
                }
            }
            else if (pocet_zaznamov==2)
            {
                strcpy(nazov_1,zaznam_1->nazov);
                strcpy(nazov_2,zaznam_2->nazov);

                if (nazov_1[0]>nazov_2[0])
                {
                    priznak=1;

                    prvy=zaznam_2;
                    zaznam_2->dalsi=zaznam_1;
                    zaznam_1->dalsi=NULL;

                    zaznam_1=zaznam_2;
                    zaznam_2=zaznam_1->dalsi;
                }
            }
        }
    }
}

//Vrati prikaz
t_prikaz poznamka(int *pozicia_poznamky)
{
    switch (*pozicia_poznamky)
    {
    case 0:
        return pridat;
        break;
    case 1:
        return odstranit;
        break;
    case 2:
        return hladat;
        break;
    case 3:
        return triedit;
        break;
    }
}

//Vrati prikaz pre triedenie
void poznamka_triedenia(int *druh_triedenia)
{
    if (*druh_triedenia<0)
    {
        *druh_triedenia=5;
    }
    else if(*druh_triedenia>5)
    {
        *druh_triedenia=0;
    }
    printf("-----------------------------------------------\n");
    switch (*druh_triedenia)
    {
    case 0:
    {
        printf("\033[1;31m");
        printf("[Od a po z] ");
        printf("\033[0m");
        printf("[Od z po a] [Mnozstvo zostupne] [Mnozstvo vzostupne] [Datum zostupne] [Datum vzostupne]");
    }
    break;
    case 1:
    {
        printf("[Od a po z] ");
        printf("\033[1;31m");
        printf("[Od z po a] ");
        printf("\033[0m");
        printf("[Mnozstvo zostupne] [Mnozstvo vzostupne] [Datum zostupne] [Datum vzostupne]");
    }
    break;
    case 2:
    {
        printf("[Od a po z] [Od z po a] ");
        printf("\033[1;31m");
        printf("[Mnozstvo zostupne] ");
        printf("\033[0m");
        printf("[Mnozstvo vzostupne] [Datum zostupne] [Datum vzostupne]");
    }
    break;
    case 3:
    {
        printf("[Od a po z] [Od z po a] [Mnozstvo zostupne] ");
        printf("\033[1;31m");
        printf("[Mnozstvo vzostupne] ");
        printf("\033[0m");
        printf("[Datum zostupne] [Datum vzostupne]");
    }
    break;
    case 4:
    {
        printf("[Od a po z] [Od z po a] [Mnozstvo zostupne] [Mnozstvo vzostupne] ");
        printf("\033[1;31m");
        printf("[Datum zostupne] ");
        printf("\033[0m");
        printf("[Datum vzostupne] ");
    }
    break;
    case 5:
    {
        printf("[Od a po z] [Od z po a] [Mnozstvo zostupne] [Mnozstvo vzostupne] [Datum zostupne] ");
        printf("\033[1;31m");
        printf("[Datum vzostupne] ");
        printf("\033[0m");
    }
    break;
    }

}

//Vrati  prikaz triedenia
t_prikaz prikaz_triedenia(int *druh_triedenia)
{
    switch (*druh_triedenia)
    {
    case 0:
        return a_tried;
        break;
    case 1:
        return z_tried;
        break;
    case 2:
        return zostupne_mnozstvo_tried;
        break;
    case 3:
        return vzostupne_mnozstvo_tried;
        break;
    case 4:
        return zostupne_datum_tried;
        break;
    case 5:
        return vzostupne_datum_tried;
        break;
    }
}

//Poznamka hladania
void tisk_poznamky_hladania(int *druh_hladania)
{
    if (*druh_hladania<0)
    {
        *druh_hladania=1;
    }
    else if(*druh_hladania>1)
    {
        *druh_hladania=0;
    }
    printf("-----------------------------------------------\n");
    switch (*druh_hladania)
    {
    case 0:
    {
        printf("\033[1;31m");
        printf("Pridat     ");
        printf("\033[0m");
        printf("Odstranit     ");
    }
    break;
    case 1:
    {
        printf("\033[0m");
        printf("Pridat     ");
        printf("\033[1;31m");
        printf("Odstranit     ");
        printf("\033[0m");
    }
    break;
    }
}

//Vrati  prikaz hladania
t_prikaz poznamka_hladania(int *druh_hladania)
{
    switch (*druh_hladania)
    {
    case 0:
        return pridat;
        break;
    case 1:
        return odstranit;
        break;
    }
}

//zvrazni triedenia
void druh_poznamka_triedenia(int *druh_triedenia)
{
    if (*druh_triedenia<0)
    {
        *druh_triedenia=2;
    }
    else if(*druh_triedenia>2)
    {
        *druh_triedenia=0;
    }
    printf("-----------------------------------------------\n");
    switch (*druh_triedenia)
    {
    case 0:
    {
        printf("Triedit podla: ");
        printf("\033[1;31m");
        printf("Nazvu     ");
        printf("\033[0m");
        printf("Mnozstva     Datumu     ");
    }
    break;
    case 1:
    {
        printf("Triedit podla: Nazvu     ");
        printf("\033[1;31m");
        printf("Mnozstva     ");
        printf("\033[0m");
        printf("Datumu     ");
    }
    break;
    case 2:
    {
        printf("Triedit podla: Nazvu     Mnozstva     ");
        printf("\033[1;31m");
        printf("Datumu     ");
        printf("\033[0m");
    }
    break;
    }
}

//vrati druh triedenia
t_prikaz druh_poznamka_triedenia1(int *druh_triedenia,int *triedenie)
{
    if (*triedenie<0)
    {
        *triedenie=1;
    }
    else if(*triedenie>1)
    {
        *triedenie=0;
    }
    printf("-----------------------------------------------\n");

    switch (*druh_triedenia)
    {
    case 0:
    {
        printf("Triedit podla: ");
        printf("Nazvu\n");

        switch (*triedenie)
        {
        case 0:
        {
            printf("\033[1;31m");
            printf("od a po z     ");
            printf("\033[0m");
            printf("od z po a     ");
        }
        break;
        case 1:
        {
            printf("od a po z     ");
            printf("\033[1;31m");
            printf("od z po a     ");
            printf("\033[0m");
        }
        break;
        }
    }
    break;
    case 1:
    {
        printf("Triedit podla: ");
        printf("Mnozstva\n");

        switch (*triedenie)
        {
        case 0:
        {
            printf("\033[1;31m");
            printf("zostupne     ");
            printf("\033[0m");
            printf("vzostupne     ");
        }
        break;
        case 1:
        {
            printf("zostupne     ");
            printf("\033[1;31m");
            printf("vzostupne     ");
            printf("\033[0m");
        }
        break;
        }
    }
    break;
    case 2:
    {
        printf("Triedit podla: ");
        printf("Datumu\n");

        switch (*triedenie)
        {
        case 0:
        {
            printf("\033[1;31m");
            printf("zostupne     ");
            printf("\033[0m");
            printf("vzostupne     ");
        }
        break;
        case 1:
        {
            printf("zostupne     ");
            printf("\033[1;31m");
            printf("vzostupne     ");
            printf("\033[0m");
        }
        break;
        }
    }
    break;

    }
}

//Vrati prikaz druh triedenia
t_prikaz druh_prikaz_triedenia(int *druh_triedenia,int *triedenie)
{
    switch (*druh_triedenia)
    {
    case 0:
    {
        switch (*triedenie)
        {
        case 0:
            return a_tried;
            break;
        case 1:
            return z_tried;
            break;
        }
    }
    break;
    case 1:
    {
        switch (*triedenie)
        {
        case 0:
            return zostupne_mnozstvo_tried;
            break;
        case 1:
            return vzostupne_mnozstvo_tried;
            break;
        }
    }
    case 2:
    {
        switch (*triedenie)
        {
        case 0:
            return zostupne_datum_tried;
            break;
        case 1:
            return vzostupne_datum_tried;
            break;
        }
    }
    break;
    }

}

//Pomocka
void pomoc(void)
{
    printf("\n\n[Ovladanie: sipky, enter, escape-koniec]");
}

//Pomocka
void pomoc_hladanie(void)
{
    printf("\n\n[Ovladanie: sipky, enter, escape-vratit sa do hlavneho menu]");
}
