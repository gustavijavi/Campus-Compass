#include <iostream>
#include <string>

#include "CampusCompass.h"

using namespace std;

int main() {

    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    int numCommands;
    string command;
    cin >> numCommands;
    cin.ignore(); // gets rid of the newline after the number is inputted

    for (int i = 0; i < numCommands; i++) {
        getline(cin, command);

        compass.ParseCommand(command);
    }
    return 0;
}