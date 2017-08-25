#include "black.h"
#include "../xll/xll.h"

using namespace xll;
using namespace black;

AddIn xai_normal_cdf(
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
	double Nx_ = Excel(xlfNormsdist, OPER(x));
	ensure (fabs(Nx - Nx_) < std::numeric_limits<double>::epsilon());
} 
TEST_END

AddIn xal_black_put_value(
	Function(XLL_DOUBLE, L"?xll_black_put_value", L"PUT.VALUE")
	.Arg(XLL_DOUBLE, L"f", L"is the forward..")
	.Arg(XLL_DOUBLE, L"sigma", L"is the volatility..")
	.Arg(XLL_DOUBLE, L"k", L"is the strike..")
	.Arg(XLL_DOUBLE, L"t", L"is the time in years to expiration")
	.Category(L"BLACK")
	.FunctionHelp(L"The forward value of a put option using the Black model")
	.Documentation()
);
double WINAPI xll_black_put_value(double f, double s, double k, double t)
{
#pragma XLLEXPORT
	double value;

	try {
		ensure (f > 0);
		ensure (s > 0);
		ensure (k > 0);
		ensure (t > 0);

		value = black::put_value(f, s, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		value = std::numeric_limits<double>::quiet_NaN();
	}

	return value;
}

TEST_BEGIN(black_put)

// Test for xll_black_put using formula (27) of  http://kalx.net/BlaSch1973.pdf 
// f = x = 100, s = v = 0.2, k = c = 100, t = t* = 0.25, and r = 0. 

	double f = 100, k = 100, s = 0.2, t = 0.25, r = 0;
	double d1 = (log(f/k) + (r + s*s/2)*t)/(s*sqrt(t));
	double d2 = (log(f/k) + (r - s*s/2)*t)/(s*sqrt(t));
	double N1 = prob::normal_cdf(-d1);
	double N2 = prob::normal_cdf(-d2);
	double v = -f*N1 + k*N2;

	ensure (v == black::put_value(f, s, k, t));

TEST_END

#pragma warning(push)
#pragma warning(disable: 100 702)

Auto<Open> xao_gsl([] {
	auto handler = [](const char* reason, const char* file, int line, int err) {
		char buf[1024];
		sprintf_s(buf, "%s\nfile: %s\nline: %d\nerrno: %d", reason, file, line, err);
		MessageBoxA(0, buf, "GSL Error", MB_OK);
	};
	using handler_t = void(__cdecl*)(const char*, const char*, int, int);
	gsl_set_error_handler(static_cast<handler_t>(handler));
	
	return TRUE;
});

//!!! Add arguments, category, and function help.
AddIn xaa_black_implied_volatilty(
	Function(XLL_DOUBLE, L"?xll_black_implied_volatility", L"IMPLIED.VOLATILITY")
	.Arg(XLL_DOUBLE, L"f", L"is the forward price")
	.Arg(XLL_DOUBLE, L"p",L"is the value of the put option ")
	.Arg(XLL_DOUBLE, L"k",L"is the strike price")
	.Arg(XLL_DOUBLE,L"t",L"is the time in years to expiration")
	.Category(L"Black")
	.FunctionHelp(L"Return the implied volatility of the stock")
);

double WINAPI xll_black_implied_volatility(double f, double p, double k, double t)
{
#pragma XLLEXPORT
	double s = std::numeric_limits<double>::quiet_NaN();
	double value;
	try {
		//!!! ensure parameters are valid
			ensure(f > 0);
			ensure(p > 0);
			ensure(k > 0);
			ensure(t > 0);
	
		//!!! compute the implied volatility
			value = black::implied_volatility(f, p, k, t);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return value;
}

TEST_BEGIN(implied_volatility)

//!!! Test the implied volatility for the values
// f = 100, s = 0.2, k = 100, and t  = 0.25. 
double f = 100, k = 100, s = 0.2, t = 0.25;
double p = black::put_value(f, s, k, t);
double v = black::implied_volatility(f,p,k,t);

ensure(fabs(s-v)<s*std::numeric_limits<double>::epsilon());

TEST_END
