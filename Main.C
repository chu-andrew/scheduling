#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


Graph Climb(const Graph baseG, default_random_engine& rng, bool debug) {
  Graph bestG = baseG;
  if (baseG.Uncolored.size() == 0) return baseG; // check if any meetings are possible at all
  double bestScore = -1;

  int populationMultiplier = baseG.Professors.size() * baseG.Students.size() / 2;

  for (int i = 0; i < populationMultiplier; i++) {
    if (debug) cout << "restart #" << i << "/" << populationMultiplier << endl;
    Graph hillClimbG = baseG;
    hillClimbG.InitialGreedyFill(rng);
    if (hillClimbG.Colored.size() == 0) continue; // disregard complete failure scenarios of initial greedy fill

    for (int j = 0 ; j < populationMultiplier; j++) {
      Graph attemptG = hillClimbG;
      double climbScore = attemptG.AttemptClimb(rng);

      if (climbScore > 0) {
        hillClimbG = attemptG;
        if (debug)  cout << " climb: #" << j << " score: \t" << climbScore << endl;
      }
      if (climbScore == 1) {
        if (debug)  cout << "100\% fulfilled [restart #" << i << "]: " << climbScore << endl;
        return hillClimbG;
      }
    }

    double score = hillClimbG.Score();
    if (score > bestScore) {
      bestScore = score;
      bestG = hillClimbG;
      if (debug) cout << "new best [restart #" << i << "]:\t*" << score << endl;
    }
  }
  return bestG;
}

int main() {

  srand(time(0));
  random_device rd = random_device {};
  default_random_engine RNG = default_random_engine {rd()};
  // default_random_engine RNG = default_random_engine {};

  Generator gen (
    10, 10, // numProfs, numStudents
    0.5, 6, // availabilityProportion, numDesires
    0, 10 // timeMin, timeMax
  );

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  cout << "\nInitial population" << G;
  G.Prune();
  G.CreateEdges();
  cout << "\nBase graph" << G;

  G = Climb(G, RNG, false);
  cout << "\nGraph after hill climb:" << G;
}
