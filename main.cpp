#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "Tour.h"
#include "SimulatedAnnealing.h"
using namespace std;

vector<City> allCitys;

// 计算接受的概率
double acceptanceProbability(int energy,int newEnergy, double temperature)
{
    if(newEnergy < energy)
        return 1.0;

    return exp((energy - newEnergy)/temperature);
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
	City city(60, 200);
	allCitys.push_back(city);

    City city2(180, 200);
	allCitys.push_back(city2);

    City city3(80, 180);
	allCitys.push_back(city3);

    City city4(140, 180);
	allCitys.push_back(city4);

    City city5(20, 160);
	allCitys.push_back(city5);

    City city6(100, 160);
	allCitys.push_back(city6);

    City city7(200, 160);
    allCitys.push_back(city7);

    City city8(140, 140);
    allCitys.push_back(city8);

    City city9(40, 120);
	allCitys.push_back(city9);

    City city10(100, 120);
	allCitys.push_back(city10);

    City city11(180, 100);
	allCitys.push_back(city11);

    City city12(60, 80);
	allCitys.push_back(city12);

    City city13(120, 80);
	allCitys.push_back(city13);

    City city14(180, 60);
	allCitys.push_back(city14);

    City city15(20, 40);
	allCitys.push_back(city15);

	City city16(100, 40);
	allCitys.push_back(city16);
	
	City city17(200, 40);
	allCitys.push_back(city17);
	
	City city18(20, 20);
	allCitys.push_back(city18);
	
	City city19(60, 20);
	allCitys.push_back(city19);
	
	City city20(160, 20);
	allCitys.push_back(city20);
}


int main()
{
	initCity();
	Tour* best = getApproximateyResult();
	cout << "Final solution distance is : " << best->getDistance() << endl;
	
	auto route = best->getTour();
	for(auto index:route)
	{
		cout << index.getX() << "|" << index.getY() << "\t";
	}

	system("pause");
}