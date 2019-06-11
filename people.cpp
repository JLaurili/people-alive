// Coding challange by Joseph Aurili 6/11/2019

#include <algorithm> 
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

class People
{
public:
	People() {}

	~People() {}

	void createPopulation(unsigned int count) const
	{
		std::ofstream file(file_name, std::ofstream::out);

		unsigned long long int year_count[yearsRange] = { 0 }; // born count by year for unique person ID

		int offset, birth, death; 

		while (count--)
		{
			offset = rand() % yearsRange;
			++year_count[offset];
			birth = 1900 + offset;
			death = birth + (rand() % (yearsRange - offset) );
			
			// create a unique person ID
			std::stringstream id;
			id << std::setfill('0') << "p" << std::setw(3) << offset << "-" << std::setw(10) << year_count[offset] << " ";

			// record id, birth and death years	
			file <<	id.str() << birth << " " << death << std::endl;
		}
	}

	// Basic logic with low memory footprint
	int getMaxPopulationYear() const
	{
		std::ifstream file(file_name);
		std::string line, id;

		bool inputFailed = false;
		int birth, death;

		unsigned long long int alivePerYear[yearsRange] = { 0 }; // alive by year

		while (getline(file, line))
		{
			std::stringstream ss(line);
			ss >> id;
			ss >> birth;
			ss >> death;

			if (ss.fail() ||
				birth < firstYear || birth > lastYear || death < firstYear || death > lastYear || birth > death)
			{
				inputFailed = true;
				break;
			}

			// increment all years alive
			for (size_t i = birth-firstYear; i <= death-firstYear; ++i)
			{
				++alivePerYear[i];
			}
		}

		if (inputFailed)
			return -1;

		int targetYear = 0;

		// find year index with most people alive
		// NOTE: a tie would be resolved as the first year with the max count
		for (int i = 0; i <= yearsRange; ++i)
		{
			if (alivePerYear[i] > alivePerYear[targetYear])
				targetYear = i;
		}

		targetYear += firstYear;

		return targetYear;
	}

	// Store and sort data set
	int getMaxPopulationYearB() const
	{
		std::vector<char> births;
		std::vector<char> deaths;

		std::ifstream file(file_name);
		std::string line, id;

		bool inputFailed = false;
		int birth, death;

		while (getline(file, line))
		{
			std::stringstream ss(line);
			ss >> id;
			ss >> birth;
			ss >> death;

			if (ss.fail() ||
				birth < firstYear || birth > lastYear || death < firstYear || death > lastYear || birth > death)
			{
				inputFailed = true;
				break;
			}

			// Store data sets
			births.push_back(birth - firstYear);
			deaths.push_back(death - firstYear);
		}

		if (inputFailed)
			return -1;

		// Sort data sets
		std::sort(births.begin(), births.end());
		std::sort(deaths.begin(), deaths.end());

		// traverse births and deaths to determine the max alive count
		unsigned long long int alive = 1, maxAlive = 1;
		int bi = 1, di = 0;
		int maxYear = 0;
		int dataSize = static_cast<int>(births.size());

		while (bi < dataSize && di < dataSize)
		{
			if (births[bi] <= deaths[di])
			{
				alive++;

				if (alive > maxAlive)
				{
					maxAlive = alive;
					maxYear = births[bi];
				}

				bi++;
			}
			else
			{
				alive--;
				di++;
			}
		}

		return maxYear + firstYear;
	}

	// Use a single map to store data and get max alive count
	auto getMaxPopulationYearC() const
	{
		std::unordered_map<unsigned char, unsigned long long int> inputs;

		std::ifstream file(file_name);
		std::string line, id;

		bool inputFailed = false;
		int birth, death;

		while (getline(file, line))
		{
			std::stringstream ss(line);
			ss >> id;
			ss >> birth;
			ss >> death;

			if (ss.fail() ||
				birth < firstYear || birth > lastYear || death < firstYear || death > lastYear || birth > death)
			{
				inputFailed = true;
				break;
			}

			// Store alive years
			for (int i = birth - firstYear; i <= death - firstYear; ++i)
				inputs[i]++;
		}

		if (inputFailed)
			return -1;

		unsigned long long int maxAlive = 0;
		int targetYear = 0;
		
		for (auto i : inputs)
		{
			if (i.second > maxAlive)
			{
				maxAlive = i.second;
				targetYear = i.first;
			}
		}

		return targetYear + firstYear;
	}

private:
	static const int firstYear = 1900;
	static const int lastYear = 2000;
	static const int yearsRange = lastYear - firstYear + 1;
	const std::string file_name = "people.txt";
};

void printYear(int year)
{
	if (year < 0)
		std::cout << "Error in data set = " << year << "\n" <<  std::endl;
	else
		std::cout << "Most people where alive in the year " << year << "\n" << std::endl;
}

void printDuration(std::string func, std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end)
{
	std::cout << func << " complete in  " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
}

int main()
{
	People p;

	//p.createPopulation(10000); // uncomment to create a new set of data

	int year;
	std::chrono::time_point<std::chrono::steady_clock> start, end;

	start = std::chrono::high_resolution_clock::now();
	year = p.getMaxPopulationYear();
	end = std::chrono::high_resolution_clock::now();
	printDuration("getMaxPopulationYear()", start, end);
	printYear(year);

	start = std::chrono::high_resolution_clock::now();
	year = p.getMaxPopulationYearB();
	end = std::chrono::high_resolution_clock::now();
	printDuration("getMaxPopulationYearB()", start, end);
	printYear(year);

	start = std::chrono::high_resolution_clock::now();
	year = p.getMaxPopulationYearC();
	end = std::chrono::high_resolution_clock::now();
	printDuration("getMaxPopulationYearC()", start, end);
	printYear(year);

	std::cout << "---tests complete---" << std::endl;

	return 0;
}
