#include "Graph.H"
#include "Generator.H"
#include "FileIO.H"

#include <iostream>
#include <time.h>

using namespace std;


void Climb(const int i, Graph& hillClimbG, Graph& bestG, double& bestScore, int populationMultiplier, default_random_engine& rng, bool debug) {
// hill climb loop
  double currentScore = 0;
  for (int j = 0 ; j < populationMultiplier && currentScore < 1; j++) {
    // create a new attempt object so that changes can be made without affecting the hillClimbG object
    Graph attemptG = hillClimbG; // return attempt graph to the best graph out of this restart (discard non-score-increasing changes)
    double climbScore = attemptG.AttemptClimb(currentScore, rng);

    if (climbScore > currentScore) { // evaluate climb attempt
      if (debug) cout << i << "\t\t" << j << "\t" << climbScore << "\t" << climbScore - currentScore << endl;

      // update graphs that have climbed
      currentScore = climbScore;
      hillClimbG = attemptG;

      // update graphs that have climbed and are better than graphs from all restarts
      if (climbScore > bestScore) {
        cout << "new best [restart #" << i << "]:\t" << climbScore << "*\t" << "delta(best): " << climbScore - bestScore << endl;

        bestScore = currentScore;
        bestG = hillClimbG;

        // reward hill climbs that have increased the score with more attempts
        j = 0; // causes of decreases in hill climb# in output
      }
    }
  }
}

Graph RandomRestart(const Graph baseG, default_random_engine& rng, bool debug) {
  Graph bestG = baseG;
  double bestScore = 0;
  if (baseG.Unconnected.size() == 0) return baseG; // check if any meetings are possible at all

  if (debug) cout << "restart#\tclimb#\tscore\tdelta" << endl;
  int populationMultiplier = baseG.Professors.size() * baseG.Students.size();

  // random restart loop
  for (int i = 0; i < populationMultiplier / 4 && bestScore < 1; i++) {
    Graph hillClimbG = baseG;
    hillClimbG.InitialGreedyFill(rng);
    if (hillClimbG.Connected.size() == 0) continue; // disregard complete failure of initial greedy fill

    Climb(i, hillClimbG, bestG, bestScore, populationMultiplier, rng, debug);
  }
  cout << "BEST: " << bestScore << endl;
  return bestG;
}


int main() {
  const bool SEED = false;

  default_random_engine RNG;
  if (SEED) {
    srand(time(0));
    random_device rd = random_device {};
    RNG = default_random_engine {rd()};
  } else {
    RNG = default_random_engine {};
  }

  Generator gen (
    10, 10, // numProfs, numStudents
    5, 6, // numTimes, numDesires
    1, 10 // timeMin, timeMax
  );

  Graph G;
  FileIO F;

  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  // cout << "\nInitial population" << G;
  G.Initialize();
  // cout << "\nBase graph" << G;

  G = RandomRestart(G, RNG, true); // debug=true displays hill climb progress for each restart
  cout << "\nGraph after hill climb:" << G;

  F.WriteAvailabilities(G, "\t");
  F.WriteSchedule(G, "\t");
}
