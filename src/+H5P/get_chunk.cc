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

DEFUN_DLD(get_chunk, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {[@var{rank, @var{dims}] = } H5P.get_chunk (@var{plist_id})\n\
@seealso{H5D.get_crreate_plist}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.get_chunk");

  // Property list ID
  hid_t plist_id = get_h5_id (args, 0, "PLIST_ID", "H5P.get_chunk");

  hsize_t dims[1024];

  int rank = H5Pget_chunk (plist_id, 1024, dims);

  if (rank < 0)
    error ("H5P.get_chunk: unable to get chunk dims");

  Matrix odims (1, rank, 0.0);

  for (int ii = 0; ii < rank; ii++)
    odims(ii) = dims[ii];

  retval.append (octave_value (rank));
  retval.append (odims);

  return retval;
}
