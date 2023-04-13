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

DEFUN_DLD(get_class_name, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{cname} =} H5E.get_class_name (@var{class_id})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.get_class_name");

  // Class id as returned in error structs
  hid_t class_id = get_h5_id (args, 0, "CLASS_ID", "H5E.get_class_name", false);

  size_t sz = 1024;
  char cname_char[sz];

  if (H5Eget_class_name (class_id, cname_char, sz) < 0)
    error ("H5E.get_class_name: unable to retrieve class name");

  return retval.append (std::string (cname_char));
}
