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

DEFUN_DLD(set_cset, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} { } H5T.set_cset (@var{type_id}, @var{charset})\n\
@var{charset} must be @qstring{\"H5T_CSET_ASCII\"} \
or @qstring{\"H5T_CSET_UTF8\"}.\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5T.set_cset");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.set_cset", false);

  // Charset
  hid_t cset_id = get_h5_id (args, 1, "TYPE_ID", "H5T.set_cset", true);

  if (H5Tset_cset (type_id, static_cast<H5T_cset_t> (cset_id)) < 0)
    error ("H5T.set_cset: unable set specified charset");

  return retval;
}
