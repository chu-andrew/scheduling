#ifndef _GRAPH_H
#define _GRAPH_H
#include "Person.H"
#include "Generator.H"

#include <vector>
#include <random>

using namespace std;

class Graph {

public:
  void ReadInData(Generator gen); // Load professor/student vectors
  void Prune(); // Delete people in Desired who do not mutually desire you.

  void CreateEdges(); // fill Edges vector after non-mutual meetings have been pruned
  int CrossCheckSchedules(Person& a, Person& b);
  void MakeScheduleGreedily(); // Random get pairs (prof,student) that mutually want to meet until stuck.

  double Score();
  bool AttemptClimb(double currentScore);
  void HillClimb(); // Try to increase the number of pairs in MeetPersonAndTime

  vector<Person> Professors;
  vector<Person> Students;

  vector<pair<int, int>> Edges; // professor Id, student Id mutually desired meetings
  vector<pair<int, int>> Colored; // mutually desired meetings are currently fulfilled
  vector<pair<int, int>> Uncolored; // mutually desired meetings that are not currently fulfilled


private:
  random_device RD = random_device {}; 
  default_random_engine RNG = default_random_engine {RD()};
};

ostream& operator<<(ostream& os, const Graph&);

#endif
