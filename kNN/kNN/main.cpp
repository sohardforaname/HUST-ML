#include "kNN.h"
kNN knn(784, 60000, 10000, 200, 10, 3);
using namespace std;
int main()
{
    knn.Work();
    return 0;
}
