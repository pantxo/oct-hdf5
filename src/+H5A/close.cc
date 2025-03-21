/*

Copyright (C) 2021-2025 Pantxo Diribarne

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

DEFUN_DLD(close, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} { } H5A.close (@var{attr_id})\n\
Terminates access to the attribute specified by @var{attr_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Attribute to release access to\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
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
    print_usage ("H5A.close");

  // Attribute ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.close", false);

  if (H5Aclose (attr_id) < 0)
    error ("H5A.close: unable to close attribute");

  return retval;
}

/*
%!test
%! fail ("H5A.close ()", "Invalid call");

%!test
%! fail ("H5E.set_auto (false); H5A.close (123456789); H5E.set_auto (true)", "unable to close attribute");
*/
