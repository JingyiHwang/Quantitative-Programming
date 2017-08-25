// fms_bootstrap.h - Bootstrap a piecewise flat forward curve.
#pragma once
#include "fms_newton.h"
#include "fms_pwflat.h"

namespace fms {
namespace pwflat {
	
	// Extrapolate curve to match price with present value.
	template<class T, class F>
	inline F bootstrap(F p, size_t m, const T* u, const F* c, size_t n, const T* t, const F* f)
	{
		auto pv = [p, m, u, c, n, t, f](double _f) {
			return -p + present_value(m, u, c, n, t, f, _f);
		};
		auto dpv = [m, u, c, n, t, f](double _f) {
			return duration_extrapolated(m, u, c, n, t, f, _f);
		};

		//!!! Use fms::root1d::newtion to solve for the extrapolated value.
		double _f = 0;                        //实际使用过程中，千万不能仍然用 F,T 这种虚拟类型！
        if(n>0)                              //由于是从现有的f的最后一项开始延伸的，所以初值应该在f的最后一项的附近（但由于向量从0开始，f的最后一项是f[n-1]
            _f=f[n-1];
        
		_f = fms::root1d::newton<double, double>(_f, pv, dpv);

		return _f;
	}

}
} // namespace fms
