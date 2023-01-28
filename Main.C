#include <iostream>
#include "Graph.H"

using namespace std;


int main(){
  Graph G;
  G.ReadInData(); // Fill up the Professors and Students vectors.
  G.MakeScheduleGreedily();
  cout << "Graph after greedily filled:" << G;
  G.HillClimb();
  cout << "Graph after hill climb:" << G;
}
