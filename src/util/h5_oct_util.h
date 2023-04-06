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

#ifndef H5_OCT_UTIL_H
#define H5_OCT_UTIL_H

#include <octave/octave.h>
#include <octave/parse.h>

#include <hdf5/serial/hdf5.h>

#include <set>
#include <string>

dim_vector get_dim_vector (hid_t space_id);

hid_t get_h5_id (const octave_value_list& args, int argnum,
                 std::string argname, std::string caller,
                 bool maybe_string = true);

struct user_callback_data
{
  // User function to be called by the handler
  octave_value callback;

  // User data
  octave_value data;
};

herr_t iter_handler (hid_t group_id, const char* name,
                     const H5L_info_t* /*info*/, void* user_data);

herr_t A_iter_handler (hid_t group_id, const char* name,
                       const H5A_info_t* /*info*/, void* user_data);

std::map<std::string, hid_t> get_constants_map (void);

#endif
