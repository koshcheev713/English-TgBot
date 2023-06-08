#include <iostream>
#include <ctime>
using namespace std;

int main() {
    while (true) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        cout << "Date: " << ltm->tm_mday << '.' << 1+ltm->tm_mon << '.' << 1900+ltm->tm_year << endl;
        cout << "Time: " << ltm->tm_hour << ':' << ltm->tm_min << ':' << ltm->tm_sec << endl; 
    }
    return 0;
}