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
	int* errorMatrix;
	int* distMemory, * distMemoryPtr;

	bool use256;
	int disFuncTag;

	Reader* dataSetReader, * TestSetReader;

	Matrix matrix;

	void ( Matrix::*disFuncTable[6] )( int* a, int* b, int* c, int len ) =
	{
		&Matrix::CalMatrix4Mul4L1, &Matrix::CalMatrix4Mul4L1_256,
		&Matrix::CalMatrix4Mul4L2, &Matrix::CalMatrix4Mul4L2_256,
		&Matrix::CalMatrix4Mul4Ln, &Matrix::CalMatrix4Mul4Ln_256
	};

	void ( Matrix::* usedDisFunc )( int* a, int* b, int* c, int len );
 
private:
	kNN() = delete;
	kNN(const kNN& _knn) = delete;
	kNN(kNN&& _knn) = delete;
	bool ReadDataSet(const char* DataSetPath);
    bool ReadTestSet(const char* TestSetPath);
	void CalculateBatch(int batchID);
	void Classify();

public:
	kNN(int _dimension, int _dataSetSize, int _testSetSize, int  _batchSize, 
		int _totalClass, int _k, bool _use256, int _disfuncTag);
	bool Work(const char* dataSetPath, const char* testSetPath, FILE* outFilePtr, bool CompleteInfo);
	~kNN();

};
#endif
