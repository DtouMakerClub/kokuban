#include "RCFilter.hpp"

RCFilter::RCFilter(const float a):_a(a)
{

}

void RCFilter::init(const long initialOutput)
{
    _out = _latest = initialOutput;
}

void RCFilter::input(long latest)
{
    _latest = latest;
}

void RCFilter::update()
{
    _out = _a * _out + (1.0 - _a) * _latest;
}

long RCFilter::output()
{
    return _out;
}