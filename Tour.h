

#ifndef TSP_PROBLEM_SIMULATEDANNEALING_TOUR_H
#define TSP_PROBLEM_SIMULATEDANNEALING_TOUR_H

#include <vector>
#include "city.h"

using namespace std;
#define city_num 20

class Tour
{
public:
	Tour(){}
    Tour(vector<City>);
    Tour(vector<City>, vector<City>);
    vector<City> getTour();
    void generateIndividual();                      
    City getCity(int tourPosition);                 
    void setCity(int tourPosition,City city);
    int getDistance();                              
    int getTourSize();                              
	void setTour(vector<City>);						
	void resetDis();
	void setTourArray(int*);
	int* getTourArray();
private:
    vector<City> tour;  
    int distance;       
	vector<City> allCity;
};


#endif 
