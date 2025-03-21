/*

Copyright (C) 2023 Vipul Cariappa

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

#include <hdf5.h>
#include <octave/oct.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD (get_num_attrs, args, nargout, "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{attr_count} =} H5A.get_num_attrs (@var{loc_id})\n\
Return the number of attributes if successful; otherwise return a negative \
value.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Identifier of a group, dataset, or named \
datatype\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_num_attrs");

  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.get_num_attrs", false);

  int attr_count = H5Aget_num_attrs (loc_id);

  if (attr_count < 0)
    error ("H5A.get_num_attr: unable determine number of attributes");

  return ovl (octave_int64 (attr_count));
}


/*
%!test
%! fail ("H5A.get_num_attrs ()", "Invalid call");

%!test
%! fail ("H5A.get_num_attrs ('toto')", "LOC_ID must be a scalar numeric identifier");

%!test
%! fail ("H5E.set_auto (false); H5A.get_num_attrs (1); H5E.set_auto (true)", "unable determine number of attributes");
*/
