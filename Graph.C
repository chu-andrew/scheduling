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
    professor.RemoveInvalidDesires(Students, Professors.size());
  }
  for (Person& student : Students) {
    student.RemoveInvalidDesires(Professors);
  }
}

void Graph::CreateEdges() {
  for (Person& prof : Professors) {
    for (int studentId : prof.Desired) {
      Edges.push_back(pair(prof.Id, studentId));
    }
  }
  Uncolored = Edges;
}

void Graph::MakeScheduleGreedily() {
  shuffle(begin(Uncolored), end(Uncolored), RNG);

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

double Graph::Score() {
// Score the graph: what proportion of desires have been fulfilled?
  int fulfilled = 0;
  int desired = 0;

  for (Person& student : Students) {
    fulfilled += student.MeetPersonAndTime.size();
    desired += student.Desired.size();
  }
  return fulfilled / (double) desired;
}

bool Graph::AttemptClimb(double currentScore) {
// Attempt a hill climb by removing one edge, greedy filling with the uncolored edges, and checking if the score has improved
  // choose an edge to uncolor
  int randEdge = rand() % Colored.size();
  Person& prof = Professors[Colored[randEdge].first];
  Person& student = Students[Colored[randEdge].second - Professors.size()];

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
  Colored.erase(Colored.begin() + randEdge);

  // attempt greedy fill and score
  MakeScheduleGreedily();
  double attemptScore = Score();

  if (attemptScore > currentScore) cout << "delta(score): +" << attemptScore - currentScore <<  endl;
  return attemptScore > currentScore;
}

void Graph::HillClimb() {
// Try to increase the number of pairs in MeetPersonAndTime
  vector<Person> profCopy = Professors;
  vector<Person> studCopy = Students;
  vector<pair<int, int>> colorCopy = Colored;
  vector<pair<int, int>> uncolorCopy = Uncolored;

  cout << "Score before hill climb: " << Score() << endl;

  for (int i = 0; i < 100000; i++) {
    double currentScore = Score();
    if (currentScore == 1) break;

    bool climbed = AttemptClimb(currentScore);
    if (climbed) {
      // update copies
      profCopy = Professors;
      studCopy = Students;
      colorCopy = Colored;
      uncolorCopy = Uncolored;
    } else {
      // revert
      Professors = profCopy;
      Students = studCopy;
      Colored = colorCopy;
      Uncolored = uncolorCopy;
    }
  }
  
  /*
  cout << "Uncolored edges:" << endl;
  for(pair<int, int> edge : Uncolored) {
    cout << edge.first << " " << edge.second << endl;
  }
  */
  cout << "Score after hill climb: " << Score() << endl;
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
