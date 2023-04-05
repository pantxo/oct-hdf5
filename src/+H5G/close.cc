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
@deftypefn {} {} H5G.close (@var{group_id})\n\
Release resources used by a group which was opened by @code{H5G.create}\n\
or @code{H5G_OPEN}.\n\
\n\
After closing a group, the group_id cannot be used again.\n\
\n\
Failure to release a group with this call will result in resource leaks.\n\
\n\
@seealso{H5G.open}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5G.close");

  hid_t group_id = get_h5_id (args, 0, "GROUP_ID", "H5G.close");

  if (H5Gclose (group_id) < 0)
    error ("H5G.close: unable close group");

  return retval;
}
