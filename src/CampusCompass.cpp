#include "CampusCompass.h"

#include <string>
#include <regex>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
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


    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
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