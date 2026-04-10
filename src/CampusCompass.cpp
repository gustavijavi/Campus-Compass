#include "CampusCompass.h"

using namespace std;


// helper function to see if the name has only numbers and is a length of 8
bool CampusCompass::isValidUfId(const string &ufId) {
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

bool CampusCompass::isValidName(const string &name){

    // goes through each char in name
    for(char c : name){
        // if char is not letter and char is not space, return false
        if(!isalpha(c) && c != ' '){
            return false;
        }
    }

    // else return true
    return true;
}

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
        
        if(!isValidName(name)){
            return false;
        }

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

        if(numClasses < 1 || numClasses > 6){
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

            for(const string &code : classCodes){
                if(classCode == code){
                    return false;
                }
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
        
        // store UF ID inputted after remove command
        string ufId = command.substr(7);

        // if it's not a valid UF ID (wrong length or uses letter), return false
        if(!isValidUfId(ufId)){
            return false;
        }

        // if it's not a student already a part of the map of students, return false
        if(students.count(stoi(ufId)) == 0){
            return false;
        }

        // erase student from map using their UF ID
        students.erase(stoi(ufId));

        // print successful
        cout << "successful" << endl;

    } else if (command.substr(0, 10) == "dropClass ") {
        
        // store the rest of the command inputted by the user
        string rest = command.substr(10);
        string extra;
        istringstream ss(rest);

        // strings for storing the UF ID and class code inputted by user command
        string ufId, classCode;

        // check for if the command matches having 2 blocks of text after the initial command
        if(!(ss >> ufId >> classCode)){
            return false;
        }

        // checks for whether the UF ID is valid and if the class code is also valid
        if(!isValidUfId(ufId) || classLocations.count(classCode) == 0){
            return false;
        }

        // checks for if there's any extra text. If there is it will return unsuccesful. Treating syntax with strictness
        if(ss >> extra){
            return false;
        }

        // if there is no student with the correlating UF ID return false
        if(students.count(stoi(ufId)) == 0){
            return false;
        }

        // boolean for if the class was successfully erased or not
        bool erased = false;

        // vector for the classes just to make text more readable
        vector<string> &classes = students[stoi(ufId)].classes;

        // looping through all the classes that the student has. If there's a hit, it erases the class, setting erased to true
        for(int i = 0; i < classes.size(); i++){
            if(classes[i] == classCode){
                classes.erase(classes.begin() + i);
                erased = true;
                break;
            }
        }

        // if there was no class that could be found to be erased, return false
        if(!erased){
            return false;
        }

        // if student has no more classes, remove the student
        if(classes.size() == 0){
            students.erase(stoi(ufId));
        }

        // print successful
        cout << "successful" << endl;


    } else if (command.substr(0, 13) == "replaceClass ") {
        
        // store the rest of the command inputted by the user
        string rest = command.substr(13);
        string extra;
        istringstream ss(rest);

        // strings for storing the UF ID and class codes inputted by user command
        string ufId, classCodeOne, classCodeTwo;

        // check for if the command matches having 3 blocks of text after the initial command
        if(!(ss >> ufId >> classCodeOne >> classCodeTwo)){
            return false;
        }

        // checks for whether the UF ID is valid and if the class code is also valid
        if(!isValidUfId(ufId) || classLocations.count(classCodeTwo) == 0){
            return false;
        }

        // checks for if there's any extra text. If there is it will return unsuccesful. Treating syntax with strictness
        if(ss >> extra){
            return false;
        }

        // if there is no student with the correlating UF ID return false
        if(students.count(stoi(ufId)) == 0){
            return false;
        }

        // boolean for checking if student has the class to be replaced and index for that class
        bool hasClass = false;
        int classIndex = 0;

        // vector reference for the classes the student has
        vector<string> &classes = students[stoi(ufId)].classes;

        // loops through all the students classes
        // if student already has class being used to replace it will return false
        // if student has class to be replaced it will set hasClass to true letting the replacement take place
        for(int i = 0; i < classes.size(); i++){
            if(classes[i] == classCodeTwo){
                return false;
            }
            if(classes[i] == classCodeOne){
                hasClass = true;
                classIndex = i;
            }
        }

        // if student doesn't have class, return false
        if(!hasClass){
            return false;
        }

        // replacing class
        classes[classIndex] = classCodeTwo;

        // print successful
        cout << "successful" << endl;


    } else if (command.substr(0, 12) == "removeClass ") {
        
        // store the rest of the command inputted by the user
        string rest = command.substr(12);
        string extra;
        istringstream ss(rest);

        // strings for storing class code inputted by user command
        string classCode;

        // check for if the command matches having 2 blocks of text after the initial command
        if(!(ss >> classCode)){
            return false;
        }

        // checks for whether the UF ID is valid and if the class code is also valid
        if(classLocations.count(classCode) == 0){
            return false;
        }

        // checks for if there's any extra text. If there is it will return unsuccesful. Treating syntax with strictness
        if(ss >> extra){
            return false;
        }

        // initializing variable for amount of classes removed
        int classCount = 0;

        vector<int> studentsToRemove;

        // traversing through all the students within map
        for(auto &pair : students){
            
            // reference to classes each student has
            vector<string> &classes = pair.second.classes;

            // traversing through all the classes each student has
            for(int i = 0; i < classes.size(); i++){
                // if student has the classCode inputted to remove, it is removed
                if(classes[i] == classCode){
                    classes.erase(classes.begin() + i);
                    classCount++;
                    break;
                }
            }

            // if student has no more classes, append student to be removed
            if(classes.size() == 0){
                studentsToRemove.push_back(pair.first);
            }
        }

        // iterate through the UF ID's of students who have no classes left, remove them
        for(int id : studentsToRemove){
            students.erase(id);
        }

        // if no classes were removed from any student, return false
        if(classCount == 0){
            return false;
        }

        // print how many classes were removed
        cout << classCount << endl;


    } else if (command.substr(0, 19) == "toggleEdgesClosure ") {
        
        // store the rest of the command inputted by the user
        string rest = command.substr(19);
        string extra;
        istringstream ss(rest);

        // int for storing num edges to be toggled
        int numEdges;

        // checks for if the command has a block for numEdges after initial command
        if(!(ss >> numEdges)){
            return false;
        }

        int locationOne, locationTwo;

        for(int i = 0; i < numEdges; i++){
            if(!(ss >> locationOne >> locationTwo)){
                return false;
            }

            if(!graph.count(locationOne)){
                return false;
            }

            bool isEdge = false;

            for(int i = 0; i < graph[locationOne].size(); i++){
                if(graph[locationOne][i].first == locationTwo){
                    isEdge = true;
                }
            }

            if(!isEdge){
                return false;
            }

            pair<int, int> edgeOne = make_pair(locationOne, locationTwo);
            pair<int, int> edgeTwo = make_pair(locationTwo, locationOne);

            if(closedEdges.count(edgeOne)){
                closedEdges.erase(edgeOne);
                closedEdges.erase(edgeTwo);
            } else {
                closedEdges.insert(edgeOne);
                closedEdges.insert(edgeTwo);
            }
        }

        // checks for if there's any extra text. If there is it will return unsuccesful. Treating syntax with strictness
        if(ss >> extra){
            return false;
        }

        cout << "successful" << endl;

    } else if (command.substr(0, 16) == "checkEdgeStatus ") {
        
        string rest = command.substr(16);
        string extra;
        istringstream ss(rest);

        // ints for storing location ID's given by command
        int locationOne, locationTwo;

        // checks for if the command has a block for numEdges after initial command
        if(!(ss >> locationOne >> locationTwo)){
            return false;
        }

        // checks if theres any extra text to return false
        if(ss >> extra){
            return false;
        }

        // checks if graph even has first location inputted
        if(graph.count(locationOne) == 0){
            return false;
        }

        // boolean for checker if it's a valid edge being inputted
        bool isEdge = false;

        // iterating through all edges from locationOne inputted to see if there exists an edge between location one and location two
        for(int i = 0; i < graph[locationOne].size(); i++){
            if(graph[locationOne][i].first == locationTwo){
                isEdge = true;
            }
        }

        // if it's not a valid edge print DNE
        if(!isEdge){
            cout << "DNE" << endl;
            return true;
        }

        // create pair for edge
        pair<int, int> edge = make_pair(locationOne, locationTwo);

        // if edge is not within closedEdges set, print open, otherwise print closed
        if(closedEdges.count(edge) == 0){
            cout << "open" << endl;
        } else {
            cout << "closed" << endl;
        }

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