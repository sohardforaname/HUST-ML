#include "reader.h"

Reader::Reader() {}

Reader::~Reader()
{
    delete[]buf;
}

char Reader::gc()
{
    if (fh == ft)
    {
        int l = fread(buf, 1, MAXBUF, fp);
        ft = (fh = buf) + l;
    }
    return *fh++;
}

void Reader::init(FILE* _fp)
{
    buf = new char[1 << 20];
    fp = _fp;
    memset(buf, 0, sizeof(buf));
    fh = ft = NULL;
}

int Reader::read()
{
    int x = 0;
    char c = gc();
    for (; c < '0' || c > '9'; c = gc())
        ;
    for (; c >= '0' && c <= '9'; c = gc())
        x = (x << 3) + (x << 1) + c - '0';
    return x;
}