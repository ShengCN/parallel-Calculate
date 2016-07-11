#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using std::vector;
using std::cout;
using std::cin;
using std::sort;
using std::endl;
using std::ifstream;

// Ωµ–Ú
bool descend(int, int);
void row_sort(vector<int>&);
// ∫· ˙≈≈–Ú
void row_clown_sort(vector<vector<int>>&);
// –±¬ ≈≈–Ú
void k_sort(vector<vector<int>>&);

int main()
{
	vector<vector<int>> test;
	ifstream cin("test.txt");
	int tmp,count = 0;
	vector<int> buffer;
	while(cin>>tmp)
	{
		buffer.push_back(tmp);
		count++;
		if(count == 7) //“ª≈≈
		{
			test.push_back(buffer);
			count = 0;
			buffer.clear();
		}
	}
	cin.close();

	cout << "=============≥ı º ˝æ›=============" << endl;
	for (auto i : test)
	{
		for (auto j : i)
		{
			cout << j << "\t";
		}
		cout << endl;
	}
	cout << "=============∞¢Ã¿∏Áæ»æ»Œ“∞…==============" << endl;

	row_clown_sort(test);
	k_sort(test);

	for(auto i:test)
	{
		for(auto j:i)
		{
			cout << j << "\t";
		}
		cout << endl;
	}

	system("pause");
}

bool descend(int a, int b)
{
	return  a >= b ? 0 : 1;
}

void row_sort(vector<int>& ary)
{
	int i = ary.size();
	//row ≈≈–Ú
	for (auto index = 0; index < i; ++index)
	{
		sort(ary.begin(), ary.end());
	}
}

void row_clown_sort(vector<vector<int>>& ary)
{
	auto i = ary.size();
	auto j = ary[0].size();
	vector<int> rBuffer(j);
	vector<int> cBuffer(i);

	//clown ≈≈–Ú
	for (auto index = 0; index < j; ++index)
	{
		for(auto in = 0; in < i;++in)
		{
			cBuffer[in] = ary[in][index];
		}
		sort(cBuffer.begin(), cBuffer.end());
		for (auto in = 0; in < i; ++in)
		{
			ary[in][index] = cBuffer[in];
		}
	}
	//row ≈≈–Ú
	for(auto index = 0; index < i; ++index)
	{
		sort(ary[index].begin(), ary[index].end());
	}

	for (auto i : ary)
	{
		for (auto j : i)
		{
			cout << j << "\t";
		}
		cout << endl;
	}

	cout << endl;
}

void k_sort(vector<vector<int>>& ary)
{
	// ≈≈–Ú¬÷¥Œ
	int turns = ary[0].size() - 1;
	int row = ary.size();
	int clown = ary[0].size();

	for(int i = 1; i <= turns; ++i)
	{
		for(int index = 1; index < clown;++index)
		{
			int clown_flag = 0;       //¡Ÿ ± ˝◊÷¡–
			int row_flag = index;     //¡Ÿ ± ˝◊÷––
			if (row_flag < row)
			{
				vector<int> buffer;
				while (row_flag >= 0)
				{
					if(clown_flag>clown)
						break;
					buffer.push_back(ary[row_flag][clown_flag]);
					--row_flag;
					clown_flag += i;
				}
				sort(buffer.begin(), buffer.end());
				//≈≈–Ú°¢–¥ªÿ
				for (auto index : buffer)
				{
					++row_flag;
					clown_flag-=i;
					ary[row_flag][clown_flag] = index;
				}

				buffer.clear();
			}
		}

		//∫·≈≈
		for(int i = 0; i < row;++i)
		{
			row_sort(ary[i]);
		}

		for (auto i : ary)
		{
			for (auto j : i)
			{
				cout << j << "\t";
			}
			cout << endl;
		}
		cout << endl;
	}
}