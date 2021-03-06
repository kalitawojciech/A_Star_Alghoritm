// A_star.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
float F_min = 0;
int G[20][20];
float H[20][20], F[20][20];
int kolumna_startowa, wiersz_startowy, kolumna_koncowa, wiersz_koncowy, aktualna_kolumna, aktualny_wiersz;
short rodzic[20][20]; // 1 -> góra, 2-> z prawej, 3-> z dołu, 4 -> z lewej 
short typ_pola[20][20]; // 0 -> pole nieodwiedzone, 1 -> pole na liście otwartej, 2 -> pole na liście zamkniętej, 5 -> przeszkoda, 4-> pole startowe
void wypelnijG(int n);
void wypelnijH();
void pokaz_droge();
void obliczG(int x, int y);
void wypelnij_rodzic();
void sprawdzPolaDookola();
void znajdzNajmniejszeFzListyOtawrtej();
void dopukiSaPolaNaLiscieOtwartej();
void wczytajPlik();
int main()
{
	kolumna_koncowa = 19;
	wiersz_koncowy = 19;
	aktualna_kolumna = kolumna_startowa = 0;
	aktualny_wiersz = wiersz_startowy = 0;
	wypelnijG(999);
	wypelnijH();
	wypelnij_rodzic();
	//std::cin >> kolumna_startowa >> wiersz_startowy >> kolumna_koncowa >> wiersz_koncowy;
	wczytajPlik();
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			std::cout << typ_pola[x][y] << " ";
		}
		std::cout << "\n";
	}
	dopukiSaPolaNaLiscieOtwartej();
	std::cout << "\n";
	std::cout << "\n";
	pokaz_droge();
	for ( int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			std::cout << typ_pola[x][y] << " ";
		}
		std::cout << "\n";
	}
}
void wypelnijG(int n) {
	for(int x=0; x<20; x++)
		for (int y = 0; y < 20; y++)
		{
			G[x][y] = n;
		}
	G[kolumna_startowa][wiersz_startowy] = 0;
}
void wypelnijH() {
	for (int wiersz = 0; wiersz < 20; wiersz++)
		for (int kolumna = 0; kolumna < 20; kolumna++)
		{
			H[wiersz][kolumna] = sqrt((float)((wiersz - wiersz_koncowy) * (wiersz - wiersz_koncowy) + (kolumna - kolumna_koncowa) * (kolumna - kolumna_koncowa)));
		}
}
void wypelnij_rodzic() {
	for (int x = 0; x < 20; x++)
		for (int y = 0; y < 20; y++)
		{
			rodzic[x][y] = 0;
		}
}
void pokaz_droge() {
	int kroki = 0;
	int wiersz = wiersz_koncowy;
	int kolumna = kolumna_koncowa;
	if (typ_pola[wiersz_koncowy][kolumna_koncowa] == 2 || typ_pola[wiersz_koncowy][kolumna_koncowa] == 1)
	{
		std::cout << "wiersz kolumna\n";
		std::cout << "   " << wiersz << "    " << kolumna << "\n";
		while (kolumna != kolumna_startowa || wiersz != wiersz_startowy)
		{
			if (rodzic[wiersz][kolumna] == 1) {
				wiersz--;
			}
			else if (rodzic[wiersz][kolumna] == 2) {
				kolumna++;
			}
			else if (rodzic[wiersz][kolumna] == 3) {
				wiersz++;
			}
			else if (rodzic[wiersz][kolumna] == 4) {
				kolumna--;
			}
			typ_pola[wiersz][kolumna] = 7;
			std::cout << "   " << wiersz << "    " << kolumna << "\n";
		}
	}
	else
	{
		std::cout << "Punkt koncowy jest nieosiagalny!!\n";
	}
}
void obliczG(int wiersz, int kolumna) {
	if (rodzic[wiersz][kolumna] == 1) { // rodzic jest na górze
		G[wiersz][kolumna] = G[wiersz -1 ][kolumna] + 1;
	}
	if (rodzic[wiersz][kolumna] == 2) { // rodzic jest po prawej
		G[wiersz][kolumna] = G[wiersz][kolumna + 1] + 1;
	}
	if (rodzic[wiersz][kolumna] == 3) { // rodzic jest na dole
		G[wiersz][kolumna] = G[wiersz + 1][kolumna] + 1;
	}
	if (rodzic[wiersz][kolumna] == 4) { // rodzic jest po lewej
		G[wiersz][kolumna] = G[wiersz][kolumna - 1] + 1;
	}
}
void sprawdzPolaDookola()
{
	//sprawdzamy pole na górze od aktualnego
	if (aktualny_wiersz > 0)
	{	
		if (typ_pola[aktualny_wiersz -1][aktualna_kolumna] == 0)
		{
			G[aktualny_wiersz - 1][aktualna_kolumna] = G[aktualny_wiersz][aktualna_kolumna] + 1;
			rodzic[aktualny_wiersz - 1][aktualna_kolumna] = 3; // ma rodzica na dole 
			typ_pola[aktualny_wiersz - 1][aktualna_kolumna] = 1; // jest polem na liście otwartej
			F[aktualny_wiersz - 1][aktualna_kolumna] = H[aktualny_wiersz - 1][aktualna_kolumna] + G[aktualny_wiersz - 1][aktualna_kolumna];
		}
	}
	//sprawdzamy pole po prawej
	if (aktualna_kolumna < 19)
	{
		if (typ_pola[aktualny_wiersz][aktualna_kolumna + 1] == 0)
		{
			G[aktualny_wiersz][aktualna_kolumna + 1] = G[aktualny_wiersz][aktualna_kolumna] + 1;
			rodzic[aktualny_wiersz][aktualna_kolumna + 1] = 4; // ma rodzica z lewej strony
			typ_pola[aktualny_wiersz][aktualna_kolumna + 1] = 1;
			F[aktualny_wiersz][aktualna_kolumna + 1] = H[aktualny_wiersz][aktualna_kolumna + 1] + G[aktualny_wiersz][aktualna_kolumna + 1];
		}
	}
	//sprawdzamy na dole
		if (aktualny_wiersz < 19)
		{
			if (typ_pola[aktualny_wiersz + 1][aktualna_kolumna] == 0)
			{
				G[aktualny_wiersz + 1][aktualna_kolumna] = G[aktualny_wiersz][aktualna_kolumna] + 1;
				rodzic[aktualny_wiersz + 1][aktualna_kolumna] = 1; // ma rodzica na górze
				typ_pola[aktualny_wiersz + 1][aktualna_kolumna] = 1;
				F[aktualny_wiersz + 1][aktualna_kolumna] = H[aktualny_wiersz + 1][aktualna_kolumna] + G[aktualny_wiersz + 1][aktualna_kolumna];
			}
		}
	//sprawdzamy z lewej
		if (aktualna_kolumna > 0)
		{
			if (typ_pola[aktualny_wiersz][aktualna_kolumna - 1] == 0)
			{
				G[aktualny_wiersz][aktualna_kolumna - 1] = G[aktualny_wiersz][aktualna_kolumna] + 1;
				rodzic[aktualny_wiersz][aktualna_kolumna - 1] = 2; // ma rodzica z prwej strony
				typ_pola[aktualny_wiersz][aktualna_kolumna - 1] = 1;
				F[aktualny_wiersz][aktualna_kolumna - 1] = H[aktualny_wiersz][aktualna_kolumna - 1] + G[aktualny_wiersz][aktualna_kolumna - 1];
			}
		}
}
void znajdzNajmniejszeFzListyOtawrtej() {
	F_min = 100000;
	for (int wiersz = 0; wiersz < 20; wiersz++)
	{
		for (int kolumna = 0; kolumna < 20; kolumna++)
		{
			if (typ_pola[wiersz][kolumna] == 1)
			{
				if (F[wiersz][kolumna] < F_min)
				{
					aktualna_kolumna = kolumna;
					aktualny_wiersz = wiersz;
					F_min = F[wiersz][kolumna];
				}
			}
		}
	}
}
void dopukiSaPolaNaLiscieOtwartej()
{
	sprawdzPolaDookola();
	do
	{
		znajdzNajmniejszeFzListyOtawrtej();
		typ_pola[aktualny_wiersz][aktualna_kolumna] = 2;
		sprawdzPolaDookola();
	} while (F_min != 100000 && typ_pola[wiersz_koncowy][kolumna_koncowa] != 2);
}
void wczytajPlik() {
	string nazwap = "grid.txt";
	std::ifstream plik(nazwap.c_str());
	for (unsigned int wiersz = 0; wiersz < 20; wiersz++)
	{
		for (unsigned int kolumna = 0; kolumna < 20; kolumna++)
		{
			plik >> typ_pola[wiersz][kolumna];
		}
	}
	plik.close();
	typ_pola[wiersz_startowy][kolumna_startowa] = 4;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie
// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
