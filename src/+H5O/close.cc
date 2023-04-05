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

#include "../+H5ML/h5_oct_util.h"

DEFUN_DLD(close, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5O.close (@var{object_id})\n\
Closes the object, dataset, or named datatype specified by @var{object_id}.\n\
\n\
This function is the companion to @code{H5O.open}, and has the same\n\
effect as calling @code{H5G.close}, @code{H5D.close}, or @code{H5T.close}.\n\
\n\
@code{H5O.close} is not used to close a dataspace, attribute, property list,\n\
or file.\n\
\n\
@seealso{H5O.open}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5O.close");

  hid_t object_id = get_h5_id (args, 0, "OBJECT_ID", "H5O.close");

  if (H5Oclose (object_id) < 0)
    error ("H5O.close: unable close object");

  return retval;
}
