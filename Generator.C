#include "Generator.H"
#include "Graph.H"
#include "Person.H"

#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

Generator::Generator(int numProfs, int numStudents, int timeMin, int timeMax) {
	tMin = timeMin;
	tMax = timeMax;

	IdRanges[0] = 0;
	IdRanges[1] = numProfs;
	IdRanges[2] = numProfs + numStudents;

	professors = generatePeople(true, numProfs);
	students = generatePeople(false, numStudents);
}

vector<Person> Generator::generatePeople(bool professor, int numPeople) {
	vector<Person> people;

	for(int i=0; i<numPeople; i++){
		Person person_i;
		int numTimes = (tMax - tMin) / 2;
		int minIdNum = professor ? IdRanges[0] : IdRanges[1];
		int numDesires = 2;

		person_i.professor = professor;
		person_i.Id = minIdNum + i;
		person_i.Hours = randomHours(numTimes);
		
		if(professor) person_i.Desired = randomIds(numDesires, IdRanges[1], IdRanges[2] - 1);
		else person_i.Desired = randomIds(numDesires, IdRanges[0], IdRanges[1] - 1);

		people.push_back(person_i);
	}

	return people;
}

vector<int> Generator::randomHours(int numTimes) {
	return randomUniqueNums(numTimes, tMin, tMax);
}

vector<int> Generator::randomIds(int numIds, int minIdRange, int maxIdRange){
	return randomUniqueNums(numIds, minIdRange, maxIdRange);
}

vector<int> Generator::randomUniqueNums(int n, int min, int max) {
	// generate n random numbers in inclusive [min, max]
	vector<int> nums;

	for(int i=0; i<n; i++) {
		int randNum = min + (rand() % (max - min + 1));

		bool duplicate = false;
		 for(int j=0; j<nums.size(); j++){
			if(nums[j] == randNum) duplicate = true;
		}
				
		if(duplicate) i--;
		else nums.push_back(randNum);
	}

	sort(nums.begin(), nums.end());
	return nums;
}