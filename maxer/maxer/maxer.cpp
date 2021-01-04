#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono> 

struct Sorter {
	int start;
	int end;

	Sorter(int l, int r) {
		start = l;
		end = r;
	}

	Sorter() {
		start = 0;
		end = 0;
	};
	bool operator ==(const Sorter& nd)
	{
		return start == nd.start && end == nd.end;
	}
};

std::vector <Sorter> data;

int main(int argc, char** argv)
{
	std::string inputFile = argv[1];
	std::ifstream infile;
	infile.open(inputFile, std::ios::in);


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

	auto start = std::chrono::high_resolution_clock::now();
	std::ios_base::sync_with_stdio(false);

	//Sortiramo intervale glede na zacetek
	std::sort(data.begin(), data.end(), [](const Sorter& a, const Sorter& b) -> bool {
		if (a.start == b.start)
			return a.end > b.end;

		return a.start < b.start;
		});

	//set c to 0
	int c = 0;
	//check if algorithm has possible solutuin ie. if any interval goes to the end of the array
	for (int i = M - 1; i >= 0; i--) {
		if (data[i].end >= N) {
			c++;
			break;
		}
	}
	if (c == 0) {
		std::cout << "-1\n";
		return 0;
	}

	//Nastavimo na 1 saj smo prvi interval ze sprejeli
	uint32_t numOfIntervalsAccepted = 1;
	//Save previous interval so we can get its end location
	Sorter prev = data[0];

	int prevEnd = 0;
	//How far into our path we got
	while (prev.end < N) {

		Sorter currBest;
		currBest.end = -1;
		int cnt = 0;

		for (int i = prevEnd; i < data.size(); i++) {
			Sorter x = data[i];

			//if interval ends before previous interval we skip it
			if (x.end <= prev.end) {
				continue;
			}
			//if next interval start is not in ours we can break
			if(i != data.size() - 1){
				if (data[i + 1].start > x.end) 
					break;
			}

			//if interval does not begin at same position that previous end we can skip it
			if (x.start > prev.end)
				break;

			//	if interval starts at same position than previous interval and it ends later that current best interval we set it as
			//	current best interval
			if (x.start <= prev.end && x.end > currBest.end) {
				cnt++;
				currBest = x;
				prevEnd = i;
			}
		}
		//If we did not set current best it means that none of the intervals began before the end of previous interval
		if (cnt == 0) {
			std::cout << "-1\n";
			return 0;
		}
		numOfIntervalsAccepted++;
		prev = currBest;
	}

	std::cout << numOfIntervalsAccepted << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	double time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	time_taken *= 1e-9;

	std::cout << "Time taken by program is : " << std::fixed
		<< time_taken << std::setprecision(9);
	std::cout << " sec" << std::endl;


	return 0;
}