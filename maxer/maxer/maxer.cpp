#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Sorter {
	int left; //reach to the left
	int right; //reach to the right
	bool active; //if current Sorter is the one we are looking for neighbours

	Sorter(int l, int r) {
		left = l; 
		right = r;
		active = true;
	}

	bool operator !=(const Sorter& nd)
	{
		if(left == nd.left && right == nd.right)
			return false;
		return true;
	}
};

int recursive(int depth, std::vector<Sorter>& data , int ending, Sorter& current);
std::vector<int> findNeighbours(Sorter who, std::vector<Sorter> data);
Sorter findFirst(std::vector<Sorter>data);

int max = INT_MAX;

int main(int argc, char** argv)
{
	std::string inputFile = argv[1];
	std::ifstream infile;
	infile.open(inputFile, std::ios::in);

	std::vector <Sorter> data; //saves all sorters availible

	if (!infile.is_open()) {
		std::cout << "Could not open file\n";
		return 0;
	}
	int N = 0; // length of list
	int M = 0; // amount of sorters

	infile >> N;
	infile >> M;

	int l, r; // to get left and rigt reach of each sortetr

	//saving sorters from file into vector and its left and right variables
	while (infile >> l >> r) {
		data.push_back(Sorter(l, r));
	}
	
	//finds first sorter with left = 1
	Sorter f = findFirst(data);

	if (f.left == -1) //no sorter is viable to start algorithm
	{
		std::cout << "No viable start\n";
		return 0;
	}

	//getting min number of sorters we need to complete a maxer
	int min = recursive(1, data, N, f);

	std::ofstream outFile;
	outFile.open("output.txt");
	outFile << min << std::endl;
}



int recursive(int depth, std::vector<Sorter>& data, int ending, Sorter& current) {
	std::vector<int> NB = findNeighbours(current, data); //gets neighbours of current sorter we are on 

	current.active = false;//so we can eliminate this one from being found by neighbours

	//Ending condtion
	if (NB.size() == 0 || current.right >= ending) {
		current.active = true;
		return depth;
	}

	//iterating over all neighbours of current sorter
	for (int x : NB) {
		int rez = recursive(depth + 1, data, ending, data[x]);
		if (rez < max) {
			max = rez;
		}
	}

	current.active = true; 	//so it can be found by other sorters again
	return max;
}

//Find intervals that lay inside who
//who -- interval that we are searching neighbours for
std::vector<int> findNeighbours(Sorter who, std::vector<Sorter> data) {
	std::vector<int>output; //vector of who's neighbours

	for (int i = 0; i < data.size(); i++) {
		//check if current intervals start is within the bounds of who, and check for duplicates with active
		if (who.left <= data[i].left && who.right >= data[i].left && who != data[i] && data[i].active)
			output.push_back(i);	
	}
	return output;
}

//if not start with one find lowest
Sorter findFirst(std::vector<Sorter>data) {
	for (Sorter x : data)
	{
		if (x.left == 1) //left = 1 means its at the start of the list 
			return x;
	}
	return Sorter(-1, -1);
}