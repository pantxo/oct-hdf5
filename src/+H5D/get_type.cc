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

DEFUN_DLD(get_type, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} = } H5D.get_type (@var{dataset_id})\n\
Return an identifier for a copy of the datatype for a dataset.\n\
\n\
If a dataset has a named datatype, then an identifier to the opened\n\
datatype is returned.  Otherwise, the returned datatype is read-only.\n\
If atomization of the datatype fails, then the datatype is closed.\n\
\n\
A datatype identifier returned from this function should be released with\n\
@code{H5T.close} when the identifier is no longer needed so that resource\n\
leaks will not occur.\n\
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
