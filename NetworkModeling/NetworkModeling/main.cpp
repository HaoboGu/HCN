#include "DataStructure.h"

void scan(double *churnrate, double *VacantPosition, double factor)
{
	//根据给定的factor对RATE_OF_NEW进行扫描，得出churnrate和VacantPosition的两组数据
	HCnetwork::FACTOR = factor;
	HCnetwork::RATE_AFTER_PROMOTION = 0;
	HCnetwork::RATE_OF_NEW = 0;
	
	for(int k = 0; k <= 20; k++)
	{
		HCnetwork net;
		net.input();
		net.initial();
		int n = 0;
		int VacantAfterLeaving[1000];
		int VacantAfterHiring[1000];
		int idx = 0;
		double a = 0;
		double b = 0;
		for(int i = 0; i<30; i++)//30 years
		{	
			net.nextMonth(VacantAfterLeaving, VacantAfterHiring);
			if(i>=5)//stable after 5 years
			{
				a = a + (double)(VacantAfterLeaving[i] - VacantAfterHiring[i-1])/(370-VacantAfterHiring[i-1]);
				b = b + (double)VacantAfterHiring[i];
				//fout<<VacantAfterLeaving[i]<<' '<<VacantAfterHiring[i]<<" "<<a<<endl;
			}
			//cout<<"afterLeaving:"<<VacantAfterLeaving[idx]<<" afterHiring:"<<VacantAfterHiring[idx]<<endl;
			idx++;	
		}
		a = a/25;
		b = b/25;
		churnrate[k] = a;
		VacantPosition[k] = b;
		HCnetwork::RATE_AFTER_PROMOTION = HCnetwork::RATE_AFTER_PROMOTION + 0.01;
		HCnetwork::RATE_OF_NEW = HCnetwork::RATE_AFTER_PROMOTION + 0.01;
	}
}
void scan1(double *churnrate, double *VacantPosition, double rate)
{
	//根据给定的rate对Factor进行扫描，得出churnrate和VacantPosition的两组数据
	HCnetwork::FACTOR = 0.6;//0.6 - 1
	HCnetwork::RATE_AFTER_PROMOTION = rate;
	HCnetwork::RATE_OF_NEW = rate;

	for(int k = 0; k <= 20; k++)
	{
		HCnetwork net;
		net.input();
		net.initial();
		int n = 0;
		int VacantAfterLeaving[1000];
		int VacantAfterHiring[1000];
		int idx = 0;
		double a = 0;
		double b = 0;
		for(int i = 0; i<30; i++)//30 years
		{	
			net.nextMonth(VacantAfterLeaving, VacantAfterHiring);
			if(i>=5)//stable after 5 years
			{
				a = a + (double)(VacantAfterLeaving[i] - VacantAfterHiring[i-1])/(370-VacantAfterHiring[i-1]);
				b = b + (double)VacantAfterHiring[i];
				//fout<<VacantAfterLeaving[i]<<' '<<VacantAfterHiring[i]<<" "<<a<<endl;
			}
			//cout<<"afterLeaving:"<<VacantAfterLeaving[idx]<<" afterHiring:"<<VacantAfterHiring[idx]<<endl;
			idx++;	
		}
		a = a/25;
		b = b/25;
		churnrate[k] = a;
		VacantPosition[k] = b;
		HCnetwork::FACTOR = HCnetwork::FACTOR + 0.02;
	}
}
int main()
{

	//one turn, a typical situation with default parameters
	HCnetwork net;
	HCnetwork::FACTOR = 0.83;
	HCnetwork::RATE_AFTER_PROMOTION = 0.01;
	HCnetwork::RATE_OF_NEW = 0.01;
	HCnetwork::MONTH_TO_PROMOTE = 1;
	net.input();
	net.initial();
	int n = 0;
	int VacantAfterLeaving[1000];
	int VacantAfterHiring[1000];
	int idx = 0;
	double a[50];
	double Pro[50];//Productivity of every year
	int years = 10;
	for(int i = 0; i < years; i++)
		Pro[i] = 0;
	/*ofstream fout("IndexOfDimssion.txt", ios::out);
	fout.close();//clear content in the file, because in nextMonth(), we add contents rather than wipe the file
	*/
	for(int i = 0; i<years; i++)//i:year
	{	
		double budget = net.nextMonth(VacantAfterLeaving, VacantAfterHiring);
		for(int j = 0; j < net.N; j++)
		{
			if(net.pos[j].status == occupied)
				Pro[i] = Pro[i] + net.pos[j].ability;
		}
		cout<<"afterLeaving:"<<VacantAfterLeaving[idx]<<" afterHiring:"<<VacantAfterHiring[idx]<<endl;
		if(i>=1)
		{
			a[i] = (double)(VacantAfterLeaving[i] - VacantAfterHiring[i-1])/(370-VacantAfterHiring[i-1]);
		}
		idx++;	
	}
	double suma = 0;
	for(int i = 5; i < years; i++)
	{
		suma = suma + a[i];
	}
	suma = suma/(years - 5);
	double sumb = 0;
	for(int i = 5; i < years; i++)
	{
		//cout<<(double)(370 - VacantAfterHiring[i])/370<<endl;
		sumb = sumb + (double)(370 - VacantAfterHiring[i])/370;
	}
	sumb = sumb/(years - 5);


	//输出每年的离职率
	ofstream fout1("ChurnRate5.txt", ios::out);
	fout1<<0.18<<endl;
	for(int i = 1; i < years; i ++)
	{
		fout1<<a[i];
		fout1<<endl;
	}
	fout1.close();

	//输出每年的在职率
	ofstream fout2("OccupiedRate5.txt", ios::out);
	for(int i = 0; i < years; i++)
	{
		fout2<<(double)(370 - VacantAfterHiring[i])/370;
		fout2<<endl;
	}
	fout2.close();

	//输出总的生产力随时间的变化
	/*ofstream fout3("Productivity.txt", ios::out);
	for(int i = 0; i < years; i++)
	{	
		fout3<<Pro[i];
		fout3<<endl;
	}
	fout3.close();*/
	/*//输出每个时间段之后空缺的职位数
	ofstream fout("vacant_5.txt", ios::out);  
	fout<<"FACTOR = 0.95 MONTH_TO_PROMOTE = 2 RATE_AFTER_PROMOTION = 0.01 RATE_OF_NEW = 0.01"<<endl;
	double a = 0;
	for(int i=0; i<50; i++)
	{
		if(i>=1)
		{
			a = (double)(VacantAfterLeaving[i] - VacantAfterHiring[i-1])/(370-VacantAfterHiring[i-1]);
			fout<<VacantAfterLeaving[i]<<' '<<VacantAfterHiring[i]<<" "<<a<<endl;
		}
		else fout<<VacantAfterLeaving[i]<<' '<<VacantAfterHiring[i]<<endl;

	}
	fout.close();*/
	
	//Scan one parameter
	/*double factor = 0.83;
	double rate = 0.01;
	HCnetwork::MONTH_TO_PROMOTE = 4;
	double churnrate[21];
	double vacant[21];
	double churnrate1[21];
	double vacant1[21];
	fstream fout1("ChurnRateChanges3.txt", ios::out);
	fstream fout2("VacantChanges3.txt", ios::out);//文件中，第一列是Factor=0.83扫描rate，第二列是rate=0.01扫描Factor
	//Factor
	scan(churnrate, vacant, factor);
	scan1(churnrate1, vacant1, rate);
	for(int j = 0; j <= 20; j++)
	{
		fout1<<churnrate[j]<<" "<<churnrate1[j]<<endl;
		fout2<<vacant[j]<<" "<<vacant1[j]<<endl;;
	}
	fout1.close();
	fout2.close();*/
	
	//Scan two parameters
	/*double factor = 0.6;
	double churnrate[21];
	double vacant[21];
	fstream fout1("churn_scan.txt", ios::out);
	fstream fout2("vacant_scan.txt", ios::out);
	for(int i = 0; i <= 20; i++)
	{
		scan(churnrate, vacant, factor);
		for(int j = 0; j <= 20; j++)
		{
			if(j == 0)
			{
				fout1<<churnrate[j];
				fout2<<vacant[j];
			}
			else
			{
				fout1<<" "<<churnrate[j];
				fout2<<" "<<vacant[j];
			}			
		}
		fout1<<endl;
		fout2<<endl;
		factor = factor + 0.02;
	}
	fout1.close();
	fout2.close();*/
	cout<<"done!"<<endl;
}

