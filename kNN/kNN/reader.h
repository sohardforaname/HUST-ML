#pragma once
#include <cstdio>
#include <cstring>
class Reader
{
private:
    const int MAXBUF = 1 << 20;
    char buf[1 << 20], * fh = NULL, * ft = NULL;
    FILE* fp = NULL;
    char gc();

public:
    Reader();
    void init(FILE* _fp);
    int read();
};