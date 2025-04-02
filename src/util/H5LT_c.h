/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// dtype_to_struct is an adaptation of the original H5LTdtype_to_text function.

#ifndef H5LT_C_H
#define H5LT_C_H

#if defined(__APPLE__) || defined(_WIN32)
    #include <hdf5.h>
#else
    #include <hdf5/serial/hdf5.h>
#endif

#include <octave/octave.h>
#include <octave/parse.h>

bool dtype_to_struct (hid_t dtype, octave_scalar_map& s);

#endif
