#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


int main() {

  srand(time(0));

  Generator gen (
    4, 5, // numProfs, numStudents
    0.5, 4, // availabilityProportion, numDesires
    0, 10 // timeMin, timeMax
  );

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  // cout << "Initial population" << G;
  G.Prune();
  G.FillComparisonOrder();
  // cout << "Pruned desires" << G;
  G.MakeScheduleGreedilyRandomly();
  cout << "Graph after greedily filled:" << G;
  G.HillClimb();
  // cout << "Graph after hill climb:" << G;
}
