/** @file coloridx.h
 *
 *  Interface to GiNaC's color indices. */

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

#ifndef __GINAC_COLORIDX_H__
#define __GINAC_COLORIDX_H__

#include <string>
#include <vector>
#include "idx.h"
#include "ex.h"

#ifndef NO_NAMESPACE_GINAC
namespace GiNaC {
#endif // ndef NO_NAMESPACE_GINAC

class coloridx : public idx
{
    GINAC_DECLARE_REGISTERED_CLASS(coloridx, idx)

    friend class color;

// member functions

    // default constructor, destructor, copy constructor assignment operator and helpers
public:
    coloridx();
    ~coloridx();
    coloridx (const coloridx & other);
    const coloridx & operator=(const coloridx & other);
protected:
    void copy(const coloridx & other);
    void destroy(bool call_parent);

    // other constructors
public:
    explicit coloridx(bool cov);
    explicit coloridx(const std::string & n, bool cov=false);
    explicit coloridx(const char * n, bool cov=false);
    explicit coloridx(unsigned v, bool cov=false); 

    // functions overriding virtual functions from bases classes
public:
    basic * duplicate() const;
    void printraw(std::ostream & os) const;
    void printtree(std::ostream & os, unsigned indent) const;
    void print(std::ostream & os, unsigned upper_precedence=0) const;
    bool info(unsigned inf) const;

    // new virtual functions which can be overridden by derived classes
    // none

    // non-virtual functions in this class
    // none

    // member variables
    // none
};

// global constants

extern const coloridx some_coloridx;
extern const type_info & typeid_coloridx;

// utility functions
inline const coloridx &ex_to_coloridx(const ex &e)
{
	return static_cast<const coloridx &>(*e.bp);
}

#ifndef NO_NAMESPACE_GINAC
} // namespace GiNaC
#endif // ndef NO_NAMESPACE_GINAC

#endif // ndef __GINAC_COLORIDX_H__
