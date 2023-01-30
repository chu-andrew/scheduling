#include "Graph.H"
#include "Person.H"
#include "Generator.H"
#include <iostream>
#include <vector>

using namespace std;


void Graph::ReadInData() {
// Read in all the info about profs and students and create the vectors containing them.
  Generator gen (3, 3, 0.50, 2, 0, 10);

  Professors = gen.professors;
  Students = gen.students;
}

void Graph::Prune() {
  for(int i=0; i<Professors.size(); i++) {
    // retrieve professor and their desires
    Person* prof = &(Professors[i]);
    CrossCheck(prof->Id, prof->Desired);
  }
}

void Graph::CrossCheck(int profId, vector<int> &profDesires) {
  for(int j=profDesires.size() - 1; j>=0; j--) {
    // find the desired student and the student's desires
    int desiredStudId = profDesires[j];
    Person desiredStud = Students[desiredStudId - Professors.size()]; // take advantage of Id allocation ranges
    vector<int> studDesires = desiredStud.Desired;

    bool mutual = false;
    for(int i=0; i<studDesires.size(); i++) {
      if(studDesires[i] == profId) {
        mutual = true;
        break;
      }
    }
    if(!mutual) profDesires.erase(profDesires.begin() + j);
  }
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




