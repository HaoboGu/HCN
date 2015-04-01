#ifndef _DataStructure_h
#define _DataStructure_h
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;
enum Lvl{
	SeniorManager,
	JuniorManager,
	ExperiencedSupervisor,
	InexperiencedSupervisor,
	ExperiencedEmployee,
	InexperiencedEmployee,
	AdministrativeClerk,
	UnknownLvl
};
enum Status{
	occupied,
	vacant,
	dimssion
};
class node{
public: 
	int department;
	Lvl level;
	int workingTime;//working time at this position
	int salary;
	double churnrate;//possibility of turnover
	int connected[370];
	int num_connected;
	Status status;//status of the position
	double ability;
public:
	int getSuperior();//department
	int getInferior();
	Lvl getNextLevel();
	Lvl getPreviouLevel();
	double getHiringProbability();
	double computeAbility();
	void refreshChurnRate(double avAbility);
	double hiringCost();
	double training();
};
class HCnetwork{
public:
	node *pos;
	int N;//总职位数
	int t;//总时间
	double *avAbility;
	static double FACTOR;
	static int MONTH_TO_PROMOTE;
	static double RATE_AFTER_PROMOTION;
	static double RATE_OF_NEW;
public:
	HCnetwork(){N=0;pos = new node[370];t=0;avAbility = new double[7];};
	~HCnetwork(){delete []pos;}
	void addPosition(int dep, Lvl l, int sly, double rate);
	void leave(int *index, int &NumOfDim);//dimission
	void promote();//promotion
	double hire();
	void clear(int i);
	void print();
	void input();
	double nextMonth(int *VacantAfterLeaving, int *VacantAfterHiring);
	void initial();
	void refreshAbility();
	void refreshCRate();
	double trainingCost();
};
#endif