#pragma once

#include <cstdio>
#include <cstdlib>

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

#pragma pack()

private:
	char tmpBuffer[50];
	size_t dataSetSize;
	BmpFileHead bfhead;
	BmpInfoHead bihead;
	BmpData* dataSet;

	void ReadSingleImage(const char* filePath, BmpData* currentDataPtr);

public:
	BmpDataReader(size_t _dataSetSize);
	~BmpDataReader();

	const BmpData* GetData(size_t i)const;
	const BmpFileHead* GetFileHead()const;
	const BmpInfoHead* GetInfoHead()const;
	const size_t size()const;
	void ReadData(const char* classPath);
};
