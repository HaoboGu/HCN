#include "DataStructure.h"

double HCnetwork::FACTOR = 0.83;//越接近1，周围人的影响越小
int HCnetwork::MONTH_TO_PROMOTE = 2;
double HCnetwork::RATE_AFTER_PROMOTION = 0.01;
double HCnetwork::RATE_OF_NEW = 0.01;


double getRan()
{
	return (rand()/double(RAND_MAX));
}
Lvl getLevel(int i)
{
	//Just for processing raw data
	switch(i)
	{
	case(1):
		return SeniorManager;
		break;
	case(2):
		return JuniorManager;
		break;
	case(3):
		return ExperiencedSupervisor;
		break;
	case(4):
		return InexperiencedSupervisor;
		break;
	case(5):
		return ExperiencedEmployee;
		break;
	case(6):
		return InexperiencedEmployee;
		break;
	case(7):
		return AdministrativeClerk;
		break;
	}
	return UnknownLvl;
}
double Sigmoid(double x)
{
	double y = 2/(1+exp(-x));
	return y;
}

/************************************************************************/
/* Functions of class HCnetwork                                         */
/************************************************************************/
void HCnetwork::addPosition(int dep, Lvl l, int sly, double rate)
{
	pos[N].department = dep;
	pos[N].level = l;
	pos[N].salary = sly;
	pos[N].churnrate = rate;
	pos[N].workingTime = 0;
	pos[N].num_connected = 0;
	pos[N].status = occupied;
	pos[N].ability = pos[N].computeAbility();
	for(int i = 0; i < N; i++)//遍历现在的网络
	{
		if(pos[i].department == dep || pos[i].getSuperior() == dep || pos[N].getSuperior() == pos[i].department)//In the same department or Superior or Inferior
		{	
			double p = getRan();
			//if(p<0.2)
			{
				pos[i].connected[pos[i].num_connected] = N;
				pos[i].num_connected++;//number of nodes connected
				pos[N].connected[pos[N].num_connected] = i;
				pos[N].num_connected++;
			}
		
		}
	}
	N = N + 1;
}
void HCnetwork::clear(int i)
{
	//Clear data of position i
	pos[i].churnrate = 0;
	//pos[i].salary = 0;
	//pos[i].num_connected = 0;
	pos[i].workingTime = 0;
	pos[i].status = vacant;
	pos[i].ability = 0;
}
void HCnetwork::leave(int *index, int &NumOfDim)
{
	//index is index of people who are left, NumOfDim is number of people who are left.
	//int NumOfDim = 0;
	//int index[370];
	NumOfDim = 0;
	for(int i = 0; i < N; i++)
	{
		double r = getRan();
		if(r < pos[i].churnrate)//find who will leave
		{
			index[NumOfDim] = i;
			NumOfDim++;
		}	
	}
	for(int k = 0; k < NumOfDim; k++)
	{
		int i = index[k];
		for(int j = 0; j < pos[i].num_connected; j++)
		{
			int idx = pos[i].connected[j];//idx of node which connected to the leaving node
			pos[idx].churnrate = 1 - FACTOR*(1-pos[idx].churnrate);
		}
		clear(i);
	}
}
void HCnetwork::promote()
{
	for(int i=0; i<N; i++)
	{
		if(pos[i].status == vacant)//the position is vacant
		{
			int j;
			for(j=0; j<N; j++)//同部门晋升
			{
				if(pos[j].department == pos[i].department && pos[j].workingTime>=MONTH_TO_PROMOTE)//workingTime>=2 and in the same dep
				{
					if(pos[i].level == pos[j].getNextLevel() && pos[j].status == occupied)
					{
						//pos[i].churnrate = pos[j].churnrate;
						pos[i].churnrate = RATE_AFTER_PROMOTION;
						pos[i].status = occupied;
						pos[j].status = vacant;
						pos[i].workingTime = 0;
						clear(j);
						break;
					}			
				}
			}
			if(j==N)//同部门没有满足条件的,则从不同部门选择满足条件的提升
			{
				for(int j=0; j<N; j++)
				{
					if(pos[i].level == pos[j].getNextLevel() && pos[j].workingTime>=MONTH_TO_PROMOTE && pos[j].status == occupied)
					{
						//pos[i].churnrate = pos[j].churnrate;
						pos[i].churnrate = RATE_AFTER_PROMOTION;
						pos[i].status = occupied;
						pos[j].status = vacant;
						pos[i].workingTime = 0;
						clear(j);
						break;
					}			
				}
			}
		}
	}
}
double HCnetwork::hire()
{
	//return cost of recruit this turn
	double budget = 0;
	for(int i = 0; i<N; i++)
	{
		double r = getRan();
		if(pos[i].status == vacant && r < 0.6667)//该职位空缺,只招2/3的人
		{
			double p = pos[i].getHiringProbability();//招到人的概率
			double r = getRan();
			if(r<=1)
			{
				pos[i].status = occupied;
				pos[i].churnrate = RATE_OF_NEW;
				pos[i].salary = 1000;
				pos[i].workingTime = 0;
				pos[i].ability = pos[i].computeAbility();
				budget = budget + pos[i].hiringCost();
			}
		}
	}
	return budget;
}
void HCnetwork::print()
{
	for(int i = 0; i < N; i++)
	{
		cout<<"index:"<<i<<" dep:"<<pos[i].department<<" level:"<<pos[i].level<<" status:"<<pos[i].status<<" crate:"<<pos[i].churnrate<<endl;
	}
}
void HCnetwork::input()
{
	ifstream fin("node.txt", ios::in);   
	if(fin.is_open())
	{
		int index;
		int dep;
		int l;
		double p;
		for(int i = 0; i < 370; i++)
		{
			fin>>index>>dep>>l>>p;
			Lvl lev = getLevel(l);
			/*Adjust churn rate */
			if(lev == JuniorManager || lev == ExperiencedSupervisor)
				p = 0.3;
			else p = 0.18;
			/*Just for task 5*/
			addPosition(dep, lev, 100, p);
		}
	}
	fin.close();
}
void HCnetwork::initial()
{
	int i;
	int a[370];
	for(i=0; i<370; ++i) 
		a[i]=i;
	for(i=369; i>=1; --i) 
		swap(a[i], a[rand()%i]);
	for(i = 0; i<N; i++)
	{
		if(a[i]<=55)
		{
			clear(i);
		}
	}
}
double HCnetwork::nextMonth(int *VacantAfterLeaving, int *VacantAfterHiring)
{
	//return costs
	int n = 0;
	int index[370];
	int NumOfDim;
	leave(index, NumOfDim);
	//输出离职人的标号
	/*ofstream fout("IndexOfDimssion.txt", ios::app);
	for(int i = 0; i < NumOfDim; i++)
	{
		fout<<index[i]<<" ";
	}
	fout<<endl;
	fout.close();*/
	for(int i = 0; i < N; i++)
	{
		if(pos[i].status == vacant)
		{
			n++;
		}
	}
	VacantAfterLeaving[t] = n;
	n = 0;
	promote();
	double hiringCosts;
	//hiringCosts = hire();
	refreshAbility();
	refreshCRate();
	double trainingCosts = trainingCost();
	for(int i = 0; i < N; i++)
	{

		if(pos[i].status == vacant)
		{
			n++;
		}
	}
	VacantAfterHiring[t] = n;
	n = 0;
	for(int i = 0; i < N; i++)
	{
		if(pos[i].status == occupied)
			pos[i].workingTime++;
	}
	t++;
	return 0;//(hiringCosts+trainingCosts);
}
void HCnetwork::refreshAbility ()
{
	double ab[7] = {0,0,0,0,0,0,0};
	int num[7] = {0,0,0,0,0,0,0};
	Lvl l;
	for(int i = 0; i < N; i++)
	{
		if(pos[i].status == occupied)
		{
			l = pos[i].level;//0:highest position, 6:lowest position
			ab[l] = ab[l] + pos[i].ability;
			num[l]++;
		}
	}
	for(int i = 0; i < 7; i ++)
	{
		ab[i] = (double)ab[i]/num[i];
		avAbility[i] = ab[i];
	}
}
void HCnetwork::refreshCRate()
{
	for(int i = 0; i < N; i++)
	{
		if(pos[i].status == occupied)
		{
			pos[i].refreshChurnRate(avAbility[pos[i].level]);
		}
	}
}
double HCnetwork::trainingCost()
{
	double costs = 0;
	for(int i = 0; i < N; i++)
	{
		if(pos[i].status == occupied)
			costs = costs + pos[i].training();
	}
	return costs;
}

/************************************************************************/
/* Functions of class node                                              */
/************************************************************************/
Lvl node::getNextLevel()
{
	switch(level)
	{
	case(AdministrativeClerk):
		return InexperiencedEmployee;
		break;
	case(InexperiencedEmployee):
		return ExperiencedEmployee;
		break;
	case(ExperiencedEmployee):
		return InexperiencedSupervisor;
		break;
	case(InexperiencedSupervisor):
		return ExperiencedSupervisor;
		break;
	case(ExperiencedSupervisor):
		return JuniorManager;
		break;
	case(JuniorManager):
		return SeniorManager;
		break;
	case(SeniorManager):
		return SeniorManager;
		break;
	}
	return UnknownLvl;
}
Lvl node::getPreviouLevel()
{
	switch(level)
	{
	case(AdministrativeClerk):
		return AdministrativeClerk;
		break;
	case(InexperiencedEmployee):
		return AdministrativeClerk;
		break;
	case(ExperiencedEmployee):
		return InexperiencedEmployee;
		break;
	case(InexperiencedSupervisor):
		return ExperiencedEmployee;
		break;
	case(ExperiencedSupervisor):
		return InexperiencedSupervisor;
		break;
	case(JuniorManager):
		return ExperiencedSupervisor;
		break;
	case(SeniorManager):
		return JuniorManager;
		break;
	}
	return UnknownLvl;
}
double node::getHiringProbability()
{
	switch(level)
	{
	case AdministrativeClerk:return 0.293;break;
	case InexperiencedEmployee:return 0.5;break;
	case ExperiencedEmployee:return 0.206;break;
	case InexperiencedSupervisor:return 0.159;break;
	case ExperiencedSupervisor:return 0.129;break;
	case JuniorManager:return 0.109;break;
	case SeniorManager:return 0.094;break;
	default:return 0;
	}
	return 0;//represents unknown level
}
int node::getSuperior()
{
	int Superior[59] = {0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,8,8,8,8,9,9,9,9,10,10,10,10,10,10,17,17,17,18,18,18,29,29,29,29,30,30,30,30,31,31,31,31,31,31,32,32,33,33,33,33,34,34,34,34};
	int sup;
	sup = Superior[department];
	return sup;
}
double node::computeAbility()
{
	double p = getRan();
	switch(level)
	{
	case(AdministrativeClerk):
		return 1.8*p;//0-1.8, av:0.9
		break;
	case(InexperiencedEmployee):
		return 1.8*p;
		break;
	case(ExperiencedEmployee):
		return 2*p;//0-2, av:1
		break;
	case(InexperiencedSupervisor):
		return 2*p+0.5;//0.5-2.5, av:1.5
		break;
	case(ExperiencedSupervisor):
		return 2*p+1;//1-3, av:2
		break;
	case(JuniorManager):
		return 4*p+2;//2-6, av:4
		break;
	case(SeniorManager):
		return 8*p+4;//4-12, av:8
		break;
	default:
		return 0;
	}
	return 0;

}
void node::refreshChurnRate(double avAbility)
{
	double x = (ability - avAbility)/avAbility;
	double y = Sigmoid(x);
	if(x > 0)
		churnrate = 1 - (2 - y) * (1 - churnrate);
	else if (x < 0)
		churnrate = y * churnrate;
}
double node::hiringCost()
{
	double cost[7] = {1.2, 0.7, 0.6, 0.6, 0.3, 0.1, 0.3};
	return cost[level];
}
double node::training()
{
	double cost[7] = {0.5, 0.6, 0.2, 0.3, 0.1, 0.3, 0.05};
	return cost[level];
}