#include "kNN.h"
using namespace std;
char dp[100], tp[100], cp[100];
int main()
{
    printf("Input the path of configation file\n");
    scanf("%s", cp);
    FILE* fp = fopen(cp, "r");
    int a, b, c, d, e, f;
    if (fp && fscanf(fp, "%d%d%d%d%d%d%s%s",
        &a, &b, &c, &d, &e, &f, dp, tp) == 8)
    {
        kNN knn(a, b, c, d, e, f);
        knn.Work(dp, tp);
    }
    else
        printf("Config file error\n");
    system("pause");
    return 0;
}
