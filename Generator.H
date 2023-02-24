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
    int numTimes, // number of available meeting times
    int numDesires, // number of desired meetings
    int timeMin, int timeMax // earliest/latest available times among all people (inclusive bounds)
  );
  
  void WritePopulation(string fname, char delimiter) const;

  vector<Person> professors, students;

private:
  vector<Person> generatePeople(bool isProfessor, int numPeople);
  vector<int> randomUniqueNums(int n, int min, int max);
  int timeMin, timeMax;
  int numTimes, numDesires;
  int IdRanges[3]; // holds ranges of Id numbers. All professor Ids are followed directly by student Ids.
};

#endif
