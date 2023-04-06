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

DEFUN_DLD(get_size, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{sz} =} H5T.get_size (@var{type_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.get_size");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_size");

  size_t sz = H5Tget_size (type_id);

  if (sz == 0)
    error ("H5T.get_qsize: unable to retrieve data type size");

  return ovl (octave_int64 (sz));
}
