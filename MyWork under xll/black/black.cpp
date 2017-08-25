#include "black.h"
#include "../xll/xll.h"
#include<cmath>
#include "iostream"


using namespace xll;
using namespace black;
using namespace std;


AddIn xbb_normal_cdf(
	Function(XLL_DOUBLE, L"?xll_normal_cdf", L"NORMAL.CDF")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Category(L"PROB")
);
double WINAPI xll_normal_cdf(double x)
{
#pragma XLLEXPORT
	return prob::normal_cdf(x);
}

TEST_BEGIN(normal_cdf)

// Compare normal_cdf to Excel function NORMSDIST.

for (double x = -6; x <= 6; x += 0.1) {
	double Nx  = xll_normal_cdf(x);
	double Nx_ = Excel(xlfNormsdist, OPER(x));   //用excel里面的函数的函数是这个形式
	ensure (fabs(Nx - Nx_) < std::numeric_limits<double>::epsilon());
} 

TEST_END


//!!! Create an addin function called BLACK.PUT that calls black::put.
//AddIn xai_black_put(...

AddIn xii_black_put(
	Function(XLL_DOUBLE, L"?xll_black_put", L"BLACK.PUT")
	.Arg(XLL_DOUBLE, L"f", L"is the stock price")
	.Arg(XLL_DOUBLE, L"s", L"is the strandard deviation rate of the return on the stock")
	.Arg(XLL_DOUBLE, L"k", L"is the value of the option")
	.Arg(XLL_DOUBLE, L"t", L"is the time to maturity date from now")
	.Category(L"BLACK")
);

double WINAPI xll_black_put(double f, double s, double k, double t)
{
#pragma XLLEXPORT
	return black::put(f,s,k,t);
}

TEST_BEGIN(black_put)

//!!! Write a test for xll_black_put using formula (27) of 
// http://kalx.net/BlaSch1973.pdf for the values
// f = x = 100, s = v = 0.2, k = c = 100, t = t* = 0.25, and r = 0.  


	double x = 100, v = 0.2, c = 100, tstar = 0.25, r = 0;
	double d1 = (log(x / c) + (r + (v*v / 2 * tstar))) / (v*sqrt(tstar));
	double d2 = (log(x / c) + (r - (v*v*tstar / 2)) / (v*sqrt(tstar)));
	ensure(black::put(100, 0.2, 100, 0.25) == c*exp(-r*tstar)*prob::normal_cdf(-d2) - x*prob::normal_cdf(-d1));


TEST_END
