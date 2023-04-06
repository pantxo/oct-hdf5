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

DEFUN_DLD(fill_value_defined, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{fill_val_id} = } H5P.fill_value_defined (@var{plist_id})\n \
@seealso{H5D.get_create_plist}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.fill_value_defined");

  // Property list ID
  hid_t plist_id = get_h5_id (args, 0, "PLIST_ID", "H5P.fill_value_defined");

  H5D_fill_value_t fill_val_id;

  if (H5Pfill_value_defined (plist_id, &fill_val_id) < 0)
    error ("H5P.fill_value_defined: unable to get creation fill value id");

  return retval.append (octave_int64 (fill_val_id));
}
