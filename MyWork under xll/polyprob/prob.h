// prob.h - Probability related functions.
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "poly.h"

#ifndef M_SQRTPI
#define M_SQRTPI   1.77245385090551602729816748334      /* sqrt(pi) */
#endif
#ifndef M_SQRT2
#define M_SQRT2    1.41421356237309504880168872421      /* sqrt(2) */
#endif

constexpr double M_SQRT2PI = M_SQRT2*M_SQRTPI;

namespace prob {

	inline double normal_pdf(double x)
	{
		return exp(-x*x/2)/(M_SQRT2PI);
	}
	inline double normal_cdf(double x)
	{
		return (1 + erf(x/M_SQRT2))/2;
	}

	// n-th derivative of cumulative normal distribution
	// Phi^(n)(x) = (-1)^{n-1} phi(x) H_{n-1}(x)
	inline double normal_dist(double x, int n)
	{
		if (n == 0)
			return normal_cdf(x);
		if (n == 1)
			return normal_pdf(x);

		return (n%2 == 0 ? -1 : 1)*normal_pdf(x)*poly::Hermite(n-1, x);  // (-1)^(n-1)的定义方式
	}

	//!!! Implement formula (3) from http://kalx.net/general-formula-option.pdf
	// where kappa are the cumulants of X. Since cumulants start at 1
	// assume kappa[0] is the first cumulant.
	inline double factorial(int i)                            //定义了阶乘！
	{
		if (i == 1)
			return 1.0;
		else
			return i*factorial(i - 1);
	}
	
	
	inline double njr_cdf(double x, int n, const double* kappa)
	{
		double phi = 0;
		for (int j = 1; j <= n; j++)
            
		{
			phi = phi + poly::Bell(j, kappa－1)*poly::Hermite(j - 1, x) / factorial(j);  //kappa就是kappa向量的第一个数据的指针表示形式，而（kappa－1)[i］=kappa[i-1]
		}
		
		double F = normal_dist(x, 0) - normal_dist(x, 1)*phi;
		return F;
	}

} // namespace prob

