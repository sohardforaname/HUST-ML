#include "kNN.hpp"

using namespace std;
int main()
{
    kNN knn(784, 60000, 10000, 200, 10, 3, 1);
    knn.Work("D:\\1.txt", "D:\\2.txt", NULL, 0);
    return 0;
}

