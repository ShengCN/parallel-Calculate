#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <algorithm>

using std::vector;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;

/**************** shared parameter *******************/
int k; // k route
int leftSize; // left numbers 
vector<vector<int>> k_route_matrix;
vector<int> leftNumbers;

/*
 * description: init array function
 * input:		k_route_matrix----- the array to be sorted and merge
 *				n----- the input array number (1¡¢2¡¢3)
 * output:		if the test array file is opened right
 *
 */
bool initArray(int n);

/*
 * description: k-merge function
 * input:		k_route_matrix----- the array to be sorted and merge
 * output:      sorted array
 *
 */
vector<int> k_merge();

/*
 * description: input the array from file
 * input:		k_route_matrix----- the array to be sorted and merge
 *				          file----- the input file number 
 * output:      is file opened 
 *
 */
bool inputArray(const string file);

// debug function
void printAll()
{
	for (auto i:k_route_matrix)
	{
		for (auto j:i)
		{
			cout << j << "\t";
		}
		cout << endl;
	}
	cout << endl << endl;
}

int main()
{
	vector<int> k_route_matrix;
	for (auto i = 1; i <= 3; ++i)
	{
		if (initArray(i))
		{
			auto res = k_merge();
			for (auto i:res)
			{
				cout << i << "\t" << endl;
			}
		}
		else
			cout << "Can't open file " << i << endl;
	}
	system("pause");
}

bool initArray(int n)
{
	switch (n)
	{
	case 1:
		return inputArray("¹é²¢ÅÅÐò²¢ÐÐ²âÊÔ1.txt");
		break;
	case 2:
		return inputArray("¹é²¢ÅÅÐò²¢ÐÐ²âÊÔ2.txt");
		break;
	case 3:
		return inputArray("¹é²¢ÅÅÐò²¢ÐÐ²âÊÔ3.txt");
		break;
	default:
		return false;
	}
}

vector<int> k_merge()
{
	/********------|||||///////************/
	auto row = k_route_matrix.size();
	auto column = k_route_matrix[0].size();

	// ----
	for (int i = 0; i < row; ++i)
	{
		std::sort(k_route_matrix[i].begin(), k_route_matrix[i].end());
	}
	//printAll();

	// ||||
	for (int j = 0; j < column; ++j)
	{
		vector<int> buffer;
		for (int i = 0; i < row; ++i)
		{
			buffer.push_back(k_route_matrix[i][j]);
		}
		std::sort(buffer.begin(), buffer.end());
		// write back
		for (int i = 0; i < row; ++i)
		{
			k_route_matrix[i][j] = buffer[i];
		}
	}
//	printAll();

	//  /////
	// fisrt session
	int slopeTimes = row - 1;
	int count;
	for (int i = slopeTimes; i > 0; --i)
	{
		count = 1;
		for (int j = 0; j < column + row - 1; ++j)
		{
			vector<int> buffer;
			int local_y = j, local_x = 0;
			for (; local_x < row; local_x = local_x + i)
			{
				if (local_y >= 0 && local_y < column)
				{
					buffer.push_back(k_route_matrix[local_x][local_y]);
				}
				local_y -= count;
			}
			std::sort(buffer.begin(), buffer.end());
			// write back
			int buffer_index = 0;
			local_y = j , local_x = 0;
			for (; local_x < row; local_x = local_x + i)
			{
				if (local_y >= 0 && local_y < column)
				{
					k_route_matrix[local_x][local_y] = buffer[buffer_index++];
				}
				local_y -= count;
			}
		}
		count++;
//		printAll();
	}
	// second session
	slopeTimes = column - 1;
	for (int i = 1; i <= slopeTimes; ++i)
	{
		count = 1;
		for (int j = 0; j < row + column - 1; ++j)
		{
			vector<int> buffer;
			int local_x = j, local_y = 0;
			for (; local_y < column; local_y = local_y + i)
			{
				if (local_x >= 0 && local_x < row)
				{
					buffer.push_back(k_route_matrix[local_x][local_y]);
				}
				local_x -= count;
			}
			std::sort(buffer.begin(), buffer.end());
			// write back
			int buffer_index = buffer.size() - 1;
			local_x = j , local_y = 0;
			for (; local_y < column; local_y = local_y + i)
			{
				if (local_x >= 0 && local_x < row)
				{
					k_route_matrix[local_x][local_y] = buffer[buffer_index--];
				}
				local_x -= count;
			}
		}
		count++;
	}

//	printAll();

	// insert point
	std::sort(leftNumbers.begin(), leftNumbers.end());

	// return result vector
	vector<int> result;
	auto iter = leftNumbers.begin();
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < column; ++j)
		{
			if ((iter != leftNumbers.end()) && k_route_matrix[i][j] > (*iter))
			{
				result.push_back(*iter++);
				j--;
			}
			result.push_back(k_route_matrix[i][j]);
		}
	}
	return result;
}

bool inputArray(const string file)
{
	// flush
	k_route_matrix.clear();

	ifstream in(file);
	if (!in.is_open())
		return false;
	else
	{
		int count = 0, int_tmp;
		in >> k;
		while (in >> int_tmp)
		{
			count++;
		}
		in.close();
		leftSize = count % k;
		int rows = count / k;

		// load in
		count = 0;
		int row_count = 0;
		in.open(file);
		in >> k;
		vector<int> buffer(k);
		while (in >> int_tmp)
		{
			if (row_count < rows)
			{
				if (count < k)
				{
					buffer[count++] = int_tmp;
				}
				else
				{
					k_route_matrix.push_back(buffer);
					count = 0;
					buffer[count++] = int_tmp;
					row_count++;
				}
			}
			else
			{
				leftNumbers.push_back(int_tmp);
			}
		}
		k_route_matrix.push_back(buffer);
		return true;
	}
}
