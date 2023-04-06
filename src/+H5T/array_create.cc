/*

Copyright (C) 2021 Pantxo Diribarne

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

Author: Pantxo Diribarne <pantxo@dibona>
Created: 2021-04-25

*/

#include <octave/oct.h>
#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(array_create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} =} H5T.array_create (@var{base_type_id}, @var{dims})\n\
@deftypefnx {} {@var{type_id} =} H5T.array_create (@var{base_type_id}, @var{rank}, @var{dims})\n\
\n\
Create a new array datatype object.\n\
\n\
@var{base_type_id} is the datatype of every element of the array, i.e.,\n\
of the number at each position in the array.\n\
\n\
@var{rank} is the number of dimensions and the size of each dimension is\n\
specified in the array @var{dims}.  The value of rank is currently limited\n\
to @qcode{'H5S_MAX_RANK'} and must be greater than 0 (zero).  All dimension\n\
sizes specified in @var{dims} must be greater than 0 (zero).\n\
\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2 && nargin != 3 && nargin != 4)
    print_usage ("H5T.array_create");

  if (nargin == 4)
    // SKIP last argument
    nargin = 3;

  // Type ID
  hid_t base_type_id = get_h5_id (args, 0, "BASE_TYPE_ID", "H5T.array_create");

  //Rank
  unsigned rank = 0;
  if (nargin == 3)
    {
      rank = static_cast<unsigned> (args(1).xuint64_value ("H5T.array_create: RANK must be a numeric positive scalar"));
      if (rank == 0)
        error ("H5T.array_create: RANK must be a numeric positive scalar");
    }

  // Dims
  uint64NDArray tmp_dims;
  if (nargin == 2)
    {
      tmp_dims = args(1).xuint64_array_value ("H5T.array_create: DIMS must be a vector of positive values");
      rank = static_cast<unsigned> (tmp_dims.dims ().ndims ());

    }
  else
    {
      tmp_dims = args(2).xuint64_array_value ("H5T.array_create: DIMS must be a vector of positive values");
      if (static_cast<unsigned> (tmp_dims.dims ().ndims ()) != rank)
        error ("H5T.array_create: inconsistent RANK and DIMS arguments");
    }

  hsize_t dims[rank];
  for (unsigned ii = 0; ii < rank; ii++)
    dims[ii] = static_cast<hsize_t> (tmp_dims(ii));

  hid_t type_id = H5Tarray_create (base_type_id, rank, dims);

  if (type_id < 0)
    error ("H5T.array_create: unable to create array data type");

  return ovl (octave_int64 (type_id));
}
