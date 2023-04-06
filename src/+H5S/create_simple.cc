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

DEFUN_DLD(create_simple, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{space_id} =} create_simple (@var{rank}, @var{dims}, @var{max_dims})\n \
Create a new simple dataspace and open it for access, returning a dataspace \
identifier.\n\
\n\
@var{rank} is the number of dimensions used in the dataspace.\n\
\n\
@var{dims} is a one-dimensional array of size @var{rank} specifying the \
size of each dimension of the dataset. maximum_dims is an array of the same \
size specifying the upper limit on the size of each dimension.\n\
\n\
Any element of @var{dims} can be 0 (zero). Note that no data can be written to \
a dataset if the size of any dimension of its current dataspace is 0. This is \
sometimes a useful initial state for a dataset.\n\
\n\
@var{max_dims} may be an empty matrix, in which case the upper limit is the \
same as @var{dims}. Otherwise, no element of @var{max_dims} should be smaller \
than the corresponding element of @var{dims}.\n\
\n\
@var{max_dims} may also be \"H5S_UNLIMITED\", in which case all dimensions \
are unlimited.\n\
\n\
Any dataset with an unlimited dimension must also be chunked. \
See @code{H5P.set_chunk}. Similarly, a dataset must be chunked if @var{dims} \
does not equal @var{max_dims}.\n\
\n\
The dataspace identifier returned from this function must be released with \
@code{H5S.close} or resource leaks will occur.\n\
@seealso{H5S.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ();

  // Rank
  int rank =  args(0).xint_value ("RANK must be a numeric scalar");

  // Dims
  uint64NDArray tmp
    = args(1).xint64_array_value ("DIMS must be a numeric vector");

  if (rank != static_cast<int> (tmp.numel ()))
    error ("H5S.create_simple: inconsistent RANK and DIMS arguments");

  hsize_t dims[rank];
  dim_vector dv = tmp.dims ();
  for (int ii = 0; ii < tmp.numel (); ii++)
    dims[ii] = static_cast<hsize_t> (tmp(ii));

  // Maxdims
  tmp = args(2).xint64_array_value ("MAXDIMS must be a numeric vector");

  if (! tmp.isempty () && rank != static_cast<int> (tmp.numel ()))
    error ("H5S.create_simple: inconsistent RANK and MAXDIMS arguments");

  hsize_t maxdims[rank];
  dv = tmp.dims ();
  for (int ii = 0; ii < rank; ii++)
    if (tmp.isempty ())
      maxdims[ii] = dims[ii];
    else if (dims[ii] <= static_cast<hsize_t> (tmp(ii)))
      maxdims[ii] = static_cast<hsize_t> (tmp(ii));
    else
      error ("H5S.create_simple: elements in MAXDIMS must be smaller than \
of egual to the elemnts in DIMS");


  hid_t sid = H5Screate_simple (rank, dims, maxdims);

  if (sid < 0)
    error ("H5S.create_simple: unable to create dataspace");

  return ovl (octave_int64 (sid));
}
