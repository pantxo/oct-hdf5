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

DEFUN_DLD(create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id})\n\
@deftypefnx {oct-hdf5} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id}, @var{aapl_id},)\n\
Create an attribute, @var{attr_name}, which is attached to the object\n\
specified by the identifier @var{loc_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Location or object identifier\n\
 @item @var{attr_name} @tab @tab Attribute name\n\
 @item @var{space_id} @tab @tab Attribute dataspace identifier\n\
 @item @var{acpl_id} @tab @tab Attribute creation property list identifier\n\
 @item @var{aapl_id} @tab @tab Attribute access property list identifier\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.open,H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin < 4 || nargin > 6)
    print_usage ("H5A.create");

  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.create", false);

  std::string attr_name
    = args(1).xstring_value ("H5A.create: ATTR_NAME must be a string");

  hid_t type_id = get_h5_id (args, 2, "TYPE_ID", "H5A.create");

  hid_t space_id = get_h5_id (args, 3, "SPACE_ID", "H5A.create");

  hid_t acpl_id = H5P_DEFAULT;
  if (nargin > 4)
    acpl_id = get_h5_id (args, 4, "ACPL_ID", "H5A.create");

  hid_t aapl_id = H5P_DEFAULT;
  if (nargin > 5)
    aapl_id = get_h5_id (args, 5, "AAPL_ID", "H5A.create");

  hid_t attribute_id =  H5Acreate2 (loc_id, attr_name.c_str (), type_id,
                                   space_id, acpl_id, aapl_id);

  if (attribute_id < 0)
    error ("H5A.create: unable to create attribute");

  return ovl (octave_int64 (1));
}

/*
%!fail ("H5A.create ()", "Invalid call");

%!fail ("H5A.create (123456789, 1, 1, 1, 1)", "ATTR_NAME must be a string");

%!fail ("H5E.set_auto (false); H5A.create (123456789, 'att', 1, 1, 1); H5E.set_auto (true)", "unable to create attribute");

%!fail ("H5A.create (123456789, 'att', 'blob', 1, 1)", "unknown TYPE_ID 'blob'");
*/
