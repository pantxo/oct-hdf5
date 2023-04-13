/*

Copyright (C) 2023 Vipul Cariappa

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

#include <hdf5/serial/hdf5.h>
#include <octave/oct.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD (get_num_attrs, args, nargout, "-*- texinfo -*-\n\
@deftypefn {} {@var{attr_count} =} H5A.get_num_attrs (@var{loc_id})\n\
Determine the number of attributes attached to @var{loc_id}.\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_num_attrs");

  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.get_num_attrs", false);

  int attr_count = H5Aget_num_attrs (loc_id);

  if (attr_count < 0)
    error ("H5A.get_num_attr: unable determine number of attributes");

  return ovl (octave_int64 (attr_count));
}
