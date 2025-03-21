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
#include "../util/h5_data_util.h"


DEFUN_DLD(write, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5A.write (@var{attribute_id}, @var{mem_type_id}, @var{data})\n\
Write an attribute, specified with @var{attribute_id}.\n\
\n\
The attribute's memory datatype is specified with @var{mem_type_id}.\n\
The entire attribute is written to the file.\n\
\n\
If @var{mem_type_id} is either a fixed-length or variable-length string,\n\
it is important to set the string length when defining the datatype.\n\
String datatypes are derived from @qcode{'H5T_C_S1'} (or\n\
@qcode{'H5T_FORTRAN_S1'} for Fortran), which defaults to 1 character\n\
in size. See @code{H5T.set_size}.\n\
\n\
Datatype conversion takes place at the time of a read or write and is\n\
automatic.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.read}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5A.write");

  // Attribute ID
  hid_t attribute_id = get_h5_id (args, 0, "ATTRIBUTE_ID", "H5A.write", false);

  // Data buffer memory type ID
  hid_t mem_type_id;

  if (args(1).is_string () && args(1).string_value () == "H5ML_DEFAULT")
    mem_type_id = -1234;
  else
    mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5A.write");

  // Write data
  __h5write__ ("H5A.write", args(2), attribute_id, mem_type_id);

  return retval;
}
