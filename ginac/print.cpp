/** @file print.cpp
 *
 *  The methods .print() are responsible for the nice default-output of
 *  objects.  All related helper-functions go in here as well.
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

#include <iostream>

#include "basic.h"
#include "ex.h"
#include "add.h"
#include "constant.h"
#include "expairseq.h"
#include "fail.h"
#include "indexed.h"
#include "inifcns.h"
#include "matrix.h"
#include "mul.h"
#include "ncmul.h"
#include "numeric.h"
#include "power.h"
#include "relational.h"
#include "series.h"
#include "symbol.h"

void ex::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("ex print",LOGLEVEL_PRINT);
    ASSERT(bp!=0);
    bp->print(os,upper_precedence);
}

void ex::dbgprint(void) const
{
    debugmsg("ex dbgprint",LOGLEVEL_PRINT);
    ASSERT(bp!=0);
    bp->dbgprint();
}

void basic::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("basic print",LOGLEVEL_PRINT);
    os << "[basic object]";
}

void basic::dbgprint(void) const
{
    print(cerr);
    cerr << endl;
}

void symbol::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("symbol print",LOGLEVEL_PRINT);
    os << name;
}

void constant::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("constant print",LOGLEVEL_PRINT);
    os << name;
}

void power::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("power print",LOGLEVEL_PRINT);
#if 1
    if (precedence<=upper_precedence) os << "(";
    basis.print(os,precedence);
    os << "^";
    exponent.print(os,precedence);
    if (precedence<=upper_precedence) os << ")";
#else
    os << "pow(" << basis << "," << exponent << ")";
#endif
}

void fail::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("fail print",LOGLEVEL_PRINT);
    os << "FAIL";
}

void expairseq::printpair(ostream & os, expair const & p, unsigned upper_precedence) const
{
    os << "[[";
    p.rest.bp->print(os,precedence);
    os << ",";
    p.coeff.bp->print(os,precedence);
    os << "]]";
}

/*
void expairseq::printseq(ostream & os, char delim, unsigned this_precedence,
                         unsigned upper_precedence) const
{
    if (this_precedence<=upper_precedence) os << "(";
    epvector::const_iterator itt,it,it_last,it_start;
    it_last=seq.end();
    --it_last;
    it_start=seq.begin();

    switch (delim) {
    case '+':
        for (it=seq.begin(); it!=it_last; ++it) {
            itt = it +1;
            expair const & k = *itt;
            printpair(os,*it, this_precedence);
            if (((is_ex_of_type(k.rest, numeric)) && (k.coeff*k.rest > 0) ) || ((!is_ex_of_type(k.rest, numeric)) && (k.coeff >0))){
                os << "+";
            }
        }
        printpair(os,*it,this_precedence);
        break;
        
    case '*':
        for (it = it_last ; it!=it_start; --it) {
            if ((*it).rest.is_equal(exMINUSONE()) &&
                (*it).coeff.is_equal(exONE())) {
                os << "-";
            } else {
                printpair(os, *it,this_precedence);
                os << delim;
            }
        }
        printpair(os,*it,this_precedence);
        break;
    default:
        clog << "Nobody expects the Spanish Inquisition: "
             << "deliminator unknown!" << endl;
    }
    if (this_precedence<=upper_precedence) os << ")";
}
*/

void expairseq::printseq(ostream & os, char delim, unsigned this_precedence,
                         unsigned upper_precedence) const
{
    if (this_precedence<=upper_precedence) os << "(";
    epvector::const_iterator it,it_last;
    it_last=seq.end();
    --it_last;
    for (it=seq.begin(); it!=it_last; ++it) {
        printpair(os,*it,this_precedence);
        os << delim;
    }
    printpair(os,*it,this_precedence);
    if (!overall_coeff.is_equal(default_overall_coeff())) {
        os << delim << overall_coeff;
    }
    if (this_precedence<=upper_precedence) os << ")";
}
    
void expairseq::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("expairseq print",LOGLEVEL_PRINT);
    os << "[[";
    printseq(os,',',precedence,upper_precedence);
    os << "]]";
}

void add::printpair(ostream & os, expair const & p, unsigned upper_precedence) const
{
    os << "(";
    if (p.coeff == -1) {
        os << "-";
    } else {
        if (p.coeff != 1) {
            os << p.coeff;
            os << "*";
        }
    }
    os << p.rest;
    os << ")";
}
    
void add::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("add print",LOGLEVEL_PRINT);
    if (precedence<=upper_precedence) os << "(";
    bool first=true;
    for (epvector::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
        if (!first) {
            if (cit->coeff > 0) os << '+';
        } else {
            first=false;
        }
        if (cit->coeff == -1) {
            os << "-";
        } else {
            if (cit->coeff != 1) {
                (cit->coeff).print(os,precedence);
                os << "*";
            }
        }
        os << cit->rest;
    }
    if (!overall_coeff.is_zero()) {
        if (overall_coeff > 0) os << '+';
        os << overall_coeff;
    }
    if (precedence<=upper_precedence) os << ")";
}

void mul::printpair(ostream & os, expair const & p, unsigned upper_precedence) const
{
    os << "(";
    if (p.coeff.compare(exONE())==0) {
        p.rest.print(os,upper_precedence);
    } else {
        // outer parens around ex needed for broken gcc-2.95 parser:
        (ex(power(p.rest,p.coeff))).print(os,upper_precedence);
    }
    os << ")";
}
    
void mul::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("mul print",LOGLEVEL_PRINT);
    if (precedence<=upper_precedence) os << "(";
    bool first=true;
    if (!overall_coeff.is_equal(exONE())) {
        overall_coeff.print(os,precedence);
        first=false;
    }
    for (epvector::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
        if (!first) {
            os << '*';
        } else {
            first=false;
        }
        recombine_pair_to_ex(*cit).print(os,precedence);
    }
    if (precedence<=upper_precedence) os << ")";
}

void ncmul::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("ncmul print",LOGLEVEL_PRINT);
    printseq(os,'(','%',')',precedence,upper_precedence);
}

/*void function::print(ostream & os, unsigned upper_precedence) const
 *{
 *    debugmsg("function print",LOGLEVEL_PRINT);
 *    os << name;
 *    printseq(os,'(',',',')',exprseq::precedence,function::precedence);
 *}*/

void series::print(ostream &os, unsigned upper_precedence) const
{
	debugmsg("symbol print", LOGLEVEL_PRINT);
	convert_to_poly().print(os, upper_precedence);
}

void relational::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("relational print",LOGLEVEL_PRINT);
    if (precedence<=upper_precedence) os << "(";
    lh.print(os,precedence);
    switch (o) {
    case equal:
        os << "==";
        break;
    case not_equal:
        os << "!=";
        break;
    case less:
        os << "<";
        break;
    case less_or_equal:
        os << "<=";
        break;
    case greater:
        os << ">";
        break;
    case greater_or_equal:
        os << ">=";
        break;
    default:
        os << "(INVALID RELATIONAL OPERATOR)";
    }
    rh.print(os,precedence);
    if (precedence<=upper_precedence) os << ")";
}

void matrix::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("matrix print",LOGLEVEL_PRINT);
    os << "[[ ";
    for (int r=0; r<row-1; ++r) {
        os << "[[";
        for (int c=0; c<col-1; ++c) {
            os << m[r*col+c] << ",";
        }
        os << m[col*(r+1)-1] << "]], ";
    }
    os << "[[";
    for (int c=0; c<col-1; ++c) {
        os << m[(row-1)*col+c] << ",";
    }
    os << m[row*col-1] << "]] ]]";
}