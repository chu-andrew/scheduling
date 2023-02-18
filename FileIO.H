#ifndef _FILEIO_H
#define _FILEIO_H

#include "Graph.H"

using namespace std;

class FileIO {

public:
  void WriteToCSV(const Graph& G);
  void WriteSchedule(const Graph& G);
  vector<vector<int>> GenerateScheduleVector(const Graph& G);

private:
};

#endif
