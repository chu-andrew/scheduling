#include "Graph.H"
#include "Person.H"
#include "Generator.H"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;


void Graph::ReadInData(Generator gen) {
// Read in all the info about profs and students and create the vectors containing them.
  Professors = gen.professors;
  Students = gen.students;
}

void Graph::Prune() {
// Remove non-mutual meeting desires
  for (Person& professor : Professors) {
    professor.RemoveUnreciprocatedDesires(Students, Professors.size());
  }
  for (Person& student : Students) {
    student.RemoveUnreciprocatedDesires(Professors);
  }
}

void Graph::FillComparisonOrder() {
  for (Person& prof : Professors) {
    for (int studentId : prof.Desired) {
      comparisonOrder.push_back(pair(prof.Id, studentId));
    }
  }
}

void Graph::MakeScheduleGreedily() {
// Fill in pairs in MeetPersonAndTime greedily until get stuck.
  for (Person& prof : Professors) {
    for (int studentId : prof.Desired) {
      Person* student = &Students[studentId - Professors.size()];

      int commonTime = CrossCheckSchedules(prof.Hours, student->Hours, prof.hoursUsed, student->hoursUsed);
      if (commonTime >= 0) {
        prof.MeetPersonAndTime.push_back(pair(student->Id, commonTime));
        student->MeetPersonAndTime.push_back(pair(prof.Id, commonTime));
      }
    }
  }
}

void Graph::MakeScheduleGreedilyRandomly() {
  auto rng = default_random_engine {};
  shuffle(begin(comparisonOrder), end(comparisonOrder), rng);

  for(pair<int, int> compare : comparisonOrder) {
    cout << compare.first << " " << compare.second << endl;

    Person& prof = Professors[compare.first];
    Person& student = Students[compare.second - Professors.size()];

    int commonTime = CrossCheckSchedules(prof.Hours, student.Hours, prof.hoursUsed, student.hoursUsed);
    if (commonTime >= 0) {
      prof.MeetPersonAndTime.push_back(pair(student.Id, commonTime));
      student.MeetPersonAndTime.push_back(pair(prof.Id, commonTime));
    }
  }
}

int Graph::CrossCheckSchedules(
  vector<int>& aTimes, vector<int>& bTimes, vector<bool>& aUsed, vector<bool>& bUsed) {
  for (int i = 0; i < aTimes.size(); i++) {
    if (aUsed[i]) continue;
    for (int j = 0; j < bTimes.size(); j++) {
      if (bUsed[j]) continue;
      else if (aTimes[i] == bTimes[j]) {
        aUsed[i] = true;
        bUsed[j] = true;
        return aTimes[i];
      }
    }
  }
  return -1;
}

double Graph::Score() {
  int fulfilled = 0;
  int desired = 0;

  for (Person& student : Students) {
    fulfilled += student.MeetPersonAndTime.size();
    desired += student.Desired.size();
  }
  // cout << "total: " << fulfilled << "/" << desired << endl;
  return fulfilled / (double) desired;
}

void Graph::HillClimb() {
// Try to increase the number of pairs in MeetPersonAndTime
  cout << Score() << endl;
}

// Use this for debugging to print out all profs and students in the graph.
ostream& operator<<(ostream& os, const Graph& x)
{
  os << endl;
  os << " Professors:" << endl;
  for (int i = 0; i < x.Professors.size(); i++) os << x.Professors[i];
  os << endl;

  os << " Students:" << endl;
  for (int i = 0; i < x.Students.size(); i++) os << x.Students[i];
  os << endl;

  os << " Meetings:" << endl;
  for (int i = 0; i < x.Students.size(); i++) {
    int StudentId = x.Students[i].Id;
    os << " Student " << StudentId << " meetings with: " << endl;
    for (int j = 0; j < x.Students[i].MeetPersonAndTime.size(); j++) {
      int ProfToBeMetID  = x.Students[i].MeetPersonAndTime[j].first;
      int ProfTimeToMeet = x.Students[i].MeetPersonAndTime[j].second;
      os << "    Professor " << ProfToBeMetID << " at time " << ProfTimeToMeet << endl;
    }
  }
  os << endl;

  return os;
}
