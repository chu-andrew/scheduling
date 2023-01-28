#include "Graph.H"
#include "Person.H"
#include "Generator.H"
#include <iostream>
#include <vector>

using namespace std;


void Graph::ReadInData() {
// Read in all the info about profs and students and create the vectors containing them.
  Generator g (3, 3, 0, 10);

  Professors = g.professors;
  Students = g.students;
}

void Graph::MakeScheduleGreedily() {
// Fill in pairs in MeetPersonAndTime greedily until get stuck.
}

void Graph::HillClimb() {
// Try to increase the number of pairs in MeetPersonAndTime
}


// Use this for debugging to print out all profs and students in the graph.
ostream& operator<<(ostream& os, const Graph& x)
{
  os << endl;
  os << " Professors:" << endl;
  for(int i=0; i< x.Professors.size(); i++) os << x.Professors[i];
  os << endl;

  os << " Students:" << endl;
  for(int i=0; i< x.Students.size(); i++) os << x.Students[i];
  os << endl;

  os << " Meetings:" << endl;
  for(int i=0; i< x.Students.size(); i++) {
    int StudentId = x.Students[i].Id;
    os << " Student " << StudentId << " meetings with: " << endl;
    for(int j=0; j< x.Students[i].MeetPersonAndTime.size(); j++) {
      int ProfToBeMetID  = x.Students[i].MeetPersonAndTime[j].first;
      int ProfTimeToMeet = x.Students[i].MeetPersonAndTime[j].second;
      os << "    Professor " << ProfToBeMetID << "  at time " << ProfTimeToMeet << endl;
    }
  }
  os << endl;
  
  return os;
}




