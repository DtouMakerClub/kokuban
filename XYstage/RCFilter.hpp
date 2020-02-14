#ifndef RC_FILTER_H
#define RC_FILTER_H

class RCFilter
{
private:
    float _a;
    long _latest = 0;
    long _out = 0;


public:
    RCFilter(const float a);
    void init(const long initialOutput);
    void input(long latest);
    void update();
    long output();
};

#endif //RC_FILTER_H