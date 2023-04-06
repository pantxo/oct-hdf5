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
#include <hdf5/serial/hdf5_hl.h>

#include "../util/H5LT_c.h"
#include "../util/h5_oct_util.h"

DEFUN_DLD(dtype_to_struct, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{str} = } H5LT.dtype_to_struct (@var{type_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5LT.dtype_to_struct");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "DATASET_ID", "H5LT.dtype_to_struct");

  octave_scalar_map s;

  if (! dtype_to_struct (type_id, s))
    error ("H5LT.dtype_to_struct: unable retrive struct info");

  return retval.append (s);
}
