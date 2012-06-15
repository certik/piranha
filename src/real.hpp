/***************************************************************************
 *   Copyright (C) 2009-2011 by Francesco Biscani                          *
 *   bluescarni@gmail.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PIRANHA_REAL_HPP
#define PIRANHA_REAL_HPP

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <cctype>
#include <iostream>
#include <memory>
#include <mpfr.h>
#include <sstream>
#include <stdexcept>
#include <string>

#include "config.hpp"
#include "exceptions.hpp"
#include "integer.hpp"
#include "rational.hpp"

namespace piranha
{

/// Arbitrary precision floating-point class.
/**
 * This class represents floating-point ("real") numbers of arbitrary size (i.e., the size is limited only by the available memory).
 * The implementation consists of a C++ wrapper around the \p mpfr_t type from the multiprecision MPFR library. Real numbers
 * are represented in binary format and they consist of an arbitrary-size significand coupled to a fixed-size exponent.
 * 
 * This implementation always uses the \p MPFR_RNDN (round to nearest) rounding mode for all operations.
 * 
 * \section interop Interoperability with fundamental types
 * 
 * Full interoperability with the same fundamental C++ types as piranha::integer is provided.
 * 
 * \section exception_safety Exception safety guarantee
 * 
 * This class provides the strong exception safety guarantee for all operations. In case of memory allocation errors by GMP/MPFR routines,
 * the program will terminate.
 * 
 * \section move_semantics Move semantics
 * 
 * Move construction and move assignment will leave the moved-from object in a state that is destructible and assignable.
 * 
 * @see http://www.mpfr.org
 * 
 * @author Francesco Biscani (bluescarni@gmail.com)
 */
class real
{
		// Default rounding mode.
		static const ::mpfr_rnd_t default_rnd = MPFR_RNDN;
		static void prec_check(const ::mpfr_prec_t &prec)
		{
			if (prec < MPFR_PREC_MIN || prec > MPFR_PREC_MAX) {
				piranha_throw(std::invalid_argument,"invalid significand precision requested");
			}
		}
		void construct_from_string(const char *str, const ::mpfr_prec_t &prec)
		{
			prec_check(prec);
			::mpfr_init2(m_value,prec);
			const int retval = ::mpfr_set_str(m_value,str,10,default_rnd);
			if (retval != 0) {
				::mpfr_clear(m_value);
				piranha_throw(std::invalid_argument,"invalid string input for real");
			}
		}
	public:
		/// Default significand precision.
		/**
		 * The precision is the number of bits used to represent the significand of a floating-point number.
		 * This default value is equivalent to the IEEE 754 quadruple-precision binary floating-point format.
		 */
		static const ::mpfr_prec_t default_prec = 113;
		/// Default constructor.
		/**
		 * Will initialize the number to zero, using \p default_prec as significand precision.
		 */
		real()
		{
			::mpfr_init2(m_value,default_prec);
			::mpfr_set_ui(m_value,0ul,default_rnd);
		}
		/// Copy constructor.
		/**
		 * Will deep-copy \p other.
		 * 
		 * @param[in] other real to be copied.
		 */
		real(const real &other)
		{
			// Init with the same precision as other, and then set.
			::mpfr_init2(m_value,other.get_prec());
			::mpfr_set(m_value,other.m_value,default_rnd);
		}
		/// Move constructor.
		/**
		 * @param[in] other real to be moved.
		 */
		real(real &&other) piranha_noexcept_spec(true)
		{
			m_value->_mpfr_prec = other.m_value->_mpfr_prec;
			m_value->_mpfr_sign = other.m_value->_mpfr_sign;
			m_value->_mpfr_exp = other.m_value->_mpfr_exp;
			m_value->_mpfr_d = other.m_value->_mpfr_d;
			// Erase other.
			other.m_value->_mpfr_prec = 0;
			other.m_value->_mpfr_sign = 0;
			other.m_value->_mpfr_exp = 0;
			other.m_value->_mpfr_d = piranha_nullptr;
		}
		/// Constructor from C string.
		/**
		 * Will use the string \p str and precision \p prec to initialize the number.
		 * The expected string format is described in the MPFR documentation.
		 * 
		 * @param[in] str string representation of the real number.
		 * @param[in] prec desired significand precision.
		 * 
		 * @throws std::invalid_argument if the conversion from string fails or if the significand precision requested
		 * is not within the range allowed by the MPFR library.
		 * 
		 * @see http://www.mpfr.org/mpfr-current/mpfr.html#Assignment-Functions
		 */
		explicit real(const char *str, const ::mpfr_prec_t &prec = default_prec)
		{
			construct_from_string(str,prec);
		}
		/// Constructor from C++ string.
		/**
		 * Equivalent to the constructor from C string.
		 * 
		 * @param[in] str string representation of the real number.
		 * @param[in] prec desired significand precision.
		 * 
		 * @throws std::invalid_argument if the conversion from string fails or if the significand precision requested
		 * is not within the range allowed by the MPFR library.
		 */
		explicit real(const std::string &str, const ::mpfr_prec_t &prec = default_prec)
		{
			construct_from_string(str.c_str(),prec);
		}
		/// Destructor.
		/**
		 * Will clear the internal MPFR variable.
		 */
		~real() piranha_noexcept_spec(true)
		{
			// TODO restore.
			//BOOST_CONCEPT_ASSERT((concept::Coefficient<integer>));
			if (m_value->_mpfr_d) {
				::mpfr_clear(m_value);
			} else {
				piranha_assert(!m_value->_mpfr_prec);
				piranha_assert(!m_value->_mpfr_sign);
				piranha_assert(!m_value->_mpfr_exp);
			}
		}
		/// Sign.
		/**
		 * @return 1 if <tt>this > 0</tt>, 0 if <tt>this == 0</tt> and -1 if <tt>this < 0</tt>.
		 */
		int sign() const
		{
			return mpfr_sgn(m_value);
		}
		/// Get precision.
		/**
		 * @return the number of bits used to represent the significand of \p this.
		 */
		::mpfr_prec_t get_prec() const
		{
			return mpfr_get_prec(m_value);
		}
		/// Overload output stream operator for piranha::real.
		/**
		 * The output format for finite numbers is normalised scientific notation, where the exponent is signalled by the letter 'e'
		 * and suppressed if null.
		 * 
		 * For non-finite numbers, the string representation is the one described in the MPFR documentation.
		 * 
		 * @param[in] os output stream.
		 * @param[in] r piranha::real to be directed to stream.
		 * 
		 * @return reference to \p os.
		 * 
		 * @throws std::invalid_argument if the conversion to string via the MPFR API fails.
		 * @throws std::overflow_error if the exponent is smaller than an implementation-defined minimum.
		 * @throws unspecified any exception thrown by memory allocation errors in standard containers.
		 * 
		 * @see http://www.mpfr.org/mpfr-current/mpfr.html#Conversion-Functions
		 */
		friend std::ostream &operator<<(std::ostream &os, const real &r)
		{
			::mpfr_exp_t exp(0);
			char *str = ::mpfr_get_str(piranha_nullptr,&exp,10,0,r.m_value,default_rnd);
			if (!str) {
				piranha_throw(std::invalid_argument,"unable to convert real to string");
			}
			// Go through unique_ptr as we are not sure the default constructor of string is nothrow.
			std::unique_ptr<std::string> cpp_str;
			try {
				cpp_str.reset(new std::string(str));
			} catch (...) {
				::mpfr_free_str(str);
				throw;
			}
			::mpfr_free_str(str);
			// Insert the radix point.
			auto it = std::find_if(cpp_str->begin(),cpp_str->end(),[](char c) {return std::isdigit(c);});
			if (it != cpp_str->end()) {
				++it;
				cpp_str->insert(it,'.');
				if (exp == boost::integer_traits< ::mpfr_exp_t>::const_min) {
					piranha_throw(std::overflow_error,"overflow in conversion of real to string");
				}
				--exp;
				if (exp != ::mpfr_exp_t(0) && r.sign() != 0) {
					cpp_str->append(std::string("e") + boost::lexical_cast<std::string>(exp));
				}
			}
			os << (*cpp_str);
			return os;
		}
	private:
		::mpfr_t m_value;
};

}

#endif