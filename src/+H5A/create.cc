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
@deftypefn {} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id})\n\
@deftypefn {} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id}, @var{aapl_id},)\n\
Create an attribute, @var{attr_name}, which is attached to the object\n\
specified by the identifier @var{loc_id}.\n\
\n\
The attribute name, @var{attr_name}, must be unique for the object.\n\
\n\
The attribute is created with the specified datatype and dataspace,\n\
@var{type_id} and @var{space_id}, which are created with the H5T and H5S\n\
interfaces, respectively.\n\
\n\
If @var{type_id} is either a fixed-length or variable-length string,\n\
it is important to set the string length when defining the datatype.\n\
String datatypes are derived from @qcode{'H5T_C_S1'} (or\n\
@qcode{'H5T_FORTRAN_S1'} for Fortran), which defaults to 1 character\n\
in size. See @code{H5T.set_size}.\n\
\n\
The access property list is currently unused, but will be used in the future.\n\
This property list should currently be @qcode{'H5P_DEFAULT'}.\n\
\n\
The attribute identifier returned by this function must be released\n\
with @code{H5A.close} or resource leaks will develop.\n\
\n\
@seealso{H5A.open,H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin < 4 && nargin > 6)
    print_usage ("H5A.create");

  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.create");

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

  hid_t attribute_id =  H5Acreate (loc_id, attr_name.c_str (), type_id,
                                   space_id, acpl_id, aapl_id);

  if (attribute_id < 0)
    error ("H5A.create: unable to create attribute");

  return ovl (octave_int64 (attribute_id));
}
