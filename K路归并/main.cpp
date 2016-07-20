#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <string>
#include <omp.h>
#include <algorithm>

using std::vector;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;

/**************** shared parameter *******************/
int k; // k route
int leftSize; // left numbers
vector<int> result;//merged array
vector<vector<int>> k_route_matrix;
vector<int> leftNumbers;
int thread;

struct TwoDimIndex
{
	int x;
	int y;
	TwoDimIndex(int x,int y)
	{
		this->x = x;
		this->y = y;
	}
};

/*
* description: init array function
* input:		k_route_matrix----- the array to be sorted and merge
*				n----- the input array number (1?¡é2?¡é3)
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

/*
 * description: binary search function(row or column)
 * input:		start----start Point
 *				  end----end Point
 *          searchKey----key to be searched
 *                isX----row or column
 * output:      the first smaller one than searchKey
 *
 */
int binarySearch(TwoDimIndex start, TwoDimIndex end, int searchKey, bool isX);

// debug function
void printAll()
{
	for (auto i : k_route_matrix)
	{
		for (auto j : i)
		{
			cout << j << "\t";
		}
		cout << endl;
	}
	cout << endl << endl;
}

int main(int argvc, char* argv[])
{
	thread = strtol(argv[1], NULL, 10);
	vector<int> k_route_matrix;
	for (auto i = 1; i <= 3; ++i)
	{
		if (initArray(i))
		{
			auto res = k_merge();
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
	// set thread
	omp_set_num_threads(thread);
	double start, finish;
	start = omp_get_wtime();

	// generate k-route array
	int k_count,row_count,leftSize = result.size() % k,rows = result.size()/k;
	leftNumbers.resize(leftSize);

#pragma omp parallel \
	default(none) shared(k_route_matrix,leftNumbers,result) private()
	{
#pragma omp for
		for (row_count = 0; row_count < rows; ++row_count)
		{
			vector<int> tmp;
			for (k_count = 0; k_count < k; ++k_count)
			{
				tmp.push_back(result[(row_count + 1)*(k_count + 1) - 1]);
			}
			k_route_matrix.push_back(tmp);
		}
#pragma omp for
		for (auto i = 0; i < leftSize; ++i)
		{
			leftNumbers[i] = result[rows*k + i];
		}
	}
	/********------|||||///////************/
	auto row = k_route_matrix.size();
	auto column = k_route_matrix[0].size();

#pragma omp parallel \
	default(none) shared(k_route_matrix) private()
	{
		// ----
#pragma omp for 
		for (int i = 0; i < row; ++i)
		{
			std::sort(k_route_matrix[i].begin(), k_route_matrix[i].end());
		}
		//printAll();

		// ||||
#pragma omp for
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
	}
	//	printAll();

	//  /////
	// fisrt session
	int slopeTimes = row - 1;
	int count;

	for (int i = slopeTimes; i > 0; --i)
	{
#pragma omp parallel \
	default(none) shared(k_route_matrix) private(count)
	{
		count = 1;
#pragma omp for schedule(static,(column + row - 1)/thread)
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
				if (local_y < 0 || local_y >= column)
					break;
			}
			std::sort(buffer.begin(), buffer.end());
			// write back
			int buffer_index = 0;
			local_y = j, local_x = 0;
			for (; local_x < row; local_x = local_x + i)
			{
				if (local_y >= 0 && local_y < column)
				{
					k_route_matrix[local_x][local_y] = buffer[buffer_index++];
				}
				local_y -= count;
				if (local_y < 0 || local_y >= column)
					break;
			}
		}
		count++;
		//		printAll();
	}
	}

	// second session
	slopeTimes = column - 1;
	for (int i = 1; i <= slopeTimes; ++i)
	{
#pragma omp parallel \
	default(none) shared(k_route_matrix) private(count)
	{
		count = 1;
#pragma omp for schedule(static,(column + row - 1)/thread)
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
				if (local_x < 0 || local_x >= row)
					break;
			}
			std::sort(buffer.begin(), buffer.end());
			// write back
			int buffer_index = buffer.size() - 1;
			local_x = j, local_y = 0;
			for (; local_y < column; local_y = local_y + i)
			{
				if (local_x >= 0 && local_x < row)
				{
					k_route_matrix[local_x][local_y] = buffer[buffer_index--];
				}
				local_x -= count;
				if (local_x < 0 || local_x >= row)
					break;
			}
		}
		count++;
	}
	}
	//	printAll();

	// insert point
	std::sort(leftNumbers.begin(), leftNumbers.end());

	// return result vector
	
	auto iter = leftNumbers.begin();
#pragma omp parallel \
	default(none) shared(k_route_matrix,iter,result) private()
#pragma omp for
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < column; ++j)
		{
			if(iter!=leftNumbers.end() && k_route_matrix[i][j]>*iter)
			{
				result[(i + 1)*(j + 1) - 1] = *iter;
				j--;
#pragma omp critical
				iter++;
				
			}
			else
			{
				result[(i + 1)*(j + 1) - 1] = k_route_matrix[i][j];
			}
		}
	}
	finish = omp_get_wtime();

	cout << endl << endl;
	cout << "=================================" << endl;
	cout << "Thread is: " << thread << endl;
	cout << "Merge ended. Time is: " << finish - start << endl;

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
		std::copy(std::istream_iterator<int>(in), std::istream_iterator<int>(), std::back_inserter(result));
		cout << "\n\n" << file << " loading end." << "\n\n";
		return true;
	}
}

int binarySearch(TwoDimIndex start, TwoDimIndex end, int searchKey, bool isX)
{
	if (isX)
	{
		// row search
		auto rowIndex = start.x;
		auto local_start = start.y, local_end = end.y;
		while (local_start <= local_end)
		{
			auto mid = local_start + (local_end - local_start) / 2;
			if (k_route_matrix[rowIndex][mid] < searchKey)
				local_start = mid + 1;
			else if (k_route_matrix[rowIndex][mid] > searchKey)
				local_end = mid - 1;
			else
				return mid;
		}
		if (searchKey <= k_route_matrix[start.x][start.y])
			return 0;
		else
			return end.y;
	}
	else
	{
		// column search
		auto columnIndex = start.y;
		auto local_start = start.x, local_end = end.x;
		while (local_start <= local_end)
		{
			auto mid = local_start + (local_end - local_start) / 2;
			if (k_route_matrix[mid][columnIndex] < searchKey)
				local_start = mid + 1;
			else if (k_route_matrix[mid][columnIndex] > searchKey)
				local_end = mid - 1;
			else
				return mid;
		}
		if (searchKey <= k_route_matrix[start.x][start.y])
			return 0;
		else
			return end.x;
	}
}#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <string>
#include <omp.h>
#include <algorithm>

using std::vector;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;

/**************** shared parameter *******************/
int k; // k route
int leftSize; // left numbers
vector<int> result;//merged array
vector<vector<int>> k_route_matrix;
vector<int> leftNumbers;
int thread;

struct TwoDimIndex
{
	int x;
	int y;
	TwoDimIndex(int x,int y)
	{
		this->x = x;
		this->y = y;
	}
};

/*
* description: init array function
* input:		k_route_matrix----- the array to be sorted and merge
*				n----- the input array number (1?¡é2?¡é3)
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

/*
 * description: binary search function(row or column)
 * input:		start----start Point
 *				  end----end Point
 *          searchKey----key to be searched
 *                isX----row or column
 * output:      the first smaller one than searchKey
 *
 */
int binarySearch(TwoDimIndex start, TwoDimIndex end, int searchKey, bool isX);

// debug function
void printAll()
{
	for (auto i : k_route_matrix)
	{
		for (auto j : i)
		{
			cout << j << "\t";
		}
		cout << endl;
	}
	cout << endl << endl;
}

int main(int argvc, char* argv[])
{
	thread = strtol(argv[1], NULL, 10);
	vector<int> k_route_matrix;
	for (auto i = 1; i <= 3; ++i)
	{
		if (initArray(i))
		{
			auto res = k_merge();
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
	// set thread
	omp_set_num_threads(thread);
	double start, finish;
	start = omp_get_wtime();

	// generate k-route array
	int k_count,row_count,leftSize = result.size() % k,rows = result.size()/k;
	leftNumbers.resize(leftSize);

#pragma omp parallel \
	default(none) shared(k_route_matrix,leftNumbers,result) private()
	{
#pragma omp for
		for (row_count = 0; row_count < rows; ++row_count)
		{
			vector<int> tmp;
			for (k_count = 0; k_count < k; ++k_count)
			{
				tmp.push_back(result[(row_count + 1)*(k_count + 1) - 1]);
			}
			k_route_matrix.push_back(tmp);
		}
#pragma omp for
		for (auto i = 0; i < leftSize; ++i)
		{
			leftNumbers[i] = result[rows*k + i];
		}
	}
	/********------|||||///////************/
	auto row = k_route_matrix.size();
	auto column = k_route_matrix[0].size();

#pragma omp parallel \
	default(none) shared(k_route_matrix) private()
	{
		// ----
#pragma omp for 
		for (int i = 0; i < row; ++i)
		{
			std::sort(k_route_matrix[i].begin(), k_route_matrix[i].end());
		}
		//printAll();

		// ||||
#pragma omp for
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
	}
	//	printAll();

	//  /////
	// fisrt session
	int slopeTimes = row - 1;
	int count;

	for (int i = slopeTimes; i > 0; --i)
	{
#pragma omp parallel \
	default(none) shared(k_route_matrix) private(count)
	{
		count = 1;
#pragma omp for schedule(static,(column + row - 1)/thread)
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
				if (local_y < 0 || local_y >= column)
					break;
			}
			std::sort(buffer.begin(), buffer.end());
			// write back
			int buffer_index = 0;
			local_y = j, local_x = 0;
			for (; local_x < row; local_x = local_x + i)
			{
				if (local_y >= 0 && local_y < column)
				{
					k_route_matrix[local_x][local_y] = buffer[buffer_index++];
				}
				local_y -= count;
				if (local_y < 0 || local_y >= column)
					break;
			}
		}
		count++;
		//		printAll();
	}
	}

	// second session
	slopeTimes = column - 1;
	for (int i = 1; i <= slopeTimes; ++i)
	{
#pragma omp parallel \
	default(none) shared(k_route_matrix) private(count)
	{
		count = 1;
#pragma omp for schedule(static,(column + row - 1)/thread)
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
				if (local_x < 0 || local_x >= row)
					break;
			}
			std::sort(buffer.begin(), buffer.end());
			// write back
			int buffer_index = buffer.size() - 1;
			local_x = j, local_y = 0;
			for (; local_y < column; local_y = local_y + i)
			{
				if (local_x >= 0 && local_x < row)
				{
					k_route_matrix[local_x][local_y] = buffer[buffer_index--];
				}
				local_x -= count;
				if (local_x < 0 || local_x >= row)
					break;
			}
		}
		count++;
	}
	}
	//	printAll();

	// insert point
	std::sort(leftNumbers.begin(), leftNumbers.end());

	// return result vector
	
	auto iter = leftNumbers.begin();
#pragma omp parallel \
	default(none) shared(k_route_matrix,iter,result) private()
#pragma omp for
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < column; ++j)
		{
			if(iter!=leftNumbers.end() && k_route_matrix[i][j]>*iter)
			{
				result[(i + 1)*(j + 1) - 1] = *iter;
				j--;
#pragma omp critical
				iter++;
				
			}
			else
			{
				result[(i + 1)*(j + 1) - 1] = k_route_matrix[i][j];
			}
		}
	}
	finish = omp_get_wtime();

	cout << endl << endl;
	cout << "=================================" << endl;
	cout << "Thread is: " << thread << endl;
	cout << "Merge ended. Time is: " << finish - start << endl;

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
		std::copy(std::istream_iterator<int>(in), std::istream_iterator<int>(), std::back_inserter(result));
		cout << "\n\n" << file << " loading end." << "\n\n";
		return true;
	}
}

int binarySearch(TwoDimIndex start, TwoDimIndex end, int searchKey, bool isX)
{
	if (isX)
	{
		// row search
		auto rowIndex = start.x;
		auto local_start = start.y, local_end = end.y;
		while (local_start <= local_end)
		{
			auto mid = local_start + (local_end - local_start) / 2;
			if (k_route_matrix[rowIndex][mid] < searchKey)
				local_start = mid + 1;
			else if (k_route_matrix[rowIndex][mid] > searchKey)
				local_end = mid - 1;
			else
				return mid;
		}
		if (searchKey <= k_route_matrix[start.x][start.y])
			return 0;
		else
			return end.y;
	}
	else
	{
		// column search
		auto columnIndex = start.y;
		auto local_start = start.x, local_end = end.x;
		while (local_start <= local_end)
		{
			auto mid = local_start + (local_end - local_start) / 2;
			if (k_route_matrix[mid][columnIndex] < searchKey)
				local_start = mid + 1;
			else if (k_route_matrix[mid][columnIndex] > searchKey)
				local_end = mid - 1;
			else
				return mid;
		}
		if (searchKey <= k_route_matrix[start.x][start.y])
			return 0;
		else
			return end.x;
	}
}