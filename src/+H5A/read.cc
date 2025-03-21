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
Import data from dataset.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Location or attribute identifier\n\
 @item @var{mem_type_id} @tab @tab Target datatype (use @code{H5ML_DEFAULT} \
for automatic conversion)\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
Only data with atomic HDF5 data types are handled. See @code{help H5D.read} \
for details about automatic data type conversion.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5D.read}\n\
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


/*
%!test
%! fail ("H5A.read ()", "Invalid call");

%!test
%! fail ("H5A.read (1, 2, 3)", "Invalid call");

%!test
%! fail ("H5E.set_auto (false); data = H5A.read (123456789, 'toto'); H5E.set_auto (true)", "unknown MEM_TYPE_ID 'toto'");
*/
