#include <stdio.h>
#include <stdlib.h>

#ifndef WIERSZE
#define WIERSZE 22
#endif

#ifndef KOLUMNY
#define KOLUMNY 80
#endif

#define ZYWA '0'
#define MARTWA '.'

struct kolumna {
    int numer;
    int liczbaSasiadow;
    char stan;
    struct kolumna * nastepnaKolumna;
};
typedef struct kolumna kolumna;

struct wiersz {
    int numer;
    int lZywychKolumn;
    struct kolumna * pierwszaKolumna;
    struct wiersz * nastepnyWiersz;
};
typedef struct wiersz wiersz;

/*stwarza nowy wiersz o danym numerze*/
wiersz * stworzWiersz(int numer) {
    wiersz * nowyWiersz = (wiersz*)malloc(sizeof(wiersz));
    nowyWiersz -> numer = numer;
    nowyWiersz -> nastepnyWiersz = NULL;
    nowyWiersz -> pierwszaKolumna = NULL;
    nowyWiersz -> lZywychKolumn = 0;
    return nowyWiersz;
}

/*stwarza nowa kolumne o danym numerze*/
kolumna * stworzKolumne(wiersz * aktW, int numer, char stan) {
    kolumna * nowaKolumna = (kolumna *)malloc(sizeof(kolumna));
    nowaKolumna -> numer = numer;
    nowaKolumna -> stan = stan;
    nowaKolumna -> liczbaSasiadow = 0;
    nowaKolumna -> nastepnaKolumna = NULL;
    if (stan == ZYWA) {
        (aktW -> lZywychKolumn) = (aktW -> lZywychKolumn) + 1;
    }
    return nowaKolumna;
}

/*3 funkcje dolaczajace WIERSZE w odpowiednich miejscach*/
wiersz * dolaczNaKoncuW(wiersz * poprzedniW, int numer) {
    wiersz * nowyWiersz = stworzWiersz(numer);
    poprzedniW -> nastepnyWiersz = nowyWiersz;
    return nowyWiersz;
}
wiersz * dolaczNaPoczW(wiersz ** generacja, int numer) {
    wiersz * nowyWiersz = stworzWiersz(numer);
    nowyWiersz -> nastepnyWiersz = *generacja;
    *generacja = nowyWiersz;
    return nowyWiersz;
}
wiersz * dolaczWSrodkuW(wiersz * poprzedniW, wiersz * nastW, int numer) {
    wiersz * nowyWiersz = stworzWiersz(numer);
    nowyWiersz -> nastepnyWiersz = nastW;
    poprzedniW -> nastepnyWiersz = nowyWiersz;
    return nowyWiersz;
}

/*3 funkcje dolaczajace KOLUMNY w odpowiednich miejscach*/
kolumna * dolaczNaKoncuK(wiersz * aktW, kolumna * poprzedniaK, int numer,
        char stan) {
    kolumna * nowaKolumna = stworzKolumne(aktW, numer, stan);
    poprzedniaK -> nastepnaKolumna = nowaKolumna;
    return nowaKolumna;
}
kolumna * dolaczNaPoczK(wiersz * aktW, int numer, char stan) {
    kolumna * nowaKolumna = stworzKolumne(aktW, numer, stan);
    nowaKolumna -> nastepnaKolumna = aktW -> pierwszaKolumna;
    aktW -> pierwszaKolumna = nowaKolumna;
    return nowaKolumna;
}
kolumna * dolaczWSrodkuK(wiersz * aktW, kolumna * poprzedniaK, kolumna * nastK,
                        int numer, char stan) {
    kolumna * nowaKolumna = stworzKolumne(aktW, numer, stan);
    nowaKolumna -> nastepnaKolumna = nastK;
    poprzedniaK -> nastepnaKolumna = nowaKolumna;
    return nowaKolumna;
}

/*Dolacza kolumne o danym numerze w odpowiednim m-scu zaczynajac od kolumny
aktK i majac jej poprzednika*/
kolumna * dolaczKolumneONumerze(wiersz * aktW, kolumna * poprzedniaK,
                                kolumna * aktK,int numer, char stan) {
        while (aktK != NULL && (aktK -> numer < numer)) {
            poprzedniaK = aktK;
            aktK = aktK -> nastepnaKolumna;
        }                                
        if (aktK == NULL && poprzedniaK == NULL) {                //wiersz pusty
			
            return dolaczNaPoczK(aktW, numer, stan);
        
        }
        else if (aktK == NULL) {                              //dolacz na koniec
        
            return dolaczNaKoncuK(aktW, poprzedniaK, numer, stan);
        
        }
        else if (aktK -> numer == numer) {                  //dana kol. istnieje            
        
            return aktK;
        
        }
        else {
            if (poprzedniaK == NULL) {           //pierwsza kol. wiersza wieksza    
        
                kolumna * dolaczanaK = dolaczNaPoczK (aktW, numer, stan);
                dolaczanaK -> nastepnaKolumna = aktK;
                return dolaczanaK;
        
            }
            else {                        //poprzedniaK mniejsza, aktK - wieksza
        
                return dolaczWSrodkuK(aktW, poprzedniaK, aktK, numer, stan);
        
            }
        }
}

/*Wszystkim kol. znajdujacym sie w wierszu sasiadujacym do wiersza aktW
dodaje +1 do l. zywych sasiadow, jesli sa sasiadami kol. zywej w aktW*/
void dodajSasiadowDlaWiersza(wiersz * aktW, wiersz * sasiadW) {
    kolumna * aktK = aktW -> pierwszaKolumna;
    kolumna * aktKSasiad = sasiadW -> pierwszaKolumna;
    kolumna * poprzedniaKSasiad = NULL;               /*od niej zaczynam szukac sasiadow kolejnych kol.*/
    while (aktK != NULL) {
        if (aktK -> stan == ZYWA) {
			
            int numer = aktK -> numer;		//nr kol. ktorej sasiadom dodaje +1
            
            kolumna * sasiadK;				//kol. w sasiadW ktorej dodaje +1 do l.zywych sasiadow/
																																										
            aktKSasiad = dolaczKolumneONumerze(sasiadW, poprzedniaKSasiad, aktKSasiad, numer - 1, MARTWA);  
                                                                                                                                                                       
            sasiadK = aktKSasiad; 			 //nr sasiadK: numer-1 
            sasiadK -> liczbaSasiadow = (sasiadK -> liczbaSasiadow) + 1;
            		
															
            sasiadK = dolaczKolumneONumerze(sasiadW,        //nr sasiadK: numer
					sasiadK, sasiadK -> nastepnaKolumna, numer, MARTWA);
            if (aktW != sasiadW) {
                sasiadK -> liczbaSasiadow = (sasiadK -> liczbaSasiadow) + 1;
            }
            
            sasiadK = dolaczKolumneONumerze(sasiadW, 	  //nr sasiadK: numer+1
					sasiadK, sasiadK -> nastepnaKolumna, numer + 1, MARTWA);
            sasiadK -> liczbaSasiadow = (sasiadK -> liczbaSasiadow) + 1;
            
            poprzedniaKSasiad = aktKSasiad;
            aktKSasiad = aktKSasiad -> nastepnaKolumna;
        }
        aktK = aktK -> nastepnaKolumna;
    }
}


/*Iteruje przez wiersze i dodaje + 1 do l.zywych sasiadow kolumnom znajdujacym
sie w wierszu sasiadujacym z iterowanym i sasiadujacymi z kol. zywymi w nim*/
void dodajSasiadow(wiersz ** generacja) {
    wiersz * poprzedniW = NULL;
    wiersz * aktW = *generacja;
     while (aktW != NULL) {
        if ((aktW -> lZywychKolumn) > 0) {
            wiersz * sasiadW;
            //dodaj sasiadow w wierszu powyzej
            if (poprzedniW == NULL) { 
                sasiadW = dolaczNaPoczW(generacja, aktW -> numer - 1);
                poprzedniW = sasiadW;
            }
            else {
                if ((poprzedniW -> numer) == (aktW -> numer - 1)) {
                    sasiadW = poprzedniW;
                }
                else {
                    sasiadW = dolaczWSrodkuW(poprzedniW, aktW,
                                             aktW -> numer - 1);
                }
            }
            dodajSasiadowDlaWiersza(aktW, sasiadW);
            //w wierszu ponizej
            if (aktW -> nastepnyWiersz == NULL) {
                sasiadW = dolaczNaKoncuW(aktW, aktW -> numer + 1);
            }
            else {
                if (aktW -> nastepnyWiersz -> numer == aktW -> numer + 1) {
                    sasiadW = aktW -> nastepnyWiersz;
                }
                else {
                    sasiadW = dolaczWSrodkuW(aktW, aktW -> nastepnyWiersz,
                                             aktW -> numer + 1);
                }
            }
            dodajSasiadowDlaWiersza(aktW, sasiadW);
            //w wierszu aktualnym
            dodajSasiadowDlaWiersza(aktW, aktW);
        }
        poprzedniW = aktW;
        aktW = aktW -> nastepnyWiersz;
    }
}

/*Sprawdza czy dana kolumna bedzie martwa czy zywa w nast gen.,
zwraca 1 jesli bedzie zywa, wpp. zwraca 0*/
int sprawdzKolumne(kolumna * aktK) {
    int liczbaSasiadow = aktK -> liczbaSasiadow;
    if ((aktK -> stan) == ZYWA) {
        if ((liczbaSasiadow == 2) || (liczbaSasiadow == 3)) {
            return 1;
        }
    }
    else {
        if (liczbaSasiadow == 3) {
            return 1;
        }
    }
    return 0;
}

/*Usuwa kolumne martwa w nastepnej generacji*/
void usunKolumne(wiersz * aktW, kolumna * poprzedniaK, kolumna * usuwanaK) {
    if ((usuwanaK -> stan) == ZYWA) {
        aktW -> lZywychKolumn = (aktW -> lZywychKolumn) - 1;
    }
    if (poprzedniaK == NULL) {
        aktW -> pierwszaKolumna = usuwanaK -> nastepnaKolumna;
    }
    else {
        poprzedniaK -> nastepnaKolumna = usuwanaK -> nastepnaKolumna;
    }
    free(usuwanaK);
}

/*Usuwa wiersz z zerowa liczba zywych kolumn w nast gen.*/
void usunWiersz(wiersz ** generacja, wiersz * usuwanyW, wiersz * poprzedniW) {
    if (poprzedniW == NULL) {
        *generacja = usuwanyW -> nastepnyWiersz;
    }
    else {
        poprzedniW -> nastepnyWiersz = usuwanyW -> nastepnyWiersz;
    }
    free(usuwanyW);
}

/*dla wiersza aktW: usuwa kolumny ktore beda martwe w nast. generacji,
pozostale oznacza jako zywe i zeruje ich l. zywych sasiadow*/
void czyscWiersz(wiersz * aktW) {
    kolumna * poprzedniaK = NULL;
    kolumna * aktK = aktW -> pierwszaKolumna;
    while (aktK != NULL) {
        if (sprawdzKolumne(aktK)) {
            aktK -> liczbaSasiadow = 0;
            if (aktK -> stan == MARTWA) {
                aktK -> stan = ZYWA;
                aktW -> lZywychKolumn = (aktW -> lZywychKolumn) + 1;
            }
            poprzedniaK = aktK;
            aktK = aktK -> nastepnaKolumna;
        }
        else {
            usunKolumne(aktW, poprzedniaK, aktK);
            if (poprzedniaK == NULL) {
                aktK = aktW -> pierwszaKolumna;
            }
            else {
                aktK = poprzedniaK -> nastepnaKolumna;
            }
        }
    }
}

/*Usuwa wiersze w ktorych w nastepnej gen. nie bedzie zywych kolumn*/
void czyscWiersze(wiersz ** generacja) {
    wiersz * poprzedniW = NULL;
    wiersz * aktW = *generacja;
    while (aktW != NULL) {
        czyscWiersz(aktW);
        if ((aktW -> lZywychKolumn) == 0 || (aktW -> pierwszaKolumna) == NULL) {
            usunWiersz(generacja, aktW, poprzedniW);
            if (poprzedniW == NULL) {
                aktW = *generacja;
            }
            else {
                aktW = poprzedniW -> nastepnyWiersz;
            }
        }
        else {
            poprzedniW = aktW;
            aktW = aktW -> nastepnyWiersz;
        }
    }
}

/*dla wiersza aktW: skanuje zywe kolumny i dolacza je do jego listy zywych
kolumn; aktK wskazuje na ostatnia kolumne w wierszu aktW*/
void zeskanujKolumnyWiersza(wiersz * aktW) {
    int numer;
    kolumna * aktK = aktW -> pierwszaKolumna;
    getchar();
    scanf("%d", &numer);
    aktK = dolaczNaPoczK(aktW, numer, ZYWA);
    while (getchar() != '\n') {
        scanf("%d", &numer);
        aktK = dolaczNaKoncuK(aktW, aktK, numer, ZYWA);
    }
}

/*Wczytuje gen. pocz. - liste wierszy z zywymi kolumnami, kazdy wskazuje na
liste zywych kolumn w nim; aktW to ostatni wczytany wiersz generacji*/
void wczytajGeneracje(wiersz ** generacja) {
    int koniec = 0;
    int numer;
    wiersz * aktW = * generacja;
    while (!koniec) {
        getchar();
        numer = getchar();
        if (numer == '\n') {
            koniec = 1;
        }
        else {
        ungetc(numer, stdin);
        scanf("%d", &numer);
        if (aktW == NULL) {
            aktW = dolaczNaPoczW(generacja, numer);
        }
        else {
            aktW = dolaczNaKoncuW(aktW, numer);
        }
        zeskanujKolumnyWiersza(aktW);
        }
    }
}

/*Oblicza n-ta kolejna generacje*/
void obliczGeneracje(wiersz ** generacja, int n) {
    for (int i = 0; i < n; i++) {
        dodajSasiadow(generacja);
        czyscWiersze(generacja);
    };
}

/*Zmienia współrzedne lewego górnego rogu okna, na wiersz w i kolumne k*/
void przesunOkno(int *lewyWierzchW, int *lewyWierzchK, int w, int k) {
        *lewyWierzchW = w;
        *lewyWierzchK = k;
}

/*Generuje opis aktualnej generacji w formacie takim jak wczytany*/
void zrzutujGeneracje(wiersz * generacja) {
    wiersz * aktW = generacja;
    while(aktW != NULL) {
        printf("/%d", aktW -> numer);
        kolumna * aktK = aktW -> pierwszaKolumna;
        while (aktK != NULL) {
            printf(" %d", aktK -> numer);
            aktK = aktK -> nastepnaKolumna;
        }
        printf("\n");
        aktW = aktW -> nastepnyWiersz;
    }
    printf("/\n");
}

void drukujGeneracje(wiersz * generacja, int lewyWierzchW, int lewyWierzchK) {
    wiersz * aktW = generacja;
    for (int w = lewyWierzchW; w < lewyWierzchW + WIERSZE; w++) {
		
        while ((aktW != NULL) && (aktW -> numer < w)) {  //szukam pierwszego wiersza zywego o numerze >= w                      
            aktW = aktW -> nastepnyWiersz;
        }
        
        if ((aktW == NULL) || aktW -> numer > w) {     //w drukowanym wierszu ma zywej komorki  
														                         
            for (int k = lewyWierzchK; k < lewyWierzchK + KOLUMNY; k++) {
                printf("%c", MARTWA);
            }           
        }    
          
        else {          //w drukowanym wierszu sa zywe komorki aktW -> numer == w
											
            kolumna * aktK = aktW -> pierwszaKolumna;
            for (int k = lewyWierzchK; k < lewyWierzchK + KOLUMNY; k++) {
                while (aktK != NULL && aktK -> numer < k) {
                    aktK = aktK -> nastepnaKolumna;
                }
                if ((aktK == NULL) || (aktK -> numer > k)) {
                    printf("%c", MARTWA);
                }
                else {
                    printf("%c", ZYWA);
                }
            }           
        }
        
        printf("\n");
    }
    for (int k = lewyWierzchK; k < lewyWierzchK + KOLUMNY; k++) {
        printf("=");
    }
    printf("\n");
}

void zwolnijPamiec(wiersz ** generacja) {
    wiersz * aktW = *generacja;
    while (aktW != NULL) {
        kolumna * aktK = aktW -> pierwszaKolumna;
        while (aktK != NULL) {
           aktW -> pierwszaKolumna = aktK -> nastepnaKolumna;
           free(aktK);
           aktK = aktW -> pierwszaKolumna;
        }
        *generacja = aktW -> nastepnyWiersz;
        free(aktW);
        aktW = *generacja;
    }
}

void graj(wiersz ** generacja) {
    int koniec = 0;
    int lewyWierzchW = 1;       //pozycja lewego wierzcholka okna - wiersz i kol
    int lewyWierzchK = 1;
    while (!koniec) {
        drukujGeneracje(*generacja, lewyWierzchW, lewyWierzchK);
        int ruch = getchar();
        if (ruch == '\n') {
            obliczGeneracje(generacja, 1);
        }
        else {
            ungetc(ruch, stdin);
            scanf("%d", &ruch);
            int nastepnySkan = getchar();
            if (nastepnySkan == ' ') {      //sa 2 elem. do wczytania
                int ruch2;
                scanf("%d", &ruch2);
                getchar();
                przesunOkno(&lewyWierzchW, &lewyWierzchK, ruch, ruch2);
            }
            else {              //jest 1 elem. do wczytania
                if (ruch == '.') {
                    koniec = 1;
                }
                else if (ruch == 0) {
                   zrzutujGeneracje(*generacja);
                }
                else {
                    obliczGeneracje(generacja, ruch);
                }
            }
        }       
    }
}

int main()
{
    wiersz * generacja = NULL;      //najnizszy numerem wiersz generacji
    wczytajGeneracje(&generacja);
    graj(&generacja);
    zwolnijPamiec(&generacja);
    return 0;
}
