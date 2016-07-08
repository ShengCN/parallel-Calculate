#ifndef CITY_H
#define CITY_H

#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;
#define test1 20
extern int cityDis[20][20];

class City
{
public:
	City() { id = 0; }
    City(int i)
    {
		id = i;
    }

    int distanceTo(int toId)
    {
		return cityDis[id-1][toId-1];
    }


	int getId() { return id; }

private:
	int id;
};

#endif CITY_H