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
#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(get_class, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{class_id} =} H5T.get_class (@var{type_id})\n\
Get the class of the data type specified by @var{type_id}.\n\
\n\
The output can be @qcode{'H5T_INTEGER'}, @qcode{'H5T_FLOAT'},\n\
@qcode{'H5T_STRING'}, @qcode{'H5T_BITFIELD'}, @qcode{'H5T_OPAQUE'},\n\
@qcode{'H5T_COMPOUND'}, @qcode{'H5T_REFERENCE'}, @qcode{'H5T_ENUM'},\n\
@qcode{'H5T_VLEN'}, or @qcode{'H5T_ARRAY'}.\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.get_class");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_class");

  H5T_class_t class_id = H5Tget_class (type_id);

  if (class_id == H5T_NO_CLASS)
    error ("H5T.get_class: unable to retrieve data type class");

  return ovl (octave_int64 (class_id));
}
