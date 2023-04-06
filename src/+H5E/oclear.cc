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

DEFUN_DLD(oclear, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5E.oclear ()\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 0)
    print_usage ("H5E.oclear");


  hid_t estack_id = H5Eget_current_stack ();

  H5Eclear (estack_id);

  H5Eclose_stack (estack_id);

  return retval;
}
