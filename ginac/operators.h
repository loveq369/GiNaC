/** @file operators.h
 *
 *  Interface to GiNaC's overloaded operators.
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

#ifndef __GINAC_OPERATORS_H__
#define __GINAC_OPERATORS_H__

#include <iostream>

class ex;
class numeric;
class relational;

// binary arithmetic operators ex with ex
ex operator+(ex const & lh, ex const & rh);
ex operator-(ex const & lh, ex const & rh);
ex operator*(ex const & lh, ex const & rh);
ex operator/(ex const & lh, ex const & rh);
ex operator%(ex const & lh, ex const & rh); // non-commutative multiplication

/*

// binary arithmetic operators ex with numeric
ex operator+(ex const & lh, numeric const & rh);
ex operator-(ex const & lh, numeric const & rh);
ex operator*(ex const & lh, numeric const & rh);
ex operator/(ex const & lh, numeric const & rh);
ex operator%(ex const & lh, numeric const & rh); // non-commutative multiplication

// binary arithmetic operators numeric with ex
ex operator+(numeric const & lh, ex const & rh);
ex operator-(numeric const & lh, ex const & rh);
ex operator*(numeric const & lh, ex const & rh);
ex operator/(numeric const & lh, ex const & rh);
ex operator%(numeric const & lh, ex const & rh); // non-commutative multiplication

*/

// binary arithmetic operators numeric with numeric
numeric operator+(numeric const & lh, numeric const & rh);
numeric operator-(numeric const & lh, numeric const & rh);
numeric operator*(numeric const & lh, numeric const & rh);
numeric operator/(numeric const & lh, numeric const & rh);

// binary arithmetic assignment operators with ex
ex const & operator+=(ex & lh, ex const & rh);
ex const & operator-=(ex & lh, ex const & rh);
ex const & operator*=(ex & lh, ex const & rh);
ex const & operator/=(ex & lh, ex const & rh);
ex const & operator%=(ex & lh, ex const & rh); // non-commutative multiplication

/*
  
// binary arithmetic assignment operators with numeric
ex const & operator+=(ex & lh, numeric const & rh);
ex const & operator-=(ex & lh, numeric const & rh);
ex const & operator*=(ex & lh, numeric const & rh);
ex const & operator/=(ex & lh, numeric const & rh);
ex const & operator%=(ex & lh, numeric const & rh); // non-commutative multiplication

*/

// binary arithmetic assignment operators with numeric
numeric const & operator+=(numeric & lh, numeric const & rh);
numeric const & operator-=(numeric & lh, numeric const & rh);
numeric const & operator*=(numeric & lh, numeric const & rh);
numeric const & operator/=(numeric & lh, numeric const & rh);

// unary operators
ex operator+(ex const & lh);
ex operator-(ex const & lh);

numeric operator+(numeric const & lh);
numeric operator-(numeric const & lh);

// binary relational operators ex with ex
relational operator==(ex const & lh, ex const & rh);
relational operator!=(ex const & lh, ex const & rh);
relational operator<(ex const & lh, ex const & rh);
relational operator<=(ex const & lh, ex const & rh);
relational operator>(ex const & lh, ex const & rh);
relational operator>=(ex const & lh, ex const & rh);

/*

// binary relational operators ex with numeric
relational operator==(ex const & lh, numeric const & rh);
relational operator!=(ex const & lh, numeric const & rh);
relational operator<(ex const & lh, numeric const & rh);
relational operator<=(ex const & lh, numeric const & rh);
relational operator>(ex const & lh, numeric const & rh);
relational operator>=(ex const & lh, numeric const & rh);

// binary relational operators numeric with ex
relational operator==(numeric const & lh, ex const & rh);
relational operator!=(numeric const & lh, ex const & rh);
relational operator<(numeric const & lh, ex const & rh);
relational operator<=(numeric const & lh, ex const & rh);
relational operator>(numeric const & lh, ex const & rh);
relational operator>=(numeric const & lh, ex const & rh);

*/

// input/output stream operators
ostream & operator<<(ostream & os, ex const & e);
istream & operator>>(istream & is, ex & e);

#endif // ndef __GINAC_OPERATORS_H__