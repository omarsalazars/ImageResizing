#include "MathUtil.h"

/**
 * Params:
 * double x - Value of X parameter in f(x)
 * int n - Size of xs/fx
 * double *xs - Array containing X axis coordinates of the image
 * double *fx - Coefficients of polynomial. In this case, fx contains the intensity values of the image.
 * 
 * For a polynomial of grade N, then N+1 coefficients needs to be calculated.
*/
double MathUtil::Lagrange(double x,int n,double *xs,double *fx)
{
    double P=0;
    for(int i=0;i<n;i++)
    {
        double d=1,k=1;
        for(int j=0;j<n;j++)
        {
            if(i==j) continue;
            k*=(double)x-xs[j];
            d*=(double)xs[i]-xs[j];
        }
        P+=(k/d)*fx[i];
    }
    if(P>255)
        return 255;
    if(P<0)
        return 0;
    return P;
}