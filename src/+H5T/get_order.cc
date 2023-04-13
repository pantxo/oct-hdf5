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

DEFUN_DLD(get_order, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{byte_order} =} H5T.get_order (@var{type_id})\n\
Get the byte order of the data type specified by @var{type_id}.\n\
\n\
The output can be @qcode{'H5T_ORDER_LE'}, @qcode{'H5T_ORDER_BE'},\n\
@qcode{'H5T_ORDER_VAX'}, @qcode{'H5T_ORDER_MIXED'},\n\
or @qcode{'H5T_ORDER_NONE'}.\n\
@seealso{H5T.set_order}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.copy");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_order");

  H5T_order_t byte_order = H5Tget_order (type_id);

  if (byte_order == H5T_ORDER_ERROR)
    error ("H5T.get_order: unable to retrieve data type byte order");

  return ovl (octave_int64 (byte_order));
}
