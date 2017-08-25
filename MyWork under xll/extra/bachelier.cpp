// bachelier.cpp - The Bachelier model
#include "../xll/xll.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "prob.h"
#include "fms_newton.h"
#include"fms_black.h"


using namespace xll;
using namespace fms;
using namespace black;


// In the Bachelier model of stock price as Brownain motion, the value of a call is
// E max{F_t - k, 0} = (f - k) N(d) + sigma sqrt(t) N'(d), where N is the normal cdf.
// Implement a function for valuing the call option in the Bachelier model
inline double bachelier_call_value(double f, double sigma, double k, double t)
{
	double srt = sigma*sqrt(t);
	return (f - k)*(prob::normal_cdf((f-k)/srt)) + srt*prob::normal_pdf((f-k) / srt);
}

// Implement an Excel add-in for this function called BACHELIER.CALL.VALUE.
static AddInX xai_bachelier_call_value(
	FunctionX(XLL_DOUBLE, L"?xll_bachelier_call_value", L"BACHELIER.CALL.VALUE")
	.Arg(XLL_DOUBLE, L"f", L"is the forward..")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility..")
	.Arg(XLL_DOUBLE, L"k", L"is the strike..")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration")
	.Category(L"BACHELIER")
	.FunctionHelp(L"The forward value of a put option using the BACHELIER model")
	.Documentation(L"bacheliermodel")
);
double WINAPI xll_bachelier_call_value(double f, double sigma, double k, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		ensure(f > 0);
		ensure(sigma > 0);
		ensure(k > 0);
		ensure(t > 0);

		value = bachelier_call_value(f, sigma, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		value = std::numeric_limits<double>::quiet_NaN();
	}

	return value;
}



// Implement tests for the bachelier_call_value function.
TEST_BEGIN(bachelier_call_value)              //下面给的信息是用来让自己验证公式的，不能直接放数据这样验证！

// f = 100, s = 0.2, k = 100, t = 0.25. 

double f = 100, k = 100, s = 0.2, t = 0.25;
double d = (f-k) / (s*sqrt(t));
double N1 = prob::normal_cdf(d);
double N2 = prob::normal_pdf(d);
double v = (f - k)*N1 + s*sqrt(t)*N2;

ensure(bachelier_call_value(f,s,k,t)==v);

TEST_END

// Create a spreadsheet that calls this function.

// What is the formula for F_t?
// F_t = f + sigma * B(t)
// B(t) as a Brownian Motion
