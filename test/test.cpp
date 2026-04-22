#include <catch2/catch_test_macros.hpp>
#include <iostream>

// change if you choose to use a different header name
#include "CampusCompass.h"

using namespace std;

TEST_CASE("Example Test Name - Change me!", "[tag]") {
  


}

TEST_CASE("Test 2", "[tag]") {



}

TEST_CASE("Example CampusCompass Output Test", "[flag]") {
  
  CampusCompass compass;
  compass.parseCSV("data/edges.csv", "data/classes.csv");

  string input = R"(6
insert "Student A" 10000001 1 1 COP3502
insert "Student B" 10000002 1 1 COP3502
insert "Student C" 10000003 1 2 COP3502 MAC2311
dropClass 10000001 COP3502
remove 10000001
removeClass COP3502
)";

  compass.parseInput(input);

  string expectedOutput = R"(successful
successful
successful
successful
unsuccessful
2
)";

  string actualOutput = compass.getStringRepresentation();

  REQUIRE(actualOutput == expectedOutput);
}
