/** @file constant.h
 *
 *  Interface to GiNaC's constant types and some special constants. */

/*
 *  GiNaC Copyright (C) 1999-2001 Johannes Gutenberg University Mainz, Germany
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

#ifndef __GINAC_CONSTANT_H__
#define __GINAC_CONSTANT_H__

#include <string>
#include "basic.h"

#ifndef NO_NAMESPACE_GINAC
namespace GiNaC {
#endif // ndef NO_NAMESPACE_GINAC

typedef ex (*evalffunctype)(void);
	
/** This class holds constants, symbols with specific numerical value. Each
 *  object of this class must either provide their own function to evaluate it
 *  to class numeric or provide the constant as a numeric (if it's an exact
 *  number). */
class constant : public basic
{
	GINAC_DECLARE_REGISTERED_CLASS(constant, basic)

// member functions

	// other constructors
public:
	constant(const std::string & initname, evalffunctype efun=0);
	constant(const std::string & initname, const numeric & initnumber);

	// functions overriding virtual functions from bases classes
public:
	void print(std::ostream & os, unsigned upper_precedence=0) const;
	void printraw(std::ostream & os) const;
	void printtree(std::ostream & os, unsigned indent) const;
	void printcsrc(std::ostream & os, unsigned type, unsigned upper_precedence=0) const;
	ex evalf(int level=0) const;
protected:
	ex derivative(const symbol & s) const;
	bool is_equal_same_type(const basic & other) const;
	
	// new virtual functions which can be overridden by derived classes
	// none

	// non-virtual functions in this class
	// none

// member variables

private:
	std::string name;
	evalffunctype ef;
	numeric * number;
	unsigned serial;  //!  unique serial number for comparision
	static unsigned next_serial;
};

extern const constant Pi;
extern const constant Catalan;
extern const constant Euler;

#ifndef NO_NAMESPACE_GINAC
} // namespace GiNaC
#endif // ndef NO_NAMESPACE_GINAC

#endif // ndef __GINAC_CONSTANT_H__
