//
// Created by CodingBlack on 16/7/6.
//

#ifndef TSP_PROBLEM_SIMULATEDANNEALING_TOUR_H
#define TSP_PROBLEM_SIMULATEDANNEALING_TOUR_H

#include <vector>
#include "city.h"

using namespace std;

class Tour
{
public:
    Tour(vector<City>);
    Tour(vector<City>, vector<City>);
    vector<City> getTour();
    void generateIndividual();                      //建立随机个体
    City getCity(int tourPosition);                 //获取城市
    void setCity(int tourPosition,City city);
    int getDistance();                              //获取当前距离的总长度
    int getTourSize();                              //获取当前路径城市数量
	void setTour(vector<City>);						//设置当前路线
	void resetDis();
private:
    vector<City> tour;  //经过的所有城市
    int distance;       //距离
	vector<City> allCity;
};


#endif //TSP_PROBLEM_SIMULATEDANNEALING_TOUR_H
