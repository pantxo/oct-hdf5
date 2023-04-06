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

Author: Pantxo Diribarne <pantxo@dibona>
Created: 2021-04-25

*/

#include <octave/oct.h>

#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(iterate, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5L.iterate (@var{group_id}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 6)
    print_usage ("H5L.iterate");

  // Group ID
  hid_t group_id = get_h5_id (args, 0, "GROUP_ID", "H5L.iterate");

  // Index type
  H5_index_t idx_type
    = static_cast<H5_index_t> (get_h5_id (args, 1, "IDX_TYPE", "H5L.iterate"));

  // Order
  H5_iter_order_t order
    = static_cast<H5_iter_order_t> (get_h5_id (args, 2, "ORDER",
                                               "H5L.iterate"));

  // Iteration index
  hsize_t idx_in
    = static_cast<hsize_t> (get_h5_id (args, 3, "ORDER", "H5L.iterate"), false);

  // Iteration handler callback
  if (! args(4).is_function_handle ())
    error ("H5L.iterate: FCN must be a function handle");

  user_callback_data ucd;
  ucd.callback = args(4);
  ucd.data = args(5);

  herr_t status = H5Literate (group_id, idx_type, order, &idx_in, iter_handler,
                              &ucd);

  retval.append (octave_value (status));
  retval.append (octave_value (idx_in));
  retval.append (ucd.data);

  return retval;
}
