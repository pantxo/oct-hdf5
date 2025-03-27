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
// PKG_ADD: autoload ("__H5O_close__", "__H5O__.oct");
// PKG_DEL: autoload ("__H5O_close__", "__H5O__.oct", "remove");
DEFUN_DLD(__H5O_close__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {} H5O.close (@var{object_id})\n\
Closes the object, dataset, or named datatype specified by @var{object_id}.\n\
\n\
This function is the companion to @code{H5O.open}, and has the same\n\
effect as calling @code{H5G.close}, @code{H5O.close}, or @code{H5T.close}.\n\
\n\
@code{H5O.close} is not used to close a dataspace, attribute, property list,\n\
or file.\n\
\n\
@seealso{H5O.open}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5O.close");

  hid_t object_id = get_h5_id (args, 0, "OBJECT_ID", "H5O.close");

  if (H5Oclose (object_id) < 0)
    error ("H5O.close: unable close object");

  return retval;
}

// PKG_ADD: autoload ("__H5O_get_info_by_name__", "__H5O__.oct");
// PKG_DEL: autoload ("__H5O_get_info_by_name__", "__H5O__.oct", "remove");
DEFUN_DLD(__H5O_get_info_by_name__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{info_struct} =} H5O.get_info_by_name (@var{loc_id}, @var{name}, @var{lapl_id})\n\
The @var{info_struct} structure contains fields:\n\
fileno;    /* File number that object is located in \n\
addr;      /* Object address in file \n\
type;      /* Basic object type (group, dataset, etc.) \n\
rc;        /* Reference count of object    \n\
atime;     /* Access time            \n\
mtime;     /* Modification time      \n\
ctime;     /* Change time            \n\
btime;     /* Birth time         \n\
num_attrs; /* # of attributes attached to object \n\
\n\
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

// PKG_ADD: autoload ("__H5O_open__", "__H5O__.oct");
// PKG_DEL: autoload ("__H5O_open__", "__H5O__.oct", "remove");
DEFUN_DLD(__H5O_open__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{dataset_id} =} H5O.open (@var{loc_id}, @var{name}, @var{lapl_id})\n\
Open a group, dataset, or committed (named) datatype specified by a location, \
@var{loc_id}, and a path name, @var{name}\n\
\n\
@seealso{H5O.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2 && nargin != 3)
    print_usage ("H5O.open");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5O.open");

  std::string name = args(1).xstring_value ("H5O.open: NAME must be a string");

  // Dataset access plist
  hid_t lapl_id = H5P_DEFAULT;
  if (nargin == 3)
    lapl_id = get_h5_id (args, 2, "LAPL_ID", "H5O.open");

  hid_t dataset_id =  H5Oopen (loc_id, name.c_str (), lapl_id);

  if (dataset_id < 0)
    error ("H5O.open: unable to open object");

  return ovl (octave_int64 (dataset_id));
}


