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
        string cell;

        // location id 1
        getline(ss, cell, ',');
        int firstId = stoi(cell);
        
        // location id 2
        getline(ss, cell, ',');
        int secondId = stoi(cell);

        // name 1 and name 2 (unused)
        getline(ss, cell, ',');
        getline(ss, cell, ',');

        // time
        getline(ss, cell, ',');
        int time = stoi(cell);

        graph[firstId].push_back(make_pair(secondId, time));
        graph[secondId].push_back(make_pair(firstId, time));
    }

    edgesFile.close();

    ifstream classesFile(classes_filepath);
    
    if(!classesFile.is_open()){
        return false;
    }

    getline(classesFile, line);

    while(getline(classesFile, line)){
        istringstream ss(line);
        string cell;

        // class code
        getline(ss, cell, ',');
        string classCode = cell;

        // class location ID
        getline(ss, cell, ',');
        int locationId = stoi(cell);

        // start time (calculated into minutes for easy use later)
        getline(ss, cell, ',');
        int pos = cell.find(':');
        int hours = stoi(cell.substr(0, pos));
        int minutes = stoi(cell.substr(pos + 1));
        int totalStartTime = hours * 60 + minutes;


        // end time (calculated into minutes for easy use later)
        getline(ss, cell, ',');
        pos = cell.find(':');
        hours = stoi(cell.substr(0, pos));
        minutes = stoi(cell.substr(pos + 1));
        int totalEndTime = hours * 60 + minutes;

        classLocations[classCode] = locationId;

        classTimes[classCode] = make_pair(totalStartTime, totalEndTime);

    }
    
    classesFile.close();

    return true;
}

bool CampusCompass::ParseCommand(const string &command) {

    if (command.substr(0, 7) == "insert ") {

        // find the quote locations so that you can find the name of the student
        size_t quoteOne = command.find('"');
        size_t quoteTwo = command.find('"', quoteOne + 1);

        // if there is no quote's found or insufficient amount of quotes, it will
        // return unsuccessful since the size_t quoteOne/quoteTwo will store an npos
        if (quoteOne == string::npos || quoteTwo == string::npos) {
            return false;
        }

        // stores the name
        string name = command.substr(quoteOne + 1, quoteTwo - quoteOne - 1);
        
        // store the rest of the command inputted by the user
        string rest = command.substr(quoteTwo + 2);
        istringstream ss(rest);
        
        // create variables for the UF ID, location ID, and the number of classes the user states they'll be inputting
        string ufIdString;
        int locationId, numClasses;

        // if it doesn't follow the correct command guidelines, it will not go through
        if(!(ss >> ufIdString >> locationId >> numClasses)){
            return false;
        }

        // checks for whether UF ID is invalid
        if (!isValidUfId(ufIdString)) {
            return false;
        }

        // stores ufId as an int rather than a string that was checked for validity
        int ufId = stoi(ufIdString);

        // if the UF ID is not in the map for students or if the location ID is not valid, return false
        if(students.count(ufId) == 1 || graph.count(locationId) == 0){
            return false;
        }

        // initialize variables for class codes vector and class code that will be taken in from command
        vector<string> classCodes;
        string classCode;

        // pushing back class codes to be put into student struct
        for(int i = 0; i < numClasses; i++){
            // if there aren't enough class codes, return false
            if(!(ss >> classCode)) {
                return false;
            }

            // if class code is not within the directory of class codes, return false
            if(classLocations.count(classCode) == 0) {
                return false;
            }

            classCodes.push_back(classCode);
        }

        string extra;
        if(ss >> extra) {
            return false; // in case there are more classes than initially input by user
        }

        // create student struct
        Student ufStudent;

        // set variables
        ufStudent.name = name;
        ufStudent.locationId = locationId;
        ufStudent.classes = classCodes;
        
        // add to students map within object
        students[ufId] = ufStudent;

        // print successful
        cout << "successful" << endl;

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