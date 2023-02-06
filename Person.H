#ifndef _PERSON_H
#define _PERSON_H

#include <vector>
#include <string>
#include <utility>

using namespace std;

class Person {

public:
  // Delete people in Desired that are not mutual or do not have compatible schedules. numProfs is only used for checking students' desires.
  void RemoveInvalidDesires(vector<Person>& B, int numProfs = 0);
  int CrossCheckSchedule(Person& b);
  bool HourUsed(int hour);

  bool professor;
  int Id;                  // Identification number
  string Name;             // e.g. "John Doe"

  vector<int> Hours;       // Available meeting hours
  vector<int> InitDesired; // Ids of People this person wants to meet; make a record of initial desires because desires will be pruned
  vector<int> Desired;     // Desires that are possible: mutual and compatible availabilities

  vector<pair<int, int> > MeetPersonAndTime; // Pairs of <Person's Id, Meet time> of an actual scheduled meeting

private:
};

ostream& operator<<(ostream& os, const Person&);

#endif
