

#include "Tour.h"
#include <stdlib.h>
#include <chrono>
#include <random>
#include <algorithm>
using namespace std;

Tour::Tour(vector<City> tour, vector<City> all)
{
	allCity = all;
    this->tour = tour;
}

Tour::Tour(vector<City> allCities)
{
	allCity = allCities;
    tour.resize(allCity.size());
}

vector<City> Tour::getTour()
{
    return tour;
}

void Tour::generateIndividual()
{
    for(int cityIndex = 0; cityIndex < allCity.size();++cityIndex)
    {
        setCity(cityIndex,allCity[cityIndex]);
    }
    random_shuffle(tour.begin(),tour.end());
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

void Tour::setTour(vector<City> cities)
{
	this->tour = cities;
	resetDis();
}

void Tour::resetDis()
{
	distance = 0;
}

void Tour::setTourArray(int* tourAry)
{
	vector<City> tmp;
	for(auto in =0; in<city_num;++in)
	{
		tmp.push_back(tourAry[in]);
	}
}

int* Tour::getTourArray()
{
	int tourCity[city_num];
	int index = 0;
	for(auto i:tour)
	{
		tourCity[index++] = i.getId();
	}
	return tourCity;
}

int Tour::getDistance()
{
    if(distance==0)
    {
        int tourDistance = 0;
        for(int cityIndex = 0; cityIndex<tour.size();++cityIndex)
        {
            City fromCity = getCity(cityIndex);
            City toCity = getCity(cityIndex);
            cityIndex + 1 < tour.size() ? toCity = getCity(cityIndex + 1) : toCity = getCity(0);
			tourDistance += fromCity.distanceTo(toCity.getId());
        }
        distance = tourDistance;
    }

    return distance;
}















