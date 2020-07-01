#include "BmpReader.h"

void BmpDataReader::ReadSingleImage(const char* filePath, BmpData* currentDataPtr)
{
	FILE* fp = fopen(filePath, "rb");
	if (!fp)
	{
		printf("Can not open the bmp file\n");
		exit(0);
	}
	fread(&bfhead, sizeof(BmpFileHead), 1, fp);
	if (bfhead.bfType != 0x4d42)
	{
		printf("Not a bmp\n");
		exit(0);
	}
	fread(&bihead, sizeof(BmpInfoHead), 1, fp);
	currentDataPtr->clrPaletteBuffer = NULL;
	if (bfhead.bfOffBits != 54)
	{
		int clrPaletteSize = ( bfhead.bfOffBits - 54 ) / 4;
		currentDataPtr->clrPaletteBuffer = new BGRA[clrPaletteSize]();
		fread(currentDataPtr->clrPaletteBuffer, sizeof(BGRA), clrPaletteSize, fp);
	}
	currentDataPtr->buffer = new unsigned char
		[1ull * abs(bihead.biHeight) * bihead.biWidth * bihead.biBitCount];
	size_t rowSize = 1ull * bihead.biWidth * bihead.biBitCount / 8ull;
	if (bihead.biHeight > 0)
		for (int i = bihead.biHeight - 1; ~i; --i)
			fread(currentDataPtr->buffer + i * rowSize,
				sizeof(unsigned char), rowSize, fp);
	else
		for (int i = 0; i < bihead.biHeight; ++i)
			fread(currentDataPtr->buffer + i * rowSize,
				sizeof(unsigned char), rowSize, fp);
}

BmpDataReader::BmpDataReader(size_t _dataSetSize) :dataSetSize(_dataSetSize)
{
	dataSet = new BmpData[dataSetSize];
}

BmpDataReader::~BmpDataReader()
{
	BmpData* dataPtr = dataSet;
	for (int i = 0; i < dataSetSize; ++i, ++dataPtr)
	{
		delete[]dataPtr->buffer;
		delete[]dataPtr->clrPaletteBuffer;
	}
	delete[]dataSet;
}

const BmpDataReader::BmpData* BmpDataReader::GetData(size_t i)const
{
	return dataSet + i;
}

const BmpDataReader::BmpFileHead* BmpDataReader::GetFileHead()const
{
	return &bfhead;
}

const BmpDataReader::BmpInfoHead* BmpDataReader::GetInfoHead()const
{
	return &bihead;
}

const size_t BmpDataReader::size()const
{
	return dataSetSize;
}

void BmpDataReader::ReadData(const char* classPath)
{
	FILE* fp = fopen(classPath, "r");
	if (!fp)
	{
		printf("Can not open class file\n");
		exit(0);
	}
	BmpData* dataSetPtr = dataSet;
	for (int i = 0; i < dataSetSize; ++i, ++dataSetPtr)
	{
		fscanf(fp, "%s %d", tmpBuffer, &dataSetPtr->id);
		ReadSingleImage(tmpBuffer, dataSetPtr);
	}
}