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
@deftypefn {oct-hdf5} {@var{attr_id} =} H5A.open (@var{obj_id}, @var{name}, @var{aapl_id})\n\
Open an existing attribute, @var{name}, that is attached to an object\n\
specified by an object identifier, @var{obj_id}.\n\
\n\
The attribute access property list, @var{aapl_id}, is currently unused\n\
and should be @qcode{'H5P_DEFAULT'}.\n\
\n\
This function, must be called before an attribute can be accessed for any\n\
further purpose, including reading, writing, or any modification.\n\
\n\
The attribute identifier @var{attr_id} returned by this function must be\n\
released with H5A.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
@seealso{H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5A.open");

  // Parse arguments
  hid_t obj_id = get_h5_id (args, 0, "OBJ_ID", "H5A.open");

  std::string name = args(1).xstring_value ("H5A.open: NAME must be a string");

  hid_t aapl_id = get_h5_id (args, 2, "AAPL_ID", "H5A.open");

  hid_t attr_id =  H5Aopen (obj_id, name.c_str (), aapl_id);

  if (attr_id < 0)
    error ("H5A.open: unable open attribute");

  return ovl (octave_int64 (attr_id));
}

/*
%!fail ("H5A.open ()", "Invalid call");

%!fail ("H5A.open (123456789, 1, 1)", "NAME must be a string")

%!fail ("H5A.open (123456789, 'toto', 'toto')", "unknown AAPL_ID 'toto'")
*/
