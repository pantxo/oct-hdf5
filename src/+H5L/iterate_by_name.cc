/*

Copyright (C) 2021-2023 Pantxo Diribarne

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

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/interpreter.h>

#include <hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(iterate_by_name, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5L.iterateiterate_by_name (@var{group_id}, @var{name}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in}, @var{lapl_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 8)
    print_usage ("H5L.iterate_by_name");

  // Group ID
  hid_t group_id = get_h5_id (args, 0, "GROUP_ID", "H5L.iterate_by_name");

  // Group name
  std::string name = args(1).xstring_value ("H5L.iterate_by_name: NAME should be a string");

  // Index type
  H5_index_t idx_type
    = static_cast<H5_index_t> (get_h5_id (args, 2, "IDX_TYPE",
                                          "H5L.iterate_by_name"));

  // Order
  H5_iter_order_t order
    = static_cast<H5_iter_order_t> (get_h5_id (args, 3, "ORDER",
                                               "H5L.iterate_by_name"));

  // Iteration index
  hsize_t idx_in
    = static_cast<hsize_t> (get_h5_id (args, 4, "IDX_IN", "H5L.iterate_by_name",
                                       false));

  // Iteration handler callback

  // Pack function handle and user data in a struct and pass it to a generic
  // iteration handler
  if (! args(5).is_function_handle ())
    error ("H5L.iterate_by_name: FCN must be a function handle");

  user_callback_data ucd;
  ucd.callback = args(5);
  ucd.data = args(6);

  // Link access plist
  hsize_t lapl_id = get_h5_id (args, 7, "LAPL_ID", "H5L.iterate_by_name");

  herr_t status = H5Literate_by_name (group_id, name.c_str (), idx_type, order,
                                      &idx_in, iter_handler,
                                      &ucd, lapl_id);

  retval.append (octave_value (status));
  retval.append (octave_value (idx_in));
  retval.append (ucd.data);

  return retval;
}
