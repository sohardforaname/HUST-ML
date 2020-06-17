#ifndef KNN_H
#define KNN_H
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>
#include <ctime>

#include "reader.h"
#include "matrix.h"

class kNN
{
private:
    std::priority_queue<std::pair<int, int>>q;

	int* dataSet, * testSet, * dist;
	int* dataSetClass, * testSetClass, * classChecker;
	int* classContainer;
	int dimension, testSetSize, dataSetSize, batchSize;
	int k, totalClass;
    int accuracy;

	Reader* dataSetReader, * TestSetReader;

	Matrix matrix;
 
private:
	kNN() = delete;
	kNN(const kNN& _knn) = delete;
	kNN(kNN&& _knn) = delete;
	bool ReadDataSet(const char* DataSetPath);
    bool ReadTestSet(const char* TestSetPath);
	void CalculateBatch(int batchID);
	void Classify();

public:
    kNN(int _dimension, int _dataSetSize, int _testSetSize, int  _batchSize, int _totalClass, int _k);
	bool Work();
	~kNN();

};
#endif
