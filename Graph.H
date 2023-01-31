#ifndef _GRAPH_H
#define _GRAPH_H
#include "Person.H"
#include "Generator.H"

#include <vector>

using namespace std;

class Graph {

public:
  void ReadInData(Generator gen); // Load professor/student vectors
  void Prune(); // Delete people in Desired who do not mutually desire you.
  int CrossCheckSchedules(vector<int>& aTimes, vector<int>& bTimes, vector<bool>& aUsed, vector<bool>& bUsed);
  void MakeScheduleGreedily(); // Random get pairs (prof,student) that mutually want to meet until stuck.
  void HillClimb(); // 

  vector<Person> Professors;
  vector<Person> Students;

private:
  
};

ostream& operator<<(ostream& os, const Graph&);


#endif
