/*

Copyright (C) 2021-2025 Pantxo Diribarne

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

#include "./util/h5_oct_util.h"
// PKG_ADD: autoload ("__H5S_close__", "__H5S__.oct");
// PKG_DEL: autoload ("__H5S_close__", "__H5S__.oct", "remove");
DEFUN_DLD(__H5S_close__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5S.close (@var{space_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5S.close");

  // Space ID
  hid_t space_id = get_h5_id (args, 0, "SPACE_ID", "H5S.close", false);

  if (H5Sclose (space_id) < 0)
    error ("H5S.close: unable to close data space object");

  return retval;
}

// PKG_ADD: autoload ("__H5S_create__", "__H5S__.oct");
// PKG_DEL: autoload ("__H5S_create__", "__H5S__.oct", "remove");
DEFUN_DLD(__H5S_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{space_id} =} create (@var{type})\n\
\n\
Create a new dataspace of a particular type.\n\
\n\
Currently supported types are as follows: @qcode{'H5S_SCALAR'},\n\
@qcode{'H5S_SIMPLE'}, and @qcode{'H5S_NULL'}. Further dataspace types\n\
may be added later.\n\
\n\
A scalar dataspace, @qcode{'H5S_SCALAR'}, has a single element,\n\
though that element may be of a complex datatype, such as a compound\n\
or array datatype. By convention, the rank of a scalar dataspace is\n\
always 0 (zero); think of it geometrically as a single, dimensionless point,\n\
though that point can be complex.\n\
\n\
A simple dataspace, @var{H5S_SIMPLE}, consists of a regular array\n\
of elements.\n\
\n\
A null dataspace, @var{H5S_NULL}, has no data elements.\n\
\n\
The dataspace identifier returned by this function can be released\n\
with @code{H5S.close} so that resource leaks will not occur.\n\
@seealso{H5S.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ();

  // Type
  H5S_class_t type =  static_cast<H5S_class_t> (get_h5_id (args, 0, "TYPE", "H5S.create"));

  hid_t sid = H5Screate (type);

  if (sid < 0)
    error ("H5S.create: unable to create dataspace");

  return ovl (octave_int64 (sid));
}

// PKG_ADD: autoload ("__H5S_create_simple__", "__H5S__.oct");
// PKG_DEL: autoload ("__H5S_create_simple__", "__H5S__.oct", "remove");
DEFUN_DLD(__H5S_create_simple__, args, nargout, 
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

// PKG_ADD: autoload ("__H5S_get_simple_extent_dims__", "__H5S__.oct");
// PKG_DEL: autoload ("__H5S_get_simple_extent_dims__", "__H5S__.oct", "remove");
DEFUN_DLD(__H5S_get_simple_extent_dims__, args, nargout, 
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

// PKG_ADD: autoload ("__H5S_get_simple_extent_type__", "__H5S__.oct");
// PKG_DEL: autoload ("__H5S_get_simple_extent_type__", "__H5S__.oct", "remove");
DEFUN_DLD(__H5S_get_simple_extent_type__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{space_type} =} H5S.get_simple_extent_type (@var{space_id})\
n\
\n\
Determine the current class @var{space_type} of a dataspace @var{space_id}.\n\
\n\
@seealso{H5S.get_simple_extent_dims}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5S.get_simple_extent_type");

  // Space ID
  hid_t space_id = get_h5_id (args, 0, "SPACE_ID", "H5S.get_simple_extent_type", false);

  H5S_class_t space_type = H5Sget_simple_extent_type (space_id);

  if (space_type < 0)
    error ("H5S.get_simple_extent_type: unable to close data space object");

  return retval.append (octave_int64 (space_type));
}


