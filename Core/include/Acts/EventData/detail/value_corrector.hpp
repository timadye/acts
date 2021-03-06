// This file is part of the Acts project.
//
// Copyright (C) 2016-2018 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
// Acts include(s)
#include "Acts/Utilities/Definitions.hpp"
#include "Acts/Utilities/ParameterDefinitions.hpp"

namespace Acts {
/// @cond detail
namespace detail {
/// @brief check and correct parameter values
///
/// @tparam params template parameter pack containing the multiple parameter
///                identifiers
///
/// Values in the given vector are interpreted as values for the given
/// parameters. As those they are checked whether they are inside the allowed
/// range and corrected if necessary.
///
/// Invocation:
///   - value_corrector<params...>::result(parVector) where `parVector`
///     contains `sizeof...(params)` elements
///
/// @post All values in the argument `parVector` are within the valid
///       parameter range.
template <BoundIndices... params>
struct value_corrector;

/// @cond
template <typename R, BoundIndices... params>
struct value_corrector_impl;

template <BoundIndices... params>
struct value_corrector {
  using ParVector_t = ActsVector<BoundScalar, sizeof...(params)>;

  static void result(ParVector_t& values) {
    value_corrector_impl<ParVector_t, params...>::calculate(values, 0);
  }
};

template <typename R, BoundIndices first, BoundIndices... others>
struct value_corrector_impl<R, first, others...> {
  static void calculate(R& values, unsigned int pos) {
    using parameter_type = BoundParameterType<first>;
    if (parameter_type::may_modify_value) {
      values(pos) = parameter_type::getValue(values(pos));
    }
    value_corrector_impl<R, others...>::calculate(values, pos + 1);
  }
};

template <typename R, BoundIndices last>
struct value_corrector_impl<R, last> {
  static void calculate(R& values, unsigned int pos) {
    using parameter_type = BoundParameterType<last>;
    if (parameter_type::may_modify_value) {
      values(pos) = parameter_type::getValue(values(pos));
    }
  }
};
/// @endcond
}  // namespace detail
/// @endcond
}  // namespace Acts
