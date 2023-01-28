#ifndef _GENERATOR_H
#define _GENERATOR_H
#include "Person.H"
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Generator {

public:
  Generator (int numProfs, int numStuds, int timeMin, int timeMax);

  vector<Person> professors, students;
  int tMin, tMax;
  int IdRanges[3];
;
private:
  vector<Person> generatePeople(bool professor, int numPeople);
  vector<int> randomHours(int numTimes);
  vector<int> randomIds(int numIds, int minIdRange, int maxIdRange);
  vector<int> randomUniqueNums(int n, int min, int max);
};

ostream& operator<<(ostream& os, const Generator&);


#endif


