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
#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(get_simple_extent_dims, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {[@var{ndims}, @var{dims}, @var{maxdims}] = } H5S.get_simple_extent_dims (@var{space_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5S.get_simple_extent_dims");

  // Space ID
  hid_t space_id = get_h5_id (args, 0, "SPACE_ID", "H5S.get_simple_extent_dims", false);

  int ndims = H5Sget_simple_extent_ndims (space_id);

  if (ndims < 0)
    error ("H5S.get_simple_extent_dims: unable to get space rank");

  hsize_t dims[ndims];
  hsize_t maxdims[ndims];

  ndims = H5Sget_simple_extent_dims (space_id, dims, maxdims);

  if (ndims < 0)
    error ("H5S.get_simple_extent_dims: unable to get space size");

  Matrix odims (1, ndims, 0);
  Matrix omaxdims (1,ndims, 0);

  for (int ii = 0; ii < ndims; ii++)
    {
      odims(ii) = dims[ii];

      if (maxdims[ii] == H5S_UNLIMITED)
        omaxdims(ii) = octave_Inf;
      else
        omaxdims(ii) = maxdims[ii];
    }

  retval.append (ndims);
  retval.append (odims);
  retval.append (omaxdims);

  return retval;
}
