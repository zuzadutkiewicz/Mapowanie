#include <iostream>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define MAX_TAB 99

using namespace std;

int tab[MAX_TAB];


void wczytajDaneZPliku(const char * nazwaPliku);
void wypiszDane();
    int liczWag = 0;
int main()
{
    wczytajDaneZPliku("plik.txt");
}


void wczytajDaneZPliku(const char * nazwaPliku)
{
    fstream plik;
    string line;

    plik.open(nazwaPliku, ios::in);
    if(plik.good() != true)
    {
        printf("Nie mozna otworzyc pliku %s", nazwaPliku);
        exit(1);
    }

    getline(plik,line);

    const char * bufWag = line.c_str();
    int i = 0;
    int j = 0;
    char buflicz[10];

    while(true)
    {
        if(bufWag[i] != ' ' && bufWag[i] !='\0')
        {
            buflicz[j]=bufWag[i];
            j++;
        }
        else
        {
            int dana = 0;
            buflicz[j] = '\0';
            dana = atoi(buflicz);
            tab[liczWag] = dana;
            cout << tab[liczWag] << " ";
            liczWag++;
            j = 0;
        }
        if(bufWag[i] == '\0')
            break;
        i++;
    }
    cout << endl << "Liczba elementow: " << liczWag << endl;
}
