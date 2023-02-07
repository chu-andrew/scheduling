#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


Graph Climb(const Graph baseG, default_random_engine& rng) {
  Graph bestG = baseG;
  double bestScore = -1;

  int populationMultiplier = baseG.Professors.size() * baseG.Students.size();

  for (int i = 0; i < populationMultiplier * 10; i++) {
    Graph randomRestartG = baseG;
    randomRestartG.ShuffleUncolored(rng);
    randomRestartG.MakeScheduleGreedily();

    Graph hillClimbG = randomRestartG;

    for (int j = 0 ; j < populationMultiplier * 10; j++) {
      Graph attemptG = hillClimbG;
      bool climbed = attemptG.AttemptClimb(rng);

      if (climbed) {
        hillClimbG = attemptG;
        // cout << "restart: " << i << " climb: " << j << " " << hillClimbG.Score() << endl;
      }

      double climbScore = hillClimbG.Score();
      if (climbScore == 1) return hillClimbG;
    }

    randomRestartG = hillClimbG;

    double score = randomRestartG.Score();
    if (score > bestScore) {
      bestScore = score;
      bestG = randomRestartG;
      cout << "new best score [restart #" << i << "]: " << score << endl;

      if (score == 1) break;
    }
  }
  return bestG;
}

int main() {

  srand(time(0));
  random_device RD = random_device {};
  default_random_engine RNG = default_random_engine {RD()};

  Generator gen (
    5, 5, // numProfs, numStudents
    0.5, 4, // availabilityProportion, numDesires
    0, 10 // timeMin, timeMax
  );

  Graph G;
  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  // cout << "Initial population" << G;
  G.Prune();
  G.CreateEdges();
  // cout << "Base graph" << G;

  G = Climb(G, RNG);
  cout << "Graph after hill climb:" << G;
  cout << G.Score() << endl;
}
