#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <random>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include "Tour.h"
#include "city.h"
using std::cout;
using std::fstream;
using std::endl;
using std::vector;
#define INITIAL_TEMPERATURE 1000
#define test1 20

int cityDis[test1][test1];
vector<City> allCitys;

//vector<int> bestCase{1,4,16,18,9,7,13,15,11,17,3,5,12,4,2,8,9,19,16,10,20};

// 计算接受的概率
double acceptanceProbability(int energy, int newEnergy, double temperature)
{
	if (newEnergy < energy)
		return 1.0;

	return exp((energy - newEnergy) / temperature);
}

void initArray()
{
	ifstream in("tsp测试数据1.txt", ios::in);
	int tmp = 0;
	int count = 0;
	if (in.is_open())
	{
		for (int i = 0; i < test1; ++i)
		{
			for (int j = 0; j < test1; ++j)
			{
				in >> cityDis[i][j];
			}
		}
	}
	else
		return;
	in.close();
}

// 获取近似解
void getApproximateyResult(int thread)
{
	random_device rd;
	
	double coolingRate = 0.3;
	int n = 100000;

	
	Tour currentSolution(allCitys);
	currentSolution.generateIndividual();

	cout << endl;
	cout << endl;
	std::cout << "Initial solution distance: " << currentSolution.getDistance() << endl;

	
	Tour best(currentSolution.getTour(), allCitys);

	auto local_currentSolution = best;
	auto local_best = best;
	int i;

	
	int blockLength[] = {16,4,16};
	MPI::Datatype oldTypes[] = { MPI_COMBINER_VECTOR,MPI_INT,MPI_COMBINER_VECTOR };
	
	MPI::Aint addressOffsets[] = { 0,16,20 };
	MPI::Datatype newType = MPI::Datatype::Create_struct(
		sizeof(blockLength) / sizeof(int),
		blockLength,
		addressOffsets,
		oldTypes
	);
	newType.Commit();
	

	int my_rank, comm_sz;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	
	for (double initialTemprature = INITIAL_TEMPERATURE; initialTemprature > 1; initialTemprature *= 1 - coolingRate)
	{

		local_currentSolution = best;

		
		for (i = my_rank*(n / comm_sz); i < (my_rank + 1)*(n / comm_sz); ++i)
		{
			
			Tour newSolution(local_currentSolution.getTour(), allCitys);

			
			int tourPos1 = abs((int)(rd() % newSolution.getTourSize()));
			int tourPos2 = abs((int)(rd() % newSolution.getTourSize()));

			City citySwap1 = newSolution.getCity(tourPos1);
			City citySwap2 = newSolution.getCity(tourPos2);

			
			newSolution.setCity(tourPos2, citySwap1);
			newSolution.setCity(tourPos1, citySwap2);

			
			int currentEnergy = local_currentSolution.getDistance();
			int neighbourEnergy = newSolution.getDistance();
			auto rate = (rd() % 10000)*0.0001;
			auto accept = acceptanceProbability(currentEnergy, neighbourEnergy, initialTemprature);

			
			if (accept > rate)
			{
				local_currentSolution.setTour(newSolution.getTour());
			}
		}

	
		if (my_rank != 0)
		{
			MPI_Send(&local_currentSolution, sizeof(local_currentSolution), newType, 0,0,MPI_COMM_WORLD);
		}
		else
		{
			if(local_currentSolution.getDistance()<best.getDistance())
			{
				best = local_currentSolution;
			}

			for(int src = 1; src<comm_sz;++src)
			{
				MPI_Recv(&local_currentSolution, sizeof(local_currentSolution), newType, src, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				if(best.getDistance()>local_currentSolution.getDistance())
				{
					best = local_currentSolution;
				}
			}
		}
	}
	
	MPI_Finalize();
	cout << "==============================================" << endl;

	cout << "Final solution distance is : " << best.getDistance() << endl;
	cout << "The best Tour is: " << endl;
	auto route = best.getTour();
	for (auto index : route)
	{
		cout << index.getId() << "----->";
	}
	cout << endl;
	cout << "==============================================" << endl;
	cout << endl << endl;

}

	


void initCity()
{
	initArray();

	for (int i = 1; i <= 20; ++i)
	{
		City city(i);
		allCitys.push_back((city));
	}

}

int main(int arvc, char* argv[])
{
	initCity();
	Tour best(allCitys);
	int thread_count = strtol(argv[1], NULL, 10);
	getApproximateyResult(thread_count);
}
