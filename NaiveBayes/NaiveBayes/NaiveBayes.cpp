#include "NaiveBayes.h"

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::preparation()
{
	if (this->classTable.size() && this->featureTable.size())
	{
		featureCounter.resize(this->classTable.size());
		for (auto& it : featureCounter)
			it.resize(this->featureTable.size());
		classCounter.resize(this->classTable.size());
	}
}

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::
	SetFeatureListbyHashMap(const std::unordered_map<featureType, int>& _featureTable)
{
	this->featureTable = _featureTable;
	preparation();
}

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::
	SetFeatureListbyTreeMap(const std::map<featureType, int>& _featureTable)
{
	for (auto &it : _featureTable)
		this->featureTable.insert(it);
	preparation();
}

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::
	SetClassListbyHashMap(const std::unordered_map<classType, int>& _classTable)
{
	this->classTable = _classTable;
	preparation();
}

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::
	SetClassListbyTreeMap(const std::map<classType, int>& _classTable)
{
	for (auto& it : _classTable)
		this->classTable.insert(it);
	preparation();
}

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::
	InputDataSet(const std::vector<std::vector<featureType>>& _dataSet, 
		const std::vector<classType>& _classSet)
{
	if (!this->classTable.size() && !this->featureTable.size())
		return;
	for (int itemIt = 0; itemIt != _dataSet.size(); ++itemIt)
		for (int dataIt = 0; dataIt != _dataSet[itemIt].size(); ++dataIt)
			++(this->featureCounter[classTable[_classSet[itemIt]]][featureTable[_dataSet[itemIt][dataIt]]]);
}

template<class featureType, class classType>
void NaiveBayes<featureType, classType>::CalculateProbability()
{
	int dictSize = this->featureTable.size();
	probTable.resize(featureCounter.size());
	for (int i = 0; i < probTable.size(); ++i)
		probTable[i].resize(featureCounter[i].size());
	for (int i = 0; i < this->featureCounter.size(); ++i)
		for (int j = 0; j < this->featureCounter[i].size(); ++j)
			this->probTable[i][j] = log2(double(featureCounter[i][j] + 1) / double(classCounter[i] + dictSize));
}

template<class featureType, class classType>
std::vector<classType> NaiveBayes<featureType, classType>::
	Predict(const std::vector<std::vector<featureType>>& _testSet)
{
	std::vector<int>classified(_testSet.size());
	std::pair<double, int>p;
	int dictSize = this->featureTable.size();
	for (int itemIt = 0; itemIt < _testSet.size(); ++itemIt)
	{
		p = { 0,0 };
		for (int classIt = 0; classIt < classCounter.size(); ++classIt)
		{
			double tmp = 1;
			for (int dataIt = 0; dataIt < _testSet[itemIt].size(); ++dataIt)
			{
				if (featureTable.find(_testSet[itemIt][dataIt]) != featureTable.end())
					tmp += probTable[itemIt][dataIt];
				else
					tmp += log2(double(1) / double(classCounter[classIt] + dictSize));
			}
			if (tmp > p.first)
			{
				p.first = tmp;
				p.second = classIt;
			}
		}
		classified[itemIt] = p.second;
	}
	return classified;
}
