#include <catch2/catch_test_macros.hpp>
#include <iostream>

// change if you choose to use a different header name
#include "CampusCompass.h"

using namespace std;

TEST_CASE("Incorrect commands", "[incorrect]") {
  string input = R"(5
insert "A11y" 45679999 1 1 COP3530
insert "Javi" 123 1 1 COP3530
insert "Evil Javi" 12345678 1 7 COP3530 MAC2311 CDA3101 EEL3701 COP3502 MAC2312 CGS2531
insrt "Javi Two" 12345678 1 1 COP3530
remove notID
)";

  string expectedOutput = R"(unsuccessful
unsuccessful
unsuccessful
unsuccessful
unsuccessful
)";

  CampusCompass compass;
  compass.parseCSV("data/edges.csv", "data/classes.csv");
  compass.parseInput(input);

  REQUIRE(compass.getStringRepresentation() == expectedOutput);

}

TEST_CASE("Edge cases", "[edge]") {

  string input = R"(7
remove 99999999
insert "Javi" 11111111 1 1 COP3530
dropClass 11111111 COP3530
remove 11111111
checkEdgeStatus 999 998
isConnected 1 1
isConnected 1 500
)";

  string expectedOutput = R"(unsuccessful
successful
successful
unsuccessful
DNE
successful
unsuccessful
)";

  CampusCompass compass;
  compass.parseCSV("data/edges.csv", "data/classes.csv");
  compass.parseInput(input);

  REQUIRE(compass.getStringRepresentation() == expectedOutput);

}

TEST_CASE("Student Class/Removal Commands", "[student]"){

  string input = R"(10
insert "Javi" 11111111 1 2 COP3530 MAC2311
insert "Javi Two" 22222222 2 2 COP3530 CDA3101
insert "Evil Javi" 33333333 3 2 COP3530 EEL3701
dropClass 11111111 COP3530
dropClass 22222222 PHY2048
replaceClass 11111111 MAC2311 CDA3101
replaceClass 11111111 MAC2311 EEL3701
removeClass COP3530
remove 11111111
remove 11111111
)";

  string expectedOutput = R"(successful
successful
successful
successful
unsuccessful
successful
unsuccessful
2
successful
unsuccessful
)";

  CampusCompass compass;
  compass.parseCSV("data/edges.csv", "data/classes.csv");
  compass.parseInput(input);

  REQUIRE(compass.getStringRepresentation() == expectedOutput);

}

TEST_CASE("printShortestEdges before and after edge closure", "[shortest]"){

  string input = R"(4
insert "Javi" 11111111 20 1 IDS2935
printShortestEdges 11111111
toggleEdgesClosure 4 8 7 5 7 5 6 43 7
printShortestEdges 11111111
)";

  string expectedOutput = R"(successful
Time For Shortest Edges: Javi
IDS2935: 7
successful
Time For Shortest Edges: Javi
IDS2935: -1
)";

  CampusCompass compass;
  compass.parseCSV("data/edges.csv", "data/classes.csv");
  compass.parseInput(input);

  REQUIRE(compass.getStringRepresentation() == expectedOutput);

}