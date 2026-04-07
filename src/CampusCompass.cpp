#include "CampusCompass.h"

using namespace std;

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    
    ifstream edgesFile(edges_filepath);

    if(!edgesFile.is_open()){
        return false;
    }

    string line;

    // get past first line of csv
    getline(edgesFile, line);

    while(getline(edgesFile, line)){
        istringstream ss(line);
        string parse;

        // location id 1
        getline(ss, parse, ',');

        
        // location id 2
        getline(ss, parse, ',');


        // name 1
        getline(ss, parse, ',');


        // name 2
        getline(ss, parse, ',');


        // time
        getline(ss, parse, ',');
    }


    ifstream classesFile(classes_filepath);
    
    if(!classesFile.is_open()){
        return false;
    }

    getline(classesFile, line);

    while(getline(classesFile, line)){
        istringstream ss(line);
        string parse;

        // class name
        getline(ss, parse, ',');

        // class id
        getline(ss, parse, ',');


        // start time
        getline(ss, parse, ',');


        // end time
        getline(ss, parse, ',');

    }

    return true;
}

bool CampusCompass::ParseCommand(const string &command) {

    if (command.substr(0, 7) == "insert ") {
        
    } else if (command.substr(0, 7) == "remove ") {
        
    } else if (command.substr(0, 10) == "dropClass ") {
        
    } else if (command.substr(0, 13) == "replaceClass ") {
        
    } else if (command.substr(0, 12) == "removeClass ") {
        
    } else if (command.substr(0, 19) == "toggleEdgesClosure ") {
        
    } else if (command.substr(0, 16) == "checkEdgeStatus ") {
        
    } else if (command.substr(0, 12) == "isConnected ") {
        
    } else if (command.substr(0, 19) == "printShortestEdges "){

    } else if (command.substr(0, 17) == "printStudentZone "){

    } else if (command.substr(0, 15) == "verifySchedule ") {

    } else {
        // anything else unrecognized should be unsuccessful as the syntax is strict
        return false;
    }

    return true;
}


// helper function to see if the name has only numbers and is a length of 8
bool isValidUfId(const string& ufId) {
    if (ufId.length() != 8) {
        return false;
    }

    for (char c : ufId) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}