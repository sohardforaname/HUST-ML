#include "kNN.h"
using namespace std;
int main()
{
    kNN* knn = new kNN(784, 60000, 10000, 200, 10, 3, 1, 1);
    knn->Work("you_data_path", "you_test_path", stdout, 0);
    delete knn;
    return 0;
}
