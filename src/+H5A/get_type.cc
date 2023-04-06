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

#include "../util/h5_oct_util.h"

DEFUN_DLD(get_type, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} = } H5A.get_type (@var{attr_id})\n\
Retrieve a copy of the datatype for an attribute @var{attr_id}.\n\
\n\
The datatype is reopened if it is a named type before returning it\n\
to the application.  The datatypes returned by this function are always\n\
read-only.  If an error occurs when atomizing the return datatype,\n\
then the datatype is closed.\n\
\n\
The datatype identifier @var{type_id} returned from this function must\n\
be released with H5T.close or resource leaks will develop.\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_type");

  // Attribue ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.get_type", false);

  hid_t type_id = H5Aget_type (attr_id);

  if (type_id < 0)
    error ("H5A.get_type: unable retrieve data type");

  return retval.append (octave_int64 (type_id));
}
