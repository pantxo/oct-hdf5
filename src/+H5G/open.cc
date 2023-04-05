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

DEFUN_DLD(open, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{group_id} =} H5G.open (@var{loc_id}, @var{name}, @var{gapl_id})\n\
Open an existing group, @var{name}, at the location specified by\n\
 @var{loc_id}.\n\
\n\
@var{gapl_id} is the group access property list.\n\
\n\
H5G.open returns a group identifier for the group that was opened.\n\
This group identifier should be released by calling @code{H5G.close} when\n\
it is no longer needed.\n\
\n\
@seealso{H5G.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5G.open");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5G.open");

  std::string name = args(1).xstring_value ("H5G.open: NAME must be a string");

  hid_t gapl_id = get_h5_id (args, 2, "GAPL_ID", "H5G.open");

  hid_t group_id =  H5Gopen (loc_id, name.c_str (), gapl_id);

  if (group_id < 0)
    error ("H5G.open: unable open group");

  return ovl (octave_int64 (group_id));
}
