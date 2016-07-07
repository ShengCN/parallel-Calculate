//
// Created by CodingBlack on 16/7/6.
//

#include "Tour.h"
#include "SimulatedAnnealing.h"
#include <stdlib.h>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;

Tour::Tour(vector<City> tour)
{
    this->tour = tour;
}

Tour::Tour()
{
    tour.resize(SimulatedAnnealing::allCitys.size());
}

vector<City> Tour::getTour()
{
    return tour;
}

//产生随机个体
void Tour::generateIndividual()
{
    for(int cityIndex = 0; cityIndex < SimulatedAnnealing::allCitys.size();++cityIndex)
    {
        setCity(cityIndex,SimulatedAnnealing::allCitys[cityIndex]);
    }
    unsigned seed = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    shuffle(tour.begin(),tour.end(),seed);
}

City Tour::getCity(int tourPosition)
{
    return tour[tourPosition];
}

void Tour::setCity(int tourPosition, City city)
{
    tour[tourPosition] = city;
    distance = 0;
}

int Tour::getTourSize()
{
    return (int) tour.size();
}

int Tour::getDistance()
{
    if(distance==0)
    {
        int tourDistance = 0;
        for(int cityIndex = 0; cityIndex<tour.size();++cityIndex)
        {
            City fromCity = getCity(cityIndex);
            City toCity;
            cityIndex + 1 < tour.size() ? toCity = getCity(cityIndex + 1) : toCity = getCity(0);
        }
        distance = tourDistance;
    }

    return distance;
}















