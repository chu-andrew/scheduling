#include "Generator.H"
#include "Graph.H"
#include "Person.H"

#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>


using namespace std;

Generator::Generator(int numProfs, int numStudents, int numTimes, int numDesires, int timeMin, int timeMax) {
	this->timeMin = timeMin;
	this->timeMax = timeMax;

	this->numTimes = numTimes;
	this->numDesires = numDesires;

	assert(numDesires <= numProfs && numDesires <= numStudents);
	assert(numTimes <= timeMax - timeMin + 1);

	IdRanges[0] = 0;
	IdRanges[1] = numProfs;
	IdRanges[2] = numProfs + numStudents;

	professors = generatePeople(true, numProfs);
	students = generatePeople(false, numStudents);
}

vector<Person> Generator::generatePeople(bool professor, int numPeople) {
// Generate a random group of people under specified parameters
	vector<Person> people;

	for (int i = 0; i < numPeople; i++) {
		Person person_i;
		int minIdNum = professor ? IdRanges[0] : IdRanges[1];

		person_i.professor = professor;
		person_i.Id = minIdNum + i;

		person_i.Hours = randomUniqueNums(numTimes, timeMin, timeMax);
		if (professor) person_i.InitDesired = randomUniqueNums(numDesires, IdRanges[1], IdRanges[2] - 1);
		else person_i.InitDesired = randomUniqueNums(numDesires, IdRanges[0], IdRanges[1] - 1);
		person_i.Desired = person_i.InitDesired; // make a record of initial desires because desires will be pruned

		people.push_back(person_i);
	}
	return people;
}

vector<int> Generator::randomUniqueNums(int n, int min, int max) {
// Generate n sorted random numbers in inclusive [min, max] without duplicates
	vector<int> nums;

	for (int i = 0; i < n; i++) {
		int randNum = min + (rand() % (max - min + 1));

		bool duplicate = false;
		for (int j = 0; j < nums.size(); j++) {
			if (nums[j] == randNum) {
				duplicate = true;
				break;
			}
		}
		if (duplicate) i--;
		else nums.push_back(randNum);
	}

	sort(nums.begin(), nums.end());
	return nums;
}

void Generator::WritePopulation(string fname, char delimiter) const {
	// write generated population to file
	/*
	numProf, numStud
	profId, numDesires, numTimes, [desires], [times]
	...
	studId, numDesires, numTimes, [desires], [times]
	...
	*/
	ofstream population;
	population.open(fname);

	population << professors.size() << delimiter << students.size() << "\n";

	for(const Person& x : professors) {
		population << x.Id << delimiter;
		population << x.Desired.size() << delimiter << x.Hours.size() << delimiter;
		for(int desire : x.Desired) population << desire << delimiter;
		for(int hour : x.Hours) population << hour << delimiter;
		population << "\n";
	}

	for(const Person& x : students) {
		population << x.Id << delimiter;
		population << x.Desired.size() << delimiter << x.Hours.size() << delimiter;
		for(int desire : x.Desired) population << desire << delimiter;
		for(int hour : x.Hours) population << hour << delimiter;
		population << "\n";
	}

	population.close();
}