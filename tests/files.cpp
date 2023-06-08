#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
	fstream f("list.txt");
	f << "good" << endl;
	f << "game" << endl;
	f << "coffee" << endl;
	
	ifstream input("list.txt");
	vector<string> vect;

	while (!input.eof()) {
		string str;
		getline(input, str);
		vect.push_back(str);
	}

	for (auto i : vect) { cout << i << endl; } 
	return 0;
}
