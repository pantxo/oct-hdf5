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

DEFUN_DLD(get_msg, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{msg} =} H5E.get_msg (@var{msg_id})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.get_msg");

  // Message id as returned in error structs for major_num and minor_num
  hid_t msg_id = get_h5_id (args, 0, "MSG_ID", "H5E.get_msg", false);

  size_t sz = 1024;
  char msg[sz];
  H5E_type_t type;

  ssize_t nret = H5Eget_msg (msg_id, &type, msg, sz);

  printf ("ID is %ld, nret is %ld, msg is %s\n", msg_id, nret, msg);

  if (nret < 0)
    error ("H5E.get_msg: unable to retrieve message for id %ld", msg_id);

  return retval.append (std::string (msg));
}
