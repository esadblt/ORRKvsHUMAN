#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "raylib.h"
#include "raymath.h"
#include <math.h>


#define GRID_CELL_SIZE 40
#define GRID_COLUMNS 20
#define GRID_ROWS 20
#define MAX_UNITS_PER_CELL 100


    int kontrol1=0;
    int kontrol2=0;
    int kontrol3=0;
    int kontrol4=0;
    int kontrol5=0;
    int kontrol6=0;
    int kontrol7=0;
    int kontrol8=0;
    int kontrol9=0;
    int kontrol10=0;
    int kontrol11=0;
    int kontrol12=0;
    int kontrol13=0;
    int kontrol14=0;
    int kontrol15=0;
    int kontrol16=0;


typedef struct {
    Texture2D texture;
    int count;
    int health;
} Unitt;


typedef enum {
    START_SCREEN,
    IN_BATTLE,
    MID_SCREEN,
    END_SCREEN
} GameState;

GameState gameState = START_SCREEN;

Unitt humanUnits[GRID_COLUMNS][GRID_ROWS] = {0};
Unitt orcUnits[GRID_COLUMNS][GRID_ROWS] = {0};


Texture2D humanPiyade;
Texture2D humanOkcu;
Texture2D orcDovuscu;
Texture2D orcMizrakci;
Texture2D humanSuvari;
Texture2D humanKusatma;
Texture2D orcVargBinici;
Texture2D orcTroller;
Texture2D background;
Texture2D gamelogo;







typedef struct {
    int deger_int;
    char deger[10];
    char aciklama[100];
} UstalikSeviyesi;


typedef struct {
    UstalikSeviyesi seviye_1;
    UstalikSeviyesi seviye_2;
    UstalikSeviyesi seviye_3;
} Ustalik;

typedef struct{
Ustalik savunma_arastirma;
Ustalik saldiri_arastirma;
Ustalik elit_egitim;
Ustalik kusatma_egitim;
}Arastirma;


typedef struct {
    char etki_degeri[10];
    float etki_degeri_float;
    char etki_turu[20];
    char etkiledigi_birim[50];
    char aciklama[100];
} Orc_yaratik;


typedef struct {
    char etki_degeri[10];
    float  etki_degeri_float;
    char etki_turu[20];
    char etkiledigi_birim[50];
    char aciklama[100];
} insan_yaratik;


typedef struct {
    char bonus_turu[20];
    char etkiledigi_birim[50];
    char bonus_degeri[10];
    float bonus_degeri_float;
    char aciklama[100];
} ork_kahraman;


typedef struct {
    char bonus_turu[20];
    char etkiledigi_birim[50];
    char bonus_degeri[10];
    float bonus_degeri_float;
    char aciklama[100];
} insan_kahraman;

typedef struct {
    float saldiri;
    float savunma;
    float saglik;
    float kritik_sans;
} Birlik;

typedef struct {
    Birlik piyadeler;
    Birlik okcular;
    Birlik suvariler;
    Birlik kusatma_makineleri;
} InsanImparatorlugu;

typedef struct {
    Birlik ork_dovusculeri;
    Birlik mizrakcilar;
    Birlik varg_binicileri;
    Birlik troller;
} OrkLegi;


typedef struct{
    float piyadeSayi;
    float suvariSayi;
    float okcuSayi;
    float kusatmamakSayi;
    int savunma_arastirmaSeviye;
    int saldiri_arastirmaSeviye;
    int elit_egitimSeviye;
    int kusatma_egitimSeviye;
    char kahraman[100];
    char canavar[100];
    float toplamPiyadeSaldirigucu;
    float toplamSuvariSaldirigucu;
    float toplamOkcuSaldirigucu;
    float toplamKusatmamakSaldirigucu;
    float insantoplamSaldirigucu;
    float toplamPiyadeSavunmagucu;
    float toplamSuvariSavunmagucu;
    float toplamOkcuSavunmagucu;
    float toplamKusatmamakSavunmagucu;
    float insantoplamSavunmagucu;
}ins_savas;

typedef struct{
    float ork_dovuscuSayi;
    float mizrakciSayi;
    float varg_biniciSayi;
    float trollerSayi;
    int savunma_arastirmaSeviye;
    int saldiri_arastirmaSeviye;
    int elit_egitimSeviye;
    int kusatma_egitimSeviye;
    char kahraman[100];
    char canavar[100];
    float toplamDovuscuSaldirigucu;
    float toplamMizrakciSaldirigucu;
    float toplamVargBiniciSaldirigucu;
    float toplamTrolSaldirigucu;
    float orktoplamSaldirigucu;
    float toplamDovuscuSavunmagucu;
    float toplamMizrakciSavunmagucu;
    float toplamVargBiniciSavunmagucu;
    float toplamTrolSavunmagucu;
    float orktoplamSavunmagucu;

}orc_savas;




char* dosyaOku(const char* dosya_isim) {
    FILE *dosya = fopen(dosya_isim, "r");
    if (dosya == NULL) {
        printf("Dosya acilamadi: %s\n", dosya_isim);
        return NULL;
    }

    fseek(dosya, 0, SEEK_END);
    long dosya_boyutu = ftell(dosya);
    fseek(dosya, 0, SEEK_SET);

    char* icerik = (char*)malloc(sizeof(char) * (dosya_boyutu + 1));
    if (icerik == NULL) {
        printf("Bellek ayirma hatasi.\n");
        fclose(dosya);
        return NULL;
    }

    fread(icerik, 1, dosya_boyutu, dosya);
    icerik[dosya_boyutu] = '\0';

    fclose(dosya);
    return icerik;
}


void degerBul(const char *json, const char *isim, const char *anahtar, char *deger) {
    char *isim_pos = strstr(json, isim);
    if (isim_pos != NULL) {
        char *anahtar_pos = strstr(isim_pos, anahtar);
        if (anahtar_pos != NULL) {
            char *ayirici_pos = strstr(anahtar_pos, ":");
            if (ayirici_pos != NULL) {
                ayirici_pos++;
                while (*ayirici_pos == ' ' || *ayirici_pos == '\"') ayirici_pos++;

                int i = 0;
                while (ayirici_pos[i] != ',' && ayirici_pos[i] != '}' && ayirici_pos[i] != '\0' && ayirici_pos[i] != '\"') {
                    deger[i] = ayirici_pos[i];
                    i++;
                }
                deger[i] = '\0';
            }
        }
    }
}

void degerBul_research(const char* json,const char*isim, const char* anahtar,const char*anahtar1,char* deger){
char *isim_pos =strstr(json,isim);
if(isim_pos!=NULL){
    char* anahtar_pos=strstr(isim_pos,anahtar);
        if(anahtar_pos!=NULL){
            char *degisken=strstr(anahtar_pos,anahtar1);
                if(degisken!=NULL){
                    char *ayirici_pos = strstr(degisken,":");
                    if(ayirici_pos!=NULL){
                     ayirici_pos++;
                     while(*ayirici_pos == ' ' || *ayirici_pos == '\"') ayirici_pos++;

                     int i = 0 ;
                    while (ayirici_pos[i] != ',' && ayirici_pos[i] != '}' && ayirici_pos[i] != '\0' && ayirici_pos[i] != '\"') {
                    deger[i] = ayirici_pos[i];
                    i++;
                    }
                    deger[i]='\0';
                }
        }
    }
}
}


void aranandegerbul_arastirma(const char *json, const char *ad, const char *arastirma_anahtar, char *deger) {
    char *ad_bul = strstr(json, ad);
    if (ad_bul) {
        char *arastirma_bul = strstr(ad_bul, "\"arastirma_seviyesi\"");
        if (arastirma_bul) {
            char *end_bul = strchr(arastirma_bul, '}');
            if (end_bul) {
                char *anahtar_bul = strstr(arastirma_bul, arastirma_anahtar);
                if (anahtar_bul && anahtar_bul < end_bul) {
                    char *iki_nokta = strchr(anahtar_bul, ':');
                    if (iki_nokta) {
                        iki_nokta++;
                        while (*iki_nokta == '\"' || *iki_nokta == ' ')
                            iki_nokta++;
                        int i = 0;
                        while (iki_nokta[i] && iki_nokta[i] != ',' && iki_nokta[i] != '}' && iki_nokta[i] != '\"') {
                            deger[i] = iki_nokta[i];
                            i++;
                        }
                        deger[i] = '\0';
                    }
                }
            }
        }
    }
}


void degerBul_int(const char* json, const char* birlik_ismi, const char* anahtar, float* deger) {
    char *isim_pos = strstr(json, birlik_ismi);
    if (isim_pos != NULL) {
        char* anahtar_pos = strstr(isim_pos, anahtar);
        if (anahtar_pos != NULL) {
            char *ayirici_pos = strstr(anahtar_pos, ":");
            if (ayirici_pos != NULL) {
                ayirici_pos++;
                while (*ayirici_pos == ' ') ayirici_pos++;


                *deger = atof(ayirici_pos);
            }
        }
    }
}

void degerBul_int_senaryo(const char* json, const char* birlik_ismi, const char* anahtar_path, const char* anahtar, float* deger) {
    char *isim_pos = strstr(json, birlik_ismi);
    if (isim_pos != NULL) {
        char *anahtar_path_pos = strstr(isim_pos, anahtar_path);
        if (anahtar_path_pos != NULL) {
            char *anahtar_pos = strstr(anahtar_path_pos, anahtar);
            if (anahtar_pos != NULL) {
                char *ayirici_pos = strstr(anahtar_pos, ":");
                if (ayirici_pos != NULL) {
                    ayirici_pos++;
                    while (*ayirici_pos == ' ') ayirici_pos++;


                    *deger = atof(ayirici_pos);
                }
            }
        }
    }
}
void degerBul_dizi(const char *json, const char *isim, const char *anahtar, char *deger) {
    char *isim_pos = strstr(json, isim);
    if (isim_pos != NULL) {
        char *anahtar_pos = strstr(isim_pos, anahtar);
        if (anahtar_pos != NULL) {
            char *acik_parantez = strchr(anahtar_pos, '[');
            if (acik_parantez != NULL) {
                char *kapali_parantez = strchr(acik_parantez, ']');
                if (kapali_parantez != NULL) {
                    acik_parantez++;
                    while (acik_parantez < kapali_parantez) {

                        while (*acik_parantez == ' ' || *acik_parantez == '\"' || *acik_parantez == ',')
                            acik_parantez++;

                        int i = 0;
                        while (acik_parantez[i] != '\"' && acik_parantez[i] != ',' && acik_parantez[i] != ']' && acik_parantez[i] != '\0') {
                            deger[i] = acik_parantez[i];
                            i++;
                        }
                        deger[i] = '\0';


                        acik_parantez += i;

                        break;
                    }
                }
            }
        }
    }
}





void degerAta_ork_yaratik(const char* json, const char* orc_name, Orc_yaratik* orc) {
    degerBul(json, orc_name, "etki_degeri", orc->etki_degeri);
    degerBul(json, orc_name, "etki_turu", orc->etki_turu);
    degerBul(json, orc_name, "aciklama", orc->aciklama);

    sscanf(orc->aciklama, "%s", orc->etkiledigi_birim);

    if (strcmp(orc->etkiledigi_birim, "Trollerin") == 0 || strcmp(orc->etkiledigi_birim, "trollerin") == 0) {
    strcpy(orc->etkiledigi_birim, "troller");
}
if (strcmp(orc->etkiledigi_birim, "suvarilerinin") == 0 || strcmp(orc->etkiledigi_birim, "suvarilerine") == 0) {
    strcpy(orc->etkiledigi_birim, "suvariler");
}
if (strcmp(orc->etkiledigi_birim, "ork_dovusculerine") == 0 || strcmp(orc->etkiledigi_birim, "ork_dovusculerinin") == 0) {
    strcpy(orc->etkiledigi_birim, "ork_dovusculeri");
}
if (strcmp(orc->etkiledigi_birim, "varg_binicilerine") == 0 || strcmp(orc->etkiledigi_birim, "varg_binicilerin") == 0) {
    strcpy(orc->etkiledigi_birim, "varg_binicileri");
}
if (strcmp(orc->etkiledigi_birim, "mizrakcilarin") == 0 || strcmp(orc->etkiledigi_birim, "mizrakcilara") == 0) {
    strcpy(orc->etkiledigi_birim, "mizrakcilar");

}
if (strcmp(orc->etkiledigi_birim, "tum_birimlere") == 0 || strcmp(orc->etkiledigi_birim, "tum_birimlerin") == 0) {
    strcpy(orc->etkiledigi_birim, "tum_birimler");
}



    orc->etki_degeri_float = atof(orc->etki_degeri);
}


void degerAta_insan_yaratik(const char* json, const char* creature_name, insan_yaratik* creature) {
    degerBul(json, creature_name, "etki_degeri", creature->etki_degeri);
    degerBul(json, creature_name, "etki_turu", creature->etki_turu);
    degerBul(json, creature_name, "aciklama", creature->aciklama);


    sscanf(creature->aciklama, "%s", creature->etkiledigi_birim);

    if (strcmp(creature->etkiledigi_birim, "piyadelere") == 0 || strcmp(creature->etkiledigi_birim, "piyadelerin") == 0) {
    strcpy(creature->etkiledigi_birim, "piyadeler");
}
if (strcmp(creature->etkiledigi_birim, "suvarilere") == 0 || strcmp(creature->etkiledigi_birim, "suvarilerin") == 0) {
    strcpy(creature->etkiledigi_birim, "suvariler");
}
if (strcmp(creature->etkiledigi_birim, "okculara") == 0 || strcmp(creature->etkiledigi_birim, "okcularin") == 0) {
    strcpy(creature->etkiledigi_birim, "okcular");
}

if (strcmp(creature->etkiledigi_birim, "kusatma_makinelerinin") == 0 || strcmp(creature->etkiledigi_birim, "kusatma_makinelerine") == 0) {
    strcpy(creature->etkiledigi_birim, "kusatma_makineleri");
}

if (strcmp(creature->etkiledigi_birim, "tum_birimlere") == 0 || strcmp(creature->etkiledigi_birim, "tum_birimlerin") == 0) {
    strcpy(creature->etkiledigi_birim, "tum_birimler");
}


    creature->etki_degeri_float = atof(creature->etki_degeri);
}


void degerAta_ork_kahraman(const char* json, const char* hero_name, ork_kahraman* hero) {
    degerBul(json, hero_name, "bonus_turu", hero->bonus_turu);
    degerBul(json, hero_name, "bonus_degeri", hero->bonus_degeri);
    degerBul(json, hero_name, "aciklama", hero->aciklama);


    sscanf(hero->aciklama, "%s", hero->etkiledigi_birim);


if (strcmp(hero->etkiledigi_birim, "ork_dovusculerine") == 0 || strcmp(hero->etkiledigi_birim, "ork_dovusculerinin") == 0) {
    strcpy(hero->etkiledigi_birim, "ork_dovusculeri");
}
if (strcmp(hero->etkiledigi_birim, "trollerin") == 0 || strcmp(hero->etkiledigi_birim, "trollere") == 0) {
    strcpy(hero->etkiledigi_birim, "troller");
}
if (strcmp(hero->etkiledigi_birim, "varg_binicilerin") == 0 || strcmp(hero->etkiledigi_birim, "varg_binicilerine") == 0) {
    strcpy(hero->etkiledigi_birim, "varg_binicileri");
}
if (strcmp(hero->etkiledigi_birim, "mizrakcilarin") == 0 || strcmp(hero->etkiledigi_birim, "mizrakcilara") == 0) {
    strcpy(hero->etkiledigi_birim, "mizrakcilar");
}
if (strcmp(hero->etkiledigi_birim, "tum_birimlere") == 0 || strcmp(hero->etkiledigi_birim, "tum_birimlerin") == 0) {
    strcpy(hero->etkiledigi_birim, "tum_birimler");
}

    hero->bonus_degeri_float = atof(hero->bonus_degeri);
}
void degerAta_bilgi_ustalik(const char* json, Ustalik* savunma, Ustalik* saldiri, Ustalik* elit_egitim, Ustalik* kusatma) {

    degerBul_research(json, "savunma_ustaligi","seviye_1","deger", savunma->seviye_1.deger);
    savunma->seviye_1.deger_int = atoi(savunma->seviye_1.deger);
    degerBul_research(json, "savunma_ustaligi","seviye_2","deger", savunma->seviye_2.deger);
    savunma->seviye_2.deger_int = atoi(savunma->seviye_2.deger);

    degerBul_research(json, "savunma_ustaligi","seviye_3","deger", savunma->seviye_3.deger);
    savunma->seviye_3.deger_int = atoi(savunma->seviye_3.deger);

    degerBul_research(json, "saldiri_gelistirmesi","seviye_1","deger", saldiri->seviye_1.deger);
    saldiri->seviye_1.deger_int=atoi(saldiri->seviye_1.deger);

    degerBul_research(json, "saldiri_gelistirmesi","seviye_2","deger", saldiri->seviye_2.deger);
    saldiri->seviye_2.deger_int=atoi(saldiri->seviye_2.deger);


    degerBul_research(json, "saldiri_gelistirmesi","seviye_3","deger", saldiri->seviye_3.deger);
    saldiri->seviye_3.deger_int=atoi(saldiri->seviye_3.deger);

    degerBul_research(json, "elit_egitim","seviye_1","deger", elit_egitim->seviye_1.deger);
    elit_egitim->seviye_1.deger_int=atoi(elit_egitim->seviye_1.deger);

    degerBul_research(json, "elit_egitim","seviye_2","deger", elit_egitim->seviye_2.deger);
    elit_egitim->seviye_2.deger_int=atoi(elit_egitim->seviye_2.deger);

    degerBul_research(json, "elit_egitim","seviye_3","deger", elit_egitim->seviye_3.deger);
    elit_egitim->seviye_3.deger_int=atoi(elit_egitim->seviye_3.deger);

    degerBul_research(json, "kusatma_ustaligi","seviye_1","deger", kusatma->seviye_1.deger);
    kusatma->seviye_1.deger_int=atoi(kusatma->seviye_1.deger);

    degerBul_research(json, "kusatma_ustaligi","seviye_2","deger", kusatma->seviye_2.deger);
    kusatma->seviye_2.deger_int=atoi(kusatma->seviye_2.deger);

    degerBul_research(json, "kusatma_ustaligi","seviye_3","deger", kusatma->seviye_3.deger);
    kusatma->seviye_3.deger_int=atoi(kusatma->seviye_3.deger);
}

void degerAta_insan_kahraman(const char* json, const char* hero_name, insan_kahraman* hero) {
    degerBul(json, hero_name, "bonus_turu", hero->bonus_turu);
    degerBul(json, hero_name, "bonus_degeri", hero->bonus_degeri);
    degerBul(json, hero_name, "aciklama", hero->aciklama);


    sscanf(hero->aciklama, "%s", hero->etkiledigi_birim);

if (strcmp(hero->etkiledigi_birim, "piyadelere") == 0 || strcmp(hero->etkiledigi_birim, "piyadelerin") == 0) {
    strcpy(hero->etkiledigi_birim, "piyadeler");
}
if (strcmp(hero->etkiledigi_birim, "kusatma_makinelerinin") == 0 || strcmp(hero->etkiledigi_birim, "kusatma_makinelerine") == 0) {
    strcpy(hero->etkiledigi_birim, "kusatma_makineleri");
}
if (strcmp(hero->etkiledigi_birim, "suvarilere") == 0 || strcmp(hero->etkiledigi_birim, "suvarilerin") == 0) {
    strcpy(hero->etkiledigi_birim, "suvariler");
}
if (strcmp(hero->etkiledigi_birim, "okcularin") == 0 || strcmp(hero->etkiledigi_birim, "okculara") == 0) {
    strcpy(hero->etkiledigi_birim, "okcular");
}
if (strcmp(hero->etkiledigi_birim, "tum_birimlere") == 0 || strcmp(hero->etkiledigi_birim, "tum_birimlerin") == 0) {
    strcpy(hero->etkiledigi_birim, "tum_birimler");
}


    hero->bonus_degeri_float = atof(hero->bonus_degeri);
}
void degerAta_birlik_data(const char* json, Birlik* birlik, const char* birlik_ismi) {
    degerBul_int(json, birlik_ismi, "saldiri", &birlik->saldiri);
    degerBul_int(json, birlik_ismi, "savunma", &birlik->savunma);
    degerBul_int(json, birlik_ismi, "saglik", &birlik->saglik);
    degerBul_int(json, birlik_ismi, "kritik_sans", &birlik->kritik_sans);
}

void degerAta_insan_imparatorlugu(const char* json, InsanImparatorlugu* insan) {
    degerAta_birlik_data(json, &insan->piyadeler, "piyadeler");
    degerAta_birlik_data(json, &insan->okcular, "okcular");
    degerAta_birlik_data(json, &insan->suvariler, "suvariler");
    degerAta_birlik_data(json, &insan->kusatma_makineleri, "kusatma_makineleri");
}

void degerAta_ork_legi(const char* json, OrkLegi* ork) {
    degerAta_birlik_data(json, &ork->ork_dovusculeri, "ork_dovusculeri");
    degerAta_birlik_data(json, &ork->mizrakcilar, "mizrakcilar");
    degerAta_birlik_data(json, &ork->varg_binicileri, "varg_binicileri");
    degerAta_birlik_data(json, &ork->troller, "troller");
}


void degerAta_senaryo_data(const char* json, ins_savas* insan, orc_savas* ork) {
    char buffer[10];


    degerBul_int_senaryo(json, "insan_imparatorlugu", "birimler", "piyadeler", &insan->piyadeSayi);
    degerBul_int_senaryo(json, "insan_imparatorlugu", "birimler", "suvariler", &insan->suvariSayi);
    degerBul_int_senaryo(json, "insan_imparatorlugu", "birimler", "okcular", &insan->okcuSayi);
    degerBul_int_senaryo(json, "insan_imparatorlugu", "birimler", "kusatma_makineleri", &insan->kusatmamakSayi);


    degerBul_dizi(json, "insan_imparatorlugu", "kahramanlar", insan->kahraman);
    degerBul_dizi(json, "insan_imparatorlugu", "canavarlar", insan->canavar);


    aranandegerbul_arastirma(json,"insan_imparatorlugu","savunma_ustaligi",buffer);
    insan->savunma_arastirmaSeviye = atoi(buffer);
    if(insan->saldiri_arastirmaSeviye=1)

    buffer[0] = '\0';
    aranandegerbul_arastirma(json,"insan_imparatorlugu","saldiri_gelistirmesi",buffer);
    insan->saldiri_arastirmaSeviye = atoi(buffer);
    buffer[0] = '\0';
    aranandegerbul_arastirma(json,"insan_imparatorlugu","elit_egitim",buffer);
    insan->elit_egitimSeviye = atoi(buffer);
    buffer[0] = '\0';
    aranandegerbul_arastirma(json,"insan_imparatorlugu","kusatma_ustaligi",buffer);
    insan->kusatma_egitimSeviye= atoi(buffer);
    buffer[0] = '\0';



    degerBul_int_senaryo(json, "ork_legi", "birimler", "ork_dovusculeri", &ork->ork_dovuscuSayi);
    degerBul_int_senaryo(json, "ork_legi", "birimler", "mizrakcilar", &ork->mizrakciSayi);
    degerBul_int_senaryo(json, "ork_legi", "birimler", "varg_binicileri", &ork->varg_biniciSayi);
    degerBul_int_senaryo(json, "ork_legi", "birimler", "troller", &ork->trollerSayi);

    degerBul_dizi(json, "ork_legi", "kahramanlar", ork->kahraman);
    degerBul_dizi(json, "ork_legi", "canavarlar", ork->canavar);




    aranandegerbul_arastirma(json,"ork_legi","savunma_ustaligi",buffer);
    ork->savunma_arastirmaSeviye = atoi(buffer);
     buffer[0] = '\0';
    aranandegerbul_arastirma(json,"ork_legi","saldiri_gelistirmesi",buffer);
    ork->saldiri_arastirmaSeviye = atoi(buffer);
     buffer[0] = '\0';
    aranandegerbul_arastirma(json,"ork_legi","elit_egitim",buffer);
    ork->elit_egitimSeviye = atoi(buffer);
     buffer[0] = '\0';
    aranandegerbul_arastirma(json,"ork_legi","kusatma_ustaligi",buffer);
    ork->kusatma_egitimSeviye = atoi(buffer);
     buffer[0] = '\0';

}


void insansaldirigucunuhesapla( ins_savas* insan,  InsanImparatorlugu*impinsan,const insan_yaratik* Ejderha, const insan_yaratik* Tepegoz,const insan_yaratik* Agri_Dagi_Devleri,const insan_yaratik* Karakurt,const insan_yaratik* Samur, const insan_kahraman* Fatih_Sultan_Mehmet,const insan_kahraman* Tugrul_Bey,const insan_kahraman* Alparslan,const insan_kahraman* Mete_Han,const insan_kahraman* Yavuz_Sultan_Selim, Ustalik* savunma, Ustalik* saldiri, Ustalik* elit_egitim, Ustalik* kusatma,float *AdimSayisi2)
{
    float piyadeSaldirigucu=0;
    float okcuSaldirigucu=0;
    float suvariSaldirigucu=0;
    float kusatmamakSaldirigucu=0;
    float piyadeKritikvurus=(impinsan->piyadeler.saldiri)*15.0/10.0;
    float okcuKritikvurus= (impinsan->okcular.saldiri)*15.0/10.0;
    float suvariKritikvurus=(impinsan->suvariler.saldiri)*15.0/10.0;
    float kusatmamakKritikvurus=(impinsan->kusatma_makineleri.saldiri)*15.0/10.0;
    float piyadeKritikSaldirigucu=0;
    float okcuKritikSaldirigucu=0;
    float suvariKritikSaldirigucu=0;
    float kusatmamakKritikSaldirigucu=0;
    float piyadeAdimsayisi=1;
    float okcuAdimsayisi=1;
    float suvariAdimsayisi=1;
    float kusatmamakAdimsayisi=1;

    float insantoplamSaldirigucu;

    float Adimsayisi2=*AdimSayisi2;

    if(insan->piyadeSayi>0)
    {
    if(kontrol1==0){
        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"kritik_sans")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"piyadeler")==0){
                        impinsan->piyadeler.kritik_sans=impinsan->piyadeler.kritik_sans*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"kritik_sans")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"piyadeler")==0){
                            impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"kritik_sans")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"piyadeler")==0){
                            impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"kritik_sans")){
                     if(strcmp(Karakurt->etkiledigi_birim,"piyadeler")==0){
                            impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"kritik_sans")==0){
                     if(strcmp(Samur->etkiledigi_birim,"piyadeler")==0){
                            impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"piyadeler")==0){
                                impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"piyadeler")==0){
                                impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"piyadeler")==0){
                                impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"piyadeler")==0){
                                impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"piyadeler")==0){
                                impinsan->piyadeler.kritik_sans = impinsan->piyadeler.kritik_sans*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }

        if(insan->elit_egitimSeviye==1){
            impinsan->piyadeler.kritik_sans=  impinsan->piyadeler.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(insan->elit_egitimSeviye==2){
            impinsan->piyadeler.kritik_sans=impinsan->piyadeler.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(insan->elit_egitimSeviye==3){
            impinsan->piyadeler.kritik_sans=impinsan->piyadeler.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }
        kontrol1++;
    }



    if((Adimsayisi2 / (100.0 / impinsan->piyadeler.kritik_sans)) >= piyadeAdimsayisi){



         piyadeSaldirigucu= (insan->piyadeSayi)*(piyadeKritikvurus);
         piyadeAdimsayisi++;

        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                     if(strcmp(Samur->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Samur->etki_degeri_float/100));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100));

                    }

            }

       }
         if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        piyadeSaldirigucu=piyadeSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
   // if(strcmp(saldiri->saldiri_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(insan->saldiri_arastirmaSeviye==2){
        piyadeSaldirigucu=piyadeSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        piyadeSaldirigucu=piyadeSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }

    }





    else{
        piyadeSaldirigucu= (insan->piyadeSayi)*(impinsan->piyadeler.saldiri);

        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                     if(strcmp(Samur->etkiledigi_birim,"piyadeler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Samur->etki_degeri_float/100));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"piyadeler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100));

                    }

            }

       }
       if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSaldirigucu = piyadeSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        piyadeSaldirigucu=piyadeSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
   // if(strcmp(saldiri->saldiri_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(insan->saldiri_arastirmaSeviye==2){
        piyadeSaldirigucu=piyadeSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        piyadeSaldirigucu=piyadeSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }




    }


    if(insan->okcuSayi>0)
    {
        if(kontrol2==0){
                if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"kritik_sans")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"okcular")==0){
                        impinsan->okcular.kritik_sans=impinsan->okcular.kritik_sans*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"kritik_sans")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"okcular")==0){
                            impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"kritik_sans")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"okcular")==0){
                            impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"kritik_sans")){
                     if(strcmp(Karakurt->etkiledigi_birim,"okcular")==0){
                            impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"kritik_sans")==0){
                     if(strcmp(Samur->etkiledigi_birim,"okcular")==0){
                            impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"okcular")==0){
                                impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"okcular")==0){
                                impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"okcular")==0){
                                impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"okcular")==0){
                                impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"okcular")==0){
                                impinsan->okcular.kritik_sans = impinsan->okcular.kritik_sans*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }



            if(insan->elit_egitimSeviye==1){
            impinsan->okcular.kritik_sans=  impinsan->okcular.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(insan->elit_egitimSeviye==2){
            impinsan->okcular.kritik_sans=impinsan->okcular.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(insan->elit_egitimSeviye==3){
            impinsan->okcular.kritik_sans=impinsan->okcular.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }

        kontrol2++;
    }




    if(Adimsayisi2/(100.0/(impinsan->okcular.kritik_sans))>=okcuAdimsayisi){

         okcuSaldirigucu= (insan->okcuSayi)*(okcuKritikvurus);
         okcuAdimsayisi++;

        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"okcular")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"okcular")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"okcular")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"okcular")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                     if(strcmp(Samur->etkiledigi_birim,"okcular")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"okcular")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"okcular")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"okcular")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"okcular")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"okcular")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }

            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        okcuSaldirigucu=okcuSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
   // if(strcmp(saldiri->saldiri_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(insan->saldiri_arastirmaSeviye==2){
        okcuSaldirigucu=okcuSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        okcuSaldirigucu=okcuSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }









    else{



        okcuSaldirigucu = (insan->okcuSayi)*(impinsan->okcular.saldiri);





            if(strcmp(insan->canavar,"Tepegoz")==0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")==0){
                        if(strcmp(Tepegoz->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1+(Tepegoz->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Ejderha")==0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                        if(strcmp(Ejderha->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1+(Ejderha->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Agri_Dagi_Devleri")==0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")==0){
                        if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1+(Agri_Dagi_Devleri->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Karakurt")==0){
                if(strcmp(Karakurt->etki_turu,"saldiri")==0){
                        if(strcmp(Karakurt->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1+(Karakurt->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Samur")==0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                        if(strcmp(Samur->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1+(Samur->etki_degeri_float/100.0));
            }
        }
    }


            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                        if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                        if(strcmp(Tugrul_Bey->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Alparslan")==0){
                if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                        if(strcmp(Alparslan->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Mete_Han")==0){
                if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                        if(strcmp(Mete_Han->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                        if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"okcular")==0){
                        okcuSaldirigucu=okcuSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                }
            }
        }

            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            okcuSaldirigucu = okcuSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                okcuSaldirigucu = okcuSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        okcuSaldirigucu=okcuSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
   // if(strcmp(saldiri->saldiri_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(insan->saldiri_arastirmaSeviye==2){
        okcuSaldirigucu=okcuSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        okcuSaldirigucu=okcuSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }

    }


}


    if(insan->suvariSayi>0)
    {

        if(kontrol3==0){
            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"kritik_sans")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"suvariler")==0){
                        impinsan->suvariler.kritik_sans=impinsan->suvariler.kritik_sans*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"kritik_sans")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"suvariler")==0){
                            impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"kritik_sans")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"suvariler")==0){
                            impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"kritik_sans")){
                     if(strcmp(Karakurt->etkiledigi_birim,"suvariler")==0){
                            impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"kritik_sans")==0){
                     if(strcmp(Samur->etkiledigi_birim,"suvariler")==0){
                            impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"suvariler")==0){
                                impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"suvariler")==0){
                                impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"suvariler")==0){
                                impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"suvariler")==0){
                                impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"suvariler")==0){
                                impinsan->suvariler.kritik_sans = impinsan->suvariler.kritik_sans*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }

            if(insan->elit_egitimSeviye==1){
            impinsan->suvariler.kritik_sans=  impinsan->suvariler.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(insan->elit_egitimSeviye==2){
            impinsan->suvariler.kritik_sans=impinsan->suvariler.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(insan->elit_egitimSeviye==3){
            impinsan->suvariler.kritik_sans=impinsan->suvariler.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }
        kontrol3++;
    }


    if(Adimsayisi2/(100.0/(impinsan->suvariler.kritik_sans))>=suvariAdimsayisi){

         suvariSaldirigucu= (insan->suvariSayi)*(suvariKritikvurus);
         suvariAdimsayisi++;

        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                     if(strcmp(Samur->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }
           if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        suvariSaldirigucu=suvariSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(insan->saldiri_arastirmaSeviye==2){
        suvariSaldirigucu=suvariSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        suvariSaldirigucu=suvariSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }



        else{


            suvariSaldirigucu=(insan->suvariSayi)*(impinsan->suvariler.saldiri);


            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"suvariler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"suvariler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }
           if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            suvariSaldirigucu = suvariSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                suvariSaldirigucu = suvariSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        suvariSaldirigucu=suvariSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
   // if(strcmp(saldiri->saldiri_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(insan->saldiri_arastirmaSeviye==2){
        suvariSaldirigucu=suvariSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        suvariSaldirigucu=suvariSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }


}

    if(insan->kusatmamakSayi>0)
    {
        if(kontrol4==0){
            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"kritik_sans")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"kusatma_makineleri")==0){
                        impinsan->kusatma_makineleri.kritik_sans=impinsan->kusatma_makineleri.kritik_sans*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"kritik_sans")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"kusatma_makineleri")==0){
                            impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"kritik_sans")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"kusatma_makineleri")==0){
                            impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"kritik_sans")){
                     if(strcmp(Karakurt->etkiledigi_birim,"kusatma_makineleri")==0){
                            impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"kritik_sans")==0){
                     if(strcmp(Samur->etkiledigi_birim,"kusatma_makineleri")==0){
                            impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"kusatma_makineleri")==0){
                                impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"kusatma_makineleri")==0){
                                impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"kusatma_makineleri")==0){
                                impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"kusatma_makineleri")==0){
                                impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"kritik_sans")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"kusatma_makineleri")==0){
                                impinsan->kusatma_makineleri.kritik_sans = impinsan->kusatma_makineleri.kritik_sans*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


            if(insan->elit_egitimSeviye==1){
            impinsan->kusatma_makineleri.kritik_sans= impinsan->kusatma_makineleri.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(insan->elit_egitimSeviye==2){
            impinsan->kusatma_makineleri.kritik_sans=impinsan->kusatma_makineleri.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(insan->elit_egitimSeviye==3){
            impinsan->kusatma_makineleri.kritik_sans=impinsan->kusatma_makineleri.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }

        kontrol4++;
    }






    if(Adimsayisi2/(100.0/(impinsan->kusatma_makineleri.kritik_sans))>=kusatmamakAdimsayisi){

         kusatmamakSaldirigucu= (insan->kusatmamakSayi)*(kusatmamakKritikvurus);
         kusatmamakAdimsayisi++;

        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                     if(strcmp(Samur->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }
           if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        kusatmamakSaldirigucu=kusatmamakSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(insan->saldiri_arastirmaSeviye==2){
        kusatmamakSaldirigucu=kusatmamakSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        kusatmamakSaldirigucu=kusatmamakSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }


        else{



            kusatmamakSaldirigucu=(insan->kusatmamakSayi)*(impinsan->kusatma_makineleri.saldiri);

            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")==0){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")==0){
                     if(strcmp(Tepegoz->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")==0){
                     if(strcmp(Karakurt->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")==0){
                     if(strcmp(Samur->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){

                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }
           if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"saldiri")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"saldiri")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"saldiri")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"saldiri")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"saldiri")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"saldiri")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"saldiri")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"saldiri")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"saldiri")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"saldiri")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSaldirigucu = kusatmamakSaldirigucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


        if(insan->saldiri_arastirmaSeviye==1){
        kusatmamakSaldirigucu=kusatmamakSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(insan->saldiri_arastirmaSeviye==2){
        kusatmamakSaldirigucu=kusatmamakSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(insan->saldiri_arastirmaSeviye==3){
        kusatmamakSaldirigucu=kusatmamakSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }
}
    insan->toplamPiyadeSaldirigucu=piyadeSaldirigucu;
    insan->toplamOkcuSaldirigucu=okcuSaldirigucu;
    insan->toplamSuvariSaldirigucu=suvariSaldirigucu;
    insan->toplamKusatmamakSaldirigucu=kusatmamakSaldirigucu;


    insantoplamSaldirigucu=okcuSaldirigucu+piyadeSaldirigucu+suvariSaldirigucu+kusatmamakSaldirigucu;
    insan->insantoplamSaldirigucu=insantoplamSaldirigucu;


    (*AdimSayisi2)+=2;
     Adimsayisi2 = *AdimSayisi2;

}


void orksaldirigucunuhesapla( orc_savas*ork, OrkLegi*legork, ork_kahraman* Goruk_Vahsi,const ork_kahraman* Thruk_Kemikkiran,const ork_kahraman* Vrog_Kafakiran,const ork_kahraman* Ugar_Zalim,const Orc_yaratik* Kara_Troll,const Orc_yaratik* Ates_Iblisi,const Orc_yaratik* Golge_Kurtlari,const Orc_yaratik* Camur_Devleri,const Orc_yaratik* Buz_Devleri,Ustalik* savunma, Ustalik* saldiri, Ustalik* elit_egitim, Ustalik* kusatma,float *AdimSayisi1)
{
    float mizrakciSaldirigucu=0;
    float dovuscuSaldirigucu=0;
    float trollerSaldirigucu=0;
    float varg_biniciSaldirigucu=0;
    float orktoplamSaldirigucu=0;
    float mizrakciKritikvurus=(legork->mizrakcilar.saldiri)*15.0/10.0;
    float dovuscuKritikvurus= (legork->ork_dovusculeri.saldiri)*15.0/10.0;
    float trollerKritikvurus=(legork->troller.saldiri)*15.0/10.0;
    float varg_biniciKritikvurus=(legork->varg_binicileri.saldiri)*15.0/10.0;
    float mizrakciKritikSaldirigucu=0;
    float dovuscuKritikSaldirigucu=0;
    float trollerKritikSaldirigucu=0;
    float varg_biniciKritikSaldirigucu=0;
    float mizrakciAdimsayisi=1;
    float dovuscuAdimsayisi=1;
    float trollerAdimsayisi=1;
    float varg_biniciAdimsayisi=1;
    float Adimsayisi1= *AdimSayisi1;



    if(ork->mizrakciSayi>0)
    {


        if(kontrol5==0){

                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"mizrakcilar")==0){
                        legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"mizrakcilar")==0){
                        legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"mizrakcilar")==0){
                        legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"mizrakcilar")==0){
                        legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"kritik_sans")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"mizrakcilar")==0){
                            legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"kritik_sans")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"mizrakcilar")==0){
                            legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"kritik_sans")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"mizrakcilar")==0){
                            legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            legork->mizrakcilar.kritik_sans = legork->mizrakcilar.kritik_sans*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }
            if(ork->elit_egitimSeviye==1){
            legork->mizrakcilar.kritik_sans=legork->mizrakcilar.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(ork->elit_egitimSeviye==2){
            legork->mizrakcilar.kritik_sans=legork->mizrakcilar.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(ork->elit_egitimSeviye==3){

                    printf("elitegitimseviyedeger:%d",elit_egitim->seviye_3.deger_int);
            legork->mizrakcilar.kritik_sans=legork->mizrakcilar.kritik_sans*(1.0+(((float)elit_egitim->seviye_3.deger_int)/100.0));

            printf("\n\nmizrakcilar kritik sans:%f\n\n",legork->mizrakcilar.kritik_sans);

        }
        kontrol5++;
    }


    if(Adimsayisi1/(100.0/(legork->mizrakcilar.kritik_sans))>=mizrakciAdimsayisi){

         mizrakciSaldirigucu= (ork->mizrakciSayi)*(mizrakciKritikvurus);
         mizrakciAdimsayisi++;

                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
            }
                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        mizrakciSaldirigucu=mizrakciSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(ork->saldiri_arastirmaSeviye==2){
        mizrakciSaldirigucu=mizrakciSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        mizrakciSaldirigucu=mizrakciSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }

        }



        else{

        mizrakciSaldirigucu= (ork->mizrakciSayi)*(legork->mizrakcilar.saldiri);



                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
            }
                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSaldirigucu = mizrakciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        mizrakciSaldirigucu=mizrakciSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(ork->saldiri_arastirmaSeviye==2){
        mizrakciSaldirigucu=mizrakciSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        mizrakciSaldirigucu=mizrakciSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }

        }

}

    if(ork->ork_dovuscuSayi>0)
    {

        if(kontrol6==0){
        if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"ork_dovusculeri")==0){
                        legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"ork_dovusculeri")==0){
                        legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"kritik_sans")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"ork_dovusculeri")==0){
                            legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"kritik_sans")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"ork_dovusculeri")==0){
                            legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"kritik_sans")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"ork_dovusculeri")==0){
                            legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            legork->ork_dovusculeri.kritik_sans = legork->ork_dovusculeri.kritik_sans*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }
            if(ork->elit_egitimSeviye==1){
            legork->ork_dovusculeri.kritik_sans=  legork->ork_dovusculeri.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(ork->elit_egitimSeviye==2){
            legork->ork_dovusculeri.kritik_sans=legork->ork_dovusculeri.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(ork->elit_egitimSeviye==3){
            legork->ork_dovusculeri.kritik_sans=legork->ork_dovusculeri.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }
        kontrol6++;
    }



    if(Adimsayisi1/(100.0/(legork->ork_dovusculeri.kritik_sans))>=dovuscuAdimsayisi){

         dovuscuSaldirigucu= (ork->ork_dovuscuSayi)*(dovuscuKritikvurus);
         dovuscuAdimsayisi++;

                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
            }
                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        dovuscuSaldirigucu=dovuscuSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(ork->saldiri_arastirmaSeviye==2){
        dovuscuSaldirigucu=dovuscuSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        dovuscuSaldirigucu=dovuscuSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }


        }

        else {
        dovuscuSaldirigucu = (ork->ork_dovuscuSayi)*(legork->ork_dovusculeri.saldiri);


        if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
            }
                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSaldirigucu = dovuscuSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        dovuscuSaldirigucu=dovuscuSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }

        if(ork->saldiri_arastirmaSeviye==2){
        dovuscuSaldirigucu=dovuscuSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        dovuscuSaldirigucu=dovuscuSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
        }
}



    if(ork->trollerSayi>0)
    {

    if(kontrol7==0)
    {

        if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"troller")==0){
                        legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"troller")==0){
                        legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"troller")==0){
                        legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"troller")==0){
                        legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"kritik_sans")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"troller")==0){
                            legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"kritik_sans")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"troller")==0){
                            legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"kritik_sans")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"troller")==0){
                            legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"troller")==0){
                            legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"troller")==0){
                            legork->troller.kritik_sans = legork->troller.kritik_sans*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }
            if(ork->elit_egitimSeviye==1){
            legork->troller.kritik_sans=  legork->troller.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(ork->elit_egitimSeviye==2){
            legork->troller.kritik_sans=legork->troller.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(ork->elit_egitimSeviye==3){
            legork->troller.kritik_sans=legork->troller.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }
            kontrol7++;
    }


    if(Adimsayisi1/(100.0/(legork->troller.kritik_sans))>=trollerAdimsayisi){

         trollerSaldirigucu= (ork->trollerSayi)*(trollerKritikvurus);
         trollerAdimsayisi++;

                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
            }
                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        trollerSaldirigucu=trollerSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(ork->saldiri_arastirmaSeviye==2){
        trollerSaldirigucu=trollerSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        trollerSaldirigucu=trollerSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }


        }


    else{


        trollerSaldirigucu=(ork->trollerSayi)*(legork->troller.saldiri);


                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"troller")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"troller")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }
            if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            trollerSaldirigucu = trollerSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        trollerSaldirigucu=trollerSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
   // if(strcmp(saldiri->saldiri_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(ork->saldiri_arastirmaSeviye==2){
        trollerSaldirigucu=trollerSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        trollerSaldirigucu=trollerSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }

}


    if(ork->varg_biniciSayi>0){


        if(kontrol8==0){


            if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"varg_binicileri")==0){
                        legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"varg_binicileri")==0){
                        legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"varg_binicileri")==0){
                        legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"kritik_sans")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"varg_binicileri")==0){
                        legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"kritik_sans")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"varg_binicileri")==0){
                            legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"kritik_sans")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"varg_binicileri")==0){
                            legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"kritik_sans")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"varg_binicileri")==0){
                            legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"kritik_sans")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            legork->varg_binicileri.kritik_sans = legork->varg_binicileri.kritik_sans*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }
            if(ork->elit_egitimSeviye==1){
            legork->varg_binicileri.kritik_sans=  legork->varg_binicileri.kritik_sans*(1.0+(elit_egitim->seviye_1.deger_int/100.0));

        }
            if(ork->elit_egitimSeviye==2){
            legork->varg_binicileri.kritik_sans=legork->varg_binicileri.kritik_sans*(1.0+(elit_egitim->seviye_2.deger_int/100.0));

            }

            if(ork->elit_egitimSeviye==3){
            legork->varg_binicileri.kritik_sans=legork->varg_binicileri.kritik_sans*(1.0+(elit_egitim->seviye_3.deger_int/100.0));

        }
        kontrol8++;
    }



    if(Adimsayisi1/(100.0/(legork->varg_binicileri.kritik_sans))>=varg_biniciAdimsayisi){

         varg_biniciSaldirigucu= (ork->varg_biniciSayi)*(varg_biniciKritikvurus);
         varg_biniciAdimsayisi++;

                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
            }
                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        varg_biniciSaldirigucu=varg_biniciSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(ork->saldiri_arastirmaSeviye==2){
        varg_biniciSaldirigucu=varg_biniciSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        varg_biniciSaldirigucu=varg_biniciSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }

        }

else{

        varg_biniciSaldirigucu=(ork->varg_biniciSayi)*(legork->varg_binicileri.saldiri);



            if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
            if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }

                if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"varg_binicileri")==0){
                                varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }
            if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"saldiri")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"saldiri")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"saldiri")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"saldiri")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"saldiri")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"saldiri")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"saldiri")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSaldirigucu = varg_biniciSaldirigucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->saldiri_arastirmaSeviye==1){
        varg_biniciSaldirigucu=varg_biniciSaldirigucu*(1.0+(saldiri->seviye_1.deger_int/100.0));

    }
        if(ork->saldiri_arastirmaSeviye==2){
        varg_biniciSaldirigucu=varg_biniciSaldirigucu*(1.0+(saldiri->seviye_2.deger_int/100.0));

        }

        if(ork->saldiri_arastirmaSeviye==3){
        varg_biniciSaldirigucu=varg_biniciSaldirigucu*(1.0+(saldiri->seviye_3.deger_int/100.0));

    }
    }


}
    ork->toplamMizrakciSaldirigucu=mizrakciSaldirigucu;
    ork->toplamDovuscuSaldirigucu=dovuscuSaldirigucu;
    ork->toplamTrolSaldirigucu=trollerSaldirigucu;
    ork->toplamVargBiniciSaldirigucu=varg_biniciSaldirigucu;


    orktoplamSaldirigucu=mizrakciSaldirigucu+dovuscuSaldirigucu+trollerSaldirigucu+varg_biniciSaldirigucu;
    ork->orktoplamSaldirigucu=orktoplamSaldirigucu;



    (*AdimSayisi1)+=2;
     Adimsayisi1 = *AdimSayisi1;


}


void insansavunmagucunuhesapla( ins_savas* insan, InsanImparatorlugu *impinsan ,const insan_yaratik*  Ejderha,const insan_yaratik* Tepegoz,const insan_yaratik* Agri_Dagi_Devleri,const insan_yaratik* Karakurt,const insan_yaratik* Samur, const insan_kahraman* Fatih_Sultan_Mehmet,const insan_kahraman* Tugrul_Bey,const insan_kahraman* Alparslan,const insan_kahraman* Mete_Han,const insan_kahraman* Yavuz_Sultan_Selim, Ustalik* savunma, Ustalik* saldiri, Ustalik* elit_egitim, Ustalik* kusatma)
{
    float piyadeSavunmagucu=0;
    float okcuSavunmagucu=0;
    float suvariSavunmagucu=0;
    float kusatmamakSavunmagucu=0;
    float insantoplamSavunmagucu=0;


    if(insan->piyadeSayi>0)
    {
        piyadeSavunmagucu= (insan->piyadeSayi)*(impinsan->piyadeler.savunma);



        if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"piyadeler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"piyadeler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"piyadeler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")){
                     if(strcmp(Karakurt->etkiledigi_birim,"piyadeler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")==0){
                     if(strcmp(Samur->etkiledigi_birim,"piyadeler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"piyadeler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"piyadeler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"piyadeler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"piyadeler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"piyadeler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


                   if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
        if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                piyadeSavunmagucu = piyadeSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }
                }
            }


    if(insan->savunma_arastirmaSeviye==1){
        piyadeSavunmagucu=piyadeSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }

        if(insan->savunma_arastirmaSeviye==2){
        piyadeSavunmagucu=piyadeSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(insan->savunma_arastirmaSeviye==3){
        piyadeSavunmagucu=piyadeSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }


    }


    if(insan->okcuSayi>0)
    {
        okcuSavunmagucu = (insan->okcuSayi)*(impinsan->okcular.savunma);

            if(strcmp(insan->canavar,"Tepegoz")==0){
                if(strcmp(Tepegoz->etki_turu,"savunma")==0){
                        if(strcmp(Tepegoz->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1+(Tepegoz->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Ejderha")==0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                        if(strcmp(Ejderha->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1+(Ejderha->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Agri_Dagi_Devleri")==0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")==0){
                        if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1+(Agri_Dagi_Devleri->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Karakurt")==0){
                if(strcmp(Karakurt->etki_turu,"savunma")==0){
                        if(strcmp(Karakurt->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1+(Karakurt->etki_degeri_float/100.0));
            }
        }
    }
            if(strcmp(insan->canavar,"Samur")==0){
                if(strcmp(Samur->etki_turu,"savunma")==0){
                        if(strcmp(Samur->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1+(Samur->etki_degeri_float/100.0));
            }
        }
    }


            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                        if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                        if(strcmp(Tugrul_Bey->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Alparslan")==0){
                if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                        if(strcmp(Alparslan->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Mete_Han")==0){
                if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                        if(strcmp(Mete_Han->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

            }
        }
    }
            if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                        if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"okcular")==0){
                        okcuSavunmagucu=okcuSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

            }
        }
    }

                if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            okcuSavunmagucu = okcuSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            okcuSavunmagucu = okcuSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            okcuSavunmagucu = okcuSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            okcuSavunmagucu = okcuSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            okcuSavunmagucu = okcuSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                okcuSavunmagucu = okcuSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                okcuSavunmagucu = okcuSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                okcuSavunmagucu = okcuSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                okcuSavunmagucu = okcuSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                okcuSavunmagucu = okcuSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


    if(insan->savunma_arastirmaSeviye==1){
        okcuSavunmagucu=okcuSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
        if(insan->savunma_arastirmaSeviye==2){
        okcuSavunmagucu=okcuSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(insan->savunma_arastirmaSeviye==3){
        okcuSavunmagucu=okcuSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

        }

    }


    if(insan->suvariSayi>0)
    {

            suvariSavunmagucu=(insan->suvariSayi)*(impinsan->suvariler.savunma);



            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"suvariler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"suvariler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"suvariler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")){
                     if(strcmp(Karakurt->etkiledigi_birim,"suvariler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")){
                     if(strcmp(Samur->etkiledigi_birim,"suvariler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"suvariler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"suvariler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"suvariler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"suvariler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"suvariler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }

                       if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            suvariSavunmagucu = suvariSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                suvariSavunmagucu = suvariSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


    if(insan->savunma_arastirmaSeviye==1){
        suvariSavunmagucu=suvariSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
        if(insan->savunma_arastirmaSeviye==2){
        suvariSavunmagucu=suvariSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(insan->savunma_arastirmaSeviye==3){
        suvariSavunmagucu=suvariSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }


    }

    if(insan->kusatmamakSayi>0)
    {
            kusatmamakSavunmagucu=(insan->kusatmamakSayi)*(impinsan->kusatma_makineleri.savunma);


            if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")==0){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")==0){
                     if(strcmp(Tepegoz->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")==0){
                     if(strcmp(Karakurt->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
        if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")==0){
                     if(strcmp(Samur->etkiledigi_birim,"kusatma_makineleri")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
       if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){

                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"kusatma_makineleri")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }

                              if((strcmp(insan->canavar,"Ejderha"))== 0){
                if(strcmp(Ejderha->etki_turu,"savunma")==0){
                     if(strcmp(Ejderha->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Ejderha->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Agri_Dagi_Devleri"))== 0){
                if(strcmp(Agri_Dagi_Devleri->etki_turu,"savunma")){
                     if(strcmp(Agri_Dagi_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Agri_Dagi_Devleri->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Tepegoz"))== 0){
                if(strcmp(Tepegoz->etki_turu,"savunma")){
                     if(strcmp(Tepegoz->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Tepegoz->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Karakurt"))== 0){
                if(strcmp(Karakurt->etki_turu,"savunma")){
                     if(strcmp(Karakurt->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Karakurt->etki_degeri_float/100.0));

                    }

            }
       }
            if((strcmp(insan->canavar,"Samur"))== 0){
                if(strcmp(Samur->etki_turu,"savunma")){
                     if(strcmp(Samur->etkiledigi_birim,"tum_birimler")==0){
                            kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Samur->etki_degeri_float/100.0));

                    }

            }
       }
            if(strcmp(insan->kahraman,"Fatih_Sultan_Mehmet")==0){
                            if(strcmp(Fatih_Sultan_Mehmet->bonus_turu,"savunma")==0){
                                if(strcmp(Fatih_Sultan_Mehmet->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Fatih_Sultan_Mehmet->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Tugrul_Bey")==0){
                            if(strcmp(Tugrul_Bey->bonus_turu,"savunma")==0){
                                if(strcmp(Tugrul_Bey->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Tugrul_Bey->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Alparslan")==0){
                            if(strcmp(Alparslan->bonus_turu,"savunma")==0){
                                if(strcmp(Alparslan->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Alparslan->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Mete_Han")==0){
                            if(strcmp(Mete_Han->bonus_turu,"savunma")==0){
                                if(strcmp(Mete_Han->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Mete_Han->bonus_degeri_float/100.0));

                    }

            }

       }
              if(strcmp(insan->kahraman,"Yavuz_Sultan_Selim")==0){
                            if(strcmp(Yavuz_Sultan_Selim->bonus_turu,"savunma")==0){
                                if(strcmp(Yavuz_Sultan_Selim->etkiledigi_birim,"tum_birimler")==0){
                                kusatmamakSavunmagucu = kusatmamakSavunmagucu*(1.0+(Yavuz_Sultan_Selim->bonus_degeri_float/100.0));

                    }

            }

       }


    if(insan->savunma_arastirmaSeviye==1){
        kusatmamakSavunmagucu=kusatmamakSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
        if(insan->savunma_arastirmaSeviye==2){
        kusatmamakSavunmagucu=kusatmamakSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(insan->savunma_arastirmaSeviye==3){
        kusatmamakSavunmagucu=kusatmamakSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }



    }
    insan->toplamOkcuSavunmagucu=okcuSavunmagucu;
    insan->toplamPiyadeSavunmagucu=piyadeSavunmagucu;
    insan->toplamSuvariSavunmagucu=suvariSavunmagucu;
    insan->toplamKusatmamakSavunmagucu=kusatmamakSavunmagucu;


    insantoplamSavunmagucu=okcuSavunmagucu+piyadeSavunmagucu+suvariSavunmagucu+kusatmamakSavunmagucu;
    insan->insantoplamSavunmagucu=insantoplamSavunmagucu;


}

void orksavunmagucunuhesapla( orc_savas*ork, OrkLegi*legork,const ork_kahraman* Goruk_Vahsi,const ork_kahraman* Thruk_Kemikkiran,const ork_kahraman* Vrog_Kafakiran,const ork_kahraman* Ugar_Zalim,const Orc_yaratik* Kara_Troll,const Orc_yaratik* Ates_Iblisi,const Orc_yaratik* Golge_Kurtlari,const Orc_yaratik* Camur_Devleri,const Orc_yaratik* Buz_Devleri,Ustalik* savunma, Ustalik* saldiri, Ustalik* elit_egitim, Ustalik* kusatma)
{
    float mizrakciSavunmagucu=0;
    float dovuscuSavunmagucu=0;
    float trollerSavunmagucu=0;
    float varg_biniciSavunmagucu=0;
    float orktoplamSavunmagucu=0;


    if(ork->mizrakciSayi>0)
    {
        mizrakciSavunmagucu= (ork->mizrakciSayi)*(legork->mizrakcilar.savunma);



                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"mizrakcilar")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"mizrakcilar")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


            if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            mizrakciSavunmagucu = mizrakciSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->savunma_arastirmaSeviye==1){
        mizrakciSavunmagucu=mizrakciSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
   // if(strcmp(savunma->savunma_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(ork->savunma_arastirmaSeviye==2){
        mizrakciSavunmagucu=mizrakciSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(ork->savunma_arastirmaSeviye==3){
        mizrakciSavunmagucu=mizrakciSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }

    }

    if(ork->ork_dovuscuSayi>0)
    {
        dovuscuSavunmagucu = (ork->ork_dovuscuSayi)*(legork->ork_dovusculeri.savunma);


        if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"ork_dovusculeri")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"ork_dovusculeri")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }


            }

                        if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            dovuscuSavunmagucu = dovuscuSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->savunma_arastirmaSeviye==1){
        dovuscuSavunmagucu=dovuscuSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
   // if(strcmp(savunma->savunma_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(ork->savunma_arastirmaSeviye==2){
        dovuscuSavunmagucu=dovuscuSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(ork->savunma_arastirmaSeviye==3){
        dovuscuSavunmagucu=dovuscuSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }

}

    if(ork->trollerSayi>0)
    {
        trollerSavunmagucu=(ork->trollerSayi)*(legork->troller.savunma);


                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"troller")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"troller")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"troller")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"troller")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"troller")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"troller")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"troller")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"troller")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"troller")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        trollerSavunmagucu = trollerSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            trollerSavunmagucu = trollerSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->savunma_arastirmaSeviye==1){
        trollerSavunmagucu=trollerSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
   // if(strcmp(savunma->savunma_arastirma.seviye_2.aciklama,"tum_birimler")==0)
        if(ork->savunma_arastirmaSeviye==2){
        trollerSavunmagucu=trollerSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(ork->savunma_arastirmaSeviye==3){
        trollerSavunmagucu=trollerSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }

}



    if(ork->varg_biniciSayi>0){

        varg_biniciSavunmagucu=(ork->varg_biniciSayi)*(legork->varg_binicileri.savunma);


            if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"varg_binicileri")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"varg_binicileri")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }



                                if(strcmp(ork->kahraman,"Goruk_Vahsi")==0){
                if(strcmp(Goruk_Vahsi->bonus_turu,"savunma")==0){
                        if(strcmp(Goruk_Vahsi->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Goruk_Vahsi->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Thruk_Kemikkiran")==0){
                if(strcmp(Thruk_Kemikkiran->bonus_turu,"savunma")==0){
                        if(strcmp(Thruk_Kemikkiran->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Thruk_Kemikkiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Vrog_Kafakiran")==0){
                if(strcmp(Vrog_Kafakiran->bonus_turu,"savunma")==0){
                        if(strcmp(Vrog_Kafakiran->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Vrog_Kafakiran->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->kahraman,"Ugar_Zalim")==0){
                if(strcmp(Ugar_Zalim->bonus_turu,"savunma")==0){
                        if(strcmp(Ugar_Zalim->etkiledigi_birim,"tum_birimler")==0){
                        varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Ugar_Zalim->bonus_degeri_float/100.0));

                        }
                }
        }
                if(strcmp(ork->canavar,"Kara_Troll")==0){
                    if(strcmp(Kara_Troll->etki_turu,"savunma")==0){
                            if(strcmp(Kara_Troll->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Kara_Troll->etki_degeri_float/100.0));

                        }
                }
        }
                        if(strcmp(ork->canavar,"Ates_Iblisi")==0){
                    if(strcmp(Ates_Iblisi->etki_turu,"savunma")==0){
                            if(strcmp(Ates_Iblisi->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Ates_Iblisi->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Golge_Kurtlari")==0){
                    if(strcmp(Golge_Kurtlari->etki_turu,"savunma")==0){
                            if(strcmp(Golge_Kurtlari->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Golge_Kurtlari->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Camur_Devleri")==0){
                    if(strcmp(Camur_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Camur_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Camur_Devleri->etki_degeri_float/100.0));

                        }
                }
        }

                    if(strcmp(ork->canavar,"Buz_Devleri")==0){
                    if(strcmp(Buz_Devleri->etki_turu,"savunma")==0){
                            if(strcmp(Buz_Devleri->etkiledigi_birim,"tum_birimler")==0){
                            varg_biniciSavunmagucu = varg_biniciSavunmagucu*(1.0+(Buz_Devleri->etki_degeri_float/100.0));

                        }
                }
        }


        if(ork->savunma_arastirmaSeviye==1){
        varg_biniciSavunmagucu=varg_biniciSavunmagucu*(1.0+(savunma->seviye_1.deger_int/100.0));

    }
        if(ork->savunma_arastirmaSeviye==2){
        varg_biniciSavunmagucu=varg_biniciSavunmagucu*(1.0+(savunma->seviye_2.deger_int/100.0));

        }

        if(ork->savunma_arastirmaSeviye==3){
        varg_biniciSavunmagucu=varg_biniciSavunmagucu*(1.0+(savunma->seviye_3.deger_int/100.0));

    }

}

    ork->toplamDovuscuSavunmagucu=dovuscuSavunmagucu;
    ork->toplamMizrakciSavunmagucu=mizrakciSavunmagucu;
    ork->toplamTrolSavunmagucu=trollerSavunmagucu;
    ork->toplamVargBiniciSavunmagucu=varg_biniciSavunmagucu;


    orktoplamSavunmagucu=mizrakciSavunmagucu+dovuscuSavunmagucu+trollerSavunmagucu+varg_biniciSavunmagucu;

     ork->orktoplamSavunmagucu=orktoplamSavunmagucu;


}


void NetHasarHesaplamaFonksiyonu(ins_savas*insan,orc_savas*ork,InsanImparatorlugu*impinsan,OrkLegi*legork,float* AdimSayisi2,float *Nethasar)
{
    float insantoplamsaldirigucuu=insan->insantoplamSaldirigucu;
    float orktoplamsavunmagucuu=ork->orktoplamSavunmagucu;
    float NetHasar=insantoplamsaldirigucuu*(1-(orktoplamsavunmagucuu/insantoplamsaldirigucuu));



    float mizrakcisayiolen=0;
    float dovuscusayiolen=0;
    float varg_biniciolen=0;
    float trollerolen=0;
    float toplambirliksayisi=ork->mizrakciSayi+ork->ork_dovuscuSayi+ork->trollerSayi+ork->varg_biniciSayi;
    float yorgunlukadimsayisi=*AdimSayisi2;
    printf("\n\nyorgunlukadimsayisi:%f",yorgunlukadimsayisi);


  if(((int)(yorgunlukadimsayisi/5)==1)&& yorgunlukadimsayisi>5){
        insantoplamsaldirigucuu=insantoplamsaldirigucuu-insantoplamsaldirigucuu*10.0/100.0;
        orktoplamsavunmagucuu=orktoplamsavunmagucuu-orktoplamsavunmagucuu*10.0/100.0;
        NetHasar=insantoplamsaldirigucuu*(1-(orktoplamsavunmagucuu/insantoplamsaldirigucuu));

    }

    if(((int)(yorgunlukadimsayisi/5)==2)&& yorgunlukadimsayisi>10){
         for(int i=0;i<2;i++){
        insantoplamsaldirigucuu=insantoplamsaldirigucuu-insantoplamsaldirigucuu*10.0/100.0;
        orktoplamsavunmagucuu=orktoplamsavunmagucuu-orktoplamsavunmagucuu*10.0/100.0;
         }

        NetHasar=insantoplamsaldirigucuu*(1-(orktoplamsavunmagucuu/insantoplamsaldirigucuu));

    }

    if(((int)(yorgunlukadimsayisi/5)==3)&& yorgunlukadimsayisi>15){
         for(int i=0;i<3;i++){
        insantoplamsaldirigucuu=insantoplamsaldirigucuu-insantoplamsaldirigucuu*10.0/100.0;
        orktoplamsavunmagucuu=orktoplamsavunmagucuu-orktoplamsavunmagucuu*10.0/100.0;
         }

        NetHasar=insantoplamsaldirigucuu*(1-(orktoplamsavunmagucuu/insantoplamsaldirigucuu));

    }
    if(((int)(yorgunlukadimsayisi/5)==4)&& yorgunlukadimsayisi>20){
         for(int i=0;i<4;i++){
        insantoplamsaldirigucuu=insantoplamsaldirigucuu-insantoplamsaldirigucuu*10.0/100.0;
        orktoplamsavunmagucuu=orktoplamsavunmagucuu-orktoplamsavunmagucuu*10.0/100.0;
         }

        NetHasar=insantoplamsaldirigucuu*(1-(orktoplamsavunmagucuu/insantoplamsaldirigucuu));

    }
    if(((int)(yorgunlukadimsayisi/5)==5)&& yorgunlukadimsayisi>25){
         for(int i=0;i<5;i++){
        insantoplamsaldirigucuu=insantoplamsaldirigucuu-insantoplamsaldirigucuu*10.0/100.0;
        orktoplamsavunmagucuu=orktoplamsavunmagucuu-orktoplamsavunmagucuu*10.0/100.0;
         }

        NetHasar=insantoplamsaldirigucuu*(1-(orktoplamsavunmagucuu/insantoplamsaldirigucuu));

    }



    if(NetHasar<0)
        NetHasar=0;

    *Nethasar=NetHasar;

    if(ork->mizrakciSayi>0){


        if(NetHasar*(ork->toplamMizrakciSavunmagucu/ork->orktoplamSavunmagucu)>=legork->mizrakcilar.saglik)
        {
        mizrakcisayiolen=(NetHasar*(ork->toplamMizrakciSavunmagucu/ork->orktoplamSavunmagucu))/legork->mizrakcilar.saglik;
        mizrakcisayiolen=(int)mizrakcisayiolen;


        printf("\nmizrakcisayiolen:%f\n",mizrakcisayiolen);



        }
        legork->mizrakcilar.saglik= legork->mizrakcilar.saglik-NetHasar*(ork->toplamMizrakciSavunmagucu/ork->orktoplamSavunmagucu)/ork->mizrakciSayi;
        if(legork->mizrakcilar.saglik<0){
            ork->mizrakciSayi=0;
            legork->mizrakcilar.saglik=0;
        }
        printf("saglik:%f\n",legork->mizrakcilar.saglik);

        ork->mizrakciSayi=ork->mizrakciSayi-mizrakcisayiolen;
            if(ork->mizrakciSayi<0)
                ork->mizrakciSayi=0;
            if(ork->mizrakciSayi==0){
                legork->mizrakcilar.saglik=0;
            }
    }
       else{
        ork->mizrakciSayi=0;
        legork->mizrakcilar.saglik=0;

       }


    if(ork->ork_dovuscuSayi>0){

         if(NetHasar*(ork->toplamDovuscuSavunmagucu/ork->orktoplamSavunmagucu)>=legork->ork_dovusculeri.saglik){
            dovuscusayiolen=NetHasar*(ork->toplamDovuscuSavunmagucu/ork->orktoplamSavunmagucu)/legork->ork_dovusculeri.saglik;
            dovuscusayiolen=(int)dovuscusayiolen;

         }
        legork->ork_dovusculeri.saglik= legork->ork_dovusculeri.saglik-NetHasar*(ork->toplamDovuscuSavunmagucu/ork->orktoplamSavunmagucu)/ork->ork_dovuscuSayi;
            if(legork->ork_dovusculeri.saglik<0){
            ork->ork_dovuscuSayi=0;
            legork->ork_dovusculeri.saglik=0;
            }


        ork->ork_dovuscuSayi=ork->ork_dovuscuSayi-dovuscusayiolen;
                if(ork->ork_dovuscuSayi<0)
                ork->ork_dovuscuSayi=0;
                if(ork->ork_dovuscuSayi==0){
                legork->ork_dovusculeri.saglik=0;
                }

    }
       else{
        ork->ork_dovuscuSayi=0;
        legork->ork_dovusculeri.saglik=0;
       }



    if(ork->varg_biniciSayi>0){

        if(NetHasar*(ork->toplamVargBiniciSavunmagucu/ork->orktoplamSavunmagucu)>=legork->varg_binicileri.saglik){
        varg_biniciolen=NetHasar*(ork->toplamVargBiniciSavunmagucu/ork->orktoplamSavunmagucu)/legork->varg_binicileri.saglik;
        varg_biniciolen=(int)varg_biniciolen;

        }
       legork->varg_binicileri.saglik= legork->varg_binicileri.saglik-NetHasar*(ork->toplamVargBiniciSavunmagucu/ork->orktoplamSavunmagucu)/ork->varg_biniciSayi;
        if(legork->varg_binicileri.saglik<0){
            ork->varg_biniciSayi=0;
            legork->varg_binicileri.saglik=0;
        }


        ork->varg_biniciSayi=ork->varg_biniciSayi-varg_biniciolen;
        if(ork->varg_biniciSayi<0)
                ork->varg_biniciSayi=0;
        if(ork->varg_biniciSayi==0){
            legork->varg_binicileri.saglik=0;
        }

    }
        else{
        ork->varg_biniciSayi=0;
        legork->varg_binicileri.saglik=0;
        }


    if(ork->trollerSayi>0){

        if(NetHasar*(ork->toplamTrolSavunmagucu/ork->orktoplamSavunmagucu)>=legork->troller.saglik){
            trollerolen=NetHasar*(ork->toplamTrolSavunmagucu/ork->orktoplamSavunmagucu)/legork->troller.saglik;
            trollerolen=(int)trollerolen;


        }
        legork->troller.saglik= legork->troller.saglik-NetHasar*(ork->toplamTrolSavunmagucu/ork->orktoplamSavunmagucu)/ork->trollerSayi;
            if(legork->troller.saglik<0){
            ork->trollerSayi=0;
            legork->troller.saglik=0;
            }

        ork->trollerSayi=ork->trollerSayi-trollerolen;
        if(ork->trollerSayi<0){
                ork->trollerSayi=0;
                legork->troller.saglik=0;
        }
        if(ork->trollerSayi==0)
            legork->troller.saglik=0;


    }
        else{
        ork->trollerSayi=0;
        legork->troller.saglik=0;
        }

}


void NetHasarHesaplamaFonksiyonu2(ins_savas* insan ,orc_savas* ork ,InsanImparatorlugu* impinsan,OrkLegi* legork,float *AdimSayisi1,float *Nethasar2)
{
    float orktoplamsaldirigucuu =ork->orktoplamSaldirigucu;
    float insantoplamsavunmagucuu=insan->insantoplamSavunmagucu;
    float piyadesayiolen=0;
    float okcusayiolen=0;
    float suvarisayiolen=0;
    float kusatmamaksayiolen=0;
    float yorgunlukadimsayisi=*AdimSayisi1;


    float NetHasar2=orktoplamsaldirigucuu*(1-(insantoplamsavunmagucuu/orktoplamsaldirigucuu));
    float toplambirliksayisi=insan->okcuSayi+insan->piyadeSayi+insan->suvariSayi+insan->kusatmamakSayi;


    if(insantoplamsavunmagucuu==0)
        NetHasar2=orktoplamsaldirigucuu;


    if(((int)(yorgunlukadimsayisi/5)==1)&& yorgunlukadimsayisi>5){
            orktoplamsaldirigucuu=orktoplamsaldirigucuu-orktoplamsaldirigucuu*10.0/100.0;
            insantoplamsavunmagucuu=insantoplamsavunmagucuu-insantoplamsavunmagucuu*10.0/100.0;
            NetHasar2=orktoplamsaldirigucuu*(1-(insantoplamsavunmagucuu/orktoplamsaldirigucuu));
        }
        if(((int)(yorgunlukadimsayisi/5)==2)&& yorgunlukadimsayisi>10){
            for(int i=0;i<2;i++){
            orktoplamsaldirigucuu=orktoplamsaldirigucuu-orktoplamsaldirigucuu*10.0/100.0;
            insantoplamsavunmagucuu=insantoplamsavunmagucuu-insantoplamsavunmagucuu*10.0/100.0;
            }
            NetHasar2=orktoplamsaldirigucuu*(1-(insantoplamsavunmagucuu/orktoplamsaldirigucuu));

        }
        if(((int)(yorgunlukadimsayisi/5)==3)&& yorgunlukadimsayisi>15){
            for(int i=0;i<3;i++){
            orktoplamsaldirigucuu=orktoplamsaldirigucuu-orktoplamsaldirigucuu*10.0/100.0;
            insantoplamsavunmagucuu=insantoplamsavunmagucuu-insantoplamsavunmagucuu*10.0/100.0;
            }
            NetHasar2=orktoplamsaldirigucuu*(1-(insantoplamsavunmagucuu/orktoplamsaldirigucuu));

        }
        if(((int)(yorgunlukadimsayisi/5)==4)&& yorgunlukadimsayisi>20){
            for(int i=0;i<4;i++){
            orktoplamsaldirigucuu=orktoplamsaldirigucuu-orktoplamsaldirigucuu*10.0/100.0;
            insantoplamsavunmagucuu=insantoplamsavunmagucuu-insantoplamsavunmagucuu*10.0/100.0;
            }
            NetHasar2=orktoplamsaldirigucuu*(1-(insantoplamsavunmagucuu/orktoplamsaldirigucuu));

        }
        if(((int)(yorgunlukadimsayisi/5)==5)&& yorgunlukadimsayisi>25){
            for(int i=0;i<4;i++){
            orktoplamsaldirigucuu=orktoplamsaldirigucuu-orktoplamsaldirigucuu*10.0/100.0;
            insantoplamsavunmagucuu=insantoplamsavunmagucuu-insantoplamsavunmagucuu*10.0/100.0;
            }
            NetHasar2=orktoplamsaldirigucuu*(1-(insantoplamsavunmagucuu/orktoplamsaldirigucuu));

        }


    if(NetHasar2<0)
        NetHasar2=0;

    *Nethasar2=NetHasar2;

    if(insan->piyadeSayi>0){

        if(NetHasar2*(insan->toplamPiyadeSavunmagucu/insan->insantoplamSavunmagucu)>=impinsan->piyadeler.saglik){
        piyadesayiolen=NetHasar2*(insan->toplamPiyadeSavunmagucu/insan->insantoplamSavunmagucu)/impinsan->piyadeler.saglik;

        piyadesayiolen=(int)piyadesayiolen;



        }
        impinsan->piyadeler.saglik=impinsan->piyadeler.saglik-NetHasar2*(insan->toplamPiyadeSavunmagucu/insan->insantoplamSavunmagucu)/insan->piyadeSayi;
        if(impinsan->piyadeler.saglik<0){
            impinsan->piyadeler.saglik=0;
            piyadesayiolen=insan->piyadeSayi;
            insan->piyadeSayi=0;

        }

        insan->piyadeSayi=insan->piyadeSayi-piyadesayiolen;
        if(insan->piyadeSayi<0)
            insan->piyadeSayi=0;
    }

    else{
    insan->piyadeSayi=0;
     impinsan->piyadeler.saglik=0;
    }


    if(insan->okcuSayi>0){


        if(NetHasar2*(insan->toplamOkcuSavunmagucu/insan->insantoplamSavunmagucu)>=impinsan->okcular.saglik){
        okcusayiolen=NetHasar2*(insan->toplamOkcuSavunmagucu/insan->insantoplamSavunmagucu)/impinsan->okcular.saglik;
        okcusayiolen=(int)okcusayiolen;

        }
        impinsan->okcular.saglik=impinsan->okcular.saglik-NetHasar2*(insan->toplamOkcuSavunmagucu/insan->insantoplamSavunmagucu)/insan->okcuSayi;
        if(impinsan->okcular.saglik<0){
           impinsan->okcular.saglik=0;
            insan->okcuSayi=0;
        }

        insan->okcuSayi=insan->okcuSayi-okcusayiolen;
        if(insan->okcuSayi<0)
            insan->okcuSayi=0;

    }

    else
    {
        insan->okcuSayi=0;
        impinsan->okcular.saglik=0;

    }


    if(insan->suvariSayi>0){

        if(NetHasar2*(insan->toplamSuvariSavunmagucu/insan->insantoplamSavunmagucu)>=impinsan->suvariler.saglik){
        suvarisayiolen=NetHasar2*(insan->toplamSuvariSavunmagucu/insan->insantoplamSavunmagucu)/impinsan->suvariler.saglik;
        suvarisayiolen=(int)suvarisayiolen;

         }
         impinsan->suvariler.saglik=impinsan->suvariler.saglik-NetHasar2*(insan->toplamSuvariSavunmagucu/insan->insantoplamSavunmagucu)/insan->suvariSayi;
                if(impinsan->suvariler.saglik<0){
               impinsan->suvariler.saglik=0;
                insan->suvariSayi=0;
                }
        insan->suvariSayi=insan->suvariSayi-suvarisayiolen;

        if(insan->suvariSayi<0)
            insan->suvariSayi=0;

    }

    else{
    insan->suvariSayi=0;
     impinsan->suvariler.saglik=0;

    }


    if(insan->kusatmamakSayi>0){


        if(NetHasar2*(insan->toplamKusatmamakSavunmagucu/insan->insantoplamSavunmagucu)>=impinsan->kusatma_makineleri.saglik){
        kusatmamaksayiolen=NetHasar2*(insan->toplamKusatmamakSavunmagucu/insan->insantoplamSavunmagucu)/impinsan->kusatma_makineleri.saglik;
        kusatmamaksayiolen=(int)kusatmamaksayiolen;



        }
        impinsan->kusatma_makineleri.saglik=impinsan->kusatma_makineleri.saglik-NetHasar2*(insan->toplamKusatmamakSavunmagucu/insan->insantoplamSavunmagucu)/insan->kusatmamakSayi;
          if(impinsan->kusatma_makineleri.saglik<0){
          impinsan->kusatma_makineleri.saglik=0;
            insan->kusatmamakSayi=0;
          }

        insan->kusatmamakSayi=insan->kusatmamakSayi-kusatmamaksayiolen;

         if(insan->kusatmamakSayi<0)
            insan->kusatmamakSayi=0;
    }

    else{
    insan->kusatmamakSayi=0;
   impinsan->kusatma_makineleri.saglik=0;

    }

}


void CanBariCiz(int x, int y, int can, int maxcan) {
    float canYuzdesi = (float)can / maxcan;
    Color barColor = GREEN;

    if (canYuzdesi < 0.3f) {
        barColor = RED;
    } else if (canYuzdesi < 0.6f) {
        barColor = YELLOW;
    }

    DrawRectangle(x, y, GRID_CELL_SIZE, 5, LIGHTGRAY);
    DrawRectangle(x, y, GRID_CELL_SIZE * canYuzdesi, 5, barColor);
}

void YerlestirHumanPiyade(int baslangic) {
    int sayiPiyade=baslangic;

    while (sayiPiyade > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiPiyade > MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiPiyade;
            humanUnits[x][y].count += toAdd;
            sayiPiyade -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanPiyade;
            humanUnits[x][y].health += toAdd * 100;
        }
    }
}
void YerlestirHumanPiyadeSon(ins_savas* ins,InsanImparatorlugu* impinsan) {
    int sayiPiyade=(int)ins->piyadeSayi;

    while (sayiPiyade > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiPiyade > MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiPiyade;
            humanUnits[x][y].count += toAdd;
            sayiPiyade -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanPiyade;
            humanUnits[x][y].health = impinsan->piyadeler.saglik*toAdd;
        }
    }
}
void YerlestirHumanOkcu(int baslangic) {
    int sayiOkcu = baslangic;

    while (sayiOkcu > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiOkcu > MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiOkcu;
            humanUnits[x][y].count += toAdd;
            sayiOkcu -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanOkcu;
            humanUnits[x][y].health += toAdd * 80;
        }
    }
}
void YerlestirHumanOkcuSon(ins_savas* ins,InsanImparatorlugu* impinsan) {
    int sayiOkcu = (int)ins->okcuSayi;

    while (sayiOkcu > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiOkcu > MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiOkcu;
            humanUnits[x][y].count += toAdd;
            sayiOkcu -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanOkcu;
            humanUnits[x][y].health = impinsan->okcular.saglik*toAdd;
        }
    }
}
void YerlestirHumanSuvari(int baslangic) {
    int sayiSuvari = baslangic;

    while (sayiSuvari > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiSuvari> MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiSuvari;
            humanUnits[x][y].count += toAdd;
            sayiSuvari -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanSuvari;
            humanUnits[x][y].health += toAdd * 120;
        }
    }
}
void YerlestirHumanSuvariSon(ins_savas* ins,InsanImparatorlugu* impinsan) {
    int sayiSuvari =(int)ins->suvariSayi;

    while (sayiSuvari > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiSuvari> MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiSuvari;
            humanUnits[x][y].count += toAdd;
            sayiSuvari -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanSuvari;
            humanUnits[x][y].health = impinsan->suvariler.saglik*toAdd;
        }
    }
}
void YerlestirHumanKusatma(int baslangic) {
    int sayiKusatma = baslangic;

    while (sayiKusatma > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiKusatma> MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiKusatma;
            humanUnits[x][y].count += toAdd;
            sayiKusatma -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanKusatma;
            humanUnits[x][y].health += toAdd * 150;
        }
    }
}
void YerlestirHumanKusatmaSon(ins_savas* ins,InsanImparatorlugu* impinsan) {
    int sayiKusatma = (int)ins->kusatmamakSayi;

    while (sayiKusatma > 0) {
        int x = GetRandomValue(0, GRID_COLUMNS / 2 - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (humanUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiKusatma> MAX_UNITS_PER_CELL - humanUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - humanUnits[x][y].count : sayiKusatma;
            humanUnits[x][y].count += toAdd;
            sayiKusatma -= toAdd;
            if (humanUnits[x][y].texture.id == 0) humanUnits[x][y].texture = humanKusatma;
            humanUnits[x][y].health = impinsan->kusatma_makineleri.saglik*toAdd;
        }
    }
}
void YerlestirOrcMizrakci(int baslangic) {
    int sayiMizrakci = baslangic;

    while (sayiMizrakci > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiMizrakci > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiMizrakci;
            orcUnits[x][y].count += toAdd;
            sayiMizrakci -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcMizrakci;
            orcUnits[x][y].health += toAdd * 90;
        }
    }
}
void YerlestirOrcMizrakciSon(orc_savas* ork,OrkLegi* legork) {
    int sayiMizrakci =(int) ork->mizrakciSayi;

    while (sayiMizrakci > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiMizrakci > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiMizrakci;
            orcUnits[x][y].count += toAdd;
            sayiMizrakci -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcMizrakci;
            orcUnits[x][y].health = legork->mizrakcilar.saglik*toAdd;
        }
    }
}
void YerlestirVargBinici(int baslangic) {
    int sayiVargBinici = baslangic;

    while (sayiVargBinici > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiVargBinici > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiVargBinici;
            orcUnits[x][y].count += toAdd;
            sayiVargBinici -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcVargBinici;
            orcUnits[x][y].health += toAdd * 130;
        }
    }
}
void YerlestirVargBiniciSon(orc_savas* ork,OrkLegi* legork) {
    int sayiVargBinici = (int)ork->varg_biniciSayi;

    while (sayiVargBinici > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiVargBinici > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiVargBinici;
            orcUnits[x][y].count += toAdd;
            sayiVargBinici -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcVargBinici;
             orcUnits[x][y].health = legork->varg_binicileri.saglik*toAdd;
        }
    }
}
void YerlestirTroller(int baslangic) {
    int sayiTroller = baslangic;

    while (sayiTroller > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiTroller > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiTroller;
            orcUnits[x][y].count += toAdd;
            sayiTroller -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcTroller;
            orcUnits[x][y].health += toAdd * 200;
        }
    }
}
void YerlestirTrollerSon(orc_savas* ork,OrkLegi* legork) {
    int sayiTroller = (int)ork->trollerSayi;

    while (sayiTroller > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiTroller > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiTroller;
            orcUnits[x][y].count += toAdd;
            sayiTroller -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcTroller;
             orcUnits[x][y].health = legork->troller.saglik*toAdd;
        }
    }
}
void YerlestirOrcDovuscu(int baslangic) {
    int sayiDovuscu = baslangic;

    while (sayiDovuscu > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiDovuscu > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiDovuscu;
            orcUnits[x][y].count += toAdd;
            sayiDovuscu -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcDovuscu;
            orcUnits[x][y].health += toAdd * 100;
        }
    }
}
void YerlestirOrcDovuscuSon(orc_savas *ork,OrkLegi* legork) {
    int sayiDovuscu = (int)ork->ork_dovuscuSayi;

    while (sayiDovuscu > 0) {
        int x = GetRandomValue(GRID_COLUMNS / 2, GRID_COLUMNS - 1);
        int y = GetRandomValue(0, GRID_ROWS - 1);

        if (orcUnits[x][y].count < MAX_UNITS_PER_CELL) {
            int toAdd = (sayiDovuscu > MAX_UNITS_PER_CELL - orcUnits[x][y].count) ?
                         MAX_UNITS_PER_CELL - orcUnits[x][y].count : sayiDovuscu;
            orcUnits[x][y].count += toAdd;
            sayiDovuscu -= toAdd;
            if (orcUnits[x][y].texture.id == 0) orcUnits[x][y].texture = orcDovuscu;
             orcUnits[x][y].health = legork->ork_dovusculeri.saglik*toAdd;
        }
    }
}
void ClearUnits() {
    for (int x = 0; x < GRID_COLUMNS; x++) {
        for (int y = 0; y < GRID_ROWS; y++) {
            humanUnits[x][y].count = 0;
            humanUnits[x][y].health = 0;
            orcUnits[x][y].count = 0;
            orcUnits[x][y].health = 0;
        }
    }
}
void ArkaplanCiz() {
    background =LoadTexture("background.png");
    gamelogo  = LoadTexture("logo.png");
}

int main() {


    setlocale(LC_ALL, "Turkish");
    const char* creatures_file = "creatures.json";
    char* creatures_json = dosyaOku(creatures_file);
    if (creatures_json == NULL) {
        free(creatures_json);
        return 1;
    }
    const char* heroes_file = "heroes.json";
    char* heroes_json = dosyaOku(heroes_file);
    if (heroes_json == NULL) {
        free(heroes_json);
        return 1;
    }

    const char* research_file = "research.json";
    char* research_json = dosyaOku(research_file);
    if(research_json == NULL ){
       free(research_json);
        return 1;
    }
    const char* unit_types_file = "unit_types.json";
    char* unit_types_json = dosyaOku(unit_types_file);
    if (unit_types_json == NULL) {
        free(unit_types_json);
        return 1;
    }

    Orc_yaratik Kara_Troll,Golge_Kurtlari,Camur_Devleri,Ates_iblisi,Buz_Devleri,Makro;
    degerAta_ork_yaratik(creatures_json, "Kara_Troll",&Kara_Troll);
    degerAta_ork_yaratik(creatures_json, "Golge_Kurtlari",&Golge_Kurtlari);
    degerAta_ork_yaratik(creatures_json, "Camur_Devleri",&Camur_Devleri);
    degerAta_ork_yaratik(creatures_json, "Ates_Iblisi",&Ates_iblisi);
    degerAta_ork_yaratik(creatures_json, "Buz_Devleri",&Buz_Devleri);
    degerAta_ork_yaratik(creatures_json,"Makrog_Savas_Beyi",&Makro);

    insan_yaratik ejderha, agri_dagi_devleri, tepegoz, karakurt, samur;
    degerAta_insan_yaratik(creatures_json, "Ejderha", &ejderha);
    degerAta_insan_yaratik(creatures_json, "Agri_Dagi_Devleri", &agri_dagi_devleri);
    degerAta_insan_yaratik(creatures_json, "Tepegoz", &tepegoz);
    degerAta_insan_yaratik(creatures_json, "Karakurt", &karakurt);
    degerAta_insan_yaratik(creatures_json, "Samur", &samur);

    ork_kahraman Goruk_Vahsi, Thruk_Kemikkiran, Vrog_Kafakiran,Ugar_Zalim;
    degerAta_ork_kahraman(heroes_json, "Goruk_Vahsi", &Goruk_Vahsi);
    degerAta_ork_kahraman(heroes_json, "Thruk_Kemikkiran", &Thruk_Kemikkiran);
    degerAta_ork_kahraman(heroes_json, "Vrog_Kafakiran", &Vrog_Kafakiran);
    degerAta_ork_kahraman(heroes_json, "Ugar_Zalim", &Ugar_Zalim);


    insan_kahraman alparslan, fatih, yavuz, tugrul,mete;
    degerAta_insan_kahraman(heroes_json, "Alparslan", &alparslan);
    degerAta_insan_kahraman(heroes_json, "Fatih_Sultan_Mehmet", &fatih);
    degerAta_insan_kahraman(heroes_json, "Yavuz_Sultan_Selim", &yavuz);
    degerAta_insan_kahraman(heroes_json, "Tugrul_Bey", &tugrul);
    degerAta_insan_kahraman(heroes_json,"Mete_Han",&mete);


    InsanImparatorlugu insan_imparatorlugu;
    OrkLegi ork_legi;


    degerAta_insan_imparatorlugu(unit_types_json, &insan_imparatorlugu);
    degerAta_ork_legi(unit_types_json, &ork_legi);

    char senaryosayi [50];
    printf("bir senaryo seciniz:");
    scanf("%s",&senaryosayi);
    if(strcmp(senaryosayi,"1")==0)
    system("  curl -o 1.json https://yapbenzet.org.tr/1.json");
    else if(strcmp(senaryosayi,"2")==0)
    system("  curl -o 2.json https://yapbenzet.org.tr/2.json");
    else if(strcmp(senaryosayi,"3")==0)
    system("  curl -o 3.json https://yapbenzet.org.tr/3.json");
    else if(strcmp(senaryosayi,"4")==0)
    system("  curl -o 4.json https://yapbenzet.org.tr/4.json");
    else if(strcmp(senaryosayi,"5")==0)
    system("  curl -o 5.json https://yapbenzet.org.tr/5.json");
    else  if(strcmp(senaryosayi,"6")==0)
    system("  curl -o 6.json https://yapbenzet.org.tr/6.json");
    else if(strcmp(senaryosayi,"7")==0)
    system("  curl -o 7.json https://yapbenzet.org.tr/7.json");
    else  if(strcmp(senaryosayi,"8")==0)
    system("  curl -o 8.json https://yapbenzet.org.tr/8.json");
    else if(strcmp(senaryosayi,"9")==0)
    system("  curl -o 9.json https://yapbenzet.org.tr/9.json");
    else if(strcmp(senaryosayi,"10")==0)
    system("  curl -o 10.json https://yapbenzet.org.tr/10.json");




  char jsontutucu[20]={".json"};
  char senaryotamad[50];
  strcpy(senaryotamad,senaryosayi);
  strcat(senaryotamad,jsontutucu);



    char* senaryo_json = dosyaOku(&senaryotamad);
    if (senaryo_json == NULL) {
        printf("Senaryo dosyasi acilamadi: \n");
        return;
    }

    ins_savas insan = {0};
    orc_savas ork = {0};

    degerAta_senaryo_data(senaryo_json, &insan, &ork);


    Ustalik savunma,saldiri,elit_egitim,kusatma;
    degerAta_bilgi_ustalik(research_json,&savunma,&saldiri,&elit_egitim,&kusatma);



    FILE *dosya;

    dosya=fopen("savas.sim.txt","w");


    if (dosya == NULL) {
        printf("Dosya acilamadi!\n");
        return 1;
    }
    fprintf(dosya,"SENARYOYA AIT BASLANGIC BILGILERI\n");
    fprintf(dosya,"---------------------------------");
    fprintf(dosya,"\nInsan_imparatorlugu Savas Bilgileri\n");
    fprintf(dosya,"--------------------------------------\n");
     if(insan.piyadeSayi>0)
             fprintf(dosya,"Piyadeler:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",insan.piyadeSayi,insan_imparatorlugu.piyadeler.saldiri,insan_imparatorlugu.piyadeler.savunma,insan_imparatorlugu.piyadeler.saglik,insan_imparatorlugu.piyadeler.kritik_sans);
    if(insan.okcuSayi>0)
            fprintf(dosya,"Okcular:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",insan.okcuSayi,insan_imparatorlugu.okcular.saldiri,insan_imparatorlugu.okcular.savunma,insan_imparatorlugu.okcular.saglik,insan_imparatorlugu.okcular.kritik_sans);
    if(insan.suvariSayi>0)
            fprintf(dosya,"Suvariler:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",insan.suvariSayi,insan_imparatorlugu.suvariler.saldiri,insan_imparatorlugu.suvariler.savunma,insan_imparatorlugu.suvariler.saglik,insan_imparatorlugu.suvariler.kritik_sans);
    if(insan.kusatmamakSayi>0)
            fprintf(dosya,"Kusatma Makineleri:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",insan.kusatmamakSayi,insan_imparatorlugu.kusatma_makineleri.saldiri,insan_imparatorlugu.kusatma_makineleri.savunma,insan_imparatorlugu.kusatma_makineleri.saglik,insan_imparatorlugu.kusatma_makineleri.kritik_sans);

    fprintf(dosya,"\nKahraman");


        if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"saldiri")==0){if(strcmp(fatih.etkiledigi_birim,"piyadeler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"savunma")==0){if(strcmp(fatih.etkiledigi_birim,"piyadeler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"kritik_sans")==0){if(strcmp(fatih.etkiledigi_birim,"piyadeler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"saldiri")==0){if(strcmp(fatih.etkiledigi_birim,"okcular")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"savunma")==0){if(strcmp(fatih.etkiledigi_birim,"okcular")==0){
                                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

            if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"kritik_sans")==0){if(strcmp(fatih.etkiledigi_birim,"okcular")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"saldiri")==0){if(strcmp(fatih.etkiledigi_birim,"suvariler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}


           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"savunma")==0){if(strcmp(fatih.etkiledigi_birim,"suvariler")==0){
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}
           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"kritik_sans")==0){if(strcmp(fatih.etkiledigi_birim,"suvariler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"saldiri")==0){if(strcmp(fatih.etkiledigi_birim,"kusatma_makineleri")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"savunma")==0){if(strcmp(fatih.etkiledigi_birim,"kusatma_makineleri")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"kritik_sans")==0){if(strcmp(fatih.etkiledigi_birim,"kusatma_makineleri")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

            if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"saldiri")==0){if(strcmp(fatih.etkiledigi_birim,"tum_birimler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"savunma")==0){if(strcmp(fatih.etkiledigi_birim,"tum_birimler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}

           if(strcmp(insan.kahraman,"Fatih_Sultan_Mehmet")==0){if(strcmp(fatih.bonus_turu,"kritik_sans")==0){if(strcmp(fatih.etkiledigi_birim,"tum_birimler")==0){
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,fatih.etkiledigi_birim,fatih.bonus_degeri_float,fatih.bonus_turu);}}}





            if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"saldiri")==0)&&(strcmp(alparslan.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"savunma")==0)&&(strcmp(alparslan.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"kritik_sans")==0)&&(strcmp(alparslan.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"saldiri")==0)&&(strcmp(alparslan.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"savunma")==0)&&(strcmp(alparslan.etkiledigi_birim,"okcular")==0))
                                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

            if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"kritik_sans")==0)&&(strcmp(alparslan.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"saldiri")==0)&&(strcmp(alparslan.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);


           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"savunma")==0)&&(strcmp(alparslan.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);
           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"kritik_sans")==0)&&(strcmp(alparslan.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"saldiri")==0)&&(strcmp(alparslan.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"savunma")==0)&&(strcmp(alparslan.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"kritik_sans")==0)&&(strcmp(alparslan.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

        if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"saldiri")==0)&&(strcmp(alparslan.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"savunma")==0)&&(strcmp(alparslan.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);

           if((strcmp(insan.kahraman,"Alparslan")==0)&&(strcmp(alparslan.bonus_turu,"kritik_sans")==0)&&(strcmp(alparslan.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,alparslan.etkiledigi_birim,alparslan.bonus_degeri_float,alparslan.bonus_turu);




            if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"saldiri")==0)&&(strcmp(mete.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"savunma")==0)&&(strcmp(mete.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"kritik_sans")==0)&&(strcmp(mete.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"saldiri")==0)&&(strcmp(mete.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"savunma")==0)&&(strcmp(mete.etkiledigi_birim,"okcular")==0))
                                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

            if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"kritik_sans")==0)&&(strcmp(mete.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"saldiri")==0)&&(strcmp(mete.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);


           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"savunma")==0)&&(strcmp(mete.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);
           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"kritik_sans")==0)&&(strcmp(mete.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"saldiri")==0)&&(strcmp(mete.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"savunma")==0)&&(strcmp(mete.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"kritik_sans")==0)&&(strcmp(mete.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);


        if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"saldiri")==0)&&(strcmp(mete.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"savunma")==0)&&(strcmp(mete.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);

           if((strcmp(insan.kahraman,"Mete_Han")==0)&&(strcmp(mete.bonus_turu,"kritik_sans")==0)&&(strcmp(mete.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,mete.etkiledigi_birim,mete.bonus_degeri_float,mete.bonus_turu);



            if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"saldiri")==0)&&(strcmp(yavuz.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"savunma")==0)&&(strcmp(yavuz.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"kritik_sans")==0)&&(strcmp(yavuz.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"saldiri")==0)&&(strcmp(yavuz.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"savunma")==0)&&(strcmp(yavuz.etkiledigi_birim,"okcular")==0))
                                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

            if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"kritik_sans")==0)&&(strcmp(yavuz.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"saldiri")==0)&&(strcmp(yavuz.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);


           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"savunma")==0)&&(strcmp(yavuz.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);
           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"kritik_sans")==0)&&(strcmp(yavuz.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"saldiri")==0)&&(strcmp(yavuz.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"savunma")==0)&&(strcmp(yavuz.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"kritik_sans")==0)&&(strcmp(yavuz.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

            if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"saldiri")==0)&&(strcmp(yavuz.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"savunma")==0)&&(strcmp(yavuz.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);

           if((strcmp(insan.kahraman,"Yavuz_Sultan_Selim")==0)&&(strcmp(yavuz.bonus_turu,"kritik_sans")==0)&&(strcmp(yavuz.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,yavuz.etkiledigi_birim,yavuz.bonus_degeri_float,yavuz.bonus_turu);



            if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"saldiri")==0)&&(strcmp(tugrul.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"savunma")==0)&&(strcmp(tugrul.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"kritik_sans")==0)&&(strcmp(tugrul.etkiledigi_birim,"piyadeler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"saldiri")==0)&&(strcmp(tugrul.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"savunma")==0)&&(strcmp(tugrul.etkiledigi_birim,"okcular")==0))
                                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

            if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"kritik_sans")==0)&&(strcmp(tugrul.etkiledigi_birim,"okcular")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"saldiri")==0)&&(strcmp(tugrul.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);


           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"savunma")==0)&&(strcmp(tugrul.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);
           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"kritik_sans")==0)&&(strcmp(tugrul.etkiledigi_birim,"suvariler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"saldiri")==0)&&(strcmp(tugrul.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"savunma")==0)&&(strcmp(tugrul.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"kritik_sans")==0)&&(strcmp(tugrul.etkiledigi_birim,"kusatma_makineleri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

            if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"saldiri")==0)&&(strcmp(tugrul.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"savunma")==0)&&(strcmp(tugrul.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);

           if((strcmp(insan.kahraman,"Tugrul_Bey")==0)&&(strcmp(tugrul.bonus_turu,"kritik_sans")==0)&&(strcmp(tugrul.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.kahraman,tugrul.etkiledigi_birim,tugrul.bonus_degeri_float,tugrul.bonus_turu);



            fprintf(dosya,"\n\nCanavar");



            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"saldiri")==0)&&(strcmp(ejderha.etkiledigi_birim,"piyadeler")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"saldiri")==0)&&(strcmp(ejderha.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"saldiri")==0)&&(strcmp(ejderha.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"saldiri")==0)&&(strcmp(ejderha.etkiledigi_birim,"kusatma_makineleri")==0))
                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"saldiri")==0)&&(strcmp(ejderha.etkiledigi_birim,"tum_birimler")==0))
                fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"savunma")==0)&&(strcmp(ejderha.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"savunma")==0)&&(strcmp(ejderha.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"savunma")==0)&&(strcmp(ejderha.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"savunma")==0)&&(strcmp(ejderha.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"savunma")==0)&&(strcmp(ejderha.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"kritik_sans")==0)&&(strcmp(ejderha.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"kritik_sans")==0)&&(strcmp(ejderha.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"kritik_sans")==0)&&(strcmp(ejderha.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"kritik_sans")==0)&&(strcmp(ejderha.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);
            if((strcmp(insan.canavar,"Ejderha")== 0)&&(strcmp(ejderha.etki_turu,"kritik_sans")==0)&&(strcmp(ejderha.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,ejderha.etkiledigi_birim,ejderha.etki_degeri_float,ejderha.etki_turu);


            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"saldiri")==0)&&(strcmp(karakurt.etkiledigi_birim,"piyadeler")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"saldiri")==0)&&(strcmp(karakurt.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"saldiri")==0)&&(strcmp(karakurt.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"saldiri")==0)&&(strcmp(karakurt.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"saldiri")==0)&&(strcmp(karakurt.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"savunma")==0)&&(strcmp(karakurt.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"savunma")==0)&&(strcmp(karakurt.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"savunma")==0)&&(strcmp(karakurt.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"savunma")==0)&&(strcmp(karakurt.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"savunma")==0)&&(strcmp(karakurt.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"kritik_sans")==0)&&(strcmp(karakurt.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"kritik_sans")==0)&&(strcmp(karakurt.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"kritik_sans")==0)&&(strcmp(karakurt.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"kritik_sans")==0)&&(strcmp(karakurt.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);
            if((strcmp(insan.canavar,"Karakurt")== 0)&&(strcmp(karakurt.etki_turu,"kritik_sans")==0)&&(strcmp(karakurt.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,karakurt.etkiledigi_birim,karakurt.etki_degeri_float,karakurt.etki_turu);


            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"saldiri")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"piyadeler")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"saldiri")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"saldiri")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"saldiri")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"saldiri")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"savunma")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"savunma")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"savunma")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"savunma")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"savunma")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);
            if((strcmp(insan.canavar,"Agri_Dagi_Devleri")== 0)&&(strcmp(agri_dagi_devleri.etki_turu,"kritik_sans")==0)&&(strcmp(agri_dagi_devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,agri_dagi_devleri.etkiledigi_birim,agri_dagi_devleri.etki_degeri_float,agri_dagi_devleri.etki_turu);


            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"saldiri")==0)&&(strcmp(tepegoz.etkiledigi_birim,"piyadeler")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"saldiri")==0)&&(strcmp(tepegoz.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"saldiri")==0)&&(strcmp(tepegoz.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"saldiri")==0)&&(strcmp(tepegoz.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"saldiri")==0)&&(strcmp(tepegoz.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"savunma")==0)&&(strcmp(tepegoz.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"savunma")==0)&&(strcmp(tepegoz.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"savunma")==0)&&(strcmp(tepegoz.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"savunma")==0)&&(strcmp(tepegoz.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"savunma")==0)&&(strcmp(tepegoz.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"kritik_sans")==0)&&(strcmp(tepegoz.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"kritik_sans")==0)&&(strcmp(tepegoz.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"kritik_sans")==0)&&(strcmp(tepegoz.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"kritik_sans")==0)&&(strcmp(tepegoz.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);
            if((strcmp(insan.canavar,"Tepegoz")== 0)&&(strcmp(tepegoz.etki_turu,"kritik_sans")==0)&&(strcmp(tepegoz.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,tepegoz.etkiledigi_birim,tepegoz.etki_degeri_float,tepegoz.etki_turu);

            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"saldiri")==0)&&(strcmp(samur.etkiledigi_birim,"piyadeler")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"saldiri")==0)&&(strcmp(samur.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"saldiri")==0)&&(strcmp(samur.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"saldiri")==0)&&(strcmp(samur.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"saldiri")==0)&&(strcmp(samur.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"savunma")==0)&&(strcmp(samur.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"savunma")==0)&&(strcmp(samur.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"savunma")==0)&&(strcmp(samur.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"savunma")==0)&&(strcmp(samur.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"savunma")==0)&&(strcmp(samur.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"kritik_sans")==0)&&(strcmp(samur.etkiledigi_birim,"piyadeler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"kritik_sans")==0)&&(strcmp(samur.etkiledigi_birim,"okcular")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"kritik_sans")==0)&&(strcmp(samur.etkiledigi_birim,"suvariler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"kritik_sans")==0)&&(strcmp(samur.etkiledigi_birim,"kusatma_makineleri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);
            if((strcmp(insan.canavar,"Samur")== 0)&&(strcmp(samur.etki_turu,"kritik_sans")==0)&&(strcmp(samur.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",insan.canavar,samur.etkiledigi_birim,samur.etki_degeri_float,samur.etki_turu);



            fprintf(dosya,"\n-----------------------------------------------------\n");
            fprintf(dosya,"Ork_Lejyonuna Ait Savas Bilgileri\n");
            fprintf(dosya,"-----------------------------------------------------\n");
            if(ork.ork_dovuscuSayi>0)
                fprintf(dosya,"Ork_Dovusculeri:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",ork.ork_dovuscuSayi,ork_legi.ork_dovusculeri.saldiri,ork_legi.ork_dovusculeri.savunma,ork_legi.ork_dovusculeri.saglik,ork_legi.ork_dovusculeri.kritik_sans);
            if(ork.mizrakciSayi>0)
                fprintf(dosya,"Mizrakcilar:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",ork.mizrakciSayi,ork_legi.mizrakcilar.saldiri,ork_legi.mizrakcilar.savunma,ork_legi.mizrakcilar.saglik,ork_legi.mizrakcilar.kritik_sans);
            if(ork.varg_biniciSayi>0)
                fprintf(dosya,"Varg_Binicileri:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",ork.varg_biniciSayi,ork_legi.varg_binicileri.saldiri,ork_legi.varg_binicileri.savunma,ork_legi.varg_binicileri.saglik,ork_legi.varg_binicileri.kritik_sans);
            if(ork.trollerSayi>0)
                fprintf(dosya,"Troller:%1.f birim(Saldiri Gucu:%f,Savunma Gucu:%f,Saglik:%f,Kritik Vurusu Sansi:%%%f\n\n",ork.trollerSayi,ork_legi.troller.saldiri,ork_legi.troller.savunma,ork_legi.troller.saglik,ork_legi.troller.kritik_sans);

             fprintf(dosya,"\nKahraman");


            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"saldiri")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"saldiri")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"saldiri")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"saldiri")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"saldiri")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"savunma")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"savunma")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"savunma")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"savunma")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"savunma")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"kritik_sans")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"kritik_sans")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"kritik_sans")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"kritik_sans")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);
            if((strcmp(ork.kahraman,"Goruk_Vahsi")==0)&&(strcmp(Goruk_Vahsi.bonus_turu,"kritik_sans")==0)&&(strcmp(Goruk_Vahsi.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Goruk_Vahsi.etkiledigi_birim,Goruk_Vahsi.bonus_degeri_float,Goruk_Vahsi.bonus_turu);

            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"saldiri")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"saldiri")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"saldiri")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"saldiri")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"saldiri")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"savunma")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"savunma")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"savunma")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"savunma")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"savunma")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);
            if((strcmp(ork.kahraman,"Thruk_Kemikkiran")==0)&&(strcmp(Thruk_Kemikkiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Thruk_Kemikkiran.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Thruk_Kemikkiran.etkiledigi_birim,Thruk_Kemikkiran.bonus_degeri_float,Thruk_Kemikkiran.bonus_turu);

            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"saldiri")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"saldiri")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"saldiri")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"saldiri")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"saldiri")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"savunma")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"savunma")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"savunma")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"savunma")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"savunma")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);
            if((strcmp(ork.kahraman,"Vrog_Kafakiran")==0)&&(strcmp(Vrog_Kafakiran.bonus_turu,"kritik_sans")==0)&&(strcmp(Vrog_Kafakiran.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Vrog_Kafakiran.etkiledigi_birim,Vrog_Kafakiran.bonus_degeri_float,Vrog_Kafakiran.bonus_turu);

            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"saldiri")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"saldiri")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"saldiri")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"saldiri")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"saldiri")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"savunma")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"savunma")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"savunma")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"savunma")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"savunma")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"kritik_sans")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"ork_dovusculeri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"kritik_sans")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"mizrakciler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"kritik_sans")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"varg_binicileri")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"kritik_sans")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"troller")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);
            if((strcmp(ork.kahraman,"Ugar_Zalim")==0)&&(strcmp(Ugar_Zalim.bonus_turu,"kritik_sans")==0)&&(strcmp(Ugar_Zalim.etkiledigi_birim,"tum_birimler")==0))
                               fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.kahraman,Ugar_Zalim.etkiledigi_birim,Ugar_Zalim.bonus_degeri_float,Ugar_Zalim.bonus_turu);


            fprintf(dosya,"\n\nCanavar");


            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"saldiri")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"ork_dovusculeri")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"saldiri")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"saldiri")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"saldiri")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"saldiri")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"savunma")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"savunma")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"savunma")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"savunma")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"savunma")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"kritik_sans")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"kritik_sans")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"kritik_sans")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"kritik_sans")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);
            if((strcmp(ork.canavar,"Kara_Troll")== 0)&&(strcmp(Kara_Troll.etki_turu,"kritik_sans")==0)&&(strcmp(Kara_Troll.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Kara_Troll.etkiledigi_birim,Kara_Troll.etki_degeri_float,Kara_Troll.etki_turu);

            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"saldiri")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"ork_dovusculeri")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"saldiri")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"saldiri")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"saldiri")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"saldiri")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"savunma")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"savunma")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"savunma")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"savunma")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"savunma")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"kritik_sans")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"kritik_sans")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"kritik_sans")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"kritik_sans")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);
            if((strcmp(ork.canavar,"Golge_Kurtlari")== 0)&&(strcmp(Golge_Kurtlari.etki_turu,"kritik_sans")==0)&&(strcmp(Golge_Kurtlari.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Golge_Kurtlari.etkiledigi_birim,Golge_Kurtlari.etki_degeri_float,Golge_Kurtlari.etki_turu);

            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"ork_dovusculeri")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"savunma")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"savunma")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"savunma")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"savunma")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"savunma")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Camur_Devleri")== 0)&&(strcmp(Camur_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Camur_Devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Camur_Devleri.etkiledigi_birim,Camur_Devleri.etki_degeri_float,Camur_Devleri.etki_turu);

            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"saldiri")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"ork_dovusculeri")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"saldiri")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"saldiri")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"saldiri")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"saldiri")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"savunma")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"savunma")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"savunma")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"savunma")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"savunma")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"kritik_sans")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"kritik_sans")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"kritik_sans")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"kritik_sans")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);
            if((strcmp(ork.canavar,"Ates_Iblisi")== 0)&&(strcmp(Ates_iblisi.etki_turu,"kritik_sans")==0)&&(strcmp(Ates_iblisi.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Ates_iblisi.etkiledigi_birim,Ates_iblisi.etki_degeri_float,Ates_iblisi.etki_turu);


            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"saldiri")==0)&&(strcmp(Makro.etkiledigi_birim,"ork_dovusculeri")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"saldiri")==0)&&(strcmp(Makro.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"saldiri")==0)&&(strcmp(Makro.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"saldiri")==0)&&(strcmp(Makro.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"saldiri")==0)&&(strcmp(Makro.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"savunma")==0)&&(strcmp(Makro.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"savunma")==0)&&(strcmp(Makro.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"savunma")==0)&&(strcmp(Makro.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"savunma")==0)&&(strcmp(Makro.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"savunma")==0)&&(strcmp(Makro.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"kritik_sans")==0)&&(strcmp(Makro.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"kritik_sans")==0)&&(strcmp(Makro.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"kritik_sans")==0)&&(strcmp(Makro.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"kritik_sans")==0)&&(strcmp(Makro.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);
            if((strcmp(ork.canavar,"Makrog_Savas_Beyi")== 0)&&(strcmp(Makro.etki_turu,"kritik_sans")==0)&&(strcmp(Makro.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Makro.etkiledigi_birim,Makro.etki_degeri_float,Makro.etki_turu);

            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"ork_dovusculeri")==0))
                        fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"saldiri")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"savunma")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"savunma")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"savunma")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"savunma")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"savunma")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"ork_dovusculeri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"mizrakciler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"varg_binicileri")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"troller")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);
            if((strcmp(ork.canavar,"Buz_Devleri")== 0)&&(strcmp(Buz_Devleri.etki_turu,"kritik_sans")==0)&&(strcmp(Buz_Devleri.etkiledigi_birim,"tum_birimler")==0))
                            fprintf(dosya,"\n%s :(%s %%%f %s Gucu Bonusu)",ork.canavar,Buz_Devleri.etkiledigi_birim,Buz_Devleri.etki_degeri_float,Buz_Devleri.etki_turu);



            fprintf(dosya,"\n\nArastirma Degerleri\n\n");

            fprintf(dosya,"Insan Imparatorlugu\n");
            if(insan.saldiri_arastirmaSeviye==1)
                fprintf(dosya,"Saldiri Gelistimesi (Seviye:%d):Tum birimlerin saldirisini  %%%d arttirir",insan.saldiri_arastirmaSeviye,saldiri.seviye_1.deger_int);
            if(insan.saldiri_arastirmaSeviye==2)
                fprintf(dosya,"Saldiri Gelistimesi (Seviye:%d):Tum birimlerin saldirisini   %%%d arttirir",insan.saldiri_arastirmaSeviye,saldiri.seviye_2.deger_int);
            if(insan.saldiri_arastirmaSeviye==3)
                fprintf(dosya,"Saldiri Gelistimesi (Seviye:%d):Tum birimlerin saldirisini   %%%d arttirir",insan.saldiri_arastirmaSeviye,saldiri.seviye_3.deger_int);

            if(insan.savunma_arastirmaSeviye==1)
                fprintf(dosya,"Savunma Ustaligi (Seviye:%d):Tum birimlerin savunmasini  %%%d arttirir",insan.savunma_arastirmaSeviye,savunma.seviye_1.deger_int);
            if(insan.savunma_arastirmaSeviye==2)
                fprintf(dosya,"Savunma Ustaligi (Seviye:%d):Tum birimlerin savunmasini   %%%d arttirir",insan.savunma_arastirmaSeviye,savunma.seviye_2.deger_int);
            if(insan.savunma_arastirmaSeviye==3)
                fprintf(dosya,"Savunma Ustaligi (Seviye:%d):Tum birimlerin savunmasini   %%%d arttirir",insan.savunma_arastirmaSeviye,savunma.seviye_3.deger_int);

            if(insan.elit_egitimSeviye==1)
                fprintf(dosya,"Elit_Egitim Ustaligi (Seviye:%d):Tum birimlerin kritik_sansini  %%%d arttirir",insan.elit_egitimSeviye,elit_egitim.seviye_1.deger_int);
            if(insan.elit_egitimSeviye==2)
                fprintf(dosya,"Elit_Egitim Ustaligi (Seviye:%d):Tum birimlerin kritik_sansini   %%%d arttirir",insan.elit_egitimSeviye,elit_egitim.seviye_2.deger_int);
            if(insan.elit_egitimSeviye==3)
                fprintf(dosya,"Elit_Egitim Ustaligi (Seviye:%d):Tum birimlerin kritik_sansini   %%%d arttirir",insan.elit_egitimSeviye,elit_egitim.seviye_3.deger_int);

            if(insan.kusatma_egitimSeviye==1)
                fprintf(dosya,"Kusatma_makineleri Ustaligi (Seviye:%d):kusatma_makinelerinin kritik_sansini  %%%d arttirir",insan.kusatma_egitimSeviye,kusatma.seviye_1.deger_int);
            if(insan.kusatma_egitimSeviye==2)
                fprintf(dosya,"Kusatma_makineleri Ustaligi (Seviye:%d):kusatma_makinelerinin kritik_sansini   %%%d arttirir",insan.kusatma_egitimSeviye,kusatma.seviye_2.deger_int);
            if(insan.kusatma_egitimSeviye==3)
                fprintf(dosya,"Kusatma_makineleri Ustaligi (Seviye:%d):kusatma_makinelerinin kritik_sansini   %%%d arttirir",insan.kusatma_egitimSeviye,kusatma.seviye_3.deger_int);


            fprintf(dosya,"\n\nOrk_Lejyonu\n");


            if(ork.saldiri_arastirmaSeviye==1)
                fprintf(dosya,"Saldiri Gelistimesi (Seviye:%d):Tum birimlerin saldirisini  %%%d arttirir",ork.saldiri_arastirmaSeviye,saldiri.seviye_1.deger_int);
            if(ork.saldiri_arastirmaSeviye==2)
                fprintf(dosya,"Saldiri Gelistimesi (Seviye:%d):Tum birimlerin saldirisini   %%%d arttirir",ork.saldiri_arastirmaSeviye,saldiri.seviye_2.deger_int);
            if(ork.saldiri_arastirmaSeviye==3)
                fprintf(dosya,"Saldiri Gelistimesi (Seviye:%d):Tum birimlerin saldirisini   %%%d arttirir",ork.saldiri_arastirmaSeviye,saldiri.seviye_3.deger_int);

            if(ork.savunma_arastirmaSeviye==1)
                fprintf(dosya,"Savunma Ustaligi (Seviye:%d):Tum birimlerin savunmasini  %%%d arttirir",ork.savunma_arastirmaSeviye,savunma.seviye_1.deger_int);
            if(ork.savunma_arastirmaSeviye==2)
                fprintf(dosya,"Savunma Ustaligi (Seviye:%d):Tum birimlerin savunmasini   %%%d arttirir",ork.savunma_arastirmaSeviye,savunma.seviye_2.deger_int);
            if(ork.savunma_arastirmaSeviye==3)
                fprintf(dosya,"Savunma Ustaligi (Seviye:%d):Tum birimlerin savunmasini   %%%d arttirir",ork.savunma_arastirmaSeviye,savunma.seviye_3.deger_int);

            if(ork.elit_egitimSeviye==1)
                fprintf(dosya,"Elit_Egitim Ustaligi (Seviye:%d):Tum birimlerin kritik_sansini  %%%d arttirir",ork.elit_egitimSeviye,elit_egitim.seviye_1.deger_int);
            if(ork.elit_egitimSeviye==2)
                fprintf(dosya,"Elit_Egitim Ustaligi (Seviye:%d):Tum birimlerin kritik_sansini   %%%d arttirir",ork.elit_egitimSeviye,elit_egitim.seviye_2.deger_int);
            if(ork.elit_egitimSeviye==3)
                fprintf(dosya,"Elit_Egitim Ustaligi (Seviye:%d):Tum birimlerin kritik_sansini   %%%d arttirir",ork.elit_egitimSeviye,elit_egitim.seviye_3.deger_int);

            fprintf(dosya,"\n\n----------------------------INSAN IMPARATORLUGU---------------VS---------------ORK LEJYONU---------------------------\n\n");



        fclose(dosya);





        float AdimSayisi2=-1;
        float AdimSayisi1=-2;
        float NetHasar=0;
        float NetHasar2=0;




    insansaldirigucunuhesapla(&insan,&insan_imparatorlugu,&ejderha,&tepegoz,&agri_dagi_devleri,&karakurt,&samur,&fatih,&tugrul,&alparslan,&mete,&yavuz,&savunma,&saldiri,&elit_egitim, &kusatma,&AdimSayisi2);
    orksavunmagucunuhesapla(&ork,&ork_legi,&Goruk_Vahsi,&Thruk_Kemikkiran,&Vrog_Kafakiran,&Ugar_Zalim,&Kara_Troll,&Ates_iblisi,&Golge_Kurtlari,&Camur_Devleri,&Buz_Devleri,&savunma,&saldiri,&elit_egitim, &kusatma);
    orksaldirigucunuhesapla(&ork,&ork_legi,&Goruk_Vahsi,&Thruk_Kemikkiran,&Vrog_Kafakiran,&Ugar_Zalim,&Kara_Troll,&Ates_iblisi,&Golge_Kurtlari,&Camur_Devleri,&Buz_Devleri,&savunma,&saldiri,&elit_egitim, &kusatma,&AdimSayisi1);
    insansavunmagucunuhesapla(&insan,&insan_imparatorlugu,&ejderha,&tepegoz,&agri_dagi_devleri,&karakurt,&samur,&fatih,&tugrul,&alparslan,&mete,&yavuz,&savunma,&saldiri,&elit_egitim, &kusatma);






        int baslangicVarg=ork.varg_biniciSayi;
        int baslangicTroll=ork.trollerSayi;
        int baslangicMizrakci=ork.mizrakciSayi;
        int baslangicDovuscu=ork.ork_dovuscuSayi;
        int baslangicPiyade=insan.piyadeSayi;
        int baslangicOkcu=insan.okcuSayi;
        int baslangicSuvari=insan.suvariSayi;
        int baslangicKusatma=insan.kusatmamakSayi;





               while(((insan.piyadeSayi>0 || insan.okcuSayi>0 || insan.suvariSayi>0 || insan.kusatmamakSayi>0)!=0) ||((ork.mizrakciSayi>0 || ork.varg_biniciSayi>0 || ork.trollerSayi>0 || ork.ork_dovuscuSayi>0)!= 0)){

        dosya=fopen("savas.sim.txt","a");


        if (dosya == NULL) {
            printf("Dosya acilamadi!\n");
            return 1;
        }
        fprintf(dosya,"%0.f.Adim (Insanlar Saldiriyor-Ork Savunuyor)\n",AdimSayisi2);
        fprintf(dosya,"------------------------------------------------------\n");
        fprintf(dosya,"Piyade toplam Saldiri gucu:%f\n Okcu toplam Saldiri gucu:%f\n Suvari toplam Saldiri gucu:%f\n Kusatmamak toplam Saldiri gucu:%f\n",insan.toplamPiyadeSaldirigucu,insan.toplamOkcuSaldirigucu,insan.toplamSuvariSaldirigucu,insan.toplamKusatmamakSaldirigucu);
        fprintf(dosya,"Insan Imparatorlugu Saldiri Gucu:%f\n",insan.insantoplamSaldirigucu);
        fprintf(dosya,"Mizrakci toplam Savunma gucu:%f\n Ork_Dovuscu toplam Savunma gucu:%f\n Varg_Binici toplam Savunma gucu:%f\n Troller toplam Savunma gucu:%f\n",ork.toplamMizrakciSavunmagucu,ork.toplamDovuscuSavunmagucu,ork.toplamVargBiniciSavunmagucu,ork.toplamTrolSavunmagucu);
        fprintf(dosya,"Ork Lejyonu Savunma Gucu:%f\n",ork.orktoplamSavunmagucu);

        NetHasarHesaplamaFonksiyonu(&insan,&ork,&insan_imparatorlugu,&ork_legi,&AdimSayisi2,&NetHasar);

        if(AdimSayisi2>5)
            fprintf(dosya,"Yorgunluk etkin\n");

        fprintf(dosya,"Net Hasar Hesaplamasi:%f\n",NetHasar);
        fprintf(dosya,"\nInsan Saldirisindan Sonra Degisen Degerler\n");
        fprintf(dosya,"------------------------------------------------------\n");

        orksaldirigucunuhesapla(&ork,&ork_legi,&Goruk_Vahsi,&Thruk_Kemikkiran,&Vrog_Kafakiran,&Ugar_Zalim,&Kara_Troll,&Ates_iblisi,&Golge_Kurtlari,&Camur_Devleri,&Buz_Devleri,&savunma,&saldiri,&elit_egitim, &kusatma,&AdimSayisi1);
        orksavunmagucunuhesapla(&ork,&ork_legi,&Goruk_Vahsi,&Thruk_Kemikkiran,&Vrog_Kafakiran,&Ugar_Zalim,&Kara_Troll,&Ates_iblisi,&Golge_Kurtlari,&Camur_Devleri,&Buz_Devleri,&savunma,&saldiri,&elit_egitim, &kusatma);

        fprintf(dosya,"Mizrakci Sayisi:%f\n",ork.mizrakciSayi);
        fprintf(dosya,"Mizrakcilarin Saglik Degeri:%f\n\n",ork_legi.mizrakcilar.saglik);
        fprintf(dosya,"Ork_Dovuscu Sayisi:%f\n",ork.ork_dovuscuSayi);
        fprintf(dosya,"Ork_Dovusculerinin Saglik Degeri:%f\n\n",ork_legi.ork_dovusculeri.saglik);
        fprintf(dosya,"Varg_Binicilerinin Sayisi:%f\n",ork.varg_biniciSayi);
        fprintf(dosya,"Varg_Binicilerinin Saglik Degeri:%f\n\n",ork_legi.varg_binicileri.saglik);
        fprintf(dosya,"Trollerin Sayisi:%f\n",ork.trollerSayi);
        fprintf(dosya,"Trollerin Saglik Degeri:%f\n\n",ork_legi.troller.saglik);
        fprintf(dosya,"---------------------------------------------------------------------\n");


                  if(((insan.okcuSayi==0)&&(insan.suvariSayi==0)&&(insan.piyadeSayi==0)&&(insan.kusatmamakSayi==0)) ||
                      ((ork.mizrakciSayi==0)&&(ork.varg_biniciSayi==0)&&(ork.ork_dovuscuSayi==0)&&(ork.trollerSayi==0)))
            {
                if((insan.okcuSayi==0)&&(insan.suvariSayi==0)&&(insan.piyadeSayi==0)&&(insan.kusatmamakSayi==0)==1){
                fprintf(dosya,"\nSavas bitti.Ork Lejyonu Insan Imparatorlugunu Yendi.\n");
                    break;
                }
                else if((ork.mizrakciSayi==0)&&(ork.varg_biniciSayi==0)&&(ork.ork_dovuscuSayi==0)&&(ork.trollerSayi==0)==1)

                {

                    fprintf(dosya,"\nSavas bitti.Insan Imparatorlugu Ork Lejyonunu Yendi .\n");
                    break;
                }


            }

        fprintf(dosya,"%0.f.Adim (Ork Saldiriyor-Insanlar Savunuyor)\n",AdimSayisi1);
        fprintf(dosya,"------------------------------------------------------\n");
        fprintf(dosya,"Mizrakci toplam Saldiri gucu:%f\n Dovuscu toplam Saldiri gucu:%f\n Varg_binici toplam Saldiri gucu:%f\n Troller toplam Saldiri gucu:%f\n",ork.toplamMizrakciSaldirigucu,ork.toplamDovuscuSaldirigucu,ork.toplamVargBiniciSaldirigucu,ork.toplamTrolSaldirigucu);
        fprintf(dosya,"Ork Lejyonu Saldiri Gucu:%f\n",ork.orktoplamSaldirigucu);
        fprintf(dosya,"Piyade toplam Savunma gucu:%f\n Okcu toplam Savunma gucu:%f\n Suvari toplam Savunma gucu:%f\n Kusatma_makineleri toplam Savunma gucu:%f\n",insan.toplamPiyadeSavunmagucu,insan.toplamOkcuSavunmagucu,insan.toplamSuvariSavunmagucu,insan.toplamKusatmamakSavunmagucu);
        fprintf(dosya,"Insan Imparatorlugu Savunma Gucu:%f\n",insan.insantoplamSavunmagucu);


        NetHasarHesaplamaFonksiyonu2(&insan,&ork,&insan_imparatorlugu,&ork_legi,&AdimSayisi1,&NetHasar2);

        if(AdimSayisi1>5)
            fprintf(dosya,"Yorgunluk etkin\n");

        fprintf(dosya,"Net Hasar Hesaplamasi:%f\n",NetHasar2);
        fprintf(dosya,"\Ork Saldirisindan Sonra Degisen Degerler\n");
        fprintf(dosya,"------------------------------------------------------\n");

        insansaldirigucunuhesapla(&insan,&insan_imparatorlugu,&ejderha,&tepegoz,&agri_dagi_devleri,&karakurt,&samur,&fatih,&tugrul,&alparslan,&mete,&yavuz,&savunma,&saldiri,&elit_egitim, &kusatma,&AdimSayisi2);
        insansavunmagucunuhesapla(&insan,&insan_imparatorlugu,&ejderha,&tepegoz,&agri_dagi_devleri,&karakurt,&samur,&fatih,&tugrul,&alparslan,&mete,&yavuz,&savunma,&saldiri,&elit_egitim, &kusatma);


        fprintf(dosya,"Piyadelerin Sayisi:%f\n",insan.piyadeSayi);
        fprintf(dosya,"Piyadelerin Saglik Degeri:%f\n\n",insan_imparatorlugu.piyadeler.saglik);
        fprintf(dosya,"Okcularin Sayisi:%f\n",insan.okcuSayi);
        fprintf(dosya,"Okcularin Saglik Degeri:%f\n\n",insan_imparatorlugu.okcular.saglik);
        fprintf(dosya,"Suvarilerin Sayisi:%f\n",insan.suvariSayi);
        fprintf(dosya,"Suvarilerin Saglik Degeri:%f\n\n",insan_imparatorlugu.suvariler.saglik);
        fprintf(dosya,"Kusatma_Makinelerinin Sayisi:%f\n",insan.kusatmamakSayi);
        fprintf(dosya,"Kusatma_Makinelerinin Saglik Degeri:%f\n\n",insan_imparatorlugu.kusatma_makineleri.saglik);
        fprintf(dosya,"---------------------------------------------------------------------\n");




         if(((insan.okcuSayi==0)&&(insan.suvariSayi==0)&&(insan.piyadeSayi==0)&&(insan.kusatmamakSayi==0)) || ((ork.mizrakciSayi==0)&&(ork.varg_biniciSayi==0)&&(ork.ork_dovuscuSayi==0)&&(ork.trollerSayi==0)))

            {
                if((insan.okcuSayi==0)&&(insan.suvariSayi==0)&&(insan.piyadeSayi==0)&&(insan.kusatmamakSayi==0)==1){
                  fprintf(dosya,"\nSavas bitti.Ork Lejyonu Insan Imparatorlugunu Yendi.\n");
                    break;
                }
                else if((ork.mizrakciSayi==0)&&(ork.varg_biniciSayi==0)&&(ork.ork_dovuscuSayi==0)&&(ork.trollerSayi==0)==1)

                {


                    fprintf(dosya,"\nSavas bitti.Insan Imparatorlugu Ork Lejyonunu Yendi .\n");
                    break;
                }


            }




            fclose(dosya);

        }





    const int screenWidth = 1500;
    const int screenHeight = 1024;
InitWindow(screenWidth, screenHeight, "ORKvsHUMAN");
humanPiyade =LoadTexture("piyade.png");
humanOkcu = LoadTexture("archer.png");
orcDovuscu = LoadTexture("ork_dovuscu.png");
orcMizrakci = LoadTexture("mizrakcilar.png");
humanSuvari=LoadTexture("suvari.png");
humanKusatma=LoadTexture("kusatma.png");
orcVargBinici=LoadTexture("varg_binici.png");
orcTroller=LoadTexture("troller.png");

ArkaplanCiz();

    SetTargetFPS(60);

    int gridWidth = GRID_COLUMNS * GRID_CELL_SIZE;
    int gridHeight = GRID_ROWS * GRID_CELL_SIZE;

    int offsetX = (screenWidth - gridWidth) / 2;
    int offsetY = (screenHeight - gridHeight) / 2;

while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);
    DrawTexture(gamelogo,500,-30,WHITE);

    if (gameState == START_SCREEN) {

        DrawText("OYUNA HOSGELDINIZ!", screenWidth / 2 - 100, screenHeight / 2 - 50, 20, WHITE);
        DrawText("SAVASI BASLATMAK ICIN PRESS ENTER ", screenWidth / 2 - 150, screenHeight / 2, 20, WHITE);


        if (IsKeyPressed(KEY_ENTER)) {
            gameState = IN_BATTLE;


            YerlestirOrcDovuscu(baslangicDovuscu);
            YerlestirOrcMizrakci(baslangicMizrakci);
            YerlestirTroller(baslangicTroll);
            YerlestirVargBinici(baslangicVarg);
            YerlestirHumanKusatma(baslangicKusatma);
            YerlestirHumanOkcu(baslangicOkcu);
            YerlestirHumanPiyade(baslangicPiyade);
            YerlestirHumanSuvari(baslangicSuvari);
        }
    } else if (gameState == IN_BATTLE) {

        for (int x = 0; x < GRID_COLUMNS; x++) {
            for (int y = 0; y < GRID_ROWS; y++) {
                Color cellColor = (x < GRID_COLUMNS / 2) ? DARKGRAY : MAROON;

                DrawRectangle(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE,
                              GRID_CELL_SIZE, GRID_CELL_SIZE, cellColor);
                DrawRectangleLines(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE,
                                   GRID_CELL_SIZE, GRID_CELL_SIZE, BLACK);


                if (x < GRID_COLUMNS / 2 && humanUnits[x][y].count > 0) {
                    float scale = (float)GRID_CELL_SIZE / fmax(humanUnits[x][y].texture.width, humanUnits[x][y].texture.height);
                    Vector2 pos = {(float)(offsetX + x * GRID_CELL_SIZE + (GRID_CELL_SIZE - (humanUnits[x][y].texture.width * scale)) / 2),
                                          (float)(offsetY + y * GRID_CELL_SIZE + (GRID_CELL_SIZE - (humanUnits[x][y].texture.height * scale)) / 2 - 5)};
                    DrawTextureEx(humanUnits[x][y].texture, pos, 0.0f, scale, WHITE);
                    DrawText(TextFormat("%d", humanUnits[x][y].count), offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE + GRID_CELL_SIZE - 20, 20, BLACK);
                    CanBariCiz(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE - 10, humanUnits[x][y].health, humanUnits[x][y].count * 100);
                }


                if (x >= GRID_COLUMNS / 2 && orcUnits[x][y].count > 0) {
                    float scale = (float)GRID_CELL_SIZE / fmax(orcUnits[x][y].texture.width, orcUnits[x][y].texture.height);
                    Vector2 pos = {(float)(offsetX + x * GRID_CELL_SIZE + (GRID_CELL_SIZE - (orcUnits[x][y].texture.width * scale)) / 2),
                                      (float)(offsetY + y * GRID_CELL_SIZE + (GRID_CELL_SIZE - (orcUnits[x][y].texture.height * scale)) / 2 - 5)};
                    DrawTextureEx(orcUnits[x][y].texture, pos, 0.0f, scale, WHITE);
                    DrawText(TextFormat("%d", orcUnits[x][y].count), offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE + GRID_CELL_SIZE - 20, 20, BLACK);
                    CanBariCiz(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE - 10, orcUnits[x][y].health, orcUnits[x][y].count * 100);
                }
            }
        }


        if (IsKeyPressed(KEY_ENTER)) {
            gameState = MID_SCREEN;

            ClearUnits();
            YerlestirOrcDovuscuSon(&ork,&ork_legi);
            YerlestirOrcMizrakciSon(&ork,&ork_legi);
            YerlestirTrollerSon(&ork,&ork_legi);
            YerlestirVargBiniciSon(&ork,&ork_legi);
            YerlestirHumanKusatmaSon(&insan,&insan_imparatorlugu);
            YerlestirHumanOkcuSon(&insan,&insan_imparatorlugu);
            YerlestirHumanPiyadeSon(&insan,&insan_imparatorlugu);
            YerlestirHumanSuvariSon(&insan,&insan_imparatorlugu);
        }
    } else if (gameState == MID_SCREEN) {

        for (int x = 0; x < GRID_COLUMNS; x++) {
            for (int y = 0; y < GRID_ROWS; y++) {
                Color cellColor = (x < GRID_COLUMNS / 2) ? DARKGRAY : MAROON;
                DrawRectangle(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE,
                              GRID_CELL_SIZE, GRID_CELL_SIZE, cellColor);
                DrawRectangleLines(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE,
                                   GRID_CELL_SIZE, GRID_CELL_SIZE, BLACK);


                if (x < GRID_COLUMNS / 2 && humanUnits[x][y].count > 0) {
                    float scale = (float)GRID_CELL_SIZE / fmax(humanUnits[x][y].texture.width, humanUnits[x][y].texture.height);
                    Vector2 pos = {(float)(offsetX + x * GRID_CELL_SIZE + (GRID_CELL_SIZE - (humanUnits[x][y].texture.width * scale)) / 2),
                                          (float)(offsetY + y * GRID_CELL_SIZE + (GRID_CELL_SIZE - (humanUnits[x][y].texture.height * scale)) / 2 - 5)};
                    DrawTextureEx(humanUnits[x][y].texture, pos, 0.0f, scale, WHITE);
                    DrawText(TextFormat("%d", humanUnits[x][y].count), offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE + GRID_CELL_SIZE - 20, 20, BLACK);
                    CanBariCiz(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE - 10, humanUnits[x][y].health, humanUnits[x][y].count * 100);
                }


                if (x >= GRID_COLUMNS / 2 && orcUnits[x][y].count > 0) {
                    float scale = (float)GRID_CELL_SIZE / fmax(orcUnits[x][y].texture.width, orcUnits[x][y].texture.height);
                    Vector2 pos = {(float)(offsetX + x * GRID_CELL_SIZE + (GRID_CELL_SIZE - (orcUnits[x][y].texture.width * scale)) / 2),
                                      (float)(offsetY + y * GRID_CELL_SIZE + (GRID_CELL_SIZE - (orcUnits[x][y].texture.height * scale)) / 2 - 5)};
                    DrawTextureEx(orcUnits[x][y].texture, pos, 0.0f, scale, WHITE);
                    DrawText(TextFormat("%d", orcUnits[x][y].count), offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE + GRID_CELL_SIZE - 20, 20, BLACK);
                    CanBariCiz(offsetX + x * GRID_CELL_SIZE, offsetY + y * GRID_CELL_SIZE - 10, orcUnits[x][y].health, orcUnits[x][y].count * 100);


                }
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
                gameState=END_SCREEN;
        }
    }else if(gameState==END_SCREEN){

            DrawText("       Savas Bitti!", screenWidth / 2 - 100, screenHeight / 2 - 50, 20, WHITE);

        }

    EndDrawing();
}


    UnloadTexture(humanPiyade);
    UnloadTexture(humanOkcu);
    UnloadTexture(orcDovuscu);
    UnloadTexture(orcMizrakci);
    UnloadTexture(humanSuvari);
    UnloadTexture(humanKusatma);
    UnloadTexture(orcVargBinici);
    UnloadTexture(orcTroller);
    UnloadTexture(gamelogo);
    UnloadTexture(background);

    CloseWindow();

    free(senaryo_json);
    free(creatures_json);
    free(heroes_json);
    free(research_json);
    free(unit_types_json);
    return 0;
}



