/** @file inifcns.cpp
 *
 *  Implementation of GiNaC's initially known functions. */

/*
 *  GiNaC Copyright (C) 1999-2000 Johannes Gutenberg University Mainz, Germany
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <vector>
#include <stdexcept>

#include "inifcns.h"
#include "ex.h"
#include "constant.h"
#include "lst.h"
#include "matrix.h"
#include "mul.h"
#include "ncmul.h"
#include "numeric.h"
#include "power.h"
#include "relational.h"
#include "pseries.h"
#include "symbol.h"
#include "utils.h"

#ifndef NO_NAMESPACE_GINAC
namespace GiNaC {
#endif // ndef NO_NAMESPACE_GINAC

//////////
// absolute value
//////////

static ex abs_evalf(const ex & x)
{
    BEGIN_TYPECHECK
        TYPECHECK(x,numeric)
    END_TYPECHECK(abs(x))
    
    return abs(ex_to_numeric(x));
}

static ex abs_eval(const ex & x)
{
    if (is_ex_exactly_of_type(x, numeric))
        return abs(ex_to_numeric(x));
    else
        return abs(x).hold();
}

REGISTER_FUNCTION(abs, eval_func(abs_eval).
                       evalf_func(abs_evalf));


//////////
// Complex sign
//////////

static ex csgn_evalf(const ex & x)
{
    BEGIN_TYPECHECK
        TYPECHECK(x,numeric)
    END_TYPECHECK(csgn(x))
    
    return csgn(ex_to_numeric(x));
}

static ex csgn_eval(const ex & x)
{
    if (is_ex_exactly_of_type(x, numeric))
        return csgn(ex_to_numeric(x));
    
    else if (is_ex_exactly_of_type(x, mul)) {
        numeric oc = ex_to_numeric(x.op(x.nops()-1));
        if (oc.is_real()) {
            if (oc > 0)
                // csgn(42*x) -> csgn(x)
                return csgn(x/oc).hold();
            else
                // csgn(-42*x) -> -csgn(x)
                return -csgn(x/oc).hold();
        }
        if (oc.real().is_zero()) {
            if (oc.imag() > 0)
                // csgn(42*I*x) -> csgn(I*x)
                return csgn(I*x/oc).hold();
            else
                // csgn(-42*I*x) -> -csgn(I*x)
                return -csgn(I*x/oc).hold();
        }
	}
   
    return csgn(x).hold();
}

static ex csgn_series(const ex & x, const relational & rel, int order)
{
    const ex x_pt = x.subs(rel);
    if (x_pt.info(info_flags::numeric)) {
        if (ex_to_numeric(x_pt).real().is_zero())
            throw (std::domain_error("csgn_series(): on imaginary axis"));
        epvector seq;
        seq.push_back(expair(csgn(x_pt), _ex0()));
        return pseries(rel,seq);
    }
    epvector seq;
    seq.push_back(expair(csgn(x_pt), _ex0()));
    return pseries(rel,seq);
}

REGISTER_FUNCTION(csgn, eval_func(csgn_eval).
                        evalf_func(csgn_evalf).
                        series_func(csgn_series));

//////////
// dilogarithm
//////////

static ex Li2_eval(const ex & x)
{
    // Li2(0) -> 0
    if (x.is_zero())
        return x;
    // Li2(1) -> Pi^2/6
    if (x.is_equal(_ex1()))
        return power(Pi,_ex2())/_ex6();
    // Li2(1/2) -> Pi^2/12 - log(2)^2/2
    if (x.is_equal(_ex1_2()))
        return power(Pi,_ex2())/_ex12() + power(log(_ex2()),_ex2())*_ex_1_2();
    // Li2(-1) -> -Pi^2/12
    if (x.is_equal(_ex_1()))
        return -power(Pi,_ex2())/_ex12();
    // Li2(I) -> -Pi^2/48+Catalan*I
    if (x.is_equal(I))
        return power(Pi,_ex2())/_ex_48() + Catalan*I;
    // Li2(-I) -> -Pi^2/48-Catalan*I
    if (x.is_equal(-I))
        return power(Pi,_ex2())/_ex_48() - Catalan*I;
    return Li2(x).hold();
}

static ex Li2_deriv(const ex & x, unsigned deriv_param)
{
    GINAC_ASSERT(deriv_param==0);
    
    // d/dx Li2(x) -> -log(1-x)/x
    return -log(1-x)/x;
}

static ex Li2_series(const ex &x, const relational &rel, int order)
{
    const ex x_pt = x.subs(rel);
    if (!x_pt.is_zero() && !x_pt.is_equal(_ex1()))
        throw do_taylor();  // caught by function::series()
    // First case: x==0 (derivatives have poles)
    if (x_pt.is_zero()) {
        // method:
        // The problem is that in d/dx Li2(x==0) == -log(1-x)/x we cannot 
        // simply substitute x==0.  The limit, however, exists: it is 1.  We
        // also know all higher derivatives' limits: (d/dx)^n Li2(x) == n!/n^2.
        // So the primitive series expansion is Li2(x==0) == x + x^2/4 + x^3/9
        // and so on.
        // We first construct such a primitive series expansion manually in
        // a dummy symbol s and then insert the argument's series expansion
        // for s.  Reexpanding the resulting series returns the desired result.
        const symbol s;
        ex ser;
        // construct manually the primitive expansion
        for (int i=1; i<order; ++i)
            ser += pow(s,i)/pow(numeric(i),numeric(2));
        // substitute the argument's series expansion
        ser = ser.subs(s==x.series(rel,order));
        // maybe that was terminanting, so add a proper order term
        epvector nseq;
        nseq.push_back(expair(Order(_ex1()), numeric(order)));
        ser += pseries(rel, nseq);
        // reexpand will collapse the series again
        ser = ser.series(rel,order);
        return ser;
    }
    // second problematic case: x real, >=1 (branch cut)
    return pseries();
    // TODO: Li2_series should do something around branch point?
    // Careful: may involve logs!
}

REGISTER_FUNCTION(Li2, eval_func(Li2_eval).
                       derivative_func(Li2_deriv).
                       series_func(Li2_series));

//////////
// trilogarithm
//////////

static ex Li3_eval(const ex & x)
{
    if (x.is_zero())
        return x;
    return Li3(x).hold();
}

REGISTER_FUNCTION(Li3, eval_func(Li3_eval));

//////////
// factorial
//////////

static ex factorial_evalf(const ex & x)
{
    return factorial(x).hold();
}

static ex factorial_eval(const ex & x)
{
    if (is_ex_exactly_of_type(x, numeric))
        return factorial(ex_to_numeric(x));
    else
        return factorial(x).hold();
}

REGISTER_FUNCTION(factorial, eval_func(factorial_eval).
                             evalf_func(factorial_evalf));

//////////
// binomial
//////////

static ex binomial_evalf(const ex & x, const ex & y)
{
    return binomial(x, y).hold();
}

static ex binomial_eval(const ex & x, const ex &y)
{
    if (is_ex_exactly_of_type(x, numeric) && is_ex_exactly_of_type(y, numeric))
        return binomial(ex_to_numeric(x), ex_to_numeric(y));
    else
        return binomial(x, y).hold();
}

REGISTER_FUNCTION(binomial, eval_func(binomial_eval).
                            evalf_func(binomial_evalf));

//////////
// Order term function (for truncated power series)
//////////

static ex Order_eval(const ex & x)
{
	if (is_ex_exactly_of_type(x, numeric)) {

		// O(c)=O(1)
		return Order(_ex1()).hold();

	} else if (is_ex_exactly_of_type(x, mul)) {

	 	mul *m = static_cast<mul *>(x.bp);
		if (is_ex_exactly_of_type(m->op(m->nops() - 1), numeric)) {

			// O(c*expr)=O(expr)
			return Order(x / m->op(m->nops() - 1)).hold();
		}
	}
	return Order(x).hold();
}

static ex Order_series(const ex & x, const relational & r, int order)
{
	// Just wrap the function into a pseries object
	epvector new_seq;
    GINAC_ASSERT(is_ex_exactly_of_type(r.lhs(),symbol));
    const symbol *s = static_cast<symbol *>(r.lhs().bp);
	new_seq.push_back(expair(Order(_ex1()), numeric(min(x.ldegree(*s), order))));
	return pseries(r, new_seq);
}

// Differentiation is handled in function::derivative because of its special requirements

REGISTER_FUNCTION(Order, eval_func(Order_eval).
                         series_func(Order_series));

//////////
// Inert partial differentiation operator
//////////

static ex Derivative_eval(const ex & f, const ex & l)
{
	if (!is_ex_exactly_of_type(f, function)) {
        throw(std::invalid_argument("Derivative(): 1st argument must be a function"));
	}
    if (!is_ex_exactly_of_type(l, lst)) {
        throw(std::invalid_argument("Derivative(): 2nd argument must be a list"));
    }
	return Derivative(f, l).hold();
}

REGISTER_FUNCTION(Derivative, eval_func(Derivative_eval));

//////////
// Solve linear system
//////////

ex lsolve(const ex &eqns, const ex &symbols)
{
    // solve a system of linear equations
    if (eqns.info(info_flags::relation_equal)) {
        if (!symbols.info(info_flags::symbol)) {
            throw(std::invalid_argument("lsolve: 2nd argument must be a symbol"));
        }
        ex sol=lsolve(lst(eqns),lst(symbols));
        
        GINAC_ASSERT(sol.nops()==1);
        GINAC_ASSERT(is_ex_exactly_of_type(sol.op(0),relational));
        
        return sol.op(0).op(1); // return rhs of first solution
    }
    
    // syntax checks
    if (!eqns.info(info_flags::list)) {
        throw(std::invalid_argument("lsolve: 1st argument must be a list"));
    }
    for (unsigned i=0; i<eqns.nops(); i++) {
        if (!eqns.op(i).info(info_flags::relation_equal)) {
            throw(std::invalid_argument("lsolve: 1st argument must be a list of equations"));
        }
    }
    if (!symbols.info(info_flags::list)) {
        throw(std::invalid_argument("lsolve: 2nd argument must be a list"));
    }
    for (unsigned i=0; i<symbols.nops(); i++) {
        if (!symbols.op(i).info(info_flags::symbol)) {
            throw(std::invalid_argument("lsolve: 2nd argument must be a list of symbols"));
        }
    }
    
    // build matrix from equation system
    matrix sys(eqns.nops(),symbols.nops());
    matrix rhs(eqns.nops(),1);
    matrix vars(symbols.nops(),1);
    
    for (unsigned r=0; r<eqns.nops(); r++) {
        ex eq = eqns.op(r).op(0)-eqns.op(r).op(1); // lhs-rhs==0
        ex linpart = eq;
        for (unsigned c=0; c<symbols.nops(); c++) {
            ex co = eq.coeff(ex_to_symbol(symbols.op(c)),1);
            linpart -= co*symbols.op(c);
            sys.set(r,c,co);
        }
        linpart=linpart.expand();
        rhs.set(r,0,-linpart);
    }
    
    // test if system is linear and fill vars matrix
    for (unsigned i=0; i<symbols.nops(); i++) {
        vars.set(i,0,symbols.op(i));
        if (sys.has(symbols.op(i)))
            throw(std::logic_error("lsolve: system is not linear"));
        if (rhs.has(symbols.op(i)))
            throw(std::logic_error("lsolve: system is not linear"));
    }
    
    //matrix solution=sys.solve(rhs);
    matrix solution;
    try {
        solution = sys.fraction_free_elim(vars,rhs);
    } catch (const runtime_error & e) {
        // probably singular matrix (or other error)
        // return empty solution list
        // cerr << e.what() << endl;
        return lst();
    }
    
    // return a list of equations
    if (solution.cols()!=1) {
        throw(std::runtime_error("lsolve: strange number of columns returned from matrix::solve"));
    }
    if (solution.rows()!=symbols.nops()) {
        cout << "symbols.nops()=" << symbols.nops() << endl;
        cout << "solution.rows()=" << solution.rows() << endl;
        throw(std::runtime_error("lsolve: strange number of rows returned from matrix::solve"));
    }
    
    // return list of the form lst(var1==sol1,var2==sol2,...)
    lst sollist;
    for (unsigned i=0; i<symbols.nops(); i++) {
        sollist.append(symbols.op(i)==solution(i,0));
    }
    
    return sollist;
}

/** non-commutative power. */
ex ncpower(const ex &basis, unsigned exponent)
{
    if (exponent==0) {
        return _ex1();
    }

    exvector v;
    v.reserve(exponent);
    for (unsigned i=0; i<exponent; ++i) {
        v.push_back(basis);
    }

    return ncmul(v,1);
}

/** Force inclusion of functions from initcns_gamma and inifcns_zeta
 *  for static lib (so ginsh will see them). */
unsigned force_include_tgamma = function_index_tgamma;
unsigned force_include_zeta1 = function_index_zeta1;

#ifndef NO_NAMESPACE_GINAC
} // namespace GiNaC
#endif // ndef NO_NAMESPACE_GINAC
