// xll_nsr.cpp - Normal short rate add-in
#include "fms_nsr.h"
#include "../xll/xll.h"

// !!! Implement NSR.E.LOGD
using namespace fms;
using namespace nsr;
using namespace xll;

AddIn xbi_nsr_E_logD(
	Function(XLL_DOUBLE, L"?xll_nsr_E_logD", L"NSR_E_LOGD")
	.Arg(XLL_DOUBLE, L"sigma", L"is the coefficient of Brownian motion")
	.Arg(XLL_DOUBLE, L"Dt", L"is D_0(t)")
	.Arg(XLL_DOUBLE, L"Du", L"is D_0(u)")
	.Arg(XLL_DOUBLE, L"t", L"date for this price")
	.Arg(XLL_DOUBLE, L"u", L"maturity date")
	.Category(L"NSR")
);

double WINAPI xll_nsr_E_logD(double sigma, double Dt, double Du, double t, double u)
{
#pragma XLLEXPORT
	return fms::nsr::E_logD(sigma, Dt, Du, t, u);        //此处注意添加error caught和 parameter check
}

// !!! Implement NSR.VAR.LOGD
AddIn xai_nsr_Var_logD(
	Function(XLL_DOUBLE, L"?xll_nsr_Var_logD", L"NSR_VAR_LOGD")
	.Arg(XLL_DOUBLE, L"sigma", L"is the coefficient of Brownian motion")
	.Arg(XLL_DOUBLE, L"t", L"date for this price")
	.Arg(XLL_DOUBLE, L"u", L"maturity date")
	.Category(L"NSR")
);

double WINAPI xll_nsr_Var_logD(double sigma, double t, double u)
{
#pragma XLLEXPORT
	return fms::nsr::Var_logD(sigma, t, u);
}
