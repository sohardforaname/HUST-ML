#include "BmpReader.h"
#include <algorithm>
#include <string>
#include <cstdlib>

BmpDataReader::BmpData* buffer;

char s[5];

int main()
{
	BmpDataReader Reader(400);
	Reader.ReadData("D:\\img\\class.txt");
	buffer = new BmpDataReader::BmpData[400];
	memcpy(buffer, Reader.GetData(0), 400 * sizeof(BmpDataReader::BmpData));
	std::string str1 = "D:\\bmpdata\\", str2 = "_1.txt", str3 = "_2.txt";
	for (int t = 0; t <= 50; ++t)
	{
		if (t)
			std::random_shuffle(buffer, buffer + 400);
		_itoa(t, s, 10);
		FILE* fp1 = fopen(( str1 + s + str2 ).c_str(), "w");
		FILE* fp2 = fopen(( str1 + s + str3 ).c_str(), "w");
		const BmpDataReader::BmpInfoHead* biptr = Reader.GetInfoHead();
		for (int i = 0; i < 400; ++i)
		{
			FILE* fp = i % 10 > 7 ? fp2 : fp1;
			fprintf(fp, "%d,", ( buffer + i )->id - 1);
			//printf("%d%c", ( buffer + i )->id, " \n"[!( ( i + 1 ) % 10 )]);
			unsigned char* cptr = ( buffer + i )->buffer;
			for (int j = 0; j < abs(biptr->biHeight) * biptr->biWidth; ++j)
				fprintf(fp, "%hu,", unsigned short(cptr[j * 3]));
		}
		fprintf(fp1, "\n");
		fprintf(fp2, "\n");
		fclose(fp1);
		fclose(fp2);
	}
	return 0;
}