#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <random>
#include <omp.h>
#include "../../Parallel/city.h"
#include "../../Parallel/Tour.h"
using namespace std;
#define INITIAL_TEMPERATURE 1000
#define test1 20

int cityDis[test1][test1];
vector<City> allCitys;
using namespace std;

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
void getApproximateyResult()
{
	random_device rd;
	// 冷却概率
	double coolingRate = 0.003;
	int n = 10;

	// 初始的解决方案
	Tour* currentSolution = new Tour(allCitys);
	currentSolution->generateIndividual();

	cout << "Initial solution distance: " << currentSolution->getDistance() << endl;

	// 设置当前为最优的方案
	Tour best(currentSolution->getTour(), allCitys);
	//	for (int num_threads = 1; num_threads <= 64;num_threads *= 4)
	//	{
	//		vector<Tour> results(num_threads, best);
	//		omp_set_num_threads(num_threads);
			// 循环直到系统冷却
	for (double initialTemprature = INITIAL_TEMPERATURE; initialTemprature > 1; initialTemprature *= 1 - coolingRate)
	{
		for (int i = 0; i < n; ++i)
		{
			// 生成一个邻居
			Tour* newSolution = new Tour(currentSolution->getTour(), allCitys);

			// 获得随机位置
			int tourPos1 = (int)(rd() % newSolution->getTourSize());
			int tourPos2 = (int)(rd() % newSolution->getTourSize());

			City citySwap1 = newSolution->getCity(tourPos1);
			City citySwap2 = newSolution->getCity(tourPos2);

			// 交换
			newSolution->setCity(tourPos2, citySwap1);
			newSolution->setCity(tourPos1, citySwap2);

			// 获得新的解决方案的话费
			int currentEnergy = currentSolution->getDistance();
			int neighbourEnergy = newSolution->getDistance();
			auto rate = (rd() % 10000)*0.0001;
			auto accept = acceptanceProbability(currentEnergy, neighbourEnergy, initialTemprature);

			// 决定是否用新的方案
			if (accept > rate)
			{
				currentSolution->setTour(newSolution->getTour());
			}

			auto getCurrentDis = currentSolution->getDistance();
			auto getBestDis = best.getDistance();

			// 记录找到的最优方案
			if (currentSolution->getDistance() < best.getDistance())
			{
				best.setTour(currentSolution->getTour());
			}
			delete newSolution;
		}


		// 打印结果
//		cout << "当" << num_threads << "线程的时候，最终结果是：" << endl;
	}
	cout << "Final solution distance is : " << best.getDistance() << endl;
	cout << "最优路径是:" << endl;
	auto route = best.getTour();
	for (auto index : route)
	{
		cout << index.getId() << "----->";
	}
	cout << endl;
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

int main()
{
	initCity();
	Tour best(allCitys);
	getApproximateyResult();
	
	system("pause");
}