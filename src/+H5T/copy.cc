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

DEFUN_DLD(copy, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} =} H5T.copy (@var{type_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.copy");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.copy");

  hid_t copied_type_id = H5Tcopy (type_id);

  if (copied_type_id < 0)
    error ("H5T.copy: unable to copy data type");

  return ovl (octave_int64 (copied_type_id));
}
