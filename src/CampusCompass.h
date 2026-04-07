#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Student {

    string name;
    int locationId;
    vector<string> classes;

};


class CampusCompass {
private:

    map<int, vector<pair<int, int>>> graph;
    set<pair<int, int>> closedEdges;
    map<int, Student> students;
    map<string, int> classLocations;
    map<string, pair<int, int>> classTimes;

public:

    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

};
