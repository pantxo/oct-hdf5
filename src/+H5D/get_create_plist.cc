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

#include "../+H5ML/h5_oct_util.h"

DEFUN_DLD(get_create_plist, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{dcpl_id} = } H5D.get_create_plist (@var{dataset_id})\n\
Return an identifier for a copy of the dataset creation property list\n\
associated with the dataset specified by @var{dataset_id}.\n\
\n\
The creation property list identifier should be released with\n\
@code{H5P.close}.\n\
\n\
@seealso{H5P.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.get_create_plist");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.get_create_plist");

  hid_t dcpl_id = H5Dget_create_plist (dataset_id);

  if (dcpl_id < 0)
    error ("H5D.get_create_plist: unable retrieve creation property list");

  return retval.append (octave_int64 (dcpl_id));
}
