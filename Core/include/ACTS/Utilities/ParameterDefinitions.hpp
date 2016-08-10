// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ACTS_PARAMETERDEFINITIONS_H
#define ACTS_PARAMETERDEFINITIONS_H 1

#ifdef ACTS_PARAMETER_DEFINITIONS_PLUGIN
#include ACTS_PARAMETER_DEFINITIONS_PLUGIN
#endif

/// testing requirements on parameter definitions
#include <type_traits>

/// typedefs for parameter identifier and parameter value must be present
static_assert(std::is_enum<Acts::ParID_t>::value,
              "'ParID_t' is not an enum type");
static_assert(std::is_floating_point<Acts::ParValue_t>::value,
              "'ParValue_t' is not floating point type");

/// parameter ID type must be convertible to size_t
static_assert(std::is_convertible<Acts::ParID_t, size_t>::value,
              "'ParID_t' is not convertible to size_t");

/// number of global parameter must be at least 2 (for the two local parameters)
static_assert(Acts::NGlobalPars > 1,
              "total number of global parameters must be >= 2");

/// several constants for the local parameters need to be defined
static_assert(Acts::eLOC_1 != Acts::eLOC_2,
              "local parameters must have different IDs");
static_assert(Acts::eLOC_R == Acts::eLOC_1 or Acts::eLOC_R == Acts::eLOC_2,
              "local radius must be a local parameter");
static_assert(Acts::eLOC_PHI == Acts::eLOC_1 or Acts::eLOC_PHI == Acts::eLOC_2,
              "local phi must be a local parameter");
static_assert(Acts::eLOC_RPHI == Acts::eLOC_1
                  or Acts::eLOC_RPHI == Acts::eLOC_2,
              "local r x phi must be a local parameter");
static_assert(Acts::eLOC_Z == Acts::eLOC_1 or Acts::eLOC_Z == Acts::eLOC_2,
              "local z must be a local parameter");
static_assert(Acts::eLOC_X == Acts::eLOC_1 or Acts::eLOC_X == Acts::eLOC_2,
              "local x must be a local parameter");
static_assert(Acts::eLOC_Y == Acts::eLOC_1 or Acts::eLOC_Y == Acts::eLOC_2,
              "local y must be a local parameter");
static_assert(Acts::eLOC_D0 == Acts::eLOC_1 or Acts::eLOC_D0 == Acts::eLOC_2,
              "d0 must be a local parameter");
static_assert(Acts::eLOC_Z0 == Acts::eLOC_1 or Acts::eLOC_Z0 == Acts::eLOC_2,
              "z0 must be a local parameter");

/// check for par_type_t definition
static_assert(sizeof(Acts::par_type_t<Acts::eLOC_1>) > 0,
              "'par_type_t' is not defined");

#endif  //  ACTS_PARAMETERDEFINITIONS_H
