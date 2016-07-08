#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "Tour.h"
#include "SimulatedAnnealing.h"
#include <fstream>
using namespace std;

int cityDis[20][20];
vector<City> allCitys;

// 计算接受的概率
double acceptanceProbability(int energy,int newEnergy, double temperature)
{
    if(newEnergy < energy)
        return 1.0;

    return exp((energy - newEnergy)/temperature);
}

void initArray()
{
	ifstream in("tsp测试数据1.txt",ios::in);
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
Tour* getApproximateyResult()
{
    // 初始温度
    double initialTemprature = 10000;

	//冷却概率
	double coolingRate = 0.003;

	srand(time(0));

    // 初始的解决方案
    Tour* currentSolution = new Tour(allCitys);
    currentSolution->generateIndividual();

    cout<<"Initial solution distance: "<<currentSolution->getDistance()<<endl;

    // 设置当前为最优的方案
    Tour* best = new Tour(currentSolution->getTour(),allCitys);

    // 循环直到系统冷却
    while(initialTemprature>1)
    {
        // 生成一个邻居
        Tour* newSolution = new Tour(currentSolution->getTour(),allCitys);

        // 获得随机位置
        int tourPos1 = (int) (rand()%(newSolution->getTourSize()));
        int tourPos2 = (int) (rand()%(newSolution->getTourSize()));

        City citySwap1 = newSolution->getCity(tourPos1);
        City citySwap2 = newSolution->getCity(tourPos2);

        // 交换
        newSolution->setCity(tourPos2,citySwap1);
        newSolution->setCity(tourPos1,citySwap2);

        // 获得新的解决方案的话费
        int currentEnergy = currentSolution->getDistance();
        int neighbourEnergy = newSolution->getDistance();

        // 决定是否用新的方案
		if (acceptanceProbability(currentEnergy, neighbourEnergy, initialTemprature) > rand())
		{
			currentSolution = new Tour(newSolution->getTour());
		}

		// 记录找到的最优方案
		if(currentSolution->getDistance() < best->getDistance())
		{
			best = new Tour(currentSolution->getTour());
		}

		// 冷却
		initialTemprature *= 1 - coolingRate;
    }
	return best;
}

void initCity()
{
	initArray();

	for(int i = 1;i <= 20; ++i)
	{
		City city(i);
		allCitys.push_back((city));
	}

}


int main()
{
	initCity();
	Tour* best = getApproximateyResult();
	cout << "Final solution distance is : " << best->getDistance() << endl;
	
	auto route = best->getTour();
	for(auto index:route)
	{
		cout << index.getId() << "----->";
	}

	cout << endl;
	system("pause");
}