#ifndef _NaiveBayes
#define _NaiveBayes

#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <cmath>

template<class featureType, class classType>
class NaiveBayes
{
private:
	std::unordered_map<featureType, int>featureTable;
	std::unordered_map<classType, int>classTable;
	std::vector<std::vector<int>>featureCounter;
	std::vector<int>classCounter;
	std::vector<std::vector<double>>probTable;
	void preparation();

public:
	void SetFeatureListbyHashMap(const std::unordered_map<featureType, int>& _featureTable);
	void SetFeatureListbyTreeMap(const std::map<featureType, int>& _featureTable);
	void SetClassListbyHashMap(const std::unordered_map<classType, int>& _classTable);
	void SetClassListbyTreeMap(const std::map<classType, int>& _classTable);
	void InputDataSet(const std::vector<std::vector<featureType>>& _dataSet, const std::vector<classType>& _classSet);
	std::vector<classType> Predict(const std::vector<std::vector<featureType>>& _testSet);
	void CalculateProbability();
};

#endif // !_NaiveBayes