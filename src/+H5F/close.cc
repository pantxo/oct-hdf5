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

DEFUN_DLD(close, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5F.close (@var{file_id})\n\
@seealso{}\n\
@end deftypefn")
{
  int nargin = args.length ();

  if (nargin != 1)
    print_usage ();

  // File id
  hid_t file_id = get_h5_id (args, 0, "FILE_ID", "H5F.close", false);

  if (H5Fclose (file_id) < 0)
    error ("H5F.close: unable to close file");

  return ovl ();
}
