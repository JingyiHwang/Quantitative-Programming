// xll_pwflat.cpp - Addin for piecewise flat curves.
#include "../../xll12/xll/xll.h"
#include "fms_pwflat.h"

using namespace fms;
using namespace xll;

TEST_BEGIN(fms_pwflat)

test_fms_pwflat();

TEST_END

AddIn xai_pwflat_value(
	Function(XLL_DOUBLE, L"?xll_pwflat_value", L"PWFLAT.VALUE")
	.Arg(XLL_DOUBLE, L"u", L"is the time at which to value the curve.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Value of piecewise flat forward curve.")
);
double WINAPI xll_pwflat_value(double u, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int n = size(*pt);
		ensure (n == size(*pf));

		fu = pwflat::value(u, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

//!!! Implement PWFLAT.INTEGRAL
AddIn xai_pwflat_integral(
	Function(XLL_DOUBLE, L"?xll_pwflat_integral", L"PWFLAT.INTEGRAL")
	.Arg(XLL_DOUBLE, L"u", L"is the time at which to value the curve.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"integral of piecewise flat forward curve.")
);
double WINAPI xll_pwflat_integral(double u, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int n = size(*pt);                                //size(* ..) 可以直接知道fp12所含数据个数
		ensure(n == size(*pf));

		fu = pwflat::integral(u, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

//!!! Implement PWFLAT.DISCOUNT
AddIn xai_pwflat_discount(
	Function(XLL_DOUBLE, L"?xll_pwflat_discount", L"PWFLAT.DISCOUNT")
	.Arg(XLL_DOUBLE, L"u", L"is the time at which to value the curve.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Discount for piecewise flat forward curve.")
);
double WINAPI xll_pwflat_value(double u, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int n = size(*pt);
		ensure(n == size(*pf));

		fu = pwflat::discount(u, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

//!!! Implement PWFLAT.SPOT
AddIn xai_pwflat_spot(
	Function(XLL_DOUBLE, L"?xll_pwflat_spot", L"PWFLAT.SPOT")
	.Arg(XLL_DOUBLE, L"u", L"is the time at which to value the curve.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Spot rate")
);
double WINAPI xll_pwflat_spot(double u, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int n = size(*pt);
		ensure(n == size(*pf));

		fu = pwflat::spot(u, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

AddIn xai_pwflat_present_value(
	Function(XLL_DOUBLE, L"?xll_pwflat_present_value", L"PWFLAT.PRESENT_VALUE")
	.Arg(XLL_FP, L"time", L"are the cash flow times.")
	.Arg(XLL_FP, L"cash", L"are the cash flows.")
	.Arg(XLL_FP, L"times", L"are the forward curve times.")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values..")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Value of piecewise flat forward curve.")
);
double WINAPI xll_pwflat_present_value(const _FP12* pu, const _FP12* pc, const _FP12* pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double pv = std::numeric_limits<double>::quiet_NaN();

	try {
		int m = size(*pu);
		ensure (m == size(*pc));
		int n = size(*pt);
		ensure (n == size(*pf));

		pv = pwflat::present_value(m, pu->array, pc->array, n, pt->array, pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return pv;
}

//!!! Implement PWFLAT.DURATION
AddIn xai_pwflat_duration(
	Function(XLL_DOUBLE, L"?xll_pwflat_duration", L"PWFLAT.DURATION")
	.Arg(XLL_FP, L"cf times", L"are the times of cash flow.")
	.Arg(XLL_FP, L"cash", L"are the cash flows.")
	.Arg(XLL_FP, L"fc times", L"are the times of forward curve")
	.Arg(XLL_FP,L"forwards",L"are the forward curve values")
	.Category(L"PWFLAT")
	.FunctionHelp(L"Duration.")
);
double WINAPI xll_pwflat_duration(const _FP12*pu, const _FP12* pc, const _FP12* pt, const _FP12*pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int p = size(*pu);
		ensure(p == size(*pc));
		int q = size(*pt);
		ensure(q == size(*pf));

		fu = pwflat::duration(p, pu->array, pc->array, q, pt->array,  pf->array);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}

//!!! Implement PWFLAT.DURATION_EXTRAPOLATED
AddIn xai_pwflat_duration_extrapolated(
	Function(XLL_DOUBLE, L"?xll_pwflat_duration_extrapolated", L"PWFLAT.DURATION_EXTRAPOLATED")
	.Arg(XLL_FP, L"cf times", L"are the times of cash flow.")
	.Arg(XLL_FP, L"cash", L"are the cash flows.")
	.Arg(XLL_FP, L"fc times", L"are the times of forward curve")
	.Arg(XLL_FP, L"forwards", L"are the forward curve values")
	.Category(L"PWFLAT")
	.FunctionHelp(L"derivative after last curve time")
);
double WINAPI xll_pwflat_deration_extrapolated(const _FP12* pu, const _FP12* pc, const _FP12*pt, const _FP12* pf)
{
#pragma XLLEXPORT
	double fu = std::numeric_limits<double>::quiet_NaN();

	try {
		int p = size(*pu);
		ensure(p == size(*pc));
		int q = size(*pt);
		ensure(q == size(*pf));

		fu = pwflat::duration_extrapolated(p, pu->array, pc->array, q, pt->array, pf->array);  //注意变成向量使用
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return fu;
}
