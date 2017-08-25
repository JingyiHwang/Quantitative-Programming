#include "../xll/xll.h"
#include <cmath>
using namespace xll;

AddIn xai_expm1(Function(XLL_DOUBLE, L"?xll_expm1", L"expm1")
	.Arg(XLL_DOUBLE, L"x")
	.Category(L"Category")
	.FunctionHelp(L"Return expm1x")
);

double WINAPI xll_expm1(double x)
{
#pragma XLLEXPORT
	return EXP(1/2^n)-1;
}
