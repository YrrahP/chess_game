#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <array>

using namespace std;


const int maxSize = 100;

int getInt() {
    int number;
    while (true) {
        cout << "Entrez un nombre entier : ";
        if (cin >> number) {
            return number;
        }
        else {
            cout << "Entree invalide. Veuillez entrer un nombre entier." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void insertInArray(array<int, maxSize>& arr, int& value, int& currentSize)
{
    if (currentSize >= maxSize) {
        cerr << "Le tableau est plein, impossible d'insérer un nouvel élément." << endl;
        return;
    }

    int insertPos = currentSize;
    for (int i = 0; i < currentSize; ++i) {
        if (arr[i] > value) {
            insertPos = i;
            break;
        }
    }

    for (int i = currentSize; i > insertPos; i--) {
        arr[i] = arr[i - 1];
    }

    arr[insertPos] = value;
    currentSize++;
}



int main() {
    array<int, maxSize> arr = { 1, 3, 4, 7, 9 };
    int currentSize = 5;

    int number = getInt();

    insertInArray(arr, number, currentSize);

    cout << "Tableau apres insertion: { ";
    for (int i = 0; i < currentSize; i++) {
        cout << arr[i] << (i < currentSize - 1 ? ", " : " ");
    }
    cout << "}" << endl;

    return 0;
}





