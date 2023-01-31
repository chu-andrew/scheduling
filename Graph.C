#include "Graph.H"
#include "Person.H"
#include "Generator.H"

#include <iostream>
#include <vector>

using namespace std;


void Graph::ReadInData(Generator gen) {
// Read in all the info about profs and students and create the vectors containing them.
  Professors = gen.professors;
  Students = gen.students;
}

void Graph::Prune() {
// Remove non-mutual meeting desires
  for(int i=0; i<Professors.size(); i++) {
    Professors[i].RemoveUnreciprocatedDesires(Students, Professors.size());
  }
  for(int i=0; i<Students.size(); i++) {
    Students[i].RemoveUnreciprocatedDesires(Professors);
  }
}

void Graph::MakeScheduleGreedily() {
// Fill in pairs in MeetPersonAndTime greedily until get stuck.
  for(int i=0; i<Professors.size(); i++) {
    Person* prof = &Professors[i];
    for(int j=0; j<prof->Desired.size(); j++) {
      Person* student = &Students[prof->Desired[j] - Professors.size()];

      int commonTime = CrossCheckSchedules(prof->Hours, student->Hours, prof->hoursUsed, student->hoursUsed);
      // cout << prof->Id << " " << student->Id << " " << commonTime << endl;
      if(commonTime >= 0) {
        prof->MeetPersonAndTime.push_back(pair(student->Id, commonTime));
        student->MeetPersonAndTime.push_back(pair(prof->Id, commonTime));
      }
    }
  }
}

int Graph::CrossCheckSchedules(vector<int>& aTimes, vector<int>& bTimes, vector<bool>& aUsed, vector<bool>& bUsed) {
  for(int i=0; i<aTimes.size(); i++) {
    if(aUsed[i]) continue;
    for(int j=0; j<bTimes.size(); j++) {
      if(bUsed[j]) continue;
      else if(aTimes[i] == bTimes[j]) {
        aUsed[i] = true;
        bUsed[j] = true;
        return aTimes[i];
      }
    }
  }
  return -1;
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
      os << "    Professor " << ProfToBeMetID << " at time " << ProfTimeToMeet << endl;
    }
  }
  os << endl;
  
  return os;
}
