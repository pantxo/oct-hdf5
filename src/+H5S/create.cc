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

DEFUN_DLD(create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{space_id} =} create (@var{type})\n\
\n\
Create a new dataspace of a particular type.\n\
\n\
Currently supported types are as follows: @qcode{'H5S_SCALAR'},\n\
@qcode{'H5S_SIMPLE'}, and @qcode{'H5S_NULL'}. Further dataspace types\n\
may be added later.\n\
\n\
A scalar dataspace, @qcode{'H5S_SCALAR'}, has a single element,\n\
though that element may be of a complex datatype, such as a compound\n\
or array datatype. By convention, the rank of a scalar dataspace is\n\
always 0 (zero); think of it geometrically as a single, dimensionless point,\n\
though that point can be complex.\n\
\n\
A simple dataspace, @var{H5S_SIMPLE}, consists of a regular array\n\
of elements.\n\
\n\
A null dataspace, @var{H5S_NULL}, has no data elements.\n\
\n\
The dataspace identifier returned by this function can be released\n\
with @code{H5S.close} so that resource leaks will not occur.\n\
@seealso{H5S.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ();

  // Type
  H5S_class_t type =  static_cast<H5S_class_t> (get_h5_id (args, 0, "TYPE", "H5S.create"));

  hid_t sid = H5Screate (type);

  if (sid < 0)
    error ("H5S.create: unable to create dataspace");

  return ovl (octave_int64 (sid));
}
