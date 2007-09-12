/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2002, 2003 Ferdinando Ametrano

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/termstructures/localvoltermstructure.hpp>

namespace QuantLib {

    LocalVolTermStructure::LocalVolTermStructure(const DayCounter& dc)
    : TermStructure(dc) {}

    LocalVolTermStructure::LocalVolTermStructure(const Date& referenceDate,
                                                 const Calendar& cal,
                                                 const DayCounter& dc)
    : TermStructure(referenceDate, cal, dc) {}

    LocalVolTermStructure::LocalVolTermStructure(Natural settlementDays,
                                                 const Calendar& cal,
                                                 const DayCounter& dc)
    : TermStructure(settlementDays, cal, dc) {}

    Volatility LocalVolTermStructure::localVol(const Date& d,
                                               Real underlyingLevel,
                                               bool extrapolate) const {
        Time t = timeFromReference(d);
        checkRange(t,underlyingLevel,extrapolate);
        return localVolImpl(t, underlyingLevel);
    }

    Volatility LocalVolTermStructure::localVol(Time t,
                                               Real underlyingLevel,
                                               bool extrapolate) const {
        checkRange(t,underlyingLevel,extrapolate);
        return localVolImpl(t, underlyingLevel);
    }

    void LocalVolTermStructure::accept(AcyclicVisitor& v) {
        Visitor<LocalVolTermStructure>* v1 =
            dynamic_cast<Visitor<LocalVolTermStructure>*>(&v);
        if (v1 != 0)
            v1->visit(*this);
        else
            QL_FAIL("not a local-volatility term structure visitor");
    }

    void LocalVolTermStructure::checkRange(Time t,
                                           Real k,
                                           bool extrapolate) const {
        TermStructure::checkRange(t, extrapolate);
        QL_REQUIRE(extrapolate || allowsExtrapolation() ||
                   (k >= minStrike() && k <= maxStrike()),
                   "strike (" << k << ") is outside the curve domain ["
                   << minStrike() << "," << maxStrike() << "]");
    }

}
