/** @file indexed.cpp
 *
 *  Implementation of GiNaC's index carrying objects. */

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

#include <string>

#include "indexed.h"
#include "ex.h"
#include "idx.h"
#include "debugmsg.h"

#ifndef NO_NAMESPACE_GINAC
namespace GiNaC {
#endif // ndef NO_NAMESPACE_GINAC

GINAC_IMPLEMENT_REGISTERED_CLASS(indexed, exprseq)

//////////
// default constructor, destructor, copy constructor assignment operator and helpers
//////////

// public

indexed::indexed()
{
	debugmsg("indexed default constructor",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
}

indexed::~indexed()
{
	debugmsg("indexed destructor",LOGLEVEL_DESTRUCT);
	destroy(false);
}

indexed::indexed(const indexed & other)
{
	debugmsg("indexed copy constructor",LOGLEVEL_CONSTRUCT);
	copy (other);
}

const indexed & indexed::operator=(const indexed & other)
{
	debugmsg("indexed operator=",LOGLEVEL_ASSIGNMENT);
	if (this != &other) {
		destroy(true);
		copy(other);
	}
	return *this;
}

// protected

void indexed::copy(const indexed & other)
{
	inherited::copy(other);
}

void indexed::destroy(bool call_parent)
{
	if (call_parent) {
		inherited::destroy(call_parent);
	}
}

//////////
// other constructors
//////////

// public

/** Construct indexed object with one index. The index must be of class idx
 *  or a subclass.
 *
 *  @param i1 The index
 *  @return newly constructed indexed object */
indexed::indexed(const ex & i1) : inherited(i1)
{
	debugmsg("indexed constructor from ex",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
	GINAC_ASSERT(all_of_type_idx());
}

/** Construct indexed object with two indices. The indices must be of class
 *  idx or a subclass.
 *
 *  @param i1 First index
 *  @param i2 Second index
 *  @return newly constructed indexed object */
indexed::indexed(const ex & i1, const ex & i2) : inherited(i1,i2)
{
	debugmsg("indexed constructor from ex,ex",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
	GINAC_ASSERT(all_of_type_idx());
}

/** Construct indexed object with three indices. The indices must be of class
 *  idx or a subclass.
 *
 *  @param i1 First index
 *  @param i2 Second index
 *  @param i3 Third index
 *  @return newly constructed indexed object */
indexed::indexed(const ex & i1, const ex & i2, const ex & i3)
  : inherited(i1,i2,i3)
{
	debugmsg("indexed constructor from ex,ex,ex",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
	GINAC_ASSERT(all_of_type_idx());
}

/** Construct indexed object with four indices. The indices must be of class
 *  idx or a subclass.
 *
 *  @param i1 First index
 *  @param i2 Second index
 *  @param i3 Third index
 *  @param i4 Fourth index
 *  @return newly constructed indexed object */
indexed::indexed(const ex & i1, const ex & i2, const ex & i3, const ex & i4)
  : inherited(i1,i2,i3,i4)
{
	debugmsg("indexed constructor from ex,ex,ex,ex",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
	GINAC_ASSERT(all_of_type_idx());
}

/** Construct indexed object with a specified vector of indices. The indices
 *  must be of class idx or a subclass.
 *
 *  @param iv Vector of indices
 *  @return newly constructed indexed object */
indexed::indexed(const exvector & iv) : inherited(iv)
{
	debugmsg("indexed constructor from exvector",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
	GINAC_ASSERT(all_of_type_idx());
}

indexed::indexed(exvector * ivp) : inherited(ivp)
{
	debugmsg("indexed constructor from exvector *",LOGLEVEL_CONSTRUCT);
	tinfo_key=TINFO_indexed;
	GINAC_ASSERT(all_of_type_idx());
}

//////////
// archiving
//////////

/** Construct object from archive_node. */
indexed::indexed(const archive_node &n, const lst &sym_lst) : inherited(n, sym_lst)
{
	debugmsg("indexed constructor from archive_node", LOGLEVEL_CONSTRUCT);
	tinfo_key = TINFO_indexed;
}

/** Unarchive the object. */
ex indexed::unarchive(const archive_node &n, const lst &sym_lst)
{
	return (new indexed(n, sym_lst))->setflag(status_flags::dynallocated);
}

/** Archive the object. */
void indexed::archive(archive_node &n) const
{
	inherited::archive(n);
}

//////////
// functions overriding virtual functions from bases classes
//////////

// public

basic * indexed::duplicate() const
{
	debugmsg("indexed duplicate",LOGLEVEL_DUPLICATE);
	return new indexed(*this);
}

void indexed::printraw(std::ostream & os) const
{
	debugmsg("indexed printraw",LOGLEVEL_PRINT);
	os << "indexed(indices=";
	printrawindices(os);
	os << ",hash=" << hashvalue << ",flags=" << flags << ")";
}

void indexed::printtree(std::ostream & os, unsigned indent) const
{
	debugmsg("indexed printtree",LOGLEVEL_PRINT);
	os << std::string(indent,' ') << "indexed: " << seq.size() << " indices";
	os << ",hash=" << hashvalue << ",flags=" << flags << std::endl;
	printtreeindices(os,indent);
}

void indexed::print(std::ostream & os, unsigned upper_precedence) const
{
	debugmsg("indexed print",LOGLEVEL_PRINT);
	os << "UNNAMEDINDEX";
	printindices(os);
}

void indexed::printcsrc(std::ostream & os, unsigned type,
                        unsigned upper_precedence) const
{
	debugmsg("indexed print csrc",LOGLEVEL_PRINT);
	print(os,upper_precedence);
}

bool indexed::info(unsigned inf) const
{
	if (inf==info_flags::indexed) return true;
	if (inf==info_flags::has_indices) return seq.size()!=0;
	return inherited::info(inf);
}

// protected

/** Implementation of ex::diff() for an indexed object. It always returns 0.
 *  @see ex::diff */
ex indexed::derivative(const symbol & s) const
{
	return _ex0();
}

int indexed::compare_same_type(const basic & other) const
{
	GINAC_ASSERT(is_of_type(other,indexed));
	return inherited::compare_same_type(other);
}

bool indexed::is_equal_same_type(const basic & other) const
{
	GINAC_ASSERT(is_of_type(other,indexed));
	return inherited::is_equal_same_type(other);
}

unsigned indexed::return_type(void) const
{
	return return_types::noncommutative;
}
   
unsigned indexed::return_type_tinfo(void) const
{
	return tinfo_key;
}

ex indexed::thisexprseq(const exvector & v) const
{
	return indexed(v);
}

ex indexed::thisexprseq(exvector * vp) const
{
	return indexed(vp);
}

//////////
// virtual functions which can be overridden by derived classes
//////////

// none

//////////
// non-virtual functions in this class
//////////

// protected

void indexed::printrawindices(std::ostream & os) const
{
	if (seq.size()!=0) {
		for (exvector::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
			(*cit).printraw(os);
			os << ",";
		}
	}
}

void indexed::printtreeindices(std::ostream & os, unsigned indent) const
{
	if (seq.size()!=0) {
		for (exvector::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
			os << std::string(indent+delta_indent,' ');
			(*cit).printraw(os);
			os << std::endl;
		}
	}
}

void indexed::printindices(std::ostream & os) const
{
	if (seq.size()!=0) {
		if (seq.size()>1) {
			os << "{";
		}
		exvector::const_iterator last=seq.end()-1;
		exvector::const_iterator cit=seq.begin();
		for (; cit!=last; ++cit) {
			(*cit).print(os);
			os << ",";
		}
		(*cit).print(os);
		if (seq.size()>1) {
			os << "}";
		}
	}
}

/** Check whether all indices are of class idx or a subclass. This function
 *  is used internally to make sure that all constructed indexed objects
 *  really carry indices and not some other classes. */
bool indexed::all_of_type_idx(void) const
{
	for (exvector::const_iterator cit=seq.begin(); cit!=seq.end(); ++cit) {
		if (!is_ex_of_type(*cit,idx)) return false;
	}
	return true;
}

#ifndef NO_NAMESPACE_GINAC
} // namespace GiNaC
#endif // ndef NO_NAMESPACE_GINAC
