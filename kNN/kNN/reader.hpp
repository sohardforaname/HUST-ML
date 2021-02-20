#pragma once

#include <cstdio>
#include <cstring>
#include <fstream>

class Reader
{
private:
    const int MAXBUF = 1 << 20;
    char* buf, * fh = NULL, * ft = NULL;
    FILE* fp = NULL;
    inline char gc()
    {
        if (fh == ft)
            ft = (fh = buf) + fread(buf, 1, MAXBUF, fp);
        return *fh++;
    }

public:
    Reader()
        : buf(new char[1 << 20])
    {
        memset(buf, 0, sizeof(buf));
    }
    ~Reader()
    {
        delete[] buf;
    }
    inline void init(FILE* _fp)
    {
        fp = _fp;
        fh = ft = NULL;
    }
    inline int read()
    {
        int x = 0;
        char c = gc();
        for (; c < '0' || c > '9'; c = gc())
            ;
        for (; c >= '0' && c <= '9'; c = gc())
            x = (x << 3) + (x << 1) + c - '0';
        return x;
    }
  
};