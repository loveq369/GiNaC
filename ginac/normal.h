/** @file normal.h
 *
 *  Functions for polynomial quotient and remainder, GCD and LCM computation
 *  and square-free factorization.
 *
 *  GiNaC Copyright (C) 1999 Johannes Gutenberg University Mainz, Germany
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

#ifndef NORMAL_H
#define NORMAL_H

class ex;
class symbol;

// Quotient q(x) of polynomials a(x) and b(x) in Q[x], so that a(x)=b(x)*q(x)+r(x)
extern ex quo(const ex &a, const ex &b, const symbol &x, bool check_args = true);

// Remainder r(x) of polynomials a(x) and b(x) in Q[x], so that a(x)=b(x)*q(x)+r(x)
extern ex rem(const ex &a, const ex &b, const symbol &x, bool check_args = true);

// Pseudo-remainder of polynomials a(x) and b(x) in Z[x]
extern ex prem(const ex &a, const ex &b, const symbol &x, bool check_args = true);

// Exact polynomial division of a(X) by b(X) in Q[X] (quotient returned in q), returns false when exact division fails
extern bool divide(const ex &a, const ex &b, ex &q, bool check_args = true);

// Polynomial GCD in Z[X], cofactors are returned in ca and cb, if desired
extern ex gcd(const ex &a, const ex &b, ex *ca = NULL, ex *cb = NULL, bool check_args = true);

// Polynomial LCM in Z[X]
extern ex lcm(const ex &a, const ex &b, bool check_args = true);

// Square-free factorization of a polynomial a(x)
extern ex sqrfree(const ex &a, const symbol &x);

#endif