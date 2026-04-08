#include <iostream>
#include <string>

#include "CampusCompass.h"

using namespace std;

int main() {

    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    if(!compass.ParseCSV("data/edges.csv", "data/classes.csv")){

        cout << "Failed to open files" << endl;
        return 0;

    }

    int numCommands;
    string command;
    cin >> numCommands;
    cin.ignore(); // gets rid of the newline after the number is inputted

    for (int i = 0; i < numCommands; i++) {
        getline(cin, command);

        if(!compass.ParseCommand(command)){
            cout << "unsuccessful" << endl;
        }
    }
    return 0;
}