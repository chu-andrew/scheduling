#include "FileIO.H"
#include "Graph.H"

#include <iostream>
#include <fstream>

using namespace std;

void FileIO::WriteToCSV(const Graph& G) {
	ofstream graphCSV;
	graphCSV.open("graph.csv");

	for (int i = 0; i < G.Professors.size(); i++) {
		// to be implemented
	}
	graphCSV.close();
}

void FileIO::WriteSchedule(const Graph& G, string delimiter) {
	vector<vector<int>> schedule = GenerateScheduleVector(G);

	ofstream fSchedule;
	fSchedule.open("schedule.csv");

	fSchedule << "[P\\S]" << delimiter;
	for (int i = G.Professors.size(); i < G.Professors.size() + G.Students.size(); i++) {
		fSchedule << "[" << i << "]" << delimiter; // studId
	}
	fSchedule << "\n";

	for (int i = 0; i < schedule.size(); i++) {
		fSchedule << "[" << i << "]" << delimiter; // profId

		for (int j = 0; j < schedule[0].size(); j++) {
			int time = schedule[i][j];
			if (time >= 0) fSchedule << schedule[i][j] << delimiter; // fulfilled meeting
			else if (time == -1) fSchedule << "X" << delimiter; // unfulfilled meeting
			else fSchedule << "." << delimiter; // no desire for meeting
		}
		fSchedule << "\n";
	}
	fSchedule.close();
}

vector<vector<int>> FileIO::GenerateScheduleVector(const Graph& G) {
	vector<vector<int>> schedule;

	int numProfs = G.Professors.size();
	int numStuds = G.Students.size();

	// fill out confirmed meetings
	for (int i = 0; i < numProfs; i++) {
		vector<int> profSchedule;
		profSchedule.resize(numStuds, -2); // no meeting = -2

		for (int j = 0; j < G.Professors[i].MeetPersonAndTime.size(); j++) {
			pair<int, int> meeting = G.Professors[i].MeetPersonAndTime[j];
			profSchedule[meeting.first - numProfs] = meeting.second; // use id indexing (studId - profId)
		}
		schedule.push_back(profSchedule);
	}

	// fill out unfulfilled meetings
	for (int i = 0; i < G.Unconnected.size(); i++) {
		int prof = G.Unconnected[i].first;
		int stud = G.Unconnected[i].second;

		schedule[prof][stud - numProfs] = -1; // unfulfilled = -1
	}

	return schedule;
}


