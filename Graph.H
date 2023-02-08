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
  void Initialize(); // create graph data that will be basis for all future iterations

  void InitialGreedyFill(default_random_engine& rng); // First greedy fill after random restart from base graph
  void ClimbGreedyFill(pair<int, int> removedEdge, default_random_engine& rng); // Iterative hill climbs' greedy fill on remaining Unconnected edges after removing one edge.

  double AttemptClimb(default_random_engine& rng);
  double Score() const;

  vector<Person> Professors;
  vector<Person> Students;

  vector<pair<int, int>> Connected; // mutually desired meetings are currently fulfilled
  vector<pair<int, int>> Unconnected; // mutually desired meetings that are not currently fulfilled

private:
  void Prune(); // Delete people in Desired who do not mutually desire you or do not have overlapping availabilities.
  void CreateEdges(); // fill Edges vector after non-mutual meetings have been pruned
  void MakeScheduleGreedily(); // Random get pairs (prof,student) that mutually want to meet until stuck.
  pair<int, int> RemoveRandomEdge(); // Remove a random connected edge in preparation for hill climb
};

ostream& operator<<(ostream& os, const Graph&);

#endif
