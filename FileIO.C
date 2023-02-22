#include "FileIO.H"
#include "Graph.H"

#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

void FileIO::WritePopulation(const Generator gen) {
	ofstream population;
	population.open("population.csv");

	// write generated population to file

	population.close();
}

void FileIO::ParsePopulation() {
	// read from generated WritePopulation() file or real data file -> Graph of population
	// perhaps this function should be located in the Graph class
}

void FileIO::WriteSchedule(const Graph& G, string delimiter) {
	ofstream fSchedule;
	fSchedule.open("schedule.csv");

	vector<vector<int>> schedule = GenerateScheduleVector(G);
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

	fSchedule << "Score: " << G.Score() << endl;
	
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

void FileIO::WriteAvailabilities(const Graph& G, string delimiter) {
	ofstream fSchedule;
	fSchedule.open("availabilities.csv");

	// find min and max time slot
	assert(G.Professors[0].Hours.size() > 0);
	int minTime = G.Professors[0].Hours[0];
	int maxTime = G.Professors[0].Hours.back();
	for (int i = 1; i < G.Professors.size(); i++) {
		if (G.Professors[i].Hours[0] < minTime) minTime = G.Professors[i].Hours[0];
		if (G.Professors[i].Hours.back() > maxTime) maxTime = G.Professors[i].Hours.back();
	}
	for (int i = 0; i < G.Students.size(); i++) {
		if (G.Students[i].Hours[0] < minTime) minTime = G.Students[i].Hours[0];
		if (G.Students[i].Hours.back() > maxTime) maxTime = G.Students[i].Hours.back();
	}
	pair<int, int> timeRange = pair(minTime, maxTime);

	// write labels
	fSchedule << "[ID\\T]" << delimiter;
	for (int i = minTime; i <= maxTime; i++) {
		fSchedule << "[" << i << "]" << delimiter;
	}
	fSchedule << "\n";

	// write availabilities per group
	WriteAvailabilitiesForGroup(G.Professors, fSchedule, timeRange, "P", delimiter);
	WriteAvailabilitiesForGroup(G.Students, fSchedule, timeRange, "S", delimiter);

	fSchedule.close();
}

void FileIO::WriteAvailabilitiesForGroup(const vector<Person>& people, ofstream& fSchedule, pair<int, int> timeRange, string affiliation, string delimiter) {
	vector<vector<bool>> availabilities = GenerateAvailabilityVector(people, timeRange);
	for (int i = 0; i < availabilities.size(); i++) {
		fSchedule << "[" << affiliation << people[i].Id << "]" << delimiter;
		for (int j = 0; j < availabilities[0].size(); j++) {
			bool available = availabilities[i][j];
			if (available) fSchedule << available << delimiter;
			else fSchedule << "." << delimiter;
		}
		fSchedule << "\n";
	}
}

vector<vector<bool>> FileIO::GenerateAvailabilityVector(const vector<Person>& people, pair<int, int> timeRange) {
	vector<vector<bool>> availabilities;
	for (int i = 0; i < people.size(); i++) {
		vector<bool> personAvailable;
		personAvailable.resize(timeRange.second - timeRange.first + 1, false);

		for (int j = 0; j < people[i].Hours.size(); j++) {
			int hour = people[i].Hours[j];
			personAvailable[hour - timeRange.first] = true;
		}
		availabilities.push_back(personAvailable);
	}
	return availabilities;
}
