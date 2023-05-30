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

DEFUN_DLD(close, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} { } H5D.close (@var{dataset_id})\n\
End access to a dataset specified by @var{dataset_id} and releases resources\n\
used by it.\n\
\n\
Further use of the dataset identifier is illegal in calls to\n\
the dataset API.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Dataset to release access to\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_CLOSE}.\n\
\n\
@seealso{H5D.open,H5D.create}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.close");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.close", false);

  if (H5Dclose (dataset_id) < 0)
    error ("H5D.close: unable to close dataset");

  return retval;
}

/*
%!fail ("H5D.close ()", "Invalid call")

%!fail ("H5D.close (1, 2)", "Invalid call")

%!fail ("H5D.close (-123456)", "unable to close dataset")
*/
