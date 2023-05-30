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
@deftypefn {} {@var{type_id} = } H5D.get_type (@var{dataset_id})\n\
Return an identifier for a copy of the datatype for a dataset.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Identifier of a dataset\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
The datatype identifier @var{type_id} returned from this function must\n\
be released with H5T.close or resource leaks will develop.\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_TYPE}.\n\
\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.get_type");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.get_type", false);

  hid_t type_id = H5Dget_type (dataset_id);

  if (type_id < 0)
    error ("H5D.get_type: unable retrieve data type");

  return retval.append (octave_int64 (type_id));
}

/*
%!fail ("H5D.get_type ()", "Invalid call");

%!fail ("H5D.get_type ('toto')", "DATASET_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5A.get_type (-12345); H5E.set_auto (true)", "unable to retrieve data type");
*/
