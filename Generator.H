#ifndef _GENERATOR_H
#define _GENERATOR_H
#include "Person.H"
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Generator {

public:
  Generator (
    int numProfs, int numStudents, 
    double availabilityProportion, int numDesires,
    int timeMin, int timeMax
  );

  vector<Person> professors, students;

private:
  vector<Person> generatePeople(bool professor, int numPeople);
  vector<int> randomUniqueNums(int n, int min, int max);
  int timeMin, timeMax;
  int numTimes, numDesires;
  int IdRanges[3];
};

ostream& operator<<(ostream& os, const Generator&);


#endif


