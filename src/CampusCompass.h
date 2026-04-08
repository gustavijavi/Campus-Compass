#pragma once
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Student {

    // name of student
    string name;
    
    // ID for where student resides
    int locationId;

    // vector of classes student is in
    vector<string> classes;

};


class CampusCompass {
private:

    // map for the location id to its neighbors and the weights to said neighbors | location ID 1 -> vector of pairs of location ID 2 and the weight (time) to that location from location ID 1
    map<int, vector<pair<int, int>>> graph;

    // set for which edges (paths) are closed
    set<pair<int, int>> closedEdges;

    // map for the students | UF ID -> Student struct
    map<int, Student> students;

    // map from class code to its location ID | class code -> location ID
    map<string, int> classLocations;

    // map from class code to its scheduled time in minutes | class code -> pair of times in minutes
    map<string, pair<int, int>> classTimes;

public:

    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

};
