/*

Copyright (C) 2021 Pantxo Diribarne

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef H5_DATA_UTIL_H
#define H5_DATA_UTIL_H

#include <octave/octave.h>
#include <hdf5/serial/hdf5.h>

#include "H5LT_c.h"

#define H5READ()                                                        \
  herr_t status;                                                        \
  if (read_fcn == 0)                                                    \
    status = H5Dread (object_id, mem_type_id, mem_space_id,             \
                      file_space_id, xfer_plist_id,                     \
                      data.fortran_vec ());                             \
  else                                                                  \
    status = H5Aread (object_id, mem_type_id, data.fortran_vec ());     \
                                                                        \
  if (status < 0)                                                       \
    error ("%s: unable to read data (status %d)", caller.c_str (),      \
           status);                                                     \
                                                                        \
  retval = octave_value (data);


octave_value
__h5_read__ (const std::string& caller, dim_vector dv, hid_t object_id,
             hid_t mem_type_id, hid_t mem_space_id = H5S_ALL,
             hid_t file_space_id = H5S_ALL, hid_t xfer_plist_id = H5P_DEFAULT,
             hid_t field_type_id = H5_INDEX_UNKNOWN);

void
__h5write__ (const std::string& caller, const octave_value& ov,
             hid_t object_id, hid_t mem_type_id, hid_t mem_space_id = H5S_ALL,
             hid_t file_space_id = H5S_ALL, hid_t xfer_plist_id = H5P_DEFAULT,
             hid_t field_type_id = H5_INDEX_UNKNOWN);
#endif
