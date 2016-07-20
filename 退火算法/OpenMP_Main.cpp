#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <random>
#include <omp.h>
#include <math.h>
#include "city.h"
#include "Tour.h"
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
	// 冷却概率
	double coolingRate = 0.3;
	int n = 100000;

	// 初始的解决方案
	Tour currentSolution(allCitys);
	currentSolution.generateIndividual();

	cout<<endl;
	cout<<endl;
	std::cout << "Initial solution distance: " << currentSolution.getDistance() << endl;

	// 设置当前为最优的方案
	Tour best(currentSolution.getTour(), allCitys);

		vector<Tour> results(thread, best);
		auto local_currentSolution = best;
		auto local_best = best;
		int i;

		omp_set_num_threads(thread);

		// time begin
		double start,finish;
		start = omp_get_wtime();

		// 循环直到系统冷却
		for (double initialTemprature = INITIAL_TEMPERATURE; initialTemprature > 1; initialTemprature *= 1 - coolingRate)
		{
#pragma omp parallel  \
	default(none) shared(results,thread,n,best,allCitys,rd,initialTemprature) private(local_currentSolution,local_best,i)	
		{
			local_currentSolution = best;
#pragma omp for schedule(static,n/thread)
		for (i = 0; i < n; ++i)
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

				auto getCurrentDis = local_currentSolution.getDistance();
				// 记录找到的最优方案
				if (best.getDistance() > getCurrentDis)
				{
#pragma omp critical
					best.setTour(local_currentSolution.getTour());
				}
			}
		}

		// local_best 重新更新值
		for (auto in : results)
		{
			if (best.getDistance() > in.getDistance())
			{
				best = in;
			}
		}
		local_currentSolution = best;
		local_best = best;
		}
		finish = omp_get_wtime();
				

		cout <<"=============================================="<< endl;
		cout << "When thread is: " << thread << endl;
		cout << "Final solution distance is : " << best.getDistance() << endl;
		cout << "The best Tour is: " << endl;
		auto route = best.getTour();
		for (auto index : route)
		{
			cout << index.getId() << "----->";
		}
		cout << endl;
		cout<<"And the time is:"<<(finish - start)<<endl;
		cout<<"=============================================="<<endl;
		cout<<endl<<endl;

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

int main(int arvc,char* argv[])
{
	initCity();
	Tour best(allCitys);
	int thread_count = strtol(argv[1],NULL,10);
	getApproximateyResult(thread_count);
}
