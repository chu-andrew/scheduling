#include "Person.H"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


void Person::RemoveInvalidDesires(vector<Person>& B, int numProfs) {
// Remove all profs and students from the Desired vector who do not mutually want each other or do not have compatible schedules.
// That is, if Prof A wants to meet Student B but not conversely, then remove B's Id from A's Desired vector.

// 'B/b' in variable names refers to the other party
  for (int i = Desired.size() - 1; i >= 0; i--) {
    // find the desired person and their desires
    int desiredBId = Desired[i];

    Person desiredB; // find other Person, taking advantage of Id allocation ranges
    if (professor) desiredB = B[desiredBId - numProfs];
    else desiredB = B[desiredBId];

    // check for mutual desires
    bool mutual = false;
    for (int desire : desiredB.Desired) {
      if (desire == Id) {
        mutual = true;
        break;
      }
    }

    // check if availabilities are compatible
    bool scheduleCompatible = CrossCheckSchedule(desiredB) >= 0;

    // update desires
    if (!mutual || !scheduleCompatible) Desired.erase(Desired.begin() + i);
  }
}

int Person::CrossCheckSchedule(Person& b) {
// Compare two people's availabilities and return when they are both available
  for (int aTime : Hours) {
    if (HourUsed(aTime)) continue;
    for (int bTime : b.Hours) {
      if (b.HourUsed(bTime)) continue;
      else if (aTime == bTime) return aTime;
    }
  }
  return -1;
}

bool Person::HourUsed(int hour) {
  for (pair<int, int> meeting : MeetPersonAndTime) {
    if (meeting.second == hour) return true;
  }
  return false;
}

// Use this for debugging to print out a prof or student.
ostream& operator<<(ostream& os, const Person& x)
{
  os << (x.professor ? "Professor " : "Student ");
  os << "#" << x.Id << ": " << endl;
  os << "  Hours: ";
  for (int i = 0; i < x.Hours.size(); i++) os << x.Hours[i] << " ";
  os << endl;

  os << "  Desired: ";
  for (int i = 0; i < x.Desired.size(); i++) os << x.Desired[i] << " ";
  os << endl;

  return os;
}

// for writing to file
ofstream& operator<<(ofstream& fout, const Person& x) {
  // to be implemented

  fout << x.Id << "\n";
  fout << "WANT: ";
  for (int i = 0; i < x.Hours.size(); i++) fout << x.Hours[i] << " ";
  fout << "\n";

  fout << "TIME: ";
  for (int i = 0; i < x.Desired.size(); i++) fout << x.Desired[i] << " ";
  fout << endl;

  return fout;
}

