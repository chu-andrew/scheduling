#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


Graph Climb(const Graph baseG, default_random_engine& rng, bool debug) {
  Graph bestG = baseG;
  double bestScore = -1;
  if (baseG.Unconnected.size() == 0) return baseG; // check if any meetings are possible at all


  // random restart loop
  if (debug) cout << "restart#\tclimb#\tscore" << endl;
  int populationMultiplier = baseG.Professors.size() * baseG.Students.size();

  for (int i = 0; i < populationMultiplier / 4; i++) {
    Graph hillClimbG = baseG;
    hillClimbG.InitialGreedyFill(rng);
    if (hillClimbG.Connected.size() == 0) continue; // disregard complete failure scenarios of initial greedy fill

    // hill climb loop
    for (int j = 0 ; j < populationMultiplier; j++) {
      // create a new attempt object so that changes can be made without affecting the hillClimbG object
      Graph attemptG = hillClimbG;
      double climbScore = attemptG.AttemptClimb(rng);

      if (climbScore > 0) { // evaluate climb attempt
        if (debug) cout << i << "\t\t" << j << "\t" << climbScore << endl;

        hillClimbG = attemptG;
        if (climbScore > bestScore) j = 0; // reward hill climbs that have increased the score with more attempts
        if (climbScore == 1) break;
      }
    }

    double score = hillClimbG.Score();
    if (score > bestScore) {
      cout << "new best [restart #" << i << "]:\t" << score << "*" << endl;

      bestScore = score;
      bestG = hillClimbG;
      if (bestScore == 1) break;
    }
  }
  cout << "BEST: " << bestScore << endl;
  return bestG;
}

int main() {
  srand(time(0));
  random_device rd = random_device {};
  default_random_engine RNG = default_random_engine {rd()};
  // default_random_engine RNG = default_random_engine {}; // unseeded

  Generator gen (
    10, 10, // numProfs, numStudents
    5, 5, // numTimes, numDesires
    0, 10 // timeMin, timeMax
  );

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  // cout << "\nInitial population" << G;
  G.Initialize();
  // cout << "\nBase graph" << G;

  G = Climb(G, RNG, false); // debug=true displays hill climb progress for each restart
  cout << "\nGraph after hill climb:" << G;
}
