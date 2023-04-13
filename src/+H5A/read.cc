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

DEFUN_DLD(read, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{data} = } H5A.read (@var{attr_id})\n\
@deftypefnx {} {@var{data} = } H5A.read (@var{attr_id}, @var{mem_type_id})\n\
Read an attribute specified with @var{attr_id}.\n\
\n\
The attribute's memory datatype is specified with @var{mem_type_id}.\n\
\n\
The entire attribute is read and returned in @var{data}.\n\
\n\
Datatype conversion takes place at the time of a read or write and is\n\
automatic.\n\
\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1 && nargin != 2)
    print_usage ("H5A.read");

  // Attribute ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.read", false);

  // Data buffer memory type ID
  hid_t mem_type_id = H5Aget_type (attr_id);

  if (nargin > 1
      && (! args(1).is_string ()
          || args(1).string_value () != "H5ML_DEFAULT"))
        mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5A.read");

  // Get output dimensions
  dim_vector dv;
  hid_t space_id = H5Aget_space (attr_id);
  dv = get_dim_vector (space_id);

  if (dv.ndims () == 0)
    retval = ovl (Matrix ());
  else
    retval.append (__h5_read__ ("H5A.read", dv, attr_id, mem_type_id));

  return retval;
}
