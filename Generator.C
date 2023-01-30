#include "Generator.H"
#include "Graph.H"
#include "Person.H"

#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;

Generator::Generator(int numProfs, int numStudents, double availabilityProportion, int numDesires, int timeMin, int timeMax) {
	this->timeMin = timeMin;
	this->timeMax = timeMax;

	numTimes = (timeMax - timeMin) * availabilityProportion;
	this->numDesires = numDesires;

	IdRanges[0] = 0;
	IdRanges[1] = numProfs;
	IdRanges[2] = numProfs + numStudents;

	professors = generatePeople(true, numProfs);
	students = generatePeople(false, numStudents);
}

vector<Person> Generator::generatePeople(bool professor, int numPeople) {
// Generate a random group of people under specified parameters
	vector<Person> people;
	
	for(int i=0; i<numPeople; i++){
		Person person_i;
		int minIdNum = professor ? IdRanges[0] : IdRanges[1];

		person_i.professor = professor;
		person_i.Id = minIdNum + i;
		person_i.Hours = randomUniqueNums(numTimes, timeMin, timeMax);
		
		if(professor) person_i.Desired = randomUniqueNums(numDesires, IdRanges[1], IdRanges[2] - 1);
		else person_i.Desired = randomUniqueNums(numDesires, IdRanges[0], IdRanges[1] - 1);

		people.push_back(person_i);
	}
	return people;
}

vector<int> Generator::randomUniqueNums(int n, int min, int max) {
// Generate n sorted random numbers in inclusive [min, max] without duplicates
	vector<int> nums;

	for(int i=0; i<n; i++) {
		int randNum = min + (rand() % (max - min + 1));

		bool duplicate = false;
		 for(int j=0; j<nums.size(); j++){
			if(nums[j] == randNum) {
				duplicate = true;
				break;
			}
		}
		if(duplicate) i--;
		else nums.push_back(randNum);
	}

	sort(nums.begin(), nums.end());
	return nums;
}
