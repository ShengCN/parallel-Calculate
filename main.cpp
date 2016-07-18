#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <random>
#include <omp.h>
#include "../../Parallel/city.h"
#include "../../Parallel/Tour.h"
using std::cout;
using std::fstream;
using std::endl;
using std::vector;
#define INITIAL_TEMPERATURE 1000
#define test1 20

int cityDis[test1][test1];
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

// ��ȡ���ƽ�
void getApproximateyResult()
{
	random_device rd;
	// ��ȴ����
	double coolingRate = 0.003;
	int n = 64;

	// ��ʼ�Ľ������
	Tour currentSolution(allCitys);
	currentSolution.generateIndividual();

	std::cout << "Initial solution distance: " << currentSolution.getDistance() << endl;

	// ���õ�ǰΪ���ŵķ���
	Tour best(currentSolution.getTour(), allCitys);

	// ���б���
	for (int thread_count = 1; thread_count <= 64; thread_count *= 4)
	{
		vector<Tour> results(thread_count, best);
		auto local_currentSolution = best;
		auto local_best = best;
		int i;

		// ѭ��ֱ��ϵͳ��ȴ
		for (double initialTemprature = INITIAL_TEMPERATURE; initialTemprature > 1; initialTemprature *= 1 - coolingRate)
		{
#pragma omp parallel num_threads(thread_count) \
	default(none) shared(results,thread_count,n,best) private(local_currentSolution,local_best,i)	
		{
			local_currentSolution = best;
			local_best = best;
#pragma omp for schedule(static,1)
			for (i = 0; i <n ; ++i)
			{
				// ����һ���ھ�
				Tour newSolution(local_currentSolution.getTour(), allCitys);

				// ������λ��
				int tourPos1 = (int)(rd() % newSolution.getTourSize());
				int tourPos2 = (int)(rd() % newSolution.getTourSize());

				City citySwap1 = newSolution.getCity(tourPos1);
				City citySwap2 = newSolution.getCity(tourPos2);

				// ����
				newSolution.setCity(tourPos2, citySwap1);
				newSolution.setCity(tourPos1, citySwap2);

				// ����µĽ�������Ļ���
				int currentEnergy = local_currentSolution.getDistance();
				int neighbourEnergy = newSolution.getDistance();
				auto rate = (rd() % 10000)*0.0001;
				auto accept = acceptanceProbability(currentEnergy, neighbourEnergy, initialTemprature);

				// �����Ƿ����µķ���
				if (accept > rate)
				{
					local_currentSolution.setTour(newSolution.getTour());
				}

				auto getCurrentDis = local_currentSolution.getDistance();
				auto getBestDis = local_best.getDistance();

				// ��¼�ҵ������ŷ���
				if (local_currentSolution.getDistance() < local_best.getDistance())
				{
					local_best.setTour(local_currentSolution.getTour());
				}
			}

			auto thread_index = omp_get_thread_num();
			results[thread_index] = local_best;
		}

		// local_best ���¸���ֵ
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
		
		cout <<"=============================================="<< endl;
		cout << "When thread is: " << thread_count << endl;
		cout << "Final solution distance is : " << best.getDistance() << endl;
		cout << "����·����:" << endl;
		auto route = best.getTour();
		for (auto index : route)
		{
			cout << index.getId() << "----->";
		}
		cout << endl;
	}
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