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

DEFUN_DLD(get_space, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{space_id} = } H5D.get_space (@var{dataset_id})\n\
Make a copy of the dataspace of the dataset specified by @var{dataset_id}.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Identifier of a dataset\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
The dataspace identifier returned from this function must be \
released with H5S.close or resource leaks will develop.\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_SPACE}.\n\
\n\
@seealso{H5S.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.get_space");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.get_space", false);

  hid_t space_id = H5Dget_space (dataset_id);

  if (space_id < 0)
    error ("H5D.get_space: unable to retrieve data space");

  return retval.append (octave_int64 (space_id));
}

/*
%!fail ("H5D.get_space ()", "Invalid call");

%!fail ("H5D.get_space ('toto')", "DATASET_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5D.get_space (1); H5E.set_auto (true)", "unable to retrieve data space");
*/
