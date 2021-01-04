#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Sorter {
	int left;
	int right;
	bool active;

	Sorter(int l, int r) {
		left = l; 
		right = r;
		active = true;
	}

	bool operator !=(const Sorter& nd)
	{
		if(left == nd.left && right == nd.right)
			return false;
		else
			return true;
	}
};

void solve(std::vector<Sorter>data, int ending);
int recursive(int depth, std::vector<Sorter>& data , int ending, Sorter& current);
std::vector<int> findNeighbours(Sorter who, std::vector<Sorter> data);
Sorter findFirst(std::vector<Sorter>data);
int max = INT_MAX;

int main(int argc, char** argv)
{
	std::string inputFile = argv[1];
	std::ifstream infile;
	infile.open(inputFile, std::ios::in);

	std::vector <Sorter> data;

	if (!infile.is_open()) {
		std::cout << "Could not open file\n";
		return 0;
	}
	int N = 0, M = 0;

	infile >> N;
	infile >> M;
	int l, r;
	while (infile >> l >> r) {
		data.push_back(Sorter(l, r));
	}
	

	Sorter f = findFirst(data);
	if (f.left == -1)
	{
		std::cout << "No viable start\n";
		return 0;
	}

	int maxT = recursive(1, data, N, f);

	std::ofstream outFile;
	outFile.open("ouput.txt");
	outFile << maxT << std::endl;
}

/*
void solve(std::vector<Sorter>data, int ending) {
	//int currentPostion = 1;
	recursive(0, data);
}*/
int recursive(int depth, std::vector<Sorter>& data, int ending, Sorter& current) {
	std::vector<int> NB = findNeighbours(current, data);
	current.active = false;
	//Ending condtion
	if (NB.size() == 0 || current.right >= ending) {
		current.active = true;
		return depth;
	}

	for (int x : NB) {
		int rez = recursive(depth + 1, data, ending, data[x]);
		if (rez < max) {
			max = rez;
		}
	}
	current.active = true;
	return max;
}

//Find intervals that lay inside who
//who -- interval that we are searching neighbours for
std::vector<int> findNeighbours(Sorter who, std::vector<Sorter> data) {
	std::vector<int>output;
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
		if (x.left == 1)
			return x;
	}
	return Sorter(-1, -1);
}