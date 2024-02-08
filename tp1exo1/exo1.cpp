#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string sentence;
    cout << "Saisissez une phrase : ";
    getline(cin, sentence);

    istringstream iss(sentence);
    string word;

    string longestWord, shortestWord;
    int wordCount = 0;
    int totalLengthOfWords = 0;

    while (iss >> word) {
        if (wordCount == 0 || word.size() > longestWord.size()) {
            longestWord = word;
        }

        if (wordCount == 0 || word.size() < shortestWord.size()) {
            shortestWord = word;
        }

        totalLengthOfWords += word.size();
        wordCount++;
    }

    double averageLength = static_cast<double>(totalLengthOfWords) / wordCount;

    cout << "Le mot le plus court est : " << shortestWord << endl;
    cout << "Le mot le plus long est : " << longestWord << endl;
    cout << "La longueur moyenne est : " << averageLength << " lettres" << endl;

    return 0;
}
