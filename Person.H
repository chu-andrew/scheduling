#ifndef _PERSON_H
#define _PERSON_H
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Person {

public:
  void RemoveUnreciprocatedDesires(); // Delete people in Desired who do not mutually desire you.

  int Id;                  // Identification number
  vector<int> Hours;       // Available meeting hours
  vector<int> Desired;     // Ids of People this person wants to meet
  vector<pair<int,int> > MeetPersonAndTime; // Pairs o <Person's Id, Meet time> of an actual scheduled meeting  
  string Name;             // e.g. "John Doe"

private:
};

ostream& operator<<(ostream& os, const Person& );


#endif



