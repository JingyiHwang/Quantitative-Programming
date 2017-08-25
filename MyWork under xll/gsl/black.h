// black.h - Black option forward pricing model.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "xll_roots.h"

namespace prob {                   // normal_pdf, normal_cdf

	inline double normal_pdf(double x)
	{
		return exp(-x*x/2)/(M_SQRT2*M_SQRTPI);
	}
	inline double normal_cdf(double x)
	{
		return (1 + erf(x/M_SQRT2))/2;       // erf 误差函数
	}

} // namespace prob

namespace black {

	// P(F <= k) where F = f exp(-s^2t/2 + s B_t)
	// B_t/sqrt(t) <= (log k/f - s^2t/2)/s sqrt(t)
	inline double P(double f, double s, double k, double t)
	{
		auto srt = s*sqrt(t);

		return prob::normal_cdf(log(k/f)/srt + srt/2);
	}

	// P(F exp(s^2t) <= k) where F = f exp(-s^2t/2 + s B_t)
	inline double P_(double f, double s, double k, double t)
	{
		auto srt = s*sqrt(t);

		return prob::normal_cdf(log(k/f)/srt - srt/2);
	}

	// E max(k - F, 0) where F = f exp(-s^2t/2 + s B_t)
	inline double put_value(double f, double s, double k, double t)
	{
		return k*P(f, s, k, t) - f*P_(f, s, k, t);
	}
	// Derivative of put value with respect to volatility.
	inline double put_vega(double f, double s, double k, double t)
	{
		auto srt = sqrt(t);

		return f*prob::normal_pdf(log(k/f)/(s*srt) + s*srt/2)*srt;
	}
#pragma warning(push)
#pragma warning(disable: 100 101)
	// Calculate volatility given price.
	//!!! *Use classes from gsl::root*
	inline double implied_volatility(double f, double p, double k, double t)
	{
		auto v = [f,p,k,t](double s) { return p - put_value(f, s, k, t); };

		auto dv = [f, p, k, t](double s)  {return (-1)*put_vega(f, s, k, t); };
		
		//!!! Find values that bracket the root.
		//!!! Create a 1-d solver from xll_roots.h
		gsl::root::fdfsolver s(gsl_root_fdfsolver_newton);

		//!!! Set up the solver.
		double epsrel = 1e-6;
		s.set(v, dv, 0.5);

		//!!! solve for the implied volatility
		double root = s.solve(gsl::root::test_delta(0, epsrel));
		return root;
	}
#pragma warning(pop) 
} // namespace black
