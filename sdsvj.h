#ifndef SDSVJ_H_INCLUDED
#define SDSVJ_H_INCLUDED

typedef enum {hore,dole,doprava,dolava,pridat,enter,odstranit,odist,hladat,odist_z_hladania,a_tried,z_tried,vzostupne_mnozstvo_tried,zostupne_mnozstvo_tried,zostupne_datum_tried,vzostupne_datum_tried,triedit} t_prikaz;
typedef enum {kg,lit,kus} t_jednotka;

typedef struct surovina
{
    char nazov[20];
    int mnozstvo;
    t_jednotka skratka;
    int den,mesiac,rok;
    struct surovina *dalsi;
} t_surovina;

t_surovina *prvy;
int pocet_zaznamov;

void pridat_s (char *nazov, int *mnozstvo, t_jednotka *skratka,int *den,int *mesiac,int *rok, int *pozicia);
void odstran(int *pozicia);
void vypis(int *pozicia);
void precitaj(char *nazov,int *mnozstvo,t_jednotka *skratka, int *den,int *mesiac,int *rok);
t_prikaz ovladanie(void);
void pozicia_hore(int *pozicia);
void pozicia_dole(int *pozicia);
int uloz (void);
void nacitaj(void);
void tisk_hlavicka(void);
void tisk_poznamky(int *pozicia_poznamky);
void najdi(char *vyraz);
void hladat_poznamka (char *vyraz);
void vypis_hladania(char *u_vyraz,int *pozicia);
void tried_vzostupne_datum(void);
void tried_zostupne_datum (void);
void tried_zostupne_mnozstvo (void);
void tried_vzostupne_mnozstvo (void);
void tried_z (void);
void tried_a (void);
t_prikaz poznamka(int *pozicia_poznamky);
void poznamka_triedenia(int *druh_triedenia);
t_prikaz prikaz_triedenia(int *druh_triedenia);
void tisk_poznamky_hladania(int *druh_hladania);
t_prikaz poznamka_hladania(int *druh_hladania);
void druh_poznamka_triedenia(int *druh_triedenia);
t_prikaz druh_poznamka_triedenia1(int *druh_triedenia,int *triedenie);
t_prikaz druh_prikaz_triedenia(int *druh_triedenia,int *triedenie);
void pomoc(void);
void pomoc_hladanie(void);


#endif // SDSVJ_H_INCLUDED
