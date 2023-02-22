#ifndef _FILEIO_H
#define _FILEIO_H

#include "Graph.H"

using namespace std;

class FileIO {

public:
  void WritePopulation(const Generator gen);
  void ParsePopulation();

  void WriteSchedule(const Graph& G, string delimiter);
  vector<vector<int>> GenerateScheduleVector(const Graph& G);

  void WriteAvailabilities(const Graph& G, string delimiter);
  void WriteAvailabilitiesForGroup(const vector<Person>& people, ofstream& fSchedule, 
    pair<int, int> timeRange, string affiliation, string delimiter);
  vector<vector<bool>> GenerateAvailabilityVector(const vector<Person>& people, pair<int, int> timeRange);

private:

};

#endif
