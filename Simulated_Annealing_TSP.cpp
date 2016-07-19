#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// 读取数据，并把数据按照20*20的格式放到数组中
void get_data(double data_set[20][20], int data_row, int data_col)
{
	ifstream myFile;

	myFile.open("./tsp测试集/tsp测试数据1.txt");

	// string line;
	// string word;

	if (myFile.is_open())
	{
		while(!myFile.eof())
		{
			// while (getline(myFile, line))
			// {
			// 	cout<<line<<endl;
			// 	istringstream instream(line);
			// 	while(instream>>word)
			// 	{
			// 		cout<<word<<endl;
			// 	}
			// 	cout<<endl;
			// }

			for (int i = 0; i < data_row; ++i)
			{
				for (int j = 0; j < data_col; ++j)
				{
					myFile>>data_set[i][j];
				}
			}
		}
	}

	// for (int i = 0; i < data_row; ++i)
	// {
	// 	for (int j = 0; j < data_col; ++j)
	// 	{
	// 		cout<<data_set[i][j]<<' ';
	// 	}
	// 	cout<<endl;
	// }

	// cout<<sizeof(data_set)/sizeof(data_set[0])<<endl;
	// cout<<sizeof(data_set[0])/sizeof(data_set[0][0])<<endl;

	myFile.close();
}


// // 模拟退火算法求解TSP问题
// void SA(data_set)
// {

// }

// 计算当前输入的路径长度
// 输入ID为每个城市的编号顺序，是一个一维数组
double calculate_path(int ID[20], double data_set[20][20])
{
	// 数组长度
	int ID_length = 20;
	// 统计路径长度
	double path = 0.0;
	for (int i = 0; i < ID_length; ++i)
	{
		// 如果当前不是最后一个元素，就计算其和后一个元素的距离
		if (i!=(ID_length-1))
		{
			path += data_set[ID[i]][ID[i+1]];
		}
		// 如果当前是最后一个元素，就计算其和第一个元素的距离
		if (i==(ID_length-1))
		{
			path += data_set[ID[i]][ID[0]];
		}
	}
	// cout<<path<<endl;
	return path;
}

int main(int argc, char const *argv[])
{
	// 计时
	clock_t start, finish;
	start = clock();

	const int data_row = 20;
	const int data_col = 20;
	double data_set[data_row][data_col];

	// 读入数据，并将数据添加进data_set二维数组中
	get_data(data_set, data_row,data_col);

	// 步长
	double k = 0.5;
	// 循环次数
	int iter = 1e4;
	// 初始化温度
	double T = 100;
	// 终止条件温度
	double terminal_T = 1;

	// 初始化路径位置
	int S_ID[data_col];
	for (int i = 0; i < data_col; ++i)
	{
		S_ID[i] = i;
	}
	// 初始化路径长度
	double S_path = calculate_path(S_ID, data_set);

	// 新的S_ID和S_path，用来进行比较
	int S_ID_new[data_col];
	double S_path_new;

	// 当前温度没有达到终止温度时
	for (double t=T; terminal_T<t; t*=k)
	{
		// srand((unsigned int)time(NULL));
		for (int it = 0; it < iter; ++it)
		{
			// 生成[0,20）间的整数，用来随机交换其中两个位置的数
			srand(it);
			// time_t ti;
			// srand((unsigned int)time(&ti));
			int a = rand()%20;
			int b = rand()%20;

			// 生成一个新的S_ID
			for (int i = 0; i < data_row; i++)
			{
				S_ID_new[i] = S_ID[i];
			}
			S_ID_new[a] = S_ID[b];
			S_ID_new[b] = S_ID[a];
			// 并对其进行路径长度计算
			S_path_new = calculate_path(S_ID_new, data_set);

			// 计算新旧路径距离的差值
			double x = S_path_new - S_path;
			
			// 退火条件判断
			if (x<0)
			{
				for (int i = 0; i < data_row; i++)
				{
					S_ID[i] = S_ID_new[i];
				}
				S_path = S_path_new;
			}
			else
			{
				srand((int)time(0));
				if (exp(-x/t)>(rand()*1.0/RAND_MAX*1.0))
				{
					for (int i = 0; i < data_row; i++)
					{
						S_ID[i] = S_ID_new[i];
					}
					S_path = S_path_new;
				}
			}

			for (int i = 0; i < data_col; ++i)
			{
				cout<<S_ID_new[i]<<" ";
			}
			cout<<endl;
			for (int i = 0; i < data_col; ++i)
			{
				cout<<S_ID[i]<<" ";
			}
			cout<<endl<<S_path<<endl;

		}

	}

	finish = clock();
	cout<<"seconds:"<<(double)(finish - start)/CLOCKS_PER_SEC<<"s"<<endl; 

	return 0 ;
}






