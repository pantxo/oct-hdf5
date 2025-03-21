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

DEFUN_DLD(get_type, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{type_id} = } H5A.get_type (@var{attr_id})\n\
Return a datatype identifier if successful; otherwise returns a negative \
value.\n\
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
The datatype identifier @var{type_id} returned from this function must\n\
be released with H5T.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_type");

  // Attribue ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.get_type", false);

  hid_t type_id = H5Aget_type (attr_id);

  if (type_id < 0)
    error ("H5A.get_type: unable to retrieve data type");

  return retval.append (octave_int64 (type_id));
}

/*
%!fail ("H5A.get_type ()", "Invalid call");

%!fail ("H5A.get_type ('toto')", "ATTR_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5A.get_type (1); H5E.set_auto (true)", "unable to retrieve data type");
*/
