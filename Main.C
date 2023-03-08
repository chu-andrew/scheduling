#include "Graph.H"
#include "Generator.H"

#include <iostream>
#include <time.h>

using namespace std;


int main() {
  // run configuration
  /*
  change consts as necessary:
    SEED affects generation of population (if being generated) and shuffling of vectors during hill climbs
    GENERATE_NEW_POP determines if a new population will be generated and written to file.
      if false, graph will be created from the previously generated file (or eventually, the real data file)
      should be turned on once, then can be turned off for subsequent reruns as needed
  */
  const bool SEED = false;
  const bool GENERATE_NEW_POP = true;

  const bool VERBOSE_COUT = false;
  const string DATA_IN = "generated_population.csv";
  const string INITIAL_GRAPH = "initial_graph.csv";
  const string LOG = "log.csv";
  const string SCHEDULE = "schedule.csv";
  const char DELIM = ',';

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
      50, 50, // numProfs, numStudents
      15, 30, // numTimes, numDesires
      0, 25 // timeMin, timeMax
    );
    GEN.WritePopulation(DATA_IN, DELIM);
  } // else use the file that has been generated previously or eventually the real data file

  Graph G;
  G.Initialize(DATA_IN, DELIM); // parse data from file, then prune impossible or non-mutual meetings
  G.WriteGraphState(INITIAL_GRAPH, DELIM); // initial_graph will not be the same as DATA_IN because meetings have been pruned

  G = RandomRestart(G, RNG, VERBOSE_COUT, LOG, SCHEDULE, DELIM);
}
