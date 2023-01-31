#include "Person.H"

#include <iostream>
#include <vector>

using namespace std;


void Person::RemoveUnreciprocatedDesires(vector<Person>& B, int numProfs) {
// Remove all profs and students from the Desired vector who do not mutually want each other.
// That is, if Prof A wants to meet Student B but not conversely, then remove B's Id from A's Desired vector.

// 'B/b' in variable names refers to the other party
  for (int i = Desired.size() - 1; i >= 0; i--) {
    // find the desired person and their desires
    int desiredBId = Desired[i];

    Person desiredB; // find other party object, take advantage of Id allocation ranges
    if (professor) desiredB = B[desiredBId - numProfs];
    else desiredB = B[desiredBId];

    vector<int> bDesires = desiredB.Desired;

    // check for mutual desires
    bool mutual = false;
    for (int j = 0; j < bDesires.size(); j++) {
      if (bDesires[j] == Id) {
        mutual = true;
        break;
      }
    }
    if (!mutual) Desired.erase(Desired.begin() + i);
  }
}


// Use this for debugging to print out a prof or student.
ostream& operator<<(ostream& os, const Person& x)
{
  os << (x.professor ? "Professor " : "Student ");
  os << "#" << x.Id << ": " << endl;
  os << "  Hours: ";
  for (int i = 0; i < x.Hours.size(); i++) os << x.Hours[i] << " ";
  os << endl;

  os << "  Used Hours: ";
  for (int i = 0; i < x.hoursUsed.size(); i++) os << x.hoursUsed[i] << " ";
  os << endl;

  os << "  Desired: ";
  for (int i = 0; i < x.Desired.size(); i++) os << x.Desired[i] << " ";
  os << endl;

  return os;
}
