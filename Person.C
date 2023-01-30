#include "Person.H"
#include <iostream>
#include <vector>

using namespace std;


void Person::RemoveUnreciprocatedDesires() {
// Remove all profs and students from the Desired vector who do not mutually want each other.
// That is, if Prof A wants to meet Student B but not conversely, then remove B's Id from A's Desired vector.
}


// Use this for debugging to print out a prof or student.
ostream& operator<<(ostream& os, const Person& x)
{
  os << (x.professor ? "Professor " : "Student ");
  os << "#" << x.Id << ": " << endl;
  os << "  Hours: ";
  for(int i=0; i< x.Hours.size(); i++) os << x.Hours[i] << " ";
  os << endl;
  
  os << "  Desired: ";
  for(int i=0; i< x.Desired.size(); i++) os << x.Desired[i] << " ";
  os << endl;

  return os;
}




