#ifndef _GRAPH_H
#define _GRAPH_H
#include "Person.H"
#include "Generator.H"

#include <vector>
#include <random>
#include <fstream>

using namespace std;

class Graph {

public:
  void Initialize(string fName); // create graph data that will be basis for all future iterations; parses file and prunes graph

  void InitialGreedyFill(default_random_engine& rng); // First greedy fill after random restart from base graph
  void ClimbGreedyFill(pair<int, int> removedEdge, default_random_engine& rng); // Iterative hill climbs' greedy fill on remaining Unconnected edges after removing one edge.

  double AttemptClimb(double currentScore, default_random_engine& rng);
  double Score() const;

  // write availabilities and schedule (meetings/desires) to file
  void WriteGraphState(string fname, char delimiter) const;

  vector<Person> Professors;
  vector<Person> Students;

  vector<pair<int, int>> Connected; // mutually desired meetings are currently fulfilled (profId, studId)
  vector<pair<int, int>> Unconnected; // mutually desired meetings that are not currently fulfilled (profId, studId)

private:
  // initialize
  void ReadInData(string fname); // Load professor/student vectors
  void ParseGroup(vector<Person>& group, int groupSize, ifstream& fPopulation);
  void Prune(); // Delete people in Desired who do not mutually desire you or do not have overlapping availabilities.
  void CreateEdges(); // fill Edges vector after non-mutual meetings have been pruned
  
  // greedy fill helper functions
  void MakeScheduleGreedily(); // Random get pairs (prof,student) that mutually want to meet until stuck.
  pair<int, int> RemoveRandomEdge(); // Remove a random connected edge in preparation for hill climb

  // file writing
  void WriteAvailabilities(ofstream& graphOut, char delimiter) const;
  void WriteAvailabilitiesForGroup(const vector<Person>& group, ofstream& graphOut,
                                   pair<int, int> timeRange, char affiliation, char delimiter) const;
  vector<vector<bool>> GenerateAvailabilityVector(const vector<Person>& group, pair<int, int> timeRange) const;

  void WriteSchedule(ofstream& graphOut, char delimiter) const;
  vector<vector<int>> GenerateScheduleVector() const;
};

// core functions for running the random restart + hill climb algorithms on Graph
Graph RandomRestart(const Graph baseG, default_random_engine& rng, bool debug);
void Climb(const int i, Graph& hillClimbG, Graph& bestG,
           double& bestScore, int populationMultiplier, default_random_engine& rng, bool debug);

ostream& operator<<(ostream& os, const Graph&);

#endif
