#!/usr/bin/perl -w

if ($#ARGV!=0) {
    die 'usage: container.pl type (type=lst or exprseq)';
}

if ($ARGV[0] eq 'lst') {
    $type='lst';
} elsif ($ARGV[0] eq 'exprseq') {
    $type='exprseq';
} else {
    die 'only lst and exprseq supported';
}

#$type='lst';
#$type='exprseq';

if ($type eq 'exprseq') {

    # settings for exprseq
    $CONTAINER="exprseq";
    $STLHEADER="vector";
    $reserve=1;
    $prepend=0;
    $let_op=0;
    $open_bracket='(';
    $close_bracket=')';
    
} elsif ($type eq 'lst') {
 
    # settings for lst
    $CONTAINER="lst";
    $STLHEADER="list";
    $reserve=0;
    $prepend=1;
    $let_op=1;
    $open_bracket='[';
    $close_bracket=']';

} else {
    die "invalid type $type";
}

$CONTAINER_UC=uc(${CONTAINER});
$STLT="ex".$STLHEADER;

if ($reserve) {
    $RESERVE_IMPLEMENTATION="#define RESERVE(s,size) (s).reserve(size)";
} else {
    $RESERVE_IMPLEMENTATION="#define RESERVE(s,size) // no reserve needed for ${STLHEADER}";
}

if ($prepend) {
    $PREPEND_INTERFACE=<<END_OF_PREPEND_INTERFACE;
    virtual ${CONTAINER} & prepend(ex const & b);
END_OF_PREPEND_INTERFACE

    $PREPEND_IMPLEMENTATION=<<END_OF_PREPEND_IMPLEMENTATION;
${CONTAINER} & ${CONTAINER}::prepend(ex const & b)
{
    ensure_if_modifiable();
    seq.push_front(b);
    return *this;
}
END_OF_PREPEND_IMPLEMENTATION
} else {
    $PREPEND_INTERFACE="    // no prepend possible for ${CONTAINER}";
    $PREPEND_IMPLEMENTATION="";
}

if ($let_op) {
    $LET_OP_IMPLEMENTATION=<<END_OF_LET_OP_IMPLEMENTATION
ex & ${CONTAINER}::let_op(int const i)
{
    ASSERT(i>=0);
    ASSERT(i<nops());

    ${STLT}::iterator it=seq.begin();
    for (int j=0; j<i; j++) {
        ++it;
    }
    return *it;
}
END_OF_LET_OP_IMPLEMENTATION
} else {
    $LET_OP_IMPLEMENTATION="// ${CONTAINER}::let_op() will be implemented by user elsewhere";
}

$interface=<<END_OF_INTERFACE;
/** \@file ${CONTAINER}.h
 *
 *  Definition of GiNaC's ${CONTAINER}. 
 *  This file was generated automatically by container.pl.
 *  Please do not modify it directly, edit the perl script instead!
 *  container.pl options: \$CONTAINER=${CONTAINER}
 *                        \$STLHEADER=${STLHEADER}
 *                        \$reserve=${reserve}
 *                        \$prepend=${prepend}
 *                        \$let_op=${let_op}
 *                        \$open_bracket=${open_bracket}
 *                        \$close_bracket=${close_bracket}
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

#ifndef __GINAC_${CONTAINER_UC}_H__
#define __GINAC_${CONTAINER_UC}_H__

#include <${STLHEADER}>
#include <ginac/basic.h>

typedef ${STLHEADER}<ex> ${STLT};

class ${CONTAINER} : public basic
{

public:
    ${CONTAINER}();
    ~${CONTAINER}();
    ${CONTAINER}(${CONTAINER} const & other);
    ${CONTAINER} const & operator=(${CONTAINER} const & other);
protected:
    void copy(${CONTAINER} const & other);
    void destroy(bool call_parent);

public:
    ${CONTAINER}(${STLT} const & s, bool discardable=0);
    ${CONTAINER}(${STLT} * vp); // vp will be deleted
    explicit ${CONTAINER}(ex const & e1);
    explicit ${CONTAINER}(ex const & e1, ex const & e2);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4, ex const & e5);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4, ex const & e5, ex const & e6);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4, ex const & e5, ex const & e6,
             ex const & e7);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4, ex const & e5, ex const & e6,
             ex const & e7, ex const & e8);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4, ex const & e5, ex const & e6,
             ex const & e7, ex const & e8, ex const & e9);
    explicit ${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
             ex const & e4, ex const & e5, ex const & e6,
             ex const & e7, ex const & e8, ex const & e9,
             ex const &e10);

public:
    basic * duplicate() const;
    void printraw(ostream & os) const;
    void print(ostream & os, unsigned upper_precedence=0) const;
    void printtree(ostream & os, unsigned indent) const;
    bool info(unsigned inf) const;
    int nops() const;
    ex & let_op(int const i);
    ex expand(unsigned options=0) const;
    bool has(ex const & other) const;
    ex eval(int level=0) const;
    ex evalf(int level=0) const;
    ex normal(lst &sym_lst, lst &repl_lst, int level=0) const;
    ex diff(symbol const & s) const;
    ex subs(lst const & ls, lst const & lr) const;
protected:
    int compare_same_type(basic const & other) const;
    bool is_equal_same_type(basic const & other) const;
    unsigned return_type(void) const;

    // new virtual functions which can be overridden by derived classes
public:
    virtual ${CONTAINER} & append(ex const & b);
${PREPEND_INTERFACE}
protected:
    virtual void printseq(ostream & os, char openbracket, char delim,
                          char closebracket, unsigned this_precedence,
                          unsigned upper_precedence=0) const;
    virtual ex this${CONTAINER}(${STLT} const & v) const;
    virtual ex this${CONTAINER}(${STLT} * vp) const;

protected:
    bool is_canonical() const;
    ${STLT} evalchildren(int level) const;
    ${STLT} evalfchildren(int level) const;
    ${STLT} normalchildren(int level) const;
    ${STLT} diffchildren(symbol const & s) const;
    ${STLT} * subschildren(lst const & ls, lst const & lr) const;

protected:
    ${STLT} seq;
    static unsigned precedence;
};

// global constants

extern const ${CONTAINER} some_${CONTAINER};
extern type_info const & typeid_${CONTAINER};

// macros

#define ex_to_${CONTAINER}(X) (static_cast<${CONTAINER} const &>(*(X).bp))

#endif // ndef __GINAC_${CONTAINER_UC}_H__

END_OF_INTERFACE

$implementation=<<END_OF_IMPLEMENTATION;
/** \@file ${CONTAINER}.cpp
 *
 *  Implementation of GiNaC's ${CONTAINER}. 
 *  This file was generated automatically by container.pl.
 *  Please do not modify it directly, edit the perl script instead!
 *  container.pl options: \$CONTAINER=${CONTAINER}
 *                        \$STLHEADER=${STLHEADER}
 *                        \$reserve=${reserve}
 *                        \$prepend=${prepend}
 *                        \$let_op=${let_op}
 *                        \$open_bracket=${open_bracket}
 *                        \$close_bracket=${close_bracket}
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
#include <stdexcept>

#include "${CONTAINER}.h"
#include "ex.h"

${RESERVE_IMPLEMENTATION}

//////////
// default constructor, destructor, copy constructor assignment operator and helpers
//////////

// public

${CONTAINER}::${CONTAINER}() : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} default constructor",LOGLEVEL_CONSTRUCT);
}

${CONTAINER}::~${CONTAINER}()
{
    debugmsg("${CONTAINER} destructor",LOGLEVEL_DESTRUCT);
    destroy(0);
}

${CONTAINER}::${CONTAINER}(${CONTAINER} const & other)
{
    debugmsg("${CONTAINER} copy constructor",LOGLEVEL_CONSTRUCT);
    copy(other);
}

${CONTAINER} const & ${CONTAINER}::operator=(${CONTAINER} const & other)
{
    debugmsg("${CONTAINER} operator=",LOGLEVEL_ASSIGNMENT);
    if (this != &other) {
        destroy(1);
        copy(other);
    }
    return *this;
}

// protected

void ${CONTAINER}::copy(${CONTAINER} const & other)
{
    basic::copy(other);
    seq=other.seq;
}

void ${CONTAINER}::destroy(bool call_parent)
{
    seq.clear();
    if (call_parent) basic::destroy(call_parent);
}

//////////
// other constructors
//////////

// public

${CONTAINER}::${CONTAINER}(${STLT} const & s, bool discardable) :  basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from ${STLT}",
             LOGLEVEL_CONSTRUCT);
    if (discardable) {
        seq.swap(const_cast<${STLT} &>(s));
    } else {
        seq=s;
    }
}

${CONTAINER}::${CONTAINER}(${STLT} * vp) : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from ${STLT} *",LOGLEVEL_CONSTRUCT);
    ASSERT(vp!=0);
    seq.swap(*vp);
    delete vp;
}

${CONTAINER}::${CONTAINER}(ex const & e1) :  basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 1 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,1);
    seq.push_back(e1);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2) : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 2 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,2);
    seq.push_back(e1);
    seq.push_back(e2);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3)
    : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 3 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,3);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4) : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 4 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,4);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4, ex const & e5) : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 5 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,5);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
    seq.push_back(e5);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4, ex const & e5, ex const & e6)
    : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 6 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,6);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
    seq.push_back(e5);
    seq.push_back(e6);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4, ex const & e5, ex const & e6,
                     ex const & e7) : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 7 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,7);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
    seq.push_back(e5);
    seq.push_back(e6);
    seq.push_back(e7);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4, ex const & e5, ex const & e6,
                     ex const & e7, ex const & e8) : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 8 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,8);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
    seq.push_back(e5);
    seq.push_back(e6);
    seq.push_back(e7);
    seq.push_back(e8);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4, ex const & e5, ex const & e6,
                     ex const & e7, ex const & e8, ex const & e9)
    : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 9 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,9);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
    seq.push_back(e5);
    seq.push_back(e6);
    seq.push_back(e7);
    seq.push_back(e8);
    seq.push_back(e9);
}

${CONTAINER}::${CONTAINER}(ex const & e1, ex const & e2, ex const & e3,
                     ex const & e4, ex const & e5, ex const & e6,
                     ex const & e7, ex const & e8, ex const & e9,
                     ex const &e10)
    : basic(TINFO_${CONTAINER})
{
    debugmsg("${CONTAINER} constructor from 10 ex",
             LOGLEVEL_CONSTRUCT);
    RESERVE(seq,10);
    seq.push_back(e1);
    seq.push_back(e2);
    seq.push_back(e3);
    seq.push_back(e4);
    seq.push_back(e5);
    seq.push_back(e6);
    seq.push_back(e7);
    seq.push_back(e8);
    seq.push_back(e9);
    seq.push_back(e10);
}

//////////
// functions overriding virtual functions from bases classes
//////////

// public

basic * ${CONTAINER}::duplicate() const
{
    debugmsg("${CONTAINER} duplicate",LOGLEVEL_DUPLICATE);
    return new ${CONTAINER}(*this);
}

void ${CONTAINER}::printraw(ostream & os) const
{
    debugmsg("${CONTAINER} printraw",LOGLEVEL_PRINT);

    os << "${CONTAINER}(";
    for (${STLT}::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
        (*cit).bp->printraw(os);
        os << ",";
    }
    os << ")";
}

void ${CONTAINER}::print(ostream & os, unsigned upper_precedence) const
{
    debugmsg("${CONTAINER} print",LOGLEVEL_PRINT);
    // always print brackets around seq, ignore upper_precedence
    printseq(os,'${open_bracket}',',','${close_bracket}',precedence,precedence+1);
}

void ${CONTAINER}::printtree(ostream & os, unsigned indent) const
{
    debugmsg("${CONTAINER} printtree",LOGLEVEL_PRINT);

    os << string(indent,' ') << "type=" << typeid(*this).name()
       << ", hash=" << hashvalue << " (0x" << hex << hashvalue << dec << ")"
       << ", flags=" << flags
       << ", nops=" << nops() << endl;
    for (${STLT}::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
        (*cit).printtree(os,indent+delta_indent);
    }
    os << string(indent+delta_indent,' ') << "=====" << endl;
}

// ${CONTAINER}::info() will be implemented by user elsewhere";

int ${CONTAINER}::nops() const
{
    return seq.size();
}

${LET_OP_IMPLEMENTATION}

ex ${CONTAINER}::expand(unsigned options) const
{
    ${STLT} s;
    RESERVE(s,seq.size());
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        s.push_back((*it).expand(options));
    }

    return this${CONTAINER}(s);
}

// a ${CONTAINER} 'has' an expression if it is this expression itself or a child 'has' it

bool ${CONTAINER}::has(ex const & other) const
{
    ASSERT(other.bp!=0);
    if (is_equal(*other.bp)) return true;
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        if ((*it).has(other)) return true;
    }
    return false;
}

ex ${CONTAINER}::eval(int level) const
{
    if (level==1) {
        return this->hold();
    }
    return this${CONTAINER}(evalchildren(level));
}

ex ${CONTAINER}::evalf(int level) const
{
    return this${CONTAINER}(evalfchildren(level));
}

/** Implementation of ex::normal() for ${CONTAINER}s. It normalizes the arguments
 *  and replaces the ${CONTAINER} by a temporary symbol.
 *  \@see ex::normal */
ex ${CONTAINER}::normal(lst &sym_lst, lst &repl_lst, int level) const
{
    ex n=this${CONTAINER}(normalchildren(level));
    return n.bp->basic::normal(sym_lst,repl_lst,level);
}

ex ${CONTAINER}::diff(symbol const & s) const
{
    return this${CONTAINER}(diffchildren(s));
}

ex ${CONTAINER}::subs(lst const & ls, lst const & lr) const
{
    ${STLT} * vp=subschildren(ls,lr);
    if (vp==0) {
        return *this;
    }
    return this${CONTAINER}(vp);
}

// protected

int ${CONTAINER}::compare_same_type(basic const & other) const
{
    ASSERT(is_of_type(other,${CONTAINER}));
    ${CONTAINER} const & o=static_cast<${CONTAINER} const &>
                                    (const_cast<basic &>(other));
    int cmpval;
    ${STLT}::const_iterator it1=seq.begin();
    ${STLT}::const_iterator it2=o.seq.begin();

    for (; (it1!=seq.end())&&(it2!=o.seq.end()); ++it1, ++it2) {
        cmpval=(*it1).compare(*it2);
        if (cmpval!=0) return cmpval;
    }

    if (it1==seq.end()) {
        return (it2==o.seq.end() ? 0 : -1);
    }

    return 1;
}

bool ${CONTAINER}::is_equal_same_type(basic const & other) const
{
    ASSERT(is_of_type(other,${CONTAINER}));
    ${CONTAINER} const & o=static_cast<${CONTAINER} const &>
                                    (const_cast<basic &>(other));
    if (seq.size()!=o.seq.size()) return false;

    ${STLT}::const_iterator it1=seq.begin();
    ${STLT}::const_iterator it2=o.seq.begin();

    for (; it1!=seq.end(); ++it1, ++it2) {
	if (!(*it1).is_equal(*it2)) return false;
    }

    return true;
}

unsigned ${CONTAINER}::return_type(void) const
{
    return return_types::noncommutative_composite;
}

//////////
// new virtual functions which can be overridden by derived classes
//////////

// public

${CONTAINER} & ${CONTAINER}::append(ex const & b)
{
    ensure_if_modifiable();
    seq.push_back(b);
    return *this;
}

${PREPEND_IMPLEMENTATION}

// protected

void ${CONTAINER}::printseq(ostream & os, char openbracket, char delim,
                         char closebracket, unsigned this_precedence,
                         unsigned upper_precedence) const
{
    if (this_precedence<=upper_precedence) os << openbracket;
    if (seq.size()!=0) {
        ${STLT}::const_iterator it,it_last;
        it=seq.begin();
        it_last=seq.end();
        --it_last;
        for (; it!=it_last; ++it) {
            (*it).bp->print(os,this_precedence);
            os << delim;
        }
        (*it).bp->print(os,this_precedence);
    }
    if (this_precedence<=upper_precedence) os << closebracket;
}

ex ${CONTAINER}::this${CONTAINER}(${STLT} const & v) const
{
    return ${CONTAINER}(v);
}

ex ${CONTAINER}::this${CONTAINER}(${STLT} * vp) const
{
    return ${CONTAINER}(vp);
}

//////////
// non-virtual functions in this class
//////////

// public

// none

// protected

bool ${CONTAINER}::is_canonical() const
{
    if (seq.size()<=1) { return 1; }

    ${STLT}::const_iterator it=seq.begin();
    ${STLT}::const_iterator it_last=it;
    for (++it; it!=seq.end(); it_last=it, ++it) {
        if ((*it_last).compare(*it)>0) {
            if ((*it_last).compare(*it)>0) {
                cout << *it_last << ">" << *it << "\\n";
                return 0;
	        }
        }
    }
    return 1;
}


${STLT} ${CONTAINER}::evalchildren(int level) const
{
    ${STLT} s;
    RESERVE(s,seq.size());

    if (level==1) {
        return seq;
    }
    if (level == -max_recursion_level) {
        throw(std::runtime_error("max recursion level reached"));
    }
    --level;
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        s.push_back((*it).eval(level));
    }
    return s;
}

${STLT} ${CONTAINER}::evalfchildren(int level) const
{
    ${STLT} s;
    RESERVE(s,seq.size());

    if (level==1) {
        return seq;
    }
    if (level == -max_recursion_level) {
        throw(std::runtime_error("max recursion level reached"));
    }
    --level;
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        s.push_back((*it).evalf(level));
    }
    return s;
}

${STLT} ${CONTAINER}::normalchildren(int level) const
{
    ${STLT} s;
    RESERVE(s,seq.size());

    if (level==1) {
        return seq;
    }
    if (level == -max_recursion_level) {
        throw(std::runtime_error("max recursion level reached"));
    }
    --level;
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        s.push_back((*it).normal(level));
    }
    return s;
}

${STLT} ${CONTAINER}::diffchildren(symbol const & y) const
{
    ${STLT} s;
    RESERVE(s,seq.size());
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        s.push_back((*it).diff(y));
    }
    return s;
}

/* obsolete subschildren
${STLT} ${CONTAINER}::subschildren(lst const & ls, lst const & lr) const
{
    ${STLT} s;
    RESERVE(s,seq.size());
    for (${STLT}::const_iterator it=seq.begin(); it!=seq.end(); ++it) {
        s.push_back((*it).subs(ls,lr));
    }
    return s;
}
*/

${STLT} * ${CONTAINER}::subschildren(lst const & ls, lst const & lr) const
{
    // returns a NULL pointer if nothing had to be substituted
    // returns a pointer to a newly created epvector otherwise
    // (which has to be deleted somewhere else)

    ${STLT}::const_iterator last=seq.end();
    ${STLT}::const_iterator cit=seq.begin();
    while (cit!=last) {
        ex const & subsed_ex=(*cit).subs(ls,lr);
        if (!are_ex_trivially_equal(*cit,subsed_ex)) {

            // something changed, copy seq, subs and return it
            ${STLT} *s=new ${STLT};
            RESERVE(*s,seq.size());

            // copy parts of seq which are known not to have changed
            ${STLT}::const_iterator cit2=seq.begin();
            while (cit2!=cit) {
                s->push_back(*cit2);
                ++cit2;
            }
            // copy first changed element
            s->push_back(subsed_ex);
            ++cit2;
            // copy rest
            while (cit2!=last) {
                s->push_back((*cit2).subs(ls,lr));
                ++cit2;
	    }
            return s;
        }
        ++cit;
    }
    
    return 0; // nothing has changed
}

//////////
// static member variables
//////////

// protected

unsigned ${CONTAINER}::precedence=10;

//////////
// global constants
//////////

const ${CONTAINER} some_${CONTAINER};
type_info const & typeid_${CONTAINER}=typeid(some_${CONTAINER});

END_OF_IMPLEMENTATION

print "Creating interface file ${CONTAINER}.h...";
open OUT,">${CONTAINER}.h" or die "cannot open ${CONTAINER}.h";
print OUT $interface;
close OUT;
print "ok.\n";

print "Creating implementation file ${CONTAINER}.cpp...";
open OUT,">${CONTAINER}.cpp" or die "cannot open ${CONTAINER}.cpp";
print OUT $implementation;
close OUT;
print "ok.\n";

print "done.\n";