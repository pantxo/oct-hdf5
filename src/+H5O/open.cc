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

DEFUN_DLD(open, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{dataset_id} =} H5O.open (@var{loc_id}, @var{name}, @var{lapl_id})\n\
Open a group, dataset, or committed (named) datatype specified by a location, \
@var{loc_id}, and a path name, @var{name}\n\
\n\
@seealso{H5O.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2 && nargin != 3)
    print_usage ("H5O.open");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5O.open");

  std::string name = args(1).xstring_value ("H5O.open: NAME must be a string");

  // Dataset access plist
  hid_t lapl_id = H5P_DEFAULT;
  if (nargin == 3)
    lapl_id = get_h5_id (args, 2, "LAPL_ID", "H5O.open");

  hid_t dataset_id =  H5Oopen (loc_id, name.c_str (), lapl_id);

  if (dataset_id < 0)
    error ("H5O.open: unable open dataset");

  return ovl (octave_int64 (dataset_id));
}
