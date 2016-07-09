#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include "../../Parallel/city.h"
#include "../../Parallel/Tour.h"
using namespace std;

int cityDis[20][20];
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
		for (int i = 0; i < 20; ++i)
		{
			for (int j = 0; j < 20; ++j)
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
Tour getApproximateyResult()
{
	// 初始温度
	double initialTemprature = 10000;

	//冷却概率
	double coolingRate = 0.0003;

	srand(time(0));

	// 初始的解决方案
	Tour* currentSolution = new Tour(allCitys);
	currentSolution->generateIndividual();

	cout << "Initial solution distance: " << currentSolution->getDistance() << endl;

	// 设置当前为最优的方案
	Tour best(currentSolution->getTour(), allCitys);

	// 循环直到系统冷却
	while (initialTemprature>1)
	{
		// 生成一个邻居
		Tour* newSolution = new Tour(currentSolution->getTour(), allCitys);

		// 获得随机位置
		int tourPos1 = (int)(rand() % newSolution->getTourSize());
		int tourPos2 = (int)(rand() % newSolution->getTourSize());

		City citySwap1 = newSolution->getCity(tourPos1);
		City citySwap2 = newSolution->getCity(tourPos2);

		// 交换
		newSolution->setCity(tourPos2, citySwap1);
		newSolution->setCity(tourPos1, citySwap2);

		// 获得新的解决方案的话费
		int currentEnergy = currentSolution->getDistance();
		int neighbourEnergy = newSolution->getDistance();
		auto rate = (rand() % 10000)*0.0001;
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

		// 冷却
		initialTemprature *= 1 - coolingRate;
		delete newSolution;
	}
	delete currentSolution;
	return best;
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

//int getBastCase()
//{
//	for(auto i:bestCase)
//	{
//		City city(i);
//		allCitys.push_back(city);
//	}
//	Tour best(allCitys);
//	return best.getDistance();
//}

int main()
{
//	auto bestCaseDis = getBastCase();
//	cout << "最优情况下的结果是： " << bestCaseDis << endl;
	initCity();
	Tour best = getApproximateyResult();
	cout << "Final solution distance is : " << best.getDistance() << endl;

	auto route = best.getTour();
	for (auto index : route)
	{
		cout << index.getId() << "----->";
	}
	cout << endl;
	cout <<"退火算法得出的最优结果是： "<< best.getDistance() << endl;
	cout << endl;
	system("pause");
}