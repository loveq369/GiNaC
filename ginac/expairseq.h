/** @file expairseq.h
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

#ifndef __GINAC_EXPAIRSEQ_H__
#define __GINAC_EXPAIRSEQ_H__

#include <vector>
#include <list>
#include <ginac/expair.h>

//#define EXPAIRSEQ_USE_HASHTAB

typedef vector<expair> epvector;
typedef epvector::iterator epviter;

inline void iter_swap(epvector::iterator it1, epvector::iterator it2)
{
    debugmsg("iter_swap epvector",LOGLEVEL_NONMEMBER_FUNCTION);
    (*it1).rest.swap((*it2).rest);
    (*it1).coeff.swap((*it2).coeff);
}

typedef epvector::iterator epp;
typedef list<epp> epplist;
typedef vector<epplist> epplistvector;

/** A sequence of class expair.
 *  This is used for time-critical classes like sums and products of terms
 *  since handling a list of coeff and rest is much faster than handling a
 *  list of products or powers, respectively. (Incidentally, Maple does it
 *  the same way.) */
class expairseq : public basic
{
// member functions

    // default constructor, destructor, copy constructor assignment operator and helpers
public:
    expairseq() : basic(TINFO_expairseq)
#ifdef EXPAIRSEQ_USE_HASHTAB
        , hashtabsize(0)
#endif // def EXPAIRSEQ_USE_HASHTAB
        {
            debugmsg("expairseq default constructor",LOGLEVEL_CONSTRUCT);
        }
    ~expairseq()
        {
            debugmsg("expairseq destructor",LOGLEVEL_DESTRUCT);
            destroy(0);
        }
    expairseq(expairseq const & other);
    expairseq const & operator=(expairseq const & other);
protected:
    void copy(expairseq const & other);
    void destroy(bool call_parent)
        {
            if (call_parent) basic::destroy(call_parent);
        };

    // other constructors
public:
    expairseq(ex const & lh, ex const & rh);
    expairseq(exvector const & v);
    expairseq(epvector const & v, ex const & oc);
    expairseq(epvector * vp, ex const & oc); // vp will be deleted

    // functions overriding virtual functions from bases classes
public:
    basic * duplicate() const;
    void printraw(ostream & os) const;
    void printtree(ostream & os, unsigned indent) const;
    void print(ostream & os, unsigned upper_precedence=0) const;
    bool info(unsigned inf) const;
    int nops() const;
    ex op(int const i) const;
    ex & let_op(int const i);
    ex eval(int level=0) const;
    ex evalf(int level=0) const;
    ex normal(lst &sym_lst, lst &repl_lst, int level=0) const;
    ex diff(symbol const & s) const;
    ex subs(lst const & ls, lst const & lr) const;
protected:
    int compare_same_type(basic const & other) const;
    bool is_equal_same_type(basic const & other) const;
    unsigned return_type(void) const;
    unsigned calchash(void) const;
    ex expand(unsigned options=0) const;

    // new virtual functions which can be overridden by derived classes
protected:
    virtual ex thisexpairseq(epvector const & v, ex const & oc) const;
    virtual ex thisexpairseq(epvector * vp, ex const & oc) const;
    virtual void printseq(ostream & os, char delim, unsigned this_precedence,
                          unsigned upper_precedence) const;
    virtual void printpair(ostream & os, expair const & p,
                           unsigned upper_precedence) const;
    virtual expair split_ex_to_pair(ex const & e) const;
    virtual expair combine_ex_with_coeff_to_pair(ex const & e,
                                                 ex const & c) const;
    virtual expair combine_pair_with_coeff_to_pair(expair const & p,
                                                   ex const & c) const;
    virtual ex recombine_pair_to_ex(expair const & p) const;
    virtual bool expair_needs_further_processing(epp it);
    virtual ex default_overall_coeff(void) const;
    virtual void combine_overall_coeff(ex const & c);
    virtual void combine_overall_coeff(ex const & c1, ex const & c2);
    virtual bool can_make_flat(expair const & p) const;
    
    // non-virtual functions in this class
protected:
    void construct_from_2_ex_via_exvector(ex const & lh, ex const & rh);
    void construct_from_2_ex(ex const & lh, ex const & rh);
    void construct_from_2_expairseq(expairseq const & s1,
                                    expairseq const & s2);
    void construct_from_expairseq_ex(expairseq const & s,
                                     ex const & e);
    void construct_from_exvector(exvector const & v);
    void construct_from_epvector(epvector const & v);
    void make_flat(exvector const & v);
    void make_flat(epvector const & v);
    epvector * bubblesort(epvector::iterator itbegin, epvector::iterator itend);
    epvector * mergesort(epvector::iterator itbegin, epvector::iterator itend);
    void canonicalize(void);
    void combine_same_terms_sorted_seq(void);
#ifdef EXPAIRSEQ_USE_HASHTAB
    void combine_same_terms(void);
    unsigned calc_hashtabsize(unsigned sz) const;
    unsigned calc_hashindex(ex const & e) const;
    void shrink_hashtab(void);
    void remove_hashtab_entry(epvector::const_iterator element);
    void move_hashtab_entry(epvector::const_iterator oldpos,
                            epvector::iterator newpos);
    void sorted_insert(epplist & eppl, epp elem);
    void build_hashtab_and_combine(epvector::iterator & first_numeric,
                                   epvector::iterator & last_non_zero,
                                   vector<bool> & touched,
                                   unsigned & number_of_zeroes);
    void drop_coeff_0_terms(epvector::iterator & first_numeric,
                            epvector::iterator & last_non_zero,
                            vector<bool> & touched,
                            unsigned & number_of_zeroes);
    bool has_coeff_0(void) const;
    void add_numerics_to_hashtab(epvector::iterator first_numeric,                                              epvector::const_iterator last_non_zero);
#endif // def EXPAIRSEQ_USE_HASHTAB
    bool is_canonical() const;
    epvector * expandchildren(unsigned options) const;
    epvector * evalchildren(int level) const;
    epvector evalfchildren(int level) const;
    epvector normalchildren(int level) const;
    epvector diffchildren(symbol const & s) const;
    epvector * subschildren(lst const & ls, lst const & lr) const;
    
// member variables
    
protected:
    epvector seq;
    ex overall_coeff;
    static unsigned precedence;
#ifdef EXPAIRSEQ_USE_HASHTAB
    epplistvector hashtab;
    unsigned hashtabsize;
    unsigned hashmask;
    static unsigned maxhashtabsize;
    static unsigned minhashtabsize;
    static unsigned hashtabfactor;
#endif // def EXPAIRSEQ_USE_HASHTAB
};

// global constants

extern const expairseq some_expairseq;
extern type_info const & typeid_expairseq;

#define ex_to_expairseq(X) static_cast<expairseq const &>(*(X).bp)

#endif // ndef __GINAC_EXPAIRSEQ_H__