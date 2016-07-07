#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "Tour.h"

using namespace std;

// 计算接受的概率
double acceptanceProbability(int energy,int newEnergy, double temperature)
{
    if(newEnergy < energy)
        return 1.0;

    return exp((energy - newEnergy)/temperature);
}

// 获取近似解
Tour getApproximateyResult()
{
    // 初始温度
    double initialTemprature = 10000;

    // 初始的解决方案
    Tour* currentSolution = new Tour();
    currentSolution->generateIndividual();

    cout<<"Initial solution distance: "<<currentSolution->getDistance()<<endl;

    // 设置当前为最优的方案
    Tour* best = new Tour(currentSolution->getTour());

    // 循环直到系统冷却
    while(initialTemprature>1)
    {
        // 生成一个邻居
        Tour* newSolution = new Tour(currentSolution->getTour());

        // 获得随机位置
        int tourPos1 = (int) (newSolution->getTourSize() * random());
        int tourPos2 = (int) (newSolution->getTourSize() * rand());

        City citySwap1 = newSolution->getCity(tourPos1);
        City citySwap2 = newSolution->getCity(tourPos2);

        // 交换
        newSolution->setCity(tourPos2,citySwap1);
        newSolution->setCity(tourPos1,citySwap2);

        // 获得新的解决方案的话费
        int currentEnergy = currentSolution->getDistance();
        int neighbourEnergy = newSolution->getDistance();

        // 决定是否用新的方案


    }

}

int main()
{

}