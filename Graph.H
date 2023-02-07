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
  void Prune(); // Delete people in Desired who do not mutually desire you or do not have overlapping availabilities.
  void CreateEdges(); // fill Edges vector after non-mutual meetings have been pruned
  
  void ShuffleUncolored(default_random_engine& rng);
  void MakeScheduleGreedily(); // Random get pairs (prof,student) that mutually want to meet until stuck.

  bool AttemptClimb(default_random_engine& rng);
  double Score();

  vector<Person> Professors;
  vector<Person> Students;

  vector<pair<int, int>> Colored; // mutually desired meetings are currently fulfilled
  vector<pair<int, int>> Uncolored; // mutually desired meetings that are not currently fulfilled

private:

};

ostream& operator<<(ostream& os, const Graph&);

#endif
