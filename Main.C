#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


int main() {
  /*
  change consts as necessary:
    SEED affects generation of population (if being generated) and shuffling of vectors during hill climbs
    GENERATE_NEW_POP determines if a new population will be generated and written to file.
      if false, graph will be created from the previously generated file (or eventually, the real data file)
      should be turned on once, then can be turned off for subsequent reruns as needed
  */
  const bool SEED = false;
  const bool GENERATE_NEW_POP = true;

  const string DATA = "generated_population.csv";
  const char DELIM = '\t';

  default_random_engine RNG;
  if (SEED) {
    srand(time(0));
    random_device rd = random_device {};
    RNG = default_random_engine {rd()};
  } else {
    RNG = default_random_engine {};
  }

  if (GENERATE_NEW_POP) {
    const Generator GEN ( // change parameters as necessary
      10, 10, // numProfs, numStudents
      5, 6, // numTimes, numDesires
      0, 9 // timeMin, timeMax
    );
    GEN.WritePopulation(DATA, DELIM);
  } // else use the file that has been generated previously or the real data file

  Graph G;
  G.Initialize(DATA); // parse data from file, then prune impossible or non-mutual meetings
  G.WriteGraphState("initial_graph.csv", DELIM);

  G = RandomRestart(G, RNG, false); // debug=true displays hill climb progress for each restart

  G.WriteGraphState("schedule.csv", DELIM);
}
