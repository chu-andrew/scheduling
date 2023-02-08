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
// Remove non-mutual and impossible-to-fulfill meeting desires
  for (Person& professor : Professors) {
    professor.RemoveInvalidDesires(Students, Professors.size());
  }
  for (Person& student : Students) {
    student.RemoveInvalidDesires(Professors);
  }
}

void Graph::CreateEdges() {
  for (Person& prof : Professors) {
    for (int studentId : prof.Desired) {
      Uncolored.push_back(pair(prof.Id, studentId));
    }
  }
}

void Graph::MakeScheduleGreedily() {
  for (int i = Uncolored.size() - 1; i >= 0; i--) {
    pair<int, int> compare = Uncolored[i];
    Person& prof = Professors[compare.first];
    Person& student = Students[compare.second - Professors.size()];

    int commonTime = prof.CrossCheckSchedule(student);

    if (commonTime >= 0) {
      prof.MeetPersonAndTime.push_back(pair(student.Id, commonTime));
      student.MeetPersonAndTime.push_back(pair(prof.Id, commonTime));

      Uncolored.erase(Uncolored.begin() + i);
      Colored.push_back(compare);
    }
  }
}

void Graph::InitialGreedyFill(default_random_engine& rng) {
  shuffle(begin(Uncolored), end(Uncolored), rng);
  MakeScheduleGreedily();
}

void Graph::ClimbGreedyFill(pair<int, int> removedEdge, default_random_engine& rng) {
  shuffle(begin(Uncolored), end(Uncolored), rng);
  Uncolored.insert(Uncolored.begin(), removedEdge); // insert the removed colored edge at beginning because MakeScheduleGreedily() evaluates from last to first index
  MakeScheduleGreedily();
}

double Graph::AttemptClimb(default_random_engine& rng) {
// Attempt a hill climb by removing one edge, greedy filling with the uncolored edges, and checking if the score has improved

  double currentScore = Score();

  // choose an edge to uncolor
  int randInt = rand() % Colored.size();
  pair<int, int> removedEdge = Colored[randInt];
  Person& prof = Professors[removedEdge.first];
  Person& student = Students[removedEdge.second - Professors.size()];

  // update Professor/Student objects and colored vector with uncolored edge
  for (int i = prof.MeetPersonAndTime.size() - 1; i >= 0 ; i--) {
    if (prof.MeetPersonAndTime[i].first == student.Id) {
      prof.MeetPersonAndTime.erase(prof.MeetPersonAndTime.begin() + i);
    }
  }
  for (int i = student.MeetPersonAndTime.size() - 1; i >= 0 ; i--) {
    if (student.MeetPersonAndTime[i].first == prof.Id) {
      student.MeetPersonAndTime.erase(student.MeetPersonAndTime.begin() + i);
    }
  }
  Colored.erase(Colored.begin() + randInt);

  // attempt greedy fill with remaining uncolored and score
  ClimbGreedyFill(removedEdge, rng);
  double attemptScore = Score();
  
  if (attemptScore > currentScore) return attemptScore;
  else return -1;
}

double Graph::Score() const {
// Score the graph: what proportion of desires have been fulfilled?
  int fulfilled = 0;
  int desired = 0;

  for (Person student : Students) {
    fulfilled += student.MeetPersonAndTime.size();
    desired += student.Desired.size();
  }
  // cout << fulfilled << " " << desired << endl;
  return fulfilled / (double) desired;
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

  os << "Unfulfilled Meetings:" << endl;
  for(pair<int, int> edge : x.Uncolored) {
    cout << " P" << edge.first << " S" << edge.second << endl;
  }
  os << endl;

  os << "Score: " << x.Score() << endl;

  return os;
}
