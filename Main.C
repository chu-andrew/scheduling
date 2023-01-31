#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


int main() {

  srand(time(0));

  Generator gen (
    3, 3, // numProfs, numStudents
    0.25, 2, // availabilityProportion, numDesires
    0, 10 // timeMin, timeMax
  );

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  cout << "Initial population" << G;
  G.Prune();
  cout << "Pruned desires" << G;
  G.MakeScheduleGreedily();
  cout << "Graph after greedily filled:" << G;
  // G.HillClimb();
  // cout << "Graph after hill climb:" << G;
}
