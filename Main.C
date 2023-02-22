#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


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
    0, 9 // timeMin, timeMax
  );

  Graph G;

  G.ReadInData(gen); // Fill up the Professors and Students vectors.
  G.Initialize();
  G.WriteGraphState("initial_population.tsv", "\t");

  G = RandomRestart(G, RNG, true); // debug=true displays hill climb progress for each restart

  G.WriteGraphState("schedule.tsv", "\t");
}
