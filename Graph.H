#ifndef _GRAPH_H
#define _GRAPH_H
#include <vector>
#include "Person.H"

using namespace std;

class Graph {

public:
  void ReadInData(); // Delete people in Desired who do not mutually desire you.
  void Prune();
  void CrossCheck(int profId, vector<int> &profDesires);
  void MakeScheduleGreedily(); // Random get pairs (prof,student) that mutually want to meet until stuck.
  void HillClimb(); // 

  vector<Person> Professors;
  vector<Person> Students;

private:
  
};

ostream& operator<<(ostream& os, const Graph& );


#endif



