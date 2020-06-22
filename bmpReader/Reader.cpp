#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct BGRA
{
	unsigned char b, g, r, a;
};

struct BGR
{
	unsigned char b, g, r;
};

class BmpDataReader
{
public:
	struct BmpData
	{
		unsigned char* buffer;
		BGRA* clrPaletteBuffer;
		int id;
	};

#pragma pack(2)

	struct BmpFileHead
	{
		unsigned short bfType;
		unsigned int bfSize;
		unsigned short bfReserved1;
		unsigned short bfReserved2;
		unsigned int bfOffBits;
	};

	struct BmpInfoHead
	{
		unsigned int biSize;
		int biWidth;
		int biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned int biCompression;
		unsigned int biSizeImage;
		int biXpelsPerMeter;
		int biYpelsPerMeter;
		unsigned int biClrUsed;
		unsigned int biClrImportant;
	};

#pragma pack(1)

private:
	char tmpBuffer[50];
	size_t dataSetSize;
	BmpFileHead bfhead;
	BmpInfoHead bihead;
	BmpData* dataSet;

	void ReadSingleImage(const char* filePath, BmpData* currentDataPtr)
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

public:
	BmpDataReader(size_t _dataSetSize) :dataSetSize(_dataSetSize) 
	{
		dataSet = new BmpData[dataSetSize];
	}

	~BmpDataReader()
	{
		BmpData* dataPtr = dataSet;
		for (int i = 0; i < dataSetSize; ++i, ++dataPtr)
		{
			delete[]dataPtr->buffer;
			delete[]dataPtr->clrPaletteBuffer;
		}
		delete[]dataSet;
	}

	const BmpData* GetData(size_t i)const
	{
		return dataSet + i;
	}

	const BmpFileHead* GetFileHead()const
	{
		return &bfhead;
	}

	const BmpInfoHead* GetInfoHead()const
	{
		return &bihead;
	}

	const size_t size()const
	{
		return dataSetSize;
	}

	void ReadData(const char* classPath)
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
}; 

BmpDataReader::BmpData* buffer;

int main()
{
	BmpDataReader Reader(400);
	Reader.ReadData("D:\\img\\class.txt");
	buffer = new BmpDataReader::BmpData[400];
	memcpy(buffer, Reader.GetData(0), 400 * sizeof(BmpDataReader::BmpData));
	random_shuffle(buffer, buffer + 400);
	FILE* fp = fopen("D:\\11.txt", "w");
	const BmpDataReader::BmpInfoHead* biptr = Reader.GetInfoHead();
	for (int i = 0; i < 360; ++i)
	{
		fprintf(fp, "%d,", ( buffer + i )->id);
		unsigned char* cptr = ( buffer + i )->buffer;
		for (int j = 0; j < abs(biptr->biHeight) * biptr->biWidth; ++j)
			fprintf(fp, "%hu,", unsigned short(cptr[j * 3]));
	}
	fprintf(fp, "\n");
	fp = fopen("D:\\12.txt", "w");
	for (int i = 360; i < 400; ++i)
	{
		fprintf(fp, "%d,", ( buffer + i )->id);
		unsigned char* cptr = ( buffer + i )->buffer;
		for (int j = 0; j < abs(biptr->biHeight) * biptr->biWidth; ++j)
			fprintf(fp, "%hu,", unsigned short(cptr[j * 3]));
	}
	fprintf(fp, "\n");
	return 0;
}