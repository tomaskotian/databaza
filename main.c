#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <sys/stat.h>
#include "sdsvj.h"
#include "sdsvj.c"

int main()
{
    char nazov[20];
    char vyraz[20];
    int mnozstvo=0;
    int pozicia=0;
    int x=0;
    int triedenie=0;
    int pozicia_poznamky=0;
    int druh_triedenia=0;
    int druh_hladania=0;
    int den,mesiac,rok;
    t_jednotka skratka=0;
    t_prikaz vyber=0;

    nacitaj();

    do
    {
        x=0;
        system("cls");
        triedenie=0;
        druh_triedenia=0;
        druh_hladania=0;
        tisk_hlavicka();
        vypis(&pozicia);
        tisk_poznamky(&pozicia_poznamky);
        pomoc();

        //Hlavna stranka
        switch (vyber=ovladanie())
        {
        case enter:
        {
            vyber=poznamka(&pozicia_poznamky);
        }
        break;
        case doprava:
        {
            pozicia_poznamky++;
            system("cls");
            tisk_hlavicka();
            vypis_hladania(vyraz,&pozicia);
            tisk_poznamky(&pozicia_poznamky);
            pomoc();
        }
        break;
        case dolava:
        {
            pozicia_poznamky--;
            system("cls");
            tisk_hlavicka();
            vypis_hladania(vyraz,&pozicia);
            tisk_poznamky(&pozicia_poznamky);
            pomoc();
        }
        break;
        case dole:
        {
            system("cls");
            tisk_hlavicka();
            pozicia_dole(&pozicia);
            vypis(&pozicia);
            tisk_poznamky(&pozicia_poznamky);
            pomoc();
        }
        break;
        case hore:
        {
            system("cls");
            tisk_hlavicka();
            pozicia_hore(&pozicia);
            vypis(&pozicia);
            tisk_poznamky(&pozicia_poznamky);
            pomoc();
        }
        break;
        };

        //Prikazy
        switch(vyber)
        {
        case pridat:
        {
            system("cls");
            precitaj(nazov,&mnozstvo,&skratka,&den,&mesiac,&rok);
            pridat_s(nazov,&mnozstvo,&skratka,&den,&mesiac,&rok,&pozicia);
            system("cls");
            tisk_hlavicka();
            vypis(&pozicia);
            tisk_poznamky(&pozicia_poznamky);
            pomoc();
        }
        break;
        case odstranit:
        {
            system("cls");
            tisk_hlavicka();
            odstran(&pozicia);
            vypis(&pozicia);
            tisk_poznamky(&pozicia_poznamky);
            pomoc();
        }
        break;
        case hladat:
        {
            system("cls");
            najdi(vyraz);
            tisk_hlavicka();
            vypis_hladania(vyraz,&pozicia);
            tisk_poznamky_hladania(&druh_hladania);
            pomoc_hladanie();
            do
            {
                switch (vyber=ovladanie())
                {
                case enter:
                {
                    vyber=poznamka_hladania(&druh_hladania);
                }
                break;
                case dole:
                {
                    system("cls");
                    hladat_poznamka(vyraz);
                    tisk_hlavicka();
                    pozicia_dole(&pozicia);
                    vypis_hladania(vyraz,&pozicia);
                    tisk_poznamky_hladania(&druh_hladania);
                    pomoc_hladanie();
                }
                break;
                case hore:
                {
                    system("cls");
                    hladat_poznamka(vyraz);
                    tisk_hlavicka();
                    pozicia_hore(&pozicia);
                    vypis_hladania(vyraz,&pozicia);
                    tisk_poznamky_hladania(&druh_hladania);
                   pomoc_hladanie();
                }
                break;
                case doprava:
                {
                    druh_hladania++;
                    pozicia_poznamky++;
                    system("cls");
                    tisk_hlavicka();
                    vypis_hladania(vyraz,&pozicia);
                    tisk_poznamky_hladania(&druh_hladania);
                    pomoc_hladanie();
                }
                break;
                case dolava:
                {
                    druh_hladania--;
                    pozicia_poznamky--;
                    system("cls");
                    tisk_hlavicka();
                    vypis_hladania(vyraz,&pozicia);
                    tisk_poznamky_hladania(&druh_hladania);
                    pomoc_hladanie();
                }
                break;
                }
                switch (vyber)
                {
                case pridat:
                {
                    system("cls");
                    precitaj(nazov,&mnozstvo,&skratka,&den,&mesiac,&rok);
                    pridat_s(nazov,&mnozstvo,&skratka,&den,&mesiac,&rok,&pozicia);
                    system("cls");
                    hladat_poznamka(vyraz);
                    tisk_hlavicka();
                    vypis_hladania(vyraz,&pozicia);
                    tisk_poznamky_hladania(&druh_hladania);
                   pomoc_hladanie();
                }
                break;
                case odstranit:
                {
                    system("cls");
                    hladat_poznamka(vyraz);
                    tisk_hlavicka();
                    odstran(&pozicia);
                    vypis_hladania(vyraz,&pozicia);
                    tisk_poznamky_hladania(&druh_hladania);
                   pomoc_hladanie();
                }
                break;
                }
            }
            while(vyber!=odist);
            vyber=0;
        }
        break;
        case triedit:
        {
            system("cls");
            tisk_hlavicka();
            vypis(&pozicia);
            druh_poznamka_triedenia(&druh_triedenia);
            pomoc();

            do
            {
                switch (vyber=ovladanie())
                {
                case enter:
                {
                    x=1;
                }
                break;
                case doprava:
                {
                    druh_triedenia++;
                    system("cls");
                    tisk_hlavicka();
                    vypis(&pozicia);
                    druh_poznamka_triedenia(&druh_triedenia);
                    pomoc();
                }
                break;
                case dolava:
                {
                    druh_triedenia--;
                    system("cls");
                    tisk_hlavicka();
                    vypis(&pozicia);
                    druh_poznamka_triedenia(&druh_triedenia);
                    pomoc();
                }
                break;
                case odist:
                {
                    x=1;
                    vyber=0;
                }
                break;
                };

            }
            while(x!=1);
            x=0;

            //vyber druhu Triedenie
            do
            {
                system("cls");
                tisk_hlavicka();
                vypis(&pozicia);
                druh_poznamka_triedenia1(&druh_triedenia,&triedenie);
                pomoc();

                switch(vyber=ovladanie())
                {

                case enter:
                {
                    vyber=druh_prikaz_triedenia(&druh_triedenia,&triedenie);
                    x=1;
                }
                break;
                case doprava:
                {
                    triedenie++;
                    system("cls");
                    tisk_hlavicka();
                    vypis(&pozicia);
                    druh_poznamka_triedenia1(&druh_triedenia,&triedenie);
                    pomoc();
                }
                break;
                case dolava:
                {
                    triedenie--;
                    system("cls");
                    tisk_hlavicka();
                    vypis(&pozicia);
                    druh_poznamka_triedenia1(&druh_triedenia,&triedenie);
                    pomoc();
                }
                break;
                }
            }
            while(x!=1);

            //triedenia
            switch (vyber)
            {
            case a_tried:
            {
                system("cls");
                tisk_hlavicka();
                tried_a();
                vypis(&pozicia);
                tisk_poznamky(&pozicia_poznamky);
                pomoc();
            }
            break;
            case z_tried:
            {
                system("cls");
                tisk_hlavicka();
                tried_z();
                vypis(&pozicia);
                tisk_poznamky(&pozicia_poznamky);
                pomoc();
            }
            break;
            case vzostupne_mnozstvo_tried:
            {
                system("cls");
                tisk_hlavicka();
                tried_vzostupne_mnozstvo();
                vypis(&pozicia);
                tisk_poznamky(&pozicia_poznamky);
                pomoc();
            }
            break;
            case zostupne_mnozstvo_tried:
            {
                system("cls");
                tisk_hlavicka();
                tried_zostupne_mnozstvo();
                vypis(&pozicia);
                tisk_poznamky(&pozicia_poznamky);
                pomoc();
            }
            break;
            case zostupne_datum_tried:
            {
                system("cls");
                tisk_hlavicka();
                tried_zostupne_datum();
                vypis(&pozicia);
                tisk_poznamky(&pozicia_poznamky);
                pomoc();
            }
            break;
            case vzostupne_datum_tried:
            {
                system("cls");
                tisk_hlavicka();
                tried_vzostupne_datum();
                vypis(&pozicia);
                tisk_poznamky(&pozicia_poznamky);
                pomoc();
            }
            break;
            }
        }
        break;
        }
    }
    while(vyber!=odist);
    uloz();
}
