#include "NaiveBayes.cpp"
#include <cstdio>
#include <algorithm>
std::unordered_map<std::string, int>strTable;
std::unordered_map<int, int>classTable;
char nums[10], tarstr[100];
std::vector<std::vector<std::string>>dataSet;
std::vector<int>classSet;
NaiveBayes<std::string, int> nb;
int main()
{
	classTable[-1] = 0;
	classTable[1] = 1;
	nb.SetClassListbyHashMap(classTable);

	classSet.resize(50);
	std::fill(classSet.begin(), classSet.begin() + 25, 1);
	std::fill(classSet.begin() + 25, classSet.begin() + 50, -1);

	std::string str1 = "D:\\bayes\\ham\\";
	std::string str2 = ".txt";

	dataSet.resize(50);
	for (int i = 1; i <= 25; ++i)
	{
		_itoa(i, nums, 10);
		FILE* fp = fopen((str1 + nums + str2).c_str(), "r");
		if (!fp)
		{
			printf("Can't open ham file: %d\n", i);
			return 0;
		}
		while (~fscanf(fp, "%s", tarstr))
		{
			dataSet[i - 1].push_back(tarstr);
			if (strTable.find(tarstr) == strTable.end())
				strTable[tarstr] = strTable.size();
		}
		fclose(fp);
	}

	str1 = "D:\\bayes\\spam\\";
	for (int i = 1; i <= 25; ++i)
	{
		_itoa(i, nums, 10);
		FILE* fp = fopen((str1 + nums + str2).c_str(), "r");
		if (!fp)
		{
			printf("Can't open spam file: %d\n", i);
			return 0;
		}
		while (~fscanf(fp, "%s", tarstr))
		{
			dataSet[i + 24].push_back(tarstr);
			if (strTable.find(tarstr) == strTable.end())
				strTable[tarstr] = strTable.size();
		}
		fclose(fp);
	}

	nb.SetFeatureListbyHashMap(strTable);
	nb.InputDataSet(dataSet, classSet);
	nb.CalculateProbability();

	return 0;
}