// poly.cpp - Polynomial functions
#include "poly.h"
#include "../xll/xll.h"

using namespace xll;

AddIn xai_Hermite(
	Function(XLL_DOUBLE, L"?xll_Hermite", L"HERMITE")
	.Arg(XLL_LONG, L"n", L"is the order of the polynomial..")
	.Arg(XLL_DOUBLE, L"x", L"is a number..")
	.Category(L"POLY")
);
double WINAPI xll_Hermite(long n, double x)
{
#pragma XLLEXPORT
	return poly::Hermite(n,x);
}

AddIn xai_Bell(
	Function(XLL_DOUBLE, L"?xll_Bell", L"BELL")
	.Arg(XLL_FP, L"kappa", L"is the vector of cumulants..")
	.Category(L"POLY")
);
double WINAPI xll_Bell(_FP12* kappa)
{
#pragma XLLEXPORT
	int n = kappa->rows*kappa->columns;

	return poly::Bell(n, kappa->array);
}

//!!! Uncomment and implement tests.
TEST_BEGIN(hermite)

//!!! Calculate H_3(x) by hand and check at x = -1, 0, 2.
ensure(xll_Hermite(3, -1) == 2);
ensure(xll_Hermite(3, 0) == 0);
ensure(xll_Hermite(3, 2) == 2);

TEST_END

TEST_BEGIN(Bell)

//!!! Calculate B_3(x_1, x_2, x_3) and check at x_1 = -1, x_2 = 0, x_3 = 2;
double x_1 = -1;
double x_2 = 0;
double x_3 = 2;
double kappa[3] = { x_1, x_2, x_3 };

ensure(poly::Bell(3,kappa) == x_1*x_1*x_1 + 3 / 2 * x_1*x_2 + x_3);

TEST_END