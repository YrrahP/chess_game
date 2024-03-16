#include <iostream>
#include <random>
#include <string>
#include <limits>

using namespace std;


int readIntInRange(const string& prompt, int min, int max)
{
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) {
                return value;
            }
            else {
                cout << "Veuillez entrer un nombre entre " << min << " et " << max << "." << endl;
            }
        }
        else {
            cout << "Entrée invalide. Veuillez entrer un nombre." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    }
}




int main()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 1000);
    int randomNumber = distrib(gen);

    int guess;
    int attempts = 0;
    cout << "Devinez le nombre entre 0 et 1000." << endl;

    do {
        guess = readIntInRange("Entrez un nombre entier : ", 0, 1000);
        attempts++;

        if (guess < randomNumber) {
            cout << "Trop bas." << endl;
        }
        else if (guess > randomNumber) {
            cout << "Trop haut." << endl;
        }
    } while (guess != randomNumber);

    cout << "Bravo! Vous avez reussi en " << attempts << " tentatives!" << endl;

    return 0;
}



