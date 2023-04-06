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

DEFUN_DLD(get_space, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{space_id} = } H5D.get_space (@var{dataset_id})\n\
Make a copy of the dataspace of the dataset specified by @var{dataset_id}.\n\
\n\
The function returns an identifier for the new copy of the dataspace.\n\
\n\
A dataspace identifier returned from this function should be released with\n\
@code{H5S.close} when the identifier is no longer needed so that resource\n\
leaks will not occur.\n\
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
    error ("H5D.get_space: unable retrieve data space");

  return retval.append (octave_int64 (space_id));
}
