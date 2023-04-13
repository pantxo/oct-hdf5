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

DEFUN_DLD(compare_values, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{tf} =} H5ML.compare_values (@var{val1}, @var{val2})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5ML.compare_values");

  hid_t val1 = get_h5_id (args, 0, "VAL1", "H5ML.compare_values");
  hid_t val2 = get_h5_id (args, 1, "VAL2", "H5ML.compare_values");

  return retval.append (val1 == val2);
}
