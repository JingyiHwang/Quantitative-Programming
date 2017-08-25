//
//  关于gsl函数详细.h
// 顺序永远是：create a solver, set up, solve.

//第一种
/* gsl_root_fsolver_brent
 这种方法要求：1. 函数保证一定有根，而且根的两边是穿过x轴的关系（两边异号）；2.先找到根可能在的区间。
*/

 
inline double implied_volatility(double f, double p, double k, double t)
{
    auto v= [f,p,k,t](double sig){return p-put_value(f,sig,k,t);} ;
    
    double s_lo, s_hi;
    
    s_lo=.1;
    s_hi=.2;
    
    while(v(s_lo)>0)   // 这里注意，由于s越小，put_value越小，v越大！故 s_lo对应v为正的情况，s_hi对应v为负的情况
        s_lo/=2;       // why..
    
    while(v(s_hi)>0)
        s_hi*=2;
    
    gsl::root::fsolver s(gsl_root_fsolver_brent);
    
    double epsrel=1e-6;
    
    solver.set(v,s_lo,s_hi);
    double root=s.solve(gsl::root::test_interval(0,epsrel));  //测试区间［a，b］上是否有convergence
    
    return root;
    
}


//第二种
/* gsl_root_fdfsolver_newton
 这种方法要求：1.a initial guess for the location of the root (close enough) 2. 先要写出f‘(x).
 */
inline double put_vega(double f, double s, double k, double t)
{
    auto srt=sqrt(t);
    return f*prob::normal_pdf(log(k/f)/(s*srt)+s*srt/2)*srt;
}


inline double implied_volatility(double f, double p, double k, double t)
{
    auto v=[f,p,k,t](double s){ return p-put_value(f,s,k,t);};
    auto dv=[f,p,k,t](double s) { return -put_vega(f,s,k,t);};
    
    double s0=0.2;
    
    gsl::root::fdfsolver solver(gsl_root_fdfsolver_newton);
    solver.set(v,dv,s0);
    
    double epsrel=1e-8;
    return solver.solver(gsl::root::test_delta(0,epsrel)); //delta test, 测试序列x0，x1，...是否满足convergence
}

