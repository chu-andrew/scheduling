#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


Graph Climb(const Graph baseG, default_random_engine& rng, bool debug) {
  Graph bestG = baseG;
  double bestScore = -1;
  if (baseG.Uncolored.size() == 0) return baseG; // check if any meetings are possible at all

  if (debug) cout << "restart#\tclimb#\tscore" << endl;

  int populationMultiplier = baseG.Professors.size() * baseG.Students.size();
  for (int i = 0; i < populationMultiplier / 4; i++) { // random restart loop
    Graph hillClimbG = baseG;
    hillClimbG.InitialGreedyFill(rng);
    if (hillClimbG.Colored.size() == 0) continue; // disregard complete failure scenarios of initial greedy fill

    for (int j = 0 ; j < populationMultiplier; j++) { // hill climb loop
      // create an attempt object that can be changed
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
    10, 15, // numProfs, numStudents
    5, 6, // numTimes, numDesires
    0, 10 // timeMin, timeMax
  );

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  // cout << "\nInitial population" << G;
  G.Initialize();
  // cout << "\nBase graph" << G;

  G = Climb(G, RNG, true); // debug=true displays hill climb progress for each restart
  // cout << "\nGraph after hill climb:" << G;
}
