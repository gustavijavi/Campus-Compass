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

// helper function for if name is valid
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

// BFS algorithm
bool CampusCompass::BFS(const int source, const int dest){

    if(source == dest){
        return true;
    }

    queue<int> q;
    set<int> visited;

    q.push(source);
    visited.insert(source);

    while(!q.empty()){

        int u = q.front();
        q.pop();

        for(auto pair : graph[u]){
            if(closedEdges.count(make_pair(u, pair.first)) == 0){
                if(pair.first == dest){
                    return true;
                }

                if(visited.count(pair.first) == 0){
                    q.push(pair.first);
                    visited.insert(pair.first);
                }
            }
        }

    }

    return false;

}

// Dijkstra Algorithm that returns a pair containing an int for the distance of the shortest route from source to dest
// and containing a vector of ints for the path of the shortest route from source to dest.
pair<int, vector<int>> CampusCompass::Dijkstra(const int source, const int dest){

    // if there is no path from source to dest, then return -1 and empty vector
    if(!BFS(source, dest)){
        return {-1, {}};
    }

    // priority queue for a min heap so we can always grab the shortest distance from the node currently on
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    map<int, int> dist; // map of all distances to different nodes from source
    map<int, int> parent; // map of all the parenting nodes for the shortest path

    vector<int> path; // path of nodes to the dest node that is the shortest route

    dist[source] = 0;

    // initializing all nodes in the map to basically infinity for dijkstra algorithm
    for(const auto &pair : graph){
        if(pair.first == source){
            continue;
        }

        dist[pair.first] = INT_MAX;
    }

    // push the source for initialization before while loop
    pq.push({0, source});

    while(!pq.empty()){

        // grab top and pop from pq
        pair<int, int> u = pq.top();
        pq.pop();

        // if there's a old entry, skip
        if(u.first > dist[u.second]){
            continue;
        }

        // if we reached the dest, find the path along the shortest route and return both the path and shortest route distance
        if(u.second == dest){
            int current = dest;

            while(current != source) {
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(source);

            return {dist[dest], path};
        }

        // go through all of the nodes neighbors and check for if it's closed first before checking distances
        for(const pair<int, int> p : graph[u.second]){

            if(closedEdges.count({u.second, p.first}) != 0){
                continue;
            }

            if (dist[p.first] > dist[u.second] + p.second){
                dist[p.first] = dist[u.second] + p.second;
                parent[p.first] = u.second;
                pq.push({dist[p.first], p.first});
            }

        }

    }

}

// Prim's algorithm that returns an int representing the student's zone cost utilizing Dijkstra's algorithm from before
int CampusCompass::Prim(set<int> subGraphNodes, const int source){

    // intitialize zone cost to add to throughout the loops
    int zoneCost = 0;

    // set for tracking which nodes are already a part of the MST
    set<int> MST;

    // insert the source node A.K.A. the students residence into the MST and erase from the subGraph
    MST.insert(source);
    subGraphNodes.erase(source);

    // grab the size of the sub graph as it's going to change as we erase from it
    int size = subGraphNodes.size();

    // loop through the size to get all the nodes over from the subgraph to the MST
    for(int i = 0; i < size; i++){

        // initialize the shortest distance between nodes in the subGraph
        // and the shortest node to keep track of so we can remove it from the subgraph and add it to the MST
        int shortestDist = INT_MAX;
        int shortestNode = 0;

        // loop through all of the MST to see all available neighbors in the subgraph
        for(const auto &node : MST){

            // loop through all the neighbors of each node in the MST
            for(const auto &pair : graph[node]){

                // if an edge is closed between the node and its neighbor, ignore it
                if(closedEdges.count({node, pair.first}) != 0){
                    
                    continue;

                }

                // if the neighbor is within the subgraph set, check if it's the shortest distance
                // and set it to be the shortest distance if so
                if(subGraphNodes.count(pair.first) != 0){

                    if(pair.second < shortestDist){

                        shortestDist = pair.second;
                        shortestNode = pair.first;

                    }

                }

            }

        }

        // insert the shortest node to the MST and remove it from the subgraph
        MST.insert(shortestNode);
        subGraphNodes.erase(shortestNode);

        // add to the zone cost for Prim's MST
        zoneCost += shortestDist;

    }

    return zoneCost;

}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    
    // open edges file
    ifstream edgesFile(edges_filepath);

    // if it didn't open properly, return false
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

    // open classes csv file
    ifstream classesFile(classes_filepath);
    
    // if failed to open, return false
    if(!classesFile.is_open()){
        return false;
    }

    // ignore first line
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
    
    // close file
    classesFile.close();

    // return true to show parsing the csv worked fine
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
        
        string rest = command.substr(12);
        string extra;
        istringstream ss(rest);

        int locationOne, locationTwo;

        if(!(ss >> locationOne >> locationTwo)){
            return false;
        }

        if(ss >> extra){
            return false;
        }

        if(graph.count(locationOne) == 0 || graph.count(locationTwo) == 0){
            return false;
        }

        if(!BFS(locationOne, locationTwo)){
            return false;
        }

        cout << "successful" << endl;

    } else if (command.substr(0, 19) == "printShortestEdges "){

        string rest = command.substr(19);
        string extra;
        istringstream ss(rest);

        int ufId; // int for UF ID

        // grabs UF ID but if nothing present after command, return false
        if(!(ss >> ufId)){
            return false;
        }

        // if there is extra jumble after command or UF ID does not exist, return false
        if(ss >> extra || students.count(ufId) == 0){
            return false;
        }

        // map for classes and the times to get to each class
        // specifically using map as it sorts all the classes lexographically anyways
        map<string, int> classRouteTimes;

        // for each class student has, run dijkstra's on it from the student's residence to get shortest time
        for(const string &classCode : students[ufId].classes){

            classRouteTimes[classCode] = Dijkstra(students[ufId].locationId, classLocations[classCode]).first;

        }

        // print it all out
        cout << "Time For Shortest Edges: " << students[ufId].name << endl;

        for(const auto &pair : classRouteTimes){
            cout << pair.first << ": " << pair.second << endl;
        }


    } else if (command.substr(0, 17) == "printStudentZone "){

        string rest = command.substr(17);
        string extra;
        istringstream ss(rest);

        // int for UF ID
        int ufId;

        // grabs UF ID and if there's nothing after command, return false
        if(!(ss >> ufId)){
            return false;
        }

        // if there's extra jumble or UF ID does not exist, return false
        if(ss >> extra || students.count(ufId) == 0){
            return false;
        }

        // create a set for the subgraph of nodes for the students classes
        set<int> subGraphNodes;

        // loop through all the students classes
        for(string classCode : students[ufId].classes){

            // loop through all the path nodes between the students residence and their class and add it to the subgraph set
            for(int pathNode : Dijkstra(students[ufId].locationId, classLocations[classCode]).second){

                subGraphNodes.insert(pathNode);

            }

        }

        // print out the student zone cost using Prim's algorithm
        cout << "Student Zone Cost For " << students[ufId].name << ": " << Prim(subGraphNodes, students[ufId].locationId) << endl;

    } else if (command.substr(0, 15) == "verifySchedule ") {



    } else {
        // anything else unrecognized should be unsuccessful as the syntax is strict
        return false;
    }

    return true;
}