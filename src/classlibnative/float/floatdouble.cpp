// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//
// File: FloatDouble.cpp
//

#include <common.h>

#include "floatdouble.h"

// The default compilation mode is /fp:precise, which disables floating-point intrinsics. This
// default compilation mode has previously caused performance regressions in floating-point code.
// We enable /fp:fast semantics for the majority of the math functions, as it will speed up performance
// and is really unlikely to cause any other code regressions.

// Sin, Cos, and Tan on AMD64 Windows were previously implemented in vm\amd64\JitHelpers_Fast.asm
// by calling x87 floating point code (fsin, fcos, fptan) because the CRT helpers were too slow. This
// is no longer the case and the CRT call is used on all platforms.

// Log, Log10 and Exp were previously slower with /fp:fast on SSE2 enabled hardware (see #500373).
// This is no longer the case and they now consume use the /fp:fast versions.

// Exp(+/-INFINITY) did not previously return the expected results of +0.0 (for -INFINITY)
// and +INFINITY (for +INFINITY) so these cases were handled specially. As this is no longer
// the case and the expected results are now returned, the special handling has been removed.

// Previously there was more special handling for the x86 Windows version of Pow.
// This additional handling was unnecessary and has since been removed.

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
///
///                         beginning of /fp:fast scope
///
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma float_control(precise, off)
#endif

/*=====================================Abs======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Abs, double x)
    FCALL_CONTRACT;

    return (double)fabs(x);
FCIMPLEND

/*=====================================Acos=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Acos, double x)
    FCALL_CONTRACT;

    return (double)acos(x);
FCIMPLEND

/*=====================================Asin=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Asin, double x)
    FCALL_CONTRACT;

    return (double)asin(x);
FCIMPLEND

/*=====================================Atan=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Atan, double x)
    FCALL_CONTRACT;

    return (double)atan(x);
FCIMPLEND

/*=====================================Atan2====================================
**
==============================================================================*/
FCIMPL2_VV(double, COMDouble::Atan2, double y, double x)
    FCALL_CONTRACT;

    return (double)atan2(y, x);
FCIMPLEND

/*====================================Ceil======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Ceil, double x)
    FCALL_CONTRACT;

    return (double)ceil(x);
FCIMPLEND

/*=====================================Cos======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Cos, double x)
    FCALL_CONTRACT;

    return (double)cos(x);
FCIMPLEND

/*=====================================Cosh=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Cosh, double x)
    FCALL_CONTRACT;

    return (double)cosh(x);
FCIMPLEND

/*=====================================Exp======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Exp, double x)
    FCALL_CONTRACT;

    return (double)exp(x);
FCIMPLEND

/*====================================Floor=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Floor, double x)
    FCALL_CONTRACT;

    return (double)floor(x);
FCIMPLEND

/*=====================================Log======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Log, double x)
    FCALL_CONTRACT;

    return (double)log(x);
FCIMPLEND

/*====================================Log10=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Log10, double x)
    FCALL_CONTRACT;

    return (double)log10(x);
FCIMPLEND

/*=====================================ModF=====================================
**
==============================================================================*/
FCIMPL1(double, COMDouble::ModF, double* iptr)
    FCALL_CONTRACT;

    return (double)modf(*iptr, iptr);
FCIMPLEND

/*=====================================Pow======================================
**
==============================================================================*/
FCIMPL2_VV(double, COMDouble::Pow, double x, double y)
    FCALL_CONTRACT;

    return (double)pow(x, y);
FCIMPLEND

/*====================================Round=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Round, double x)
    FCALL_CONTRACT;

    // If the number has no fractional part do nothing
    // This shortcut is necessary to workaround precision loss in borderline cases on some platforms
    if (x == (double)((INT64)x)) {
        return x;
    }

    // We had a number that was equally close to 2 integers.
    // We need to return the even one.

    double flrTempVal = floor(x + 0.5);

    if ((x == (floor(x) + 0.5)) && (fmod(flrTempVal, 2.0) != 0)) {
        flrTempVal -= 1.0;
    }

    return _copysign(flrTempVal, x);
FCIMPLEND

/*=====================================Sin======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Sin, double x)
    FCALL_CONTRACT;

    return (double)sin(x);
FCIMPLEND

/*=====================================Sinh=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Sinh, double x)
    FCALL_CONTRACT;

    return (double)sinh(x);
FCIMPLEND

/*=====================================Sqrt=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Sqrt, double x)
    FCALL_CONTRACT;

    return (double)sqrt(x);
FCIMPLEND

/*=====================================Tan======================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Tan, double x)
    FCALL_CONTRACT;

    return (double)tan(x);
FCIMPLEND

/*=====================================Tanh=====================================
**
==============================================================================*/
FCIMPL1_V(double, COMDouble::Tanh, double x)
    FCALL_CONTRACT;

    return (double)tanh(x);
FCIMPLEND

#ifdef _MSC_VER
#pragma float_control(precise, on )
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
///
///                         End of /fp:fast scope
///
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
