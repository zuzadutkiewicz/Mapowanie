/*
Copyright (C) 2016  Zuzanna Dutkiewicz
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctime>

#define MAX_TAB 300

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
int idxMaxElem = 0;
int idxMinElem = 0;
int poziomOdciecia = -1; // poziom powyzej ktorego nie sa brane liczby do variancji
// -1 oznacza wylaczenie opcji
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
int sprawdzLiczPom();

int main()
{
    int ret = 0;
    int ret1 = 0;
    wczytajDaneZPliku("plik.txt");
    liczbElemPierwLin();
    // ret1 = sprawdzLiczPom();
    // if (ret1 == 0)
    /*{
         cout << "Brak rozwiazania! Zla liczba danych." << endl;
         return 0;
     }*/
    drukujTablice();
    zerujPierwLinia();
    zerujNastLinia();
    zerujPominV();
    clock_t start=clock();
    ret =  variancja(-1);
    if (ret == 0)
    {
        cout<< "Brak rozwiazania."<< endl;
        return 0;
    }
    printf("Czas wykonywania: %lu ms\n",((1000*(clock()-start))/CLOCKS_PER_SEC));


}

void liczbElemPierwLin()
{
    liczbaPierwLinia = (-1+sqrt(1+8*liczbaElem))/2;
    cout << " liczbaPierwszaLinia=" << liczbaPierwLinia << endl;
}

int sprawdzLiczPom()
{
    if (liczbaElem == 1 ||liczbaElem == 3 || liczbaElem == 6 || liczbaElem == 10 || liczbaElem == 15 || liczbaElem == 21 || liczbaElem == 28 || liczbaElem == 36 || liczbaElem == 45  || liczbaElem == 55  || liczbaElem == 66 ||liczbaElem == 78 ||liczbaElem == 91 ||liczbaElem == 105 ||liczbaElem == 120)
    {
        return 1;
    }
    else
    {
        return 0;
    }
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
    if(liczPom > MAX_TAB)
    {
        cout << "Liczba elementow (" << liczPom <<
             ") przekracza dozwolona wartosc MAX_TAB (" << MAX_TAB << "}" << endl;
        cout << " Nalezy zwiekszyc MAX_TAB i zrekompilowac program." << endl;
        exit(1);
    }
    liczbaElem = liczPom;
}


int variancja(int glebokosc)
{
    static unsigned long licznik;
    static clock_t czas = clock();
    int nrElem = 0;
    int ret = 0;
    glebokosc++;
    if(glebokosc >= liczbaPierwLinia)
    {
        licznik++;
        if(licznik % 10000000 == 0)
        {
            int liczPom = licznik / 1000000;
            int czasPom = ((1000*(clock()-czas))/CLOCKS_PER_SEC);
            cout << "Liczba: " << liczPom  << " mln " << "Czas : " << czasPom << " ms" << endl;
            // czas = clock();
            //drukujVariancje();
        }
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

    // jezeli pierwszy nie jest w linii zerowej od razu return
    if(tab[idxMinElem].pierwLinia == -1)
        return 0;

    // suma wszystkich jako najwiêkszy element
    for(int i = 0; i < liczbaElem; i++)
        if( tab[i].pierwLinia >= 0 )
        {
            tabPom[tab[i].pierwLinia] = tab[i].liczba;
            sumaWszy = sumaWszy + tab[i].liczba;
        }
    if(sumaWszy != tab[idxMaxElem].liczba)
    {
        return 0;
    }
    zerujNastLinia();

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

    for(int i = 0; i < liczbaElem; i++)
        if ( tab[i].pierwLinia == -1 && tab[i].nastLinia == -1)
        {

            cout << " Blad: " << tab[i].liczba << " ("<< tab[i].pierwLinia << "," << tab[i].nastLinia << ") ";
            cout << endl;
            return 0;
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
    // ustawienie wartosci poczatkowych dla wyszukiwania na element zerowy tablicy
    int minElemPom = tab[0].liczba;
    idxMinElem = 0;
    int maxElemPom = tab[0].liczba;
    idxMaxElem = 0;
    for(int i = 0; i < liczbaElem; i++)
    {

        // ustawienie poziomu odciecia
        if(poziomOdciecia != -1 && tab[i].liczba > poziomOdciecia)
            tab[i].pominV = 1;
        else
            tab[i].pominV = -1;

        if(minElemPom > tab[i].liczba)
        {
            minElemPom = tab[i].liczba;
            idxMinElem = i;
        }

        if(maxElemPom < tab[i].liczba)
        {
            maxElemPom = tab[i].liczba;
            idxMaxElem = i;
        }
    }
    // najwiekszy element zawsze pomijany poniewaz jest suma wszystkich
    tab[idxMaxElem].pominV = 1;


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
//    cout << "tablica" << endl;
//    for(int i = 0; i < liczbaElem; i++)
//        if ( tab[i].pierwLinia == -1 && tab[i].nastLinia == -1)
//            cout << tab[i].liczba << " ("<< tab[i].pierwLinia << "," << tab[i].nastLinia << ") ";
//    cout << endl;
}


void drukujTablice()
{
    cout << endl << "Liczba elementow: " << liczbaElem << endl;
    cout << " Tablica: ";
    for(int i = 0; i < liczbaElem; i++)
        cout << tab[i].liczba << " ";
    cout << endl;

}
