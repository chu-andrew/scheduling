#ifndef _PERSON_H
#define _PERSON_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

class Person {

public:
  // Delete people in Desired who do not mutually desire you. numProfs is only used for checking students' desires.
  void RemoveUnreciprocatedDesires(vector<Person>& B, int numProfs = 0);

  bool professor;
  int Id;                  // Identification number
  vector<int> Hours;       // Available meeting hours
  vector<bool> hoursUsed;     // Indicate hours that are being occupied with meetings
  vector<int> Desired;     // Ids of People this person wants to meet
  vector<pair<int, int> > MeetPersonAndTime; // Pairs of <Person's Id, Meet time> of an actual scheduled meeting
  string Name;             // e.g. "John Doe"

private:
};

ostream& operator<<(ostream& os, const Person&);


#endif
