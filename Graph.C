#include "Graph.H"
#include "Person.H"
#include "Generator.H"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;


// INITIALIZATION
void Graph::ReadInData(Generator gen) {
// Read in all the info about profs and students and create the vectors containing them.
  Professors = gen.professors;
  Students = gen.students;
}

void Graph::Initialize() {
  Prune();
  CreateEdges();
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
      Unconnected.push_back(pair(prof.Id, studentId));
    }
  }
}

// GREEDY FILL
void Graph::InitialGreedyFill(default_random_engine& rng) {
// Used for first greedy fill after random restart from base graph
  shuffle(begin(Unconnected), end(Unconnected), rng);
  MakeScheduleGreedily();
}

void Graph::ClimbGreedyFill(pair<int, int> removedEdge, default_random_engine& rng) {
// Used for iterative hill climbs' greedy fills on the remaining Unconnected edges after removing one edge from Connected.
  shuffle(begin(Unconnected), end(Unconnected), rng);
  Unconnected.insert(Unconnected.begin(), removedEdge); // insert the removed Connected edge at beginning because MakeScheduleGreedily() evaluates from last to first index
  MakeScheduleGreedily();
}

void Graph::MakeScheduleGreedily() {
// Fill graph greedily with Unconnected edges
  for (int i = Unconnected.size() - 1; i >= 0; i--) {
    pair<int, int> compare = Unconnected[i];
    Person& prof = Professors[compare.first];
    Person& student = Students[compare.second - Professors.size()];

    int commonTime = prof.CrossCheckSchedule(student);

    if (commonTime >= 0) {
      prof.MeetPersonAndTime.push_back(pair(student.Id, commonTime));
      student.MeetPersonAndTime.push_back(pair(prof.Id, commonTime));

      Unconnected.erase(Unconnected.begin() + i);
      Connected.push_back(compare);
    }
  }
}

// HILL CLIMB
double Graph::AttemptClimb(default_random_engine& rng) {
// Attempt a hill climb by removing one edge, greedy filling with the Unconnected edges, and checking if the score has improved

  double currentScore = Score();
  if (currentScore == 1) return 1;

  pair<int, int> removedEdge = RemoveRandomEdge();

  // attempt greedy fill with remaining Unconnected and score
  ClimbGreedyFill(removedEdge, rng);
  double attemptScore = Score();

  if (attemptScore > currentScore) return attemptScore;
  else return -1; // disregard any non-improvements
}

pair<int, int> Graph::RemoveRandomEdge() {
// Remove a random Connected edge in preparation for hill climb

  // choose an edge to connect
  int randInt = rand() % Connected.size();
  pair<int, int> removedEdge = Connected[randInt];

  Person& prof = Professors[removedEdge.first];
  Person& student = Students[removedEdge.second - Professors.size()];

  // update Professor/Student objects and Connected vector with Unconnected edge
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
  Connected.erase(Connected.begin() + randInt);

  return removedEdge;
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
  for (pair<int, int> edge : x.Unconnected) {
    cout << " P" << edge.first << " S" << edge.second << endl;
  }
  os << endl;

  os << "Score: " << x.Score() << endl;

  return os;
}
