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

// ������ܵĸ���
double acceptanceProbability(int energy, int newEnergy, double temperature)
{
	if (newEnergy < energy)
		return 1.0;

	return exp((energy - newEnergy) / temperature);
}

void initArray()
{
	ifstream in("tsp��������1.txt", ios::in);
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

// ��ȡ���ƽ�
Tour getApproximateyResult()
{
	// ��ʼ�¶�
	double initialTemprature = 10000;

	//��ȴ����
	double coolingRate = 0.0003;

	srand(time(0));

	// ��ʼ�Ľ������
	Tour* currentSolution = new Tour(allCitys);
	currentSolution->generateIndividual();

	cout << "Initial solution distance: " << currentSolution->getDistance() << endl;

	// ���õ�ǰΪ���ŵķ���
	Tour best(currentSolution->getTour(), allCitys);

	// ѭ��ֱ��ϵͳ��ȴ
	while (initialTemprature>1)
	{
		// ����һ���ھ�
		Tour* newSolution = new Tour(currentSolution->getTour(), allCitys);

		// ������λ��
		int tourPos1 = (int)(rand() % newSolution->getTourSize());
		int tourPos2 = (int)(rand() % newSolution->getTourSize());

		City citySwap1 = newSolution->getCity(tourPos1);
		City citySwap2 = newSolution->getCity(tourPos2);

		// ����
		newSolution->setCity(tourPos2, citySwap1);
		newSolution->setCity(tourPos1, citySwap2);

		// ����µĽ�������Ļ���
		int currentEnergy = currentSolution->getDistance();
		int neighbourEnergy = newSolution->getDistance();
		auto rate = (rand() % 10000)*0.0001;
		auto accept = acceptanceProbability(currentEnergy, neighbourEnergy, initialTemprature);

		// �����Ƿ����µķ���
		if (accept > rate)
		{
			currentSolution->setTour(newSolution->getTour());
		}

		auto getCurrentDis = currentSolution->getDistance();
		auto getBestDis = best.getDistance();

		// ��¼�ҵ������ŷ���
		if (currentSolution->getDistance() < best.getDistance())
		{
			best.setTour(currentSolution->getTour());
		}

		// ��ȴ
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
//	cout << "��������µĽ���ǣ� " << bestCaseDis << endl;
	initCity();
	Tour best = getApproximateyResult();
	cout << "Final solution distance is : " << best.getDistance() << endl;

	auto route = best.getTour();
	for (auto index : route)
	{
		cout << index.getId() << "----->";
	}
	cout << endl;
	cout <<"�˻��㷨�ó������Ž���ǣ� "<< best.getDistance() << endl;
	cout << endl;
	system("pause");
}