#ifndef CITY_H
#define CITY_H

#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;

class City
{
public:
    City(int x,int y)
    {
        this->x = x;
        this->y = y;
    }

    City()
    {
		srand(time(0));
        x = rand()%200;
        y = rand()%200;
    }

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

    double distanceTo(City city)
    {
        int xDistance = abs(city.getX() - x);
        int yDistance = abs(city.getY() - y);
        return sqrt((xDistance*xDistance)+(yDistance*yDistance));
    }

private:
    int x;
    int y;
};


#endif CITY_H