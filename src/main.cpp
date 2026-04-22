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

    int numCommands; // num of commands that will be made
    string command; // string for the actual command
    cin >> numCommands; // take in the num of commands from user input
    cin.ignore(); // gets rid of the newline after the number is inputted

    // loop for each command inputted by user
    for (int i = 0; i < numCommands; i++) {
        getline(cin, command); // get each command inputted by user

        // if the parsing did not work via user error, print unsuccessful
        if(!compass.ParseCommand(command)){
            cout << "unsuccessful" << endl;
        }

        cout << compass.getStringRepresentation(); // print what ParseCommand returned in its output

    }
    return 0;
}