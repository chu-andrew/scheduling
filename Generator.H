#ifndef _GENERATOR_H
#define _GENERATOR_H
#include "Person.H"

#include <string>
#include <utility>
#include <vector>

using namespace std;

class Generator {

public:
  Generator (
    int numProfs, int numStudents,
    double availabilityProportion, // proportion of everybody's time range in which a person is available
    int numDesires, // number of desired meetings
    int timeMin, int timeMax // earliest/latest available times among all people
  );

  vector<Person> professors, students;

private:
  vector<Person> generatePeople(bool professor, int numPeople);
  vector<int> randomUniqueNums(int n, int min, int max);
  int timeMin, timeMax;
  int numTimes, numDesires;
  int IdRanges[3]; // holds ranges of Id numbers. All professor Ids are followed directly by student Ids.
};

#endif
