#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

string plikZAdresatami = "Ksiazka adresowa.txt";
string plikZUzytkownikami = "Uzytkownicy - logowanie i rejestracja.txt";
string plikTymczasowy = "Ksiazka adresowa - plik tymczasowy.txt";

struct Uzytkownik {
	int id;
	string login, haslo;
};

struct Adresat {
    int id = 0;
    int idUzytkownika = 0;
    string imie = "", nazwisko = "", numerTelefonu = "", email = "" , adres = "";
};

int zamianaStringNaInt(string cyfry) {
    int liczba;
    istringstream iss(cyfry);
    iss >> liczba;
    return liczba;
}

string wczytajLinie() {
    string wejscie = "";
    string pustyTekst = "";
    getline(cin, wejscie);
    while (wejscie == pustyTekst) {
        cout << "Prosze nie zostawiac pustego pola. Wpisz ponownie." << endl;
        getline(cin, wejscie);
    }
    return wejscie;
}

char wczytajZnak() {
    string wejscie = "";
    char znak  = {0};

    while (true) {
        getline(cin, wejscie);

        if (wejscie.length() == 1) {
            znak = wejscie[0];
            break;
        }
        cout << "To nie jest pojedynczy znak. Wpisz ponownie." << endl;
    }
    return znak;
}

void dodawanieNowegoAdresataDoPlikuZDanymiAdresatow(Adresat adresat, int idOstatniegoAdresata) {
    fstream plik;

    plik.open(plikZAdresatami, ios::out|ios::app);
    if (!plik.good()) {
        cout << "Plik nie istnieje" << endl;
        exit(0);
    }
        plik << adresat.id << '|';
        plik << adresat.idUzytkownika << '|';
        plik << adresat.imie << '|';
        plik << adresat.nazwisko << '|';
        plik << adresat.numerTelefonu << '|';
        plik << adresat.email << '|';
        plik << adresat.adres << '|';
        plik << endl;

    plik.close();
}

void aktualizowaniePlikuZDanymiAdresatow(vector<Adresat> &adresaci, int idAdresataDoEdycji) {
    fstream plik, plik2;
    string liniaZDanymiAdresataWPliku = "";
    string cyfra = "";
    int idAdresataWLiniiZDanymiAdresataWPliku = 0;
    int numerElementuWektora = 0;
    bool sprawdzenieCzyZnajdujeTakiElementWektora = false;

    plik.open(plikZAdresatami, ios::in);
    if (!plik.good()) {
        cout << "Plik nie istnieje" << endl;
        exit(0);
    }

    plik2.open(plikTymczasowy, ios::out);
    if (!plik2.good()) {
        cout << "Plik nie istnieje" << endl;
        exit(0);
    }
    for (int i = 0; i < (int) adresaci.size(); i++) {
        if (adresaci[i].id == idAdresataDoEdycji) {
            sprawdzenieCzyZnajdujeTakiElementWektora = true; //po usunieciu wpisu z wektora dochodzi do przesuniecia numerow jego elementow
            numerElementuWektora = i;
            break;
        }
    }
    while (getline(plik,liniaZDanymiAdresataWPliku)) {
        cyfra = liniaZDanymiAdresataWPliku[0];
        idAdresataWLiniiZDanymiAdresataWPliku = zamianaStringNaInt(cyfra);
        if (idAdresataWLiniiZDanymiAdresataWPliku != idAdresataDoEdycji) {
            plik2 << liniaZDanymiAdresataWPliku << endl;
        }
        else {
            if (sprawdzenieCzyZnajdujeTakiElementWektora == true) {
                plik2 << adresaci[numerElementuWektora].id << '|';
                plik2 << adresaci[numerElementuWektora].idUzytkownika << '|';
                plik2 << adresaci[numerElementuWektora].imie << '|';
                plik2 << adresaci[numerElementuWektora].nazwisko << '|';
                plik2 << adresaci[numerElementuWektora].numerTelefonu << '|';
                plik2 << adresaci[numerElementuWektora].email << '|';
                plik2 << adresaci[numerElementuWektora].adres << '|';
                plik2 << endl;
            }
            else continue;
        }
    }
    plik.close();
    plik2.close();
    remove("Ksiazka adresowa.txt");
    rename("Ksiazka adresowa - plik tymczasowy.txt", "Ksiazka adresowa.txt");
}

Uzytkownik zaciaganieDanychUzytkownika(string linia) {
    Uzytkownik uzytkownik;
    string daneUzytkownika = "";
    int numerDanejUzytkownika = 0;

    for (int i = 0; i < (int) linia.length(); i++) {
        if (linia[i] != '|') {
            daneUzytkownika += linia[i];
        }
        else {
            numerDanejUzytkownika++;

            if (numerDanejUzytkownika == 1) {
                    uzytkownik.id = zamianaStringNaInt(daneUzytkownika);
                    daneUzytkownika = "";
            }
            if (numerDanejUzytkownika == 2) {
                    uzytkownik.login = daneUzytkownika;
                    daneUzytkownika = "";
            }
            if (numerDanejUzytkownika == 3) {
                    uzytkownik.haslo = daneUzytkownika;
                    daneUzytkownika = "";
                    break;
            }
        }
    }
    return uzytkownik;
}

Adresat zaciaganieDanychAdresata(string linia) {
    Adresat adresat;
    string daneAdresata = "";
    int numerDanejAdresata = 0;

    for (int i = 0; i < (int) linia.length(); i++) {
            if (linia[i] != '|') {
                daneAdresata += linia[i];
            }
            else {
                numerDanejAdresata++;

                if (numerDanejAdresata == 1) {
                    adresat.id = zamianaStringNaInt(daneAdresata);
                    daneAdresata = "";
                }
                if (numerDanejAdresata == 2) {
                    adresat.idUzytkownika = zamianaStringNaInt(daneAdresata);
                    daneAdresata = "";
                }
                if (numerDanejAdresata == 3) {
                    adresat.imie = daneAdresata;
                    daneAdresata = "";
                }
                if (numerDanejAdresata == 4) {
                    adresat.nazwisko = daneAdresata;
                    daneAdresata = "";
                }
                if (numerDanejAdresata == 5) {
                    adresat.numerTelefonu = daneAdresata;
                    daneAdresata = "";
                }
                if (numerDanejAdresata == 6) {
                    adresat.email = daneAdresata;
                    daneAdresata = "";
                }
                if (numerDanejAdresata == 7) {
                    adresat.adres = daneAdresata;
                    daneAdresata = "";
                }
            }
    }
    return adresat;
}

int wyodrebnijOstatnieIdAdresata(string daneOstatniegoAdresataWPliku) {
    string idAdresata = "";
    int wyodrebnioneIdOstatniegoAdresata = 0;

    for (int i = 0; i < (int) daneOstatniegoAdresataWPliku.length(); i++) {
        if (isdigit(daneOstatniegoAdresataWPliku[i])) {
            idAdresata += daneOstatniegoAdresataWPliku[i];
        }
        else break;
    }
    wyodrebnioneIdOstatniegoAdresata = zamianaStringNaInt(idAdresata);
    return wyodrebnioneIdOstatniegoAdresata;
}

void zapiszNowegoUzytkownikaDoPliku(vector<Uzytkownik> &uzytkownicy) {
    fstream plik;
    plik.open(plikZUzytkownikami, ios::out | ios::app);

    if (plik.good()) {
        plik << uzytkownicy.back().id << "|" << uzytkownicy.back().login << "|" << uzytkownicy.back().haslo << "|" << endl;
        plik.close();
    }
    else cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
}

void aktualizowaniePlikuZUzytkownikami(vector<Uzytkownik> &uzytkownicy) {
    fstream plik;

    plik.open(plikZUzytkownikami, ios::out);

    if (!plik.good()) {
        cout << "Plik nie istnieje" << endl;
        exit(0);
    }
    for (int i = 0; i < (int) uzytkownicy.size(); i++) {
        plik << uzytkownicy[i].id << '|';
        plik << uzytkownicy[i].login << '|';
        plik << uzytkownicy[i].haslo << '|';
        plik << endl;
    }
    plik.close();
}

int wczytajUzytkownikowZPliku(vector<Uzytkownik> &uzytkownicy) {
    Uzytkownik uzytkownik;
    int ostatnieIdUzytkownika = 0;
    string liniaZDanymiUzytkownikaWPliku = "";
    string daneOstatniegoUzytkownikaWPliku = "";
    fstream plik;

    plik.open(plikZUzytkownikami, ios::in);

    if (plik.good()) {
        while (getline(plik,liniaZDanymiUzytkownikaWPliku)) {
            uzytkownik = zaciaganieDanychUzytkownika(liniaZDanymiUzytkownikaWPliku);
            if (uzytkownik.id != 0) uzytkownicy.push_back(uzytkownik);
        }
        daneOstatniegoUzytkownikaWPliku = liniaZDanymiUzytkownikaWPliku;
    }
    else cout << "Plik nie zostal odczytany!" << endl;

    plik.close();

    if (daneOstatniegoUzytkownikaWPliku != "") {
        ostatnieIdUzytkownika = wyodrebnijOstatnieIdAdresata(daneOstatniegoUzytkownikaWPliku);
        return ostatnieIdUzytkownika;
    }
    else return 0;
}

int wczytajAdresatowZPliku(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    Adresat adresat;
    int ostatnieIdAdresata = 0;
    string liniaZDanymiAdresataWPliku = "";
    string daneOstatniegoAdresataWPliku = "";
    fstream plik;

    plik.open(plikZAdresatami, ios::in);

    if (plik.good()) {
        while (getline(plik,liniaZDanymiAdresataWPliku)) {
            adresat = zaciaganieDanychAdresata(liniaZDanymiAdresataWPliku);
            if (adresat.idUzytkownika == idZalogowanegoUzytkownika) {
                if (adresat.id != 0) adresaci.push_back(adresat);
            }
        if (liniaZDanymiAdresataWPliku != "") daneOstatniegoAdresataWPliku = liniaZDanymiAdresataWPliku;
        }
    }
    else cout << "Plik nie zostal odczytany!" << endl;

    plik.close();

    if (daneOstatniegoAdresataWPliku != "") {
        ostatnieIdAdresata = wyodrebnijOstatnieIdAdresata(daneOstatniegoAdresataWPliku);
        return ostatnieIdAdresata;
    }
    else return 0;
}

void zmianaHaslaUzytkownika(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika) {
    string haslo, noweHaslo;
    bool prawidlowaZmianaHasla = false;

    while (prawidlowaZmianaHasla == false) {
        cout << endl << "Podaj swoje obecne haslo: " << endl;
        haslo = wczytajLinie();

        if (haslo == uzytkownicy[idZalogowanegoUzytkownika - 1].haslo) {
            cout << endl << "Podaj nowe haslo:" << endl;
            noweHaslo = wczytajLinie();
            uzytkownicy[idZalogowanegoUzytkownika - 1].haslo = noweHaslo;
            aktualizowaniePlikuZUzytkownikami(uzytkownicy);
            prawidlowaZmianaHasla = true;
            cout << endl << "Haslo zostalo zmienione." << endl << endl;
            system("pause");
        }
        else cout << endl << "Haslo niepoprawne" << endl;
    }
}

int sprawdzenieLogowania(vector<Uzytkownik> &uzytkownicy, string login) {
    int idUzytkownika = 0;
    string haslo;
    bool sprawdzenie = false;

    for (int i = 0; i < (int) uzytkownicy.size(); i++) {
        if (login == uzytkownicy[i].login) {
            idUzytkownika = uzytkownicy[i].id;
            for (int j =3; j > 0; j--) {
                cout << endl << "Prosze podac haslo. Pozostalo: " << j << " prob." << endl;
                haslo = wczytajLinie();
                if (haslo == uzytkownicy[i].haslo) {
                    sprawdzenie = true;
                    break;
                }
                else cout << endl << "Haslo niepoprawne." << endl;
            }
        }
    }
    if (sprawdzenie == false) {
        cout << endl << "Dane nieprawidlowe." << endl;
        return idUzytkownika = 0;
    }
    return idUzytkownika;
}

int logowanie(vector<Uzytkownik> &uzytkownicy) {
    string login;
    cout << endl << "Podaj login: " << endl;
    login = wczytajLinie();
    int idUzytkownika = sprawdzenieLogowania(uzytkownicy,login);

    if (idUzytkownika != 0) {
        cout << endl << "Zalogowano" << endl << endl;
        system("pause");
        return idUzytkownika;
    }
    cout << endl << "Nie udalo sie zalogowac" << endl;
    system("pause");
    return 0;
}

void rejestracja(vector<Uzytkownik> &uzytkownicy) {
    Uzytkownik uzytkownik;
    string loginDoSprawdzenia = "";
    bool sprawdzenieLoginu = false;

    if (uzytkownicy.size() == 0) {
        uzytkownik.id = 1;
        cout << endl << "Prosze nadac login: ";
        loginDoSprawdzenia = wczytajLinie();
        sprawdzenieLoginu = true;
    }
    else {
        uzytkownik.id = (uzytkownicy.back().id)+1;
        while (sprawdzenieLoginu == false) {
            cout << endl << "Prosze nadac login: ";
            loginDoSprawdzenia = wczytajLinie();
            for (int i=0; i < (int) uzytkownicy.size(); i++) {
                if (loginDoSprawdzenia == uzytkownicy[i].login) {
                    cout << endl << "Taki login juz istnieje, prosze podac inny." << endl;
                    sprawdzenieLoginu = false;
                    break;
                }
                else sprawdzenieLoginu = true;
            }
        }
    }
    if (sprawdzenieLoginu == true) {
        uzytkownik.login = loginDoSprawdzenia;
        cout << "Prosze nadac haslo: ";
        uzytkownik.haslo = wczytajLinie();
        uzytkownicy.push_back(uzytkownik);
        zapiszNowegoUzytkownikaDoPliku(uzytkownicy);
        cout << endl << "Nowy uzytkownik zostal utworzony. Mozna sie teraz zalogowac." << endl << endl;
        system("pause");
    }
    return;
}

void wyswietlenieInformacjiOAdresacie(Adresat adresat) {
    cout << endl;
    cout << "ID: " << adresat.id << endl;
    cout << adresat.imie << " " << adresat.nazwisko << endl;
    cout << "Telefon: " << adresat.numerTelefonu << endl;
    cout << "Email: " << adresat.email << endl;
    cout << "Adres: " << adresat.adres << endl;
    cout << endl;
}

int dodajAdresata(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika, int idOstatniegoAdresata) {
    Adresat adresat;

    adresat.id = ++idOstatniegoAdresata;
    adresat.idUzytkownika = idZalogowanegoUzytkownika;
    cout << "Podaj imie: ";
    adresat.imie = wczytajLinie();
    cout << "Podaj nazwisko: ";
    adresat.nazwisko = wczytajLinie();
    cout << "Podaj numer telefonu: ";
    adresat.numerTelefonu = wczytajLinie();
    cout << "Podaj email: ";
    adresat.email = wczytajLinie();
    cout << "Podaj adres: ";
    adresat.adres = wczytajLinie();

    wyswietlenieInformacjiOAdresacie(adresat);

    adresaci.push_back(adresat);
    dodawanieNowegoAdresataDoPlikuZDanymiAdresatow(adresat,idOstatniegoAdresata);
    cout << endl << "Osoba zostala dodana" << endl;
    system("pause");
    return idOstatniegoAdresata;
}

void wyszukajAdresataPoImieniu(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    string imie;
    bool sprawdzenie = false;
    cout << "Podaj imie osoby jaka mamy wyszukac: ";
    imie = wczytajLinie();
    for (int i = 0; i < (int)adresaci.size(); i++) {
        if (adresaci[i].idUzytkownika == idZalogowanegoUzytkownika) {
            if (adresaci[i].imie == imie) {
                sprawdzenie = true;
                cout << endl;
                cout << "ID: " << adresaci[i].id << endl;
                cout << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
                cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
                cout << "Email: " << adresaci[i].email << endl;
                cout << "Adres: " << adresaci[i].adres << endl;
                cout << endl;
            }
        }
    }
    if (!sprawdzenie) cout << endl << "Brak takiego wpisu w ksiazce adresowej" << endl << endl;
    system("pause");
}

void wyszukajAdresataPoNazwisku(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    string nazwisko;
    bool sprawdzenie = false;
    cout << "Poda nazwisko osoby jaka mamy wyszukac: ";
    nazwisko = wczytajLinie();
    for (int i = 0; i < (int)adresaci.size(); i++) {
        if (adresaci[i].idUzytkownika == idZalogowanegoUzytkownika) {
            if (adresaci[i].nazwisko == nazwisko) {
                sprawdzenie = true;
                cout << endl;
                cout << "ID: " << adresaci[i].id << endl;
                cout << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
                cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
                cout << "Email: " << adresaci[i].email << endl;
                cout << "Adres: " << adresaci[i].adres << endl;
                cout << endl;
            }
        }
    }
    if (!sprawdzenie) cout << endl << "Brak takiego wpisu w ksiazce adresowej" << endl << endl;
    system("pause");
}

void wyswietlWszystkichAdresatow(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    bool sprawdzenieCzyKsiazkaAdresowaJestPustaDlaDanegoUzytkownika = true;
    if (!adresaci.empty()) {
        cout << "Wszyscy zapisani adresaci:" << endl;
        int i = 0;
        for (vector <Adresat> :: iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (adresaci[i].idUzytkownika == idZalogowanegoUzytkownika) {
                wyswietlenieInformacjiOAdresacie(*itr);
                sprawdzenieCzyKsiazkaAdresowaJestPustaDlaDanegoUzytkownika = false;
            }
            i++;
        }

        if (sprawdzenieCzyKsiazkaAdresowaJestPustaDlaDanegoUzytkownika == true) cout << endl << "Brak wpisow w ksiazce adresowej" << endl;
    }
    else cout << endl << "Brak wpisow w ksiazce adresowej" << endl << endl;
    cout << endl;
    system("pause");
}

void edytujAdresata(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    char wybor;
    int idAdresataDoEdycji = 0;
    bool sprawdzenieCzyTakiUzytkownikIstnieje = false;
    bool zakonczenieEdytowaniaAdesata = false;

    cout << "Wpisz ID adresata do edycji" << endl;

    while (zakonczenieEdytowaniaAdesata == false) {
        cin >> idAdresataDoEdycji;
        cin.sync();
        for (int i=0; i < (int) adresaci.size(); i++) {
            if (adresaci[i].idUzytkownika == idZalogowanegoUzytkownika) {
                if (idAdresataDoEdycji == adresaci[i].id) {
                    sprawdzenieCzyTakiUzytkownikIstnieje = true;
                    cout << "Znaleziono adresata o ID: " << adresaci[i].id << endl << endl;
                    cout << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
                    cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
                    cout << "Email: " << adresaci[i].email << endl;
                    cout << "Adres: " << adresaci[i].adres << endl << endl;

                    cout << "Wybierz dana, ktora chcesz edytowac:" << endl << endl;
                    cout << "1. Imie" << endl;
                    cout << "2. Nazwisko" << endl;
                    cout << "3. Numer telefonu" << endl;
                    cout << "4. Email" << endl;
                    cout << "5. Adres" << endl;
                    cout << "6. Powrot do menu glownego" << endl << endl;

                    wybor = wczytajZnak();

                    switch (wybor) {
                        case '1': {
                            cout << "Podaj nowe imie: ";
                            adresaci[i].imie = wczytajLinie();
                            aktualizowaniePlikuZDanymiAdresatow(adresaci,idAdresataDoEdycji);
                            break;
                        }
                        case '2': {
                            cout << "Podaj nowe nazwisko: ";
                            adresaci[i].nazwisko = wczytajLinie();
                            aktualizowaniePlikuZDanymiAdresatow(adresaci,idAdresataDoEdycji);
                            break;
                        }
                        case '3': {
                            cout << "Podaj nowy numer telefonu: ";
                            adresaci[i].numerTelefonu = wczytajLinie();
                            aktualizowaniePlikuZDanymiAdresatow(adresaci,idAdresataDoEdycji);
                            break;
                        }
                        case '4': {
                            cout << "Podaj nowy email: ";
                            adresaci[i].email = wczytajLinie();
                            aktualizowaniePlikuZDanymiAdresatow(adresaci,idAdresataDoEdycji);
                            break;
                        }
                        case '5': {
                            cout << "Podaj nowy adres: ";
                            adresaci[i].adres = wczytajLinie();
                            aktualizowaniePlikuZDanymiAdresatow(adresaci,idAdresataDoEdycji);
                            break;
                        }
                        case '6': {
                            cout << "Nastapi powrot do menu glownego" << endl;
                            zakonczenieEdytowaniaAdesata = true;
                            break;
                        }
                    }
                    if (wybor != '6') cout << endl << "Dane zostaly zaktualizowane" << endl;
                    zakonczenieEdytowaniaAdesata = true;
                    system("pause");
                }
            }
        }
        if (sprawdzenieCzyTakiUzytkownikIstnieje == false) {
            cout << "Nie znaleziono adresata o podanym numerze ID." << endl;
            cout << "1. Podaj inny numer ID." << endl;
            cout << "2. Powroc do menu glownego." << endl;

            wybor = wczytajZnak();

            switch (wybor) {
                case '1': {
                    cout << "Podaj inny numer ID: ";
                    break;
                }
                case '2': {
                    zakonczenieEdytowaniaAdesata = true;
                    break;
                }
            }
        }
    }
}

void usunAdresata(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    char wybor;
    int idAdresataDoUsuniecia = 0;
    bool sprawdzenieCzyTakiUzytkownikIstnieje = false;
    bool zakonczenieUsuwaniaAdesata = false;

    cout << "Wpisz ID adresata do usuniecia" << endl;

    while (zakonczenieUsuwaniaAdesata == false) {
        cin >> idAdresataDoUsuniecia;
        cin.sync();
        for (int i=0; i < (int) adresaci.size(); i++) {
            if (adresaci[i].idUzytkownika == idZalogowanegoUzytkownika) {
                if (idAdresataDoUsuniecia == adresaci[i].id) {
                    sprawdzenieCzyTakiUzytkownikIstnieje = true;
                    cout << "Znaleziono adresata o ID: " << adresaci[i].id << endl << endl;
                    cout << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
                    cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
                    cout << "Email: " << adresaci[i].email << endl;
                    cout << "Adres: " << adresaci[i].adres << endl << endl;
                    cout << "Czy na pewno chcesz usunac tego adresata? Wpisz 't', aby potwierdzic lub 'n' aby odrzucic." << endl << endl;

                    wybor = wczytajZnak();

                    switch (wybor) {
                        case 't': {
                            for (int i = 0; i < (int) adresaci.size(); i++) {
                                if (adresaci[i].id == idAdresataDoUsuniecia) adresaci.erase(adresaci.begin() + i);
                            }
                            aktualizowaniePlikuZDanymiAdresatow(adresaci,idAdresataDoUsuniecia);
                            break;
                        }
                        case 'n': {
                            zakonczenieUsuwaniaAdesata = true;
                            break;
                        }
                    }
                    if (wybor != 'n') cout << endl << "Dane zostaly zaktualizowane" << endl;
                    zakonczenieUsuwaniaAdesata = true;
                    system("pause");
                }
            }
        }

        if (sprawdzenieCzyTakiUzytkownikIstnieje == false) {
            cout << "Nie znaleziono adresata o podanym numerze ID." << endl;
            cout << "1. Podaj inny numer ID." << endl;
            cout << "2. Powroc do menu glownego." << endl;

            wybor = wczytajZnak();

            switch (wybor) {
                case '1': {
                    cout << "Podaj inny numer ID: ";
                    break;
                }
                case '2': {
                    zakonczenieUsuwaniaAdesata = true;
                    break;
                }
            }
        }
    }
}

void pokazMenuLogowaniaRejestracji() {
    system("cls");
    cout << ">>> KSIAZKA ADRESOWA - STRONA STARTOWA <<<" << endl;
    cout << "1. Logowanie" << endl;
    cout << "2. Rejestracja nowego uzytkownika" << endl;
    cout << "9. Zakoncz program" << endl;
    cout << endl;
}

void pokazMenuKsiazkiAdresowej(int idZalogowanegoUzytkownika) {
    system("cls");
    cout << ">>> KSIAZKA ADRESOWA <<<" << endl;
    cout << endl << "Jestes zalogowany jako uzytkownik o ID: " << idZalogowanegoUzytkownika << endl << endl;
    cout << "1. Dodaj adresata" << endl;
    cout << "2. Wyszukaj po imieniu" << endl;
    cout << "3. Wyszukaj po nazwisku" << endl;
    cout << "4. Wyswietl wszystkich adresatow" << endl;
    cout << "5. Edytuj adresata (wymagane podanie ID wpisu)" << endl;
    cout << "6. Usun adresata (wymagane podanie ID wpisu)" << endl;
    cout << "7. Zmiana hasla" << endl;
    cout << "9. Wyloguj sie" << endl;
    cout << endl;
}

void menuKsiazkiAdresowej(vector<Uzytkownik> &uzytkownicy, int idUzytkownika) {
    vector<Adresat> adresaci;
    int idZalogowanegoUzytkownika = idUzytkownika;
    char wybor;

    int idOstatniegoAdresata = wczytajAdresatowZPliku(adresaci,idZalogowanegoUzytkownika);

    while (true) {
        pokazMenuKsiazkiAdresowej(idZalogowanegoUzytkownika);
        wybor = wczytajZnak();
        switch (wybor) {
            case '1': {
                idOstatniegoAdresata = dodajAdresata(adresaci,idZalogowanegoUzytkownika,idOstatniegoAdresata);
                break;
            }
            case '2': {
                wyszukajAdresataPoImieniu(adresaci,idZalogowanegoUzytkownika);
                break;
            }
            case '3': {
                wyszukajAdresataPoNazwisku(adresaci,idZalogowanegoUzytkownika);
                break;
            }
            case '4': {
                wyswietlWszystkichAdresatow(adresaci, idZalogowanegoUzytkownika);
                break;
            }
            case '5': {
                edytujAdresata(adresaci,idZalogowanegoUzytkownika);
                break;
            }
            case '6': {
                usunAdresata(adresaci,idZalogowanegoUzytkownika);
                break;
            }
            case '7': {
                zmianaHaslaUzytkownika(uzytkownicy, idZalogowanegoUzytkownika);
                break;
            }
                                    /*case '8': {
                                        aktualizowaniePlikuZDanymiAdresatow(adresaci);
                                        break;
                                    }*/
            case '9': {
                //
                return;

            }
            default: {
                cout << endl << "Klawisz nieobslugiwany. Sprobuj ponownie" << endl << endl;
                system("pause");
                break;
            }
        }
    }
}

void menuLogowanieRejestracja(vector<Uzytkownik> &uzytkownicy) {
    char wybor;
    int idUzytkownika = 0;

    while(true) {
        pokazMenuLogowaniaRejestracji();
        wybor = wczytajZnak();

        switch (wybor) {
            case '1': {
                idUzytkownika = logowanie(uzytkownicy);
                if (idUzytkownika != 0) menuKsiazkiAdresowej(uzytkownicy, idUzytkownika);
                break;
            }
            case '2': {
                rejestracja(uzytkownicy);
                break;
            }
            case '9': {
                exit(0);
                break;
            }
            default: {
                cout << endl << "Klawisz nieobslugiwany. Sprobuj ponownie" << endl << endl;
                system("pause");
                break;
            }
        }
    }
}

int main() {
    vector<Uzytkownik> uzytkownicy;

    wczytajUzytkownikowZPliku(uzytkownicy);
    menuLogowanieRejestracja(uzytkownicy);

    return 0;
}
