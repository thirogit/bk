#include "stdafx.h"
#include <string>
#include "RegressionLine.hpp"
#include <math.h>

using namespace std;

RegressionLine::RegressionLine(Points & points)
{
    int n = (int)points.size();
    if (n < 2)
        throw (string("Must have at least two points"));


    double sumx=0,sumy=0,sumx2=0,sumy2=0,sumxy=0;
    double sxx,syy,sxy;

    // Conpute some things we need 
    map<double, double>::const_iterator i;
    for (i = points.begin(); i != points.end(); i++)
    {
        double x = i->first;
        double y = i->second;

        sumx += x;
        sumy += y;
        sumx2 += (x * x);
        sumy2 += (y * y);
        sumxy += (x * y);
    }
    sxx = sumx2 - (sumx * sumx / n);
    syy = sumy2 - (sumy * sumy / n);
    sxy = sumxy - (sumx * sumy / n);

    // Infinite slope_, non existant yIntercept
    if (abs(sxx) == 0)
        throw (string("Inifinite Slope"));

    // Calculate the slope_ and yIntercept
    slope_ = sxy / sxx;
    yIntercept_ = sumy / n - slope_ * sumx / n;

    // Compute the regression coefficient
    if (abs(syy) == 0)
        regressionCoefficient_ = 1;
    else
        regressionCoefficient_ = sxy / sqrt(sxx * syy);
}

RegressionLine::~RegressionLine()
{
}

const double RegressionLine::slope() const
{
    return slope_;
}

const double RegressionLine::yIntercept() const
{
    return yIntercept_;
}

const double RegressionLine::regressionCoefficient() const
{
    return regressionCoefficient_;
}

/*
int main()
{
    const int count = 10;
    double times[count] = { 
        -0.20707, 0.706672, 1.63739, 2.03117, 3.31874, 
        5.38201, 6.79971, 6.31814, 8.20829, 8.53994 };
    double water[count] = { 
        -0.319029, 0.0931669, 2.17286, 2.76818, 3.56743, 
        4.11772, 5.52709, 7.46613, 8.7654, 9.58096 };

    RegressionLine::Points pnts;

    for (int i = 0; i < count; i++)
    {
        pnts[times[i]] = water[i];
    }

    RegressionLine myLine(pnts);

    cout << "Slope = " << myLine.slope() << endl;
    cout << "yIntercept = " << myLine.yIntercept() << endl;
    cout << "Regression Coefficient = " 
         << myLine.regressionCoefficient() << endl;
}
*/
