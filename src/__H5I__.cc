/*

Copyright (C) 2021-2025 Pantxo Diribarne

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

#include "./util/h5_oct_util.h"

// PKG_ADD: autoload ("__H5I_get_name__", "__H5I__.oct");
// PKG_DEL: autoload ("__H5I_get_name__", "__H5I__.oct", "remove");
DEFUN_DLD(__H5I_get_name__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{info_struct} =} H5I.get_name (@var{obj_id})\n\
Retrieve a name for the object identified by @var{obj_id}.\n\
\n\
@seealso{}\n\
@end deftypefn")
{
  std::string retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5I.get_name");

  // Objext ID
  hid_t obj_id = get_h5_id (args, 0, "OBJ_ID", "H5I.get_name");

  ssize_t slen = H5Iget_name (obj_id, nullptr, 0);

  if (slen > 0)
    {
      OCTAVE_LOCAL_BUFFER (char, str, slen + 1);

      slen = H5Iget_name (obj_id, str, slen + 1);

      if (slen < 0)
        error ("H5I.get_name: unable to get object name");
      else if (slen > 0)
        retval = std::string (str);
    }

  return ovl (retval);
}


