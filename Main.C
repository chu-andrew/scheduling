#include "Graph.H"
#include "Generator.H"

#include <iostream>

using namespace std;


int main() {
  Generator gen (5, 5, 0.50, 3, 0, 10);

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  cout << "Initial population" << G;
  G.Prune();
  cout << "Pruned desires" << G;
  // G.MakeScheduleGreedily();
  // cout << "Graph after greedily filled:" << G;
  // G.HillClimb();
  // cout << "Graph after hill climb:" << G;
}
