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

DEFUN_DLD(get_info_by_name, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{info_struct} =} H5O.get_info_by_name (@var{loc_id}, @var{name}, @var{lapl_id})\n\
The @var{info_struct} structure contains fields:\n\
unsigned long  fileno;    /* File number that object is located in \n\
addr;      /* Object address in file \n\
type;      /* Basic object type (group, dataset, etc.) \n\
rc;        /* Reference count of object    \n\
atime;     /* Access time            \n\
mtime;     /* Modification time      \n\
ctime;     /* Change time            \n\
btime;     /* Birth time         \n\
num_attrs; /* # of attributes attached to object \n\
(not implemented: H5O_hdr_info_t hdr;       /* Object header information )\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5O.get_info_by_name");

  // Location ID
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5O.get_info_by_name");

  // Object name
  std::string name
    = args(1).xstring_value ("H5O.get_info_by_name: NAME must be astring");

  // Link access property list
  hid_t lapl_id = get_h5_id (args, 2, "LAPL_ID", "H5O.get_info_by_name");

  // Infor structur output
  H5O_info_t oinfo;

  if (H5Oget_info_by_name1 (loc_id, name.c_str (), &oinfo, lapl_id) < 0)
    error ("H5O_info2_t *oinfo: unable to get object info");

  // Build ouput structure
  octave_scalar_map info_struct;
  info_struct.assign ("fileno", octave_int64 (oinfo.fileno));
  info_struct.assign ("addr", octave_int64 (oinfo.addr));
  info_struct.assign ("type", octave_int64 (oinfo.type));
  info_struct.assign ("rc", octave_int64 (oinfo.rc));
  info_struct.assign ("atime", octave_int64 (oinfo.atime));
  info_struct.assign ("mtime", octave_int64 (oinfo.mtime));
  info_struct.assign ("ctime", octave_int64 (oinfo.ctime));
  info_struct.assign ("btime", octave_int64 (oinfo.btime));
  info_struct.assign ("num_attrs", octave_int64 (oinfo.num_attrs));

  return ovl (info_struct);
}
