#include <iostream>
using namespace std;

struct Test {
    int a;
    int b;
    int expected;
};

int russianMultiplication(int a, int b) {
    int result = 0;
    while (a > 0) {
        if (a % 2 != 0) { 
            result += b;
        }
        a /= 2;   
        b *= 2;   
    }
    return result;
}

int main() {
    Test tests[] = {
        {37, 129, 37 * 129},
        {25, 4, 25 * 4},
        {13, 7, 13 * 7}
    };

    int testsPassed = 0;
    for (const auto& test : tests) {
        int result = russianMultiplication(test.a, test.b);
        if (result == test.expected) {
            testsPassed++;
        }
    }

    cout << testsPassed << "/" << sizeof(tests) / sizeof(tests[0]) << " tests passent." << endl;

    return 0;
}






























