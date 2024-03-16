#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

double nextTerm(double currentTerm) {
	return sqrt(2.0 + currentTerm);
}


double calculateLimit(double precision) {
	double currentTerm = 1.0;
	double nextTermValue = nextTerm(currentTerm);

	while (fabs(nextTermValue - currentTerm) > precision) {
		currentTerm = nextTermValue;
		nextTermValue = nextTerm(currentTerm);
	}

	return nextTermValue;
}


int main() {
	double precision = 0.00001;
	double limit = calculateLimit(precision);

	cout << "La limite de la suite est: " << setprecision(7) << limit << endl;

	return 0;
}