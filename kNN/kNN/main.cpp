#include "kNN.h"
using namespace std;
int main()
{
    kNN* knn = new kNN(784, 60000, 10000, 200, 10, 3, 1, 1);
    //my specific format data
    knn->Work("D:\\1.txt", "D:\\2.txt", stdout, 0);
    delete knn;
    return 0;
}
