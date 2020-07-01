#include "kNN.h"

kNN::~kNN()
{
	delete dataSetReader;
	delete TestSetReader;
	delete[]dataSet;
	delete[]dataSetClass;
	delete[]testSet;
	delete[]testSetClass;
	delete[]dist;
	delete[]classContainer;
	delete[]classChecker;
	delete[]errorMatrix;
	delete[]distMemory;
}

kNN::kNN(int _dimension, int _dataSetSize, int _testSetSize, int  _batchSize, int _totalClass, int _k, bool _use256, int _disFuncTag)
	:dimension(_dimension), dataSetSize(_dataSetSize), testSetSize(_testSetSize), batchSize(_batchSize), totalClass(_totalClass), k(_k),
	use256(_use256), disFuncTag(_disFuncTag)
{
	dataSet = new int[dataSetSize * dimension];
	dataSetClass = new int[dataSetSize];

	testSet = new int[testSetSize * dimension];
	testSetClass = new int[testSetSize];

	dist = new int[batchSize * dataSetSize];
	classContainer = new int[batchSize * totalClass];

	classChecker = new int[batchSize];
	accuracy = 0;

	dataSetReader = new Reader();
	TestSetReader = new Reader();

	matrix.bufferA = new int[dimension << 2];
	matrix.bufferB = new int[dimension << 2];
	matrix.bufferC = new int[16];

	errorMatrix = new int[totalClass * totalClass];
	memset(errorMatrix, 0, sizeof(int) * totalClass * totalClass);

	distMemory = new int[k * testSetSize << 1];
	distMemoryPtr = distMemory;

	usedDisFunc = disFuncTable[disFuncTag * 2 + use256];
}

bool kNN::ReadDataSet(const char* DataSetPath)
{
	FILE* fileReader = fopen(DataSetPath, "r");
	if (!fileReader)
		return false;

	dataSetReader->init(fileReader);
	int* dataPtr = dataSet, * dataClassPtr = dataSetClass;

	for (int i = 0; i < dataSetSize; ++i)
	{
		*(dataClassPtr++) = dataSetReader->read();
		for (int j = 0; j < dimension; ++j)
			*(dataPtr++) = dataSetReader->read();
	}

	fclose(fileReader);
	return true;
}

bool kNN::ReadTestSet(const char* TestSetPath)
{
	FILE* fileReader = fopen(TestSetPath, "r");
	if (!fileReader)
		return false;

	TestSetReader->init(fileReader);
	int* testPtr = testSet, * testClassPtr = testSetClass;

	for (int i = 0; i < testSetSize; ++i)
	{
		*(testClassPtr++) = TestSetReader->read();
		for (int j = 0; j < dimension; ++j)
			*(testPtr++) = TestSetReader->read();
	}

	fclose(fileReader);
	return true;
}

void kNN::CalculateBatch(int batchID)
{
	memset(dist, 0, sizeof(int) * batchSize * dataSetSize);

	int* dataPtr = dataSet;
	int* testPtr = testSet + batchID * batchSize * dimension;

	int* bufferPtrA = matrix.bufferA;
	int* bufferPtrB = matrix.bufferB;
	int* bufferPtrC = matrix.bufferC;

	int* d_ptr_1;
	int* d_ptr_2;
	int* d_ptr_3;
	int* d_ptr_4;

	int cpysize = dimension << 2;
	for (int i = 0; i < batchSize; i += 4, dataPtr = dataSet)
	{
		memcpy(bufferPtrA, testPtr, cpysize * sizeof(int));
		testPtr += cpysize;

		d_ptr_1 = dist + i * dataSetSize;
		d_ptr_2 = d_ptr_1 + dataSetSize;
		d_ptr_3 = d_ptr_2 + dataSetSize;
		d_ptr_4 = d_ptr_3 + dataSetSize;

		for (int j = 0; j < dataSetSize; j += 4)
		{
			memcpy(bufferPtrB, dataPtr, cpysize * sizeof(int));
			dataPtr += cpysize;

			((this->matrix).*usedDisFunc)(bufferPtrA, bufferPtrB, bufferPtrC, dimension);

			memcpy(d_ptr_1, bufferPtrC, 16);
			memcpy(d_ptr_2, bufferPtrC + 4, 16);
			memcpy(d_ptr_3, bufferPtrC + 8, 16);
			memcpy(d_ptr_4, bufferPtrC + 12, 16);

			d_ptr_1 += 4;
			d_ptr_2 += 4;
			d_ptr_3 += 4;
			d_ptr_4 += 4;
		}
	}
}

void kNN::Classify()
{
	memset(classContainer, 0, sizeof(int) * batchSize * totalClass);

	int* distPtr = dist, * dataPtr;
	int* distMemoryPtr = distMemory;
	for (int i = 0; i < batchSize; ++i)
	{
		dataPtr = dataSetClass;
		for (int j = 0; j < dataSetSize; ++j)
		{
			if (q.size() < k)
				q.push({*distPtr,*dataPtr});
			else if (q.top().first > * distPtr)
			{
				q.pop();
				q.push({*distPtr,*dataPtr});
			}
			++distPtr, ++dataPtr;
		}
		while (q.size())
		{
			++classContainer[i * totalClass + q.top().second];
			*(distMemoryPtr++) = q.top().second;
			*(distMemoryPtr++) = q.top().first;
			q.pop();
		}
	}
	int* checkerPtr = classChecker;
	for (int i = 0; i < batchSize; ++i)
		*(checkerPtr++) = std::max_element(classContainer + i * totalClass,
			classContainer + (i + 1) * totalClass) - (classContainer + (i * totalClass));
}

bool kNN::Work(const char* dataSetPath, const char* testSetPath, FILE* outFilePtr, bool CompleteInfo)
{
	if (testSetSize % batchSize || dimension % 4 || dataSetSize % 4)
		return printf("There will be something that can not be classified\n"), false;
	if (!outFilePtr)
		return printf("Can not open output file\n"), false;

	clock_t st = clock();
	if (!ReadDataSet(dataSetPath))
		return printf("Can not open dataset\n"), false;
	printf("Read dataset OK!\n");
	clock_t en = clock();
	printf("Time: %d ms\n", en - st);

	st = clock();
	if (!ReadTestSet(testSetPath))
		return printf("Can not open testset\n"), false;
	printf("Read testset OK!\n");
	en = clock();
	printf("Time: %d ms\n", en - st);

	printf("%s\n", use256 ? "Use AVX-2" : "Not Use AVX-2");
	printf("distance function: %s\n",
		disFuncTag ? (disFuncTag - 1 ? "Chebyshev" : "Eucliden") : "Manhattan");
	printf("start Classfy\n");

	int time = testSetSize / batchSize;
	int* testPtr = testSetClass, * checkerPtr;

	st = clock();
	for (int i = 0; i < time; ++i)
	{
		CalculateBatch(i);
		Classify();
		checkerPtr = classChecker;
		for (int j = 0; j < batchSize; ++j)
		{
			++errorMatrix[*checkerPtr * totalClass + *testPtr];
			if (*(checkerPtr++) == *(testPtr++))
				++accuracy;
		}
		en = clock();
		printf("finish batch: %d, acc: %.6f\n", i + 1, (double)accuracy / ((i + 1) * batchSize));
		printf("Time: per graph %d ms\n", (en - st) / ((i + 1) * batchSize));
	}

	if (CompleteInfo)
	{
		fprintf(outFilePtr, "Info:\n");
		fprintf(outFilePtr, "acc, %.6f\n", (double)accuracy / (time * batchSize));
		fprintf(outFilePtr, "Classfy result:\n%-10s,%-10s,%-10s\n", "Result", "True", "Count");
		for (int i = 0; i < totalClass; ++i)
			for (int j = 0; j < totalClass; ++j)
				if (errorMatrix[i * totalClass + j])
					fprintf(outFilePtr, "%-10d,%-10d,%-10d\n", i, j, errorMatrix[i * totalClass + j]);
		int* tmpPtr = distMemory;
		for (int i = 0; i < testSetSize; ++i)
		{
			fprintf(outFilePtr, "%d,", testSetClass[i]);
			for (int j = 0; j < k; ++j)
			{
				int tmp1 = *(tmpPtr++), tmp2 = *(tmpPtr++);
				fprintf(outFilePtr, "%d,%d%c", tmp1, tmp2, ",\n"[j == k - 1]);
			}
		}
		if (outFilePtr != stdout)
			fclose(outFilePtr);
	}
	return printf("finish classified\n"), true;
}