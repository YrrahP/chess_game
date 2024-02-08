#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


vector<string> loadGrid(const string& filename) {
    vector<string> grid;
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur : Impossible d'ouvrir le fichier '" << filename << "'" << endl;
        exit(1);
    }

    string line, processedLine;
    while (getline(file, line)) {
        processedLine.clear();
        for (char ch : line) {
            if (ch != ' ') {
                processedLine.push_back(ch);
            }
        }
        grid.push_back(processedLine);
    }
    return grid;
}

string checkGameState(const vector<string>& grid) {
    
    for (int i = 0; i < 3; i++) {
        
        if (grid[i][0] == 'x' && grid[i][1] == 'x' && grid[i][2] == 'x') {
            return "Le joueur 1 gagne.";
        }
        
        if (grid[0][i] == 'x' && grid[1][i] == 'x' && grid[2][i] == 'x') {
            return "Le joueur 1 gagne.";
        }
    }
    
    if ((grid[0][0] == 'x' && grid[1][1] == 'x' && grid[2][2] == 'x') ||
        (grid[0][2] == 'x' && grid[1][1] == 'x' && grid[2][0] == 'x')) {
        return "Le joueur 1 gagne.";
    }

    
    for (int i = 0; i < 3; i++) {
        
        if (grid[i][0] == 'o' && grid[i][1] == 'o' && grid[i][2] == 'o') {
            return "Le joueur 2 gagne.";
        }
        
        if (grid[0][i] == 'o' && grid[1][i] == 'o' && grid[2][i] == 'o') {
            return "Le joueur 2 gagne.";
        }
    }
    
    if ((grid[0][0] == 'o' && grid[1][1] == 'o' && grid[2][2] == 'o') ||
        (grid[0][2] == 'o' && grid[1][1] == 'o' && grid[2][0] == 'o')) {
        return "Le joueur 2 gagne.";
    }

    return "Egalite";
}




int main() {
    string filename;
    cout << "Entrez le nom du fichier de la grille: ";
    cin >> filename;

    vector<string> grid = loadGrid(filename);
    string result = checkGameState(grid);
    cout << result << endl;

    return 0;
}