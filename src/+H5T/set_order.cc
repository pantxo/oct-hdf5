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
#include <hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(set_order, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} { } H5T.set_order (@var{type_id}, @var{byte_order})\n                       \
Set the byte order of the data type specified by @var{type_id}.\n\
\n\
The input @var{byte_order} can be @qcode{'H5T_ORDER_LE'},\n\
@qcode{'H5T_ORDER_BE'}, or @qcode{'H5T_ORDER_VAX'}.\n\
@seealso{H5T.get_order}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  std::map<std::string, hid_t> h5_oct_constants = get_constants_map ();

  if (nargin != 2)
    print_usage ("H5T.set_order");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_order");

  // Byte order
  H5T_order_t byte_order
    = static_cast<H5T_order_t> (get_h5_id (args, 0, "BYTE_ORDER",
                                           "H5T.set_order"));

  if (H5Tset_order (type_id, byte_order) < 1)
    error ("H5T.set_order: unable to change byte order of specified type");

  return retval;
}
