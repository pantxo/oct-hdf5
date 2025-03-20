/*

Copyright (C) 2025 Pantxo Diribarne

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

DEFUN_DLD(insert, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {} H5T.insert (@var{parent_id}, @var{name}, @var{offset}, @var{member_id})\n\
Adds a new member to a compound datatype.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{parent_id} @tab @tab Datatype identifier\n\
 @item @var{name} @tab @tab Name of the field to insert\n\
 @item @var{offset} @tab @tab Offset in memory structure of the field to insert\n\
 @item @var{member_id} @tab @tab Datatype identifier of the field to insert\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___c_o_m_p_o_u_n_d.html}.\n\
\n\
@seealso{H5T.open,H5T.close}\n\
@end deftypefn")
{
  int nargin = args.length ();

  if (nargin != 4)
    print_usage ("H5T.insert");

  // parent_id
  hid_t parent_id = get_h5_id (args, 0, "PARENT_ID", "H5T.insert", false);

  // name
  std::string name = args(1).xstring_value ("H5T.insert: NAME must be a string");

  // offset
  size_t offset = args(2).xuint64_value ("H5T.insert: OFFSET must be a numeric");
  // member_id
  hid_t member_id = get_h5_id (args, 3, "MEMBER_ID", "H5T.insert");
  
  herr_t err =  H5Tinsert (parent_id, name.c_str (), offset, member_id);

  if (err < 0)
    error ("H5T.insert: unable to insert member");

  return ovl ();
}

/*
%!fail ("H5T.insert ()", "Invalid call");
*/
