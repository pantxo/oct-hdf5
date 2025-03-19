/*

Copyright (C) 2025 Pantxo Diribarne

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

DEFUN_DLD(create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{type_id}} H5T.create (@var{class}, @var{size})\n\
Create a new datatype of the specified class with the specified number \
of bytes.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{class} @tab @tab Class of datatype among @qcode{'H5T_COMPOUND'}, \
@qcode{'H5T_OPAQUE'}, @qcode{'H5T_ENUM'}, @qcode{'H5T_STRING'}\n\
 @item @var{size} @tab @tab Size, in bytes, of the datatype being created\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5T_CREATE}.\n\
\n\
@seealso{H5T.open,H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5T.create");

  H5T_class_t cls =
    static_cast<H5T_class_t> (get_h5_id (args, 0, "CLASS", "H5T.create"));

  size_t sz =
    args(1).xuint64_value ("H5T.create: SIZE must be a numeric scalar");

  hid_t type_id =  H5Tcreate (cls, sz);

  if (type_id < 0)
    error ("H5T.create: unable to create type");

  return ovl (octave_int64 (type_id));
}

/*
%!fail ("H5T.create ()", "Invalid call");
*/
