#include "Graph.H"
#include "Person.H"
#include "Generator.H"

#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;


// INITIALIZATION
void Graph::Initialize(string fName, char delimiter) {
  ReadInData(fName, delimiter);
  Prune();
  CreateEdges();
}

void Graph::ReadInData(string fName, char delimiter) {
// Read in all the info about profs and students and create the vectors containing them.
// read from generated WritePopulation() file or real data file to Graph

  ifstream fPopulation;
  fPopulation.open(fName);

  Professors = ParseGroup(fPopulation, true, delimiter);
  Students = ParseGroup(fPopulation, false, delimiter);
  assert(Professors.size() > 0 && Students.size() > 0);
  fPopulation.close();
}

vector<Person> Graph::ParseGroup(ifstream& fPopulation, bool isProfessor, char delimiter) {
  vector<Person> group;

  string line;
  while (getline(fPopulation, line)) {
    stringstream lineStream(line);
    if(line.empty()) break; // if newline is found, indicates new group (prof -> stud)
    string item;

    Person x;
    x.isProfessor = isProfessor;
    bool idFound = false;
    bool desiresFound = false;

    while (getline(lineStream, item, ',')) {
      if(!idFound) {
        x.Id = stoi(item);
        idFound = true;
      } else if(!desiresFound) {
        if(item.empty()) {
          desiresFound = true;
          continue;
        }
        x.Desired.push_back(stoi(item));
      } else {
        x.Hours.push_back(stoi(item));
      }
    }
    group.push_back(x);
  }
  return group;
}

void Graph::Prune() {
// Remove non-mutual and impossible-to-fulfill meeting desires
  for (Person& professor : Professors) {
    professor.RemoveInvalidDesires(Students, Professors.size());
  }
  for (Person& student : Students) {
    student.RemoveInvalidDesires(Professors);
  }
}

void Graph::CreateEdges() {
  for (Person& prof : Professors) {
    for (int studentId : prof.Desired) {
      Unconnected.push_back(pair(prof.Id, studentId));
    }
  }
}

// GREEDY FILL
void Graph::InitialGreedyFill(default_random_engine& rng) {
// Used for first greedy fill after random restart from base graph
  shuffle(begin(Unconnected), end(Unconnected), rng);
  MakeScheduleGreedily();
}

void Graph::ClimbGreedyFill(pair<int, int> removedEdge, default_random_engine& rng) {
// Used for iterative hill climbs' greedy fills on the remaining Unconnected edges after removing one edge from Connected.
  shuffle(begin(Unconnected), end(Unconnected), rng);
  Unconnected.insert(Unconnected.begin(), removedEdge); // insert the removed Connected edge at beginning because MakeScheduleGreedily() evaluates from last to first index
  MakeScheduleGreedily();
}

void Graph::MakeScheduleGreedily() {
// Fill graph greedily with Unconnected edges
  for (int i = Unconnected.size() - 1; i >= 0; i--) {
    pair<int, int> compare = Unconnected[i];
    Person& prof = Professors[compare.first];
    Person& student = Students[compare.second - Professors.size()];

    int commonTime = prof.CrossCheckSchedule(student);

    if (commonTime >= 0) {
      prof.MeetPersonAndTime.push_back(pair(student.Id, commonTime));
      student.MeetPersonAndTime.push_back(pair(prof.Id, commonTime));

      Unconnected.erase(Unconnected.begin() + i);
      Connected.push_back(compare);
    }
  }
}

// HILL CLIMB
double Graph::AttemptClimb(double currentScore, default_random_engine& rng) {
// Attempt a hill climb by removing one edge, greedy filling with the Unconnected edges, and checking if the score has improved
  pair<int, int> removedEdge = RemoveRandomEdge();

  // attempt greedy fill with remaining Unconnected and score
  ClimbGreedyFill(removedEdge, rng);

  return Score();
}

pair<int, int> Graph::RemoveRandomEdge() {
// Remove a random Connected edge in preparation for hill climb

  // choose an edge to connect
  int randInt = rand() % Connected.size();
  pair<int, int> removedEdge = Connected[randInt];

  Person& prof = Professors[removedEdge.first];
  Person& student = Students[removedEdge.second - Professors.size()];

  // update Professor/Student objects and Connected vector with Unconnected edge
  for (int i = prof.MeetPersonAndTime.size() - 1; i >= 0 ; i--) {
    if (prof.MeetPersonAndTime[i].first == student.Id) {
      prof.MeetPersonAndTime.erase(prof.MeetPersonAndTime.begin() + i);
    }
  }
  for (int i = student.MeetPersonAndTime.size() - 1; i >= 0 ; i--) {
    if (student.MeetPersonAndTime[i].first == prof.Id) {
      student.MeetPersonAndTime.erase(student.MeetPersonAndTime.begin() + i);
    }
  }
  Connected.erase(Connected.begin() + randInt);

  return removedEdge;
}

double Graph::Score() const {
// Score the graph: what proportion of desires have been fulfilled?
  int fulfilled = 0;
  int desired = 0;

  for (Person student : Students) {
    fulfilled += student.MeetPersonAndTime.size();
    desired += student.Desired.size();
  }
  // cout << fulfilled << " " << desired << endl;
  return fulfilled / (double) desired;
}

// FILE OUTPUT
void Graph::WriteGraphState(string fname, char delimiter) const {
  ofstream graphOut;
  graphOut.open(fname);

  graphOut << "Availabilities\n";
  WriteAvailabilities(graphOut, delimiter);
  graphOut << "\nPossible & Mutual Desires (X) / Meetings (int)\n";
  WriteSchedule(graphOut, delimiter);

  graphOut.close();
}

void Graph::WriteAvailabilities(ofstream& graphOut, char delimiter) const {
  // find min and max time slot (relies on sorted availabilities)
  assert(Professors[0].Hours.size() > 0);
  int minTime = Professors[0].Hours[0];
  int maxTime = Professors[0].Hours.back();
  for (Person prof : Professors) {
    if (prof.Hours[0] < minTime) minTime = prof.Hours[0];
    if (prof.Hours.back() > maxTime) maxTime =  prof.Hours.back();
  }
  for (Person stud : Students) {
    if (stud.Hours[0] < minTime) minTime = stud.Hours[0];
    if (stud.Hours.back() > maxTime) maxTime = stud.Hours.back();
  }
  pair<int, int> timeRange = pair(minTime, maxTime);

  // write labels
  graphOut << "[ID\\T]" << delimiter;
  for (int i = minTime; i <= maxTime; i++) {
    graphOut << "[" << i << "]" << delimiter;
  }
  graphOut << "\n";

  // write availabilities per group
  WriteAvailabilitiesForGroup(Professors, graphOut, timeRange, delimiter);
  WriteAvailabilitiesForGroup(Students, graphOut, timeRange, delimiter);
}

void Graph::WriteAvailabilitiesForGroup(const vector<Person>& group, ofstream& graphOut,
                                        pair<int, int> timeRange, char delimiter) const {
  vector<vector<bool>> availabilities = GenerateAvailabilityVector(group, timeRange);

  char affiliation = group[0].isProfessor ? 'P' : 'S';

  for (int i = 0; i < availabilities.size(); i++) {
    graphOut << "[" << affiliation << group[i].Id << "]" << delimiter;

    for (int j = 0; j < availabilities[0].size(); j++) {
      bool available = availabilities[i][j];
      graphOut << available << delimiter;
    }
    graphOut << "\n";
  }
}

vector<vector<bool>> Graph::GenerateAvailabilityVector(const vector<Person>& group, pair<int, int> timeRange) const {
  vector<vector<bool>> availabilities;
  for (Person x : group) {
    vector<bool> personAvailable;
    personAvailable.resize(timeRange.second - timeRange.first + 1, false);

    for (int hour : x.Hours) {
      personAvailable[hour - timeRange.first] = true;
    }
    availabilities.push_back(personAvailable);
  }
  return availabilities;
}

void Graph::WriteSchedule(ofstream& graphOut, char delimiter) const {
  vector<vector<int>> schedule = GenerateScheduleVector();
  graphOut << "[P\\S]" << delimiter;
  for (int i = Professors.size(); i < Professors.size() + Students.size(); i++) {
    graphOut << "[" << i << "]" << delimiter; // studId
  }
  graphOut << "\n";

  for (int i = 0; i < schedule.size(); i++) {
    graphOut << "[" << i << "]" << delimiter; // profId

    for (int j = 0; j < schedule[0].size(); j++) {
      int time = schedule[i][j];
      if (time >= 0) graphOut << schedule[i][j] << delimiter; // fulfilled meeting
      else if (time == -1) graphOut << "X" << delimiter; // unfulfilled meeting
      else graphOut << "." << delimiter; // no desire for meeting
    }
    graphOut << "\n";
  }

  graphOut << "Score: " << Score() << endl;
}

vector<vector<int>> Graph::GenerateScheduleVector() const {
  vector<vector<int>> schedule;

  int numProfs = Professors.size();
  int numStuds = Students.size();

  // fill out confirmed meetings
  for (Person prof : Professors) {
    vector<int> profSchedule;
    profSchedule.resize(numStuds, -2); // no meeting = -2

    for (pair<int, int> meeting : prof.MeetPersonAndTime) {
      profSchedule[meeting.first - numProfs] = meeting.second; // use id indexing (studId - profId)
    }
    schedule.push_back(profSchedule);
  }

  // fill out unfulfilled meetings
  for (pair<int, int> x : Unconnected) {
    int prof = x.first;
    int stud = x.second;

    schedule[prof][stud - numProfs] = -1; // unfulfilled = -1
  }

  return schedule;
}

// CORE FUNCTIONS
// run random restart on graph while iterating
Graph RandomRestart(const Graph baseG, default_random_engine& rng, bool verbose) {
  Graph bestG = baseG;
  double bestScore = 0;
  if (baseG.Unconnected.size() == 0) return baseG; // check if any meetings are possible at all

  if (verbose) cout << "restart#\tclimb#\tscore\tdelta" << endl;
  int populationMultiplier = baseG.Professors.size() * baseG.Students.size();

  // random restart loop
  for (int i = 0; i < populationMultiplier / 10 && bestScore < 1; i++) {
    Graph hillClimbG = baseG;
    hillClimbG.InitialGreedyFill(rng);
    if (hillClimbG.Connected.size() == 0) continue; // disregard complete failure of initial greedy fill

    Climb(i, hillClimbG, bestG, bestScore, populationMultiplier, rng, verbose);
  }
  cout << "BEST: " << bestScore << endl;
  return bestG;
}

// run hill climb on each random restarted graph
void Climb(const int i, Graph& hillClimbG, Graph& bestG, double& bestScore, int populationMultiplier, default_random_engine& rng, bool verbose) {
// hill climb loop
  double currentScore = 0;
  int climbLimit = populationMultiplier;
  for (int j = 0 ; j < climbLimit && currentScore < 1; j++) {
    // create a new attempt object so that changes can be made without affecting the hillClimbG object
    Graph attemptG = hillClimbG; // return attempt graph to the best graph out of this restart (discard non-score-increasing changes)
    double climbScore = attemptG.AttemptClimb(currentScore, rng);

    if (climbScore > currentScore) { // evaluate climb attempt
      if (verbose) cout << i << "\t\t" << j << "\t" << climbScore << "\t" << climbScore - currentScore << endl;

      // update graphs that have climbed
      currentScore = climbScore;
      hillClimbG = attemptG;

      // update graphs that have climbed and are better than graphs from all restarts
      if (climbScore > bestScore) {
        cout << "global max [restart " << i << " climb " << j << "]:\t" << climbScore << "\t" << "delta(best): " << climbScore - bestScore << endl;

        bestScore = currentScore;
        bestG = hillClimbG;

        // reward hill climbs that have increased the score with more attempts
        climbLimit += populationMultiplier;
      }
    }
  }
}

// Use this for debugging to print out all profs and students in the graph.
ostream& operator<<(ostream& os, const Graph& x)
{
  os << endl;
  os << " Professors:" << endl;
  for (int i = 0; i < x.Professors.size(); i++) os << x.Professors[i];
  os << endl;

  os << " Students:" << endl;
  for (int i = 0; i < x.Students.size(); i++) os << x.Students[i];
  os << endl;

  os << " Meetings:" << endl;
  for (int i = 0; i < x.Students.size(); i++) {
    int StudentId = x.Students[i].Id;
    os << " Student " << StudentId << " meetings with: " << endl;
    for (int j = 0; j < x.Students[i].MeetPersonAndTime.size(); j++) {
      int ProfToBeMetID  = x.Students[i].MeetPersonAndTime[j].first;
      int ProfTimeToMeet = x.Students[i].MeetPersonAndTime[j].second;
      os << "    Professor " << ProfToBeMetID << " at time " << ProfTimeToMeet << endl;
    }
  }
  os << endl;

  os << "Unfulfilled Meetings:" << endl;
  for (pair<int, int> edge : x.Unconnected) {
    cout << " P" << edge.first << " S" << edge.second << endl;
  }
  os << endl;

  os << "Score: " << x.Score() << endl;

  return os;
}
