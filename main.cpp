#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctime>

#define MAX_TAB 99

using namespace std;

typedef struct element
{
    int liczba;
    int pierwLinia; // oznaczenie kolejnosci elementow z pierwszej linii
    int nastLinia;  // oznaczenie zxnalezionych elementow sumy
    int pominV;     // oznaczenie elementow pomijanych przy wariancji
} Element;

Element tab[MAX_TAB];

int liczbaElem = 0; // liczba wszystkich elementow - obliczana przy wczytywaniu
int liczbaPierwLinia = 0; // liczba elementow pierwszej linii - obliczana w liczbElemPierwLin
void wczytajDaneZPliku(const char * nazwaPliku);
void wypiszDane();

int variancja(int glebokosc);
void drukujVariancje();
void zerujPierwLinia();
void zerujNastLinia();
void zerujPominV();
void drukujTablice();
int sprawdz();
void liczbElemPierwLin();

int main()
{
    int ret = 0;
    wczytajDaneZPliku("plik.txt");
    liczbElemPierwLin();
    drukujTablice();
    zerujPierwLinia();
    zerujNastLinia();
    zerujPominV();
    clock_t start=clock();
    ret = variancja(-1);
    printf("Czas wykonywania: %lu ms\n",((1000*(clock()-start))/CLOCKS_PER_SEC));


}

void liczbElemPierwLin()
{
    liczbaPierwLinia = (-1+sqrt(1+8*liczbaElem))/2;
}
void wczytajDaneZPliku(const char * nazwaPliku)
{
    fstream plik;
    string line;
    int liczPom = 0;
    plik.open(nazwaPliku, ios::in);
    if(plik.good() != true)
    {
        printf("Nie mozna otworzyc pliku %s", nazwaPliku);
        exit(1);
    }

    getline(plik,line);

    const char * bufLin = line.c_str();
    int i = 0;
    int j = 0;
    char buflicz[10];

    while(true)
    {
        if(bufLin[i] != ' ' && bufLin[i] !='\0')
        {
            buflicz[j]=bufLin[i];
            j++;
        }
        else
        {
            int dana = 0;
            buflicz[j] = '\0';
            dana = atoi(buflicz);
            tab[liczPom].liczba = dana;
            liczPom++;
            j = 0;
        }
        if(bufLin[i] == '\0')
            break;
        i++;
    }
    liczbaElem = liczPom;
}


int variancja(int glebokosc)
{
    static unsigned long licznik;
    int nrElem = 0;
    int ret = 0;
    glebokosc++;
    if(glebokosc >= liczbaPierwLinia)
    {
        licznik++;
        if(licznik % 1000000 == 0)
            cout << "Licznik: " << licznik << endl;
        if(tab[0].pierwLinia == -1)
            return 0;
        ret = sprawdz();
        if(ret == 1)
        {
            drukujVariancje();
            return 1;
        }
        return 0;
    }

    while(true)
    {
        if( nrElem >= liczbaElem)
            return 0;
        if( tab[nrElem].pierwLinia == -1 && tab[nrElem].pominV == -1)
        {
            tab[nrElem].pierwLinia = glebokosc;
            ret = variancja(glebokosc);
            if (ret == 1)
            {
                return 1;
            }
            tab[nrElem].pierwLinia = -1;
        }
        nrElem++;
    }
}


int sprawdz()
{
    int tabPom[liczbaPierwLinia];
    int sumaWszy = 0;
    zerujNastLinia();

    // suma wszystkich jako najwiêkszy element
    for(int i = 0; i < liczbaElem; i++)
        if( tab[i].pierwLinia >= 0 )
        {
            tabPom[tab[i].pierwLinia] = tab[i].liczba;
            sumaWszy = sumaWszy + tab[i].liczba;
        }
    if(sumaWszy != tab[liczbaElem-1].liczba)
        return 0;

    for(int i = liczbaPierwLinia - 1; i >= 1 ; i--)
    {
        for(int s = 0; s <= liczbaPierwLinia - 1 - i; s++)
        {
            int sumPom = 0;
            for(int j = s; j <= i + s; j++)
                sumPom = sumPom + tabPom[j];

            int znaleziony = 0;
            for(int k = 0; k < liczbaElem; k++)
                if(tab[k].liczba     == sumPom &&
                        tab[k].pierwLinia == -1 &&
                        tab[k].nastLinia  == -1)
                {
                    tab[k].nastLinia = 1;
                    znaleziony = 1;
                    break;
                }

            if(znaleziony == 0)
                return 0;
        }
    }
    return 1;

}

void zerujPierwLinia()
{
    for(int i = 0; i < liczbaElem; i++)
        tab[i].pierwLinia = -1;
}

void zerujNastLinia()
{
    for(int i = 0; i < liczbaElem; i++)
        tab[i].nastLinia = -1;
}

void zerujPominV()
{
    for(int i = 0; i < liczbaElem; i++)
    {
        tab[i].pominV = -1;

    }// ostatni element zawsze pomijany poniewaz jest suma wszystkich
   tab[liczbaElem -1].pominV = 1;
}


void drukujVariancje()
{
    static long licznik = 0;
    int tabPom[liczbaPierwLinia];
    int tabPom1[liczbaPierwLinia];
    licznik++;
    // if( (licznik % 1000) != 0)
    //     return;
    for(int i = 0; i < liczbaElem; i++)
        if( tab[i].pierwLinia >= 0 )
        {
            tabPom[tab[i].pierwLinia] = tab[i].liczba;
            tabPom1[tab[i].pierwLinia] = i;
        }
    cout << "Permutacja(" << licznik << "):" ;
    for(int i = 0; i < liczbaPierwLinia; i++)
        cout << tabPom[i] << "("<< tabPom1[i] << ") ";
    cout << endl;
    cout << "tablica" << endl;
    for(int i = 0; i < liczbaElem; i++)
        if ( tab[i].pierwLinia == -1 && tab[i].nastLinia == -1)
        cout << tab[i].liczba << " ("<< tab[i].pierwLinia << "," << tab[i].nastLinia << ") ";
    cout << endl;
}


void drukujTablice()
{
    cout << endl << "Liczba elementow: " << liczbaElem << endl;
    cout << " Tablica: ";
    for(int i = 0; i < liczbaElem; i++)
        cout << tab[i].liczba << " ";
    cout << endl;

}
