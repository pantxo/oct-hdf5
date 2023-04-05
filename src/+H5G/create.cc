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
@deftypefn {} {@var{group_id} =} H5G.create (@var{loc_id}, @var{name})\n\
@deftypefnx {} {@var{group_id} =} H5G.create (@var{loc_id}, @var{name}, @var{lcpl_id}, @var{gcpl_id}, @var{gapl_id})\n\
Create a new group in a file.\n\
After a group has been created, links to datasets and to other groups can\n\
be added.\n\
\n\
The @var{loc_id} and @var{name} parameters specify where the group is\n\
located. @var{loc_id} may be a file, group, dataset, named datatype or\n\
attribute in the file.  If an attribute, dataset, or named datatype is\n\
specified for @var{loc_id} then the group will be created at the location\n\
where the attribute, dataset, or named datatype is attached.\n\
@var{name} is the link to the group; @var{name} may be either an absolute\n\
path in the file (the links from the root group to the new group) or a\n\
relative path from @var{loc_id} (the link(s) from the group specified by\n\
@var{loc_id} to the new group).\n\
\n\
@var{lcpl_id}, @var{gcpl_id}, and @var{gapl_id} (optional) are property list\n\
identifiers. These property lists govern how the link to the group is\n\
created, how the group is created, and how the group can be accessed in\n\
the future, respectively. The default is @qcode{'H5P_DEFAULT'}.\n\
Currently, there are no APIs for the group access property list;\n\
use @qcode{'H5P_DEFAULT'}.\n\
\n\
To conserve and release resources, the group should be closed when access\n\
is no longer required.\n\
\n\
@seealso{H5G.open,H5G.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin < 2 && nargin > 5)
    print_usage ("H5G.create");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5G.create");

  std::string name
    = args(1).xstring_value ("H5G.create: NAME must be a string");

  hid_t lcpl_id = H5P_DEFAULT;
  if (nargin > 2)
    lcpl_id = get_h5_id (args, 2, "LCPL_ID", "H5G.create");

  hid_t gcpl_id = H5P_DEFAULT;
  if (nargin > 3)
    lcpl_id = get_h5_id (args, 3, "GCPL_ID", "H5G.create");

  hid_t gapl_id = H5P_DEFAULT;
  if (nargin > 4)
    lcpl_id = get_h5_id (args, 4, "GAPL_ID", "H5G.create");

  hid_t group_id =  H5Gcreate (loc_id, name.c_str (), lcpl_id, gcpl_id,
                               gapl_id);

  if (group_id < 0)
    error ("H5G.create: unable to create group");

  return ovl (octave_int64 (group_id));
}
