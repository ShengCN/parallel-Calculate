#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <random>
#include <omp.h>
#include <math.h>
#include "Tour.h"
#include "city.h"
#include <mpi.h>
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
void getApproximateyResult(int thread = 1)
{
	random_device rd;
	// 冷却概率
	double coolingRate = 0.003;
	int n = 10;

	// 初始的解决方案
	Tour currentSolution(allCitys);
	currentSolution.generateIndividual();

	cout << endl;
	cout << endl;
	std::cout << "Initial solution distance: " << currentSolution.getDistance() << endl;

	// 设置当前为最优的方案
	Tour best(currentSolution.getTour(), allCitys);

	auto local_currentSolution = best;
	auto local_best = best;
	int i;
	int* local_current_tour = best.getTourArray();
	double start, end, local_elapsed, elapsed;

	int my_rank, comm_sz;
	// parallel region
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	omp_set_num_threads(thread);

	start = MPI_Wtime();
	// 循环直到系统冷却
	for (double initialTemprature = INITIAL_TEMPERATURE; initialTemprature > 1; initialTemprature *= 1 - coolingRate)
	{

		local_currentSolution = best;
		auto local_current_dis = local_currentSolution.getDistance();
		// parallel calculate
		// openmp parallel part
		auto leftPoint = my_rank*(n / comm_sz), rightPoint = (my_rank + 1)*(n / comm_sz);
		auto tasks = rightPoint - leftPoint;
#pragma omp parallel \
	default(none) shared(local_currentSolution,leftPoint,rightPoint) private(i)
		{
#pragma omp for schedule(static,tasks/thread)
			for (i = omp_get_thread_num()*(tasks/thread); i < (omp_get_thread_num()+1)*(n/thread); ++i)
			{
				// 生成一个邻居
				Tour newSolution(local_currentSolution.getTour(), allCitys);

				// 获得随机位置
				int tourPos1 = abs((int)(rd() % newSolution.getTourSize()));
				int tourPos2 = abs((int)(rd() % newSolution.getTourSize()));

				City citySwap1 = newSolution.getCity(tourPos1);
				City citySwap2 = newSolution.getCity(tourPos2);

				// 交换
				newSolution.setCity(tourPos2, citySwap1);
				newSolution.setCity(tourPos1, citySwap2);

				// 获得新的解决方案的话费
				int currentEnergy = local_currentSolution.getDistance();
				int neighbourEnergy = newSolution.getDistance();
				auto rate = (rd() % 10000)*0.0001;
				auto accept = acceptanceProbability(currentEnergy, neighbourEnergy, initialTemprature);

				// 决定是否用新的方案
				if (accept > rate)
				{
					local_currentSolution.setTour(newSolution.getTour());
				}
			}
		}
		// local_best 重新更新值
		// send and receive to get 
		if (my_rank != 0)
		{
			MPI_Send(&local_current_dis, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			local_current_tour = local_currentSolution.getTourArray();
			MPI_Send(&local_current_tour, test1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		else
		{
			if (local_currentSolution.getDistance()<best.getDistance())
			{
				best = local_currentSolution;
			}

			for (int src = 1; src<comm_sz; ++src)
			{
				MPI_Recv(&local_current_dis, 1, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				if (best.getDistance()>local_current_dis)
				{
					MPI_Recv(&local_current_tour, test1, MPI_INT, src, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
					best.setTourArray(local_current_tour);
				}
			}
		}
	}
	end = MPI_Wtime();
	local_elapsed = end - start;
	MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		cout << "==============================================" << endl;

		cout << "Final solution distance is : " << best.getDistance() << endl;
		cout << "The best Tour is: " << endl;
		auto route = best.getTour();
		for (auto index : route)
		{
			cout << index.getId() << "----->";
		}
		cout << endl;
		cout << "And the time is:" << elapsed << endl;
		cout << "==============================================" << endl;
		cout << endl << endl;
	}

	// end MPI
	MPI_Finalize();
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
