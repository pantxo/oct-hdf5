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

DEFUN_DLD(get_space, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{space_id} = } H5A.get_space (@var{attr_id})\n\
Retrieve a copy of the dataspace for an attribute @var{attr_id}.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Identifier of an attribute\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
The dataspace identifier returned from this function must be \
released with H5S.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.open}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_space");

  // Attribue ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.get_space", false);

  hid_t space_id = H5Aget_space (attr_id);

  if (space_id < 0)
    error ("H5A.get_space: unable to retrieve data space");

  return retval.append (octave_int64 (space_id));
}

/*
%!fail ("H5A.get_space ()", "Invalid call");

%!fail ("H5A.get_space ('toto')", "ATTR_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5A.get_space (1); H5E.set_auto (true)", "unable to retrieve data space");
*/
