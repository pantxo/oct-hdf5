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

DEFUN_DLD(open_name, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{attr_id} =} H5A.open_name (@var{loc_id}, @var{name})\n\
Open an attribute specified by its name, @var{name}, which is attached to\n\
the object specified with @var{loc_id}. \n\
\n\
The location object may be either a group, dataset, or named datatype,\n\
which may have any sort of attribute.\n\
\n\
The attribute identifier @var{attr_id} returned by this function must be\n\
released with H5A.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5A.open_name");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.open_name");

  std::string name = args(1).xstring_value ("H5A.open_name: NAME must be a string");

  hid_t attr_id =  H5Aopen_name (loc_id, name.c_str ());

  if (attr_id < 0)
    error ("H5A.open_name: unable open attribute");

  return ovl (octave_int64 (attr_id));
}
