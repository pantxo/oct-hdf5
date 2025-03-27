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
// PKG_ADD: autoload ("__H5P_close__", "__H5P__.oct");
// PKG_DEL: autoload ("__H5P_close__", "__H5P__.oct", "remove");
DEFUN_DLD(__H5P_close__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5P.close (@var{plist})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.close");

  // Property list ID
  hid_t plist_id = get_h5_id (args, 0, "PLIST_ID", "H5P.close", false);

  if (H5Pclose (plist_id) < 0)
    error ("H5P.close: unable to close property list");

  return retval;
}

// PKG_ADD: autoload ("__H5P_create__", "__H5P__.oct");
// PKG_DEL: autoload ("__H5P_create__", "__H5P__.oct", "remove");
DEFUN_DLD(__H5P_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{plist_id} =} H5P.create (@var{class_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.create");

  // Class ID
  hid_t class_id = get_h5_id (args, 0, "CLASS_ID", "H5P.create");

  hid_t plist_id = H5Pcreate (class_id);

  if (plist_id < 0)
    error ("H5P.create: unable to create property list");

  return ovl (octave_int64 (plist_id));
}

// PKG_ADD: autoload ("__H5P_fill_value_defined__", "__H5P__.oct");
// PKG_DEL: autoload ("__H5P_fill_value_defined__", "__H5P__.oct", "remove");
DEFUN_DLD(__H5P_fill_value_defined__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{fill_val_id} = } H5P.fill_value_defined (@var{plist_id})\n \
@seealso{H5D.get_create_plist}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.fill_value_defined");

  // Property list ID
  hid_t plist_id = get_h5_id (args, 0, "PLIST_ID", "H5P.fill_value_defined");

  H5D_fill_value_t fill_val_id;

  if (H5Pfill_value_defined (plist_id, &fill_val_id) < 0)
    error ("H5P.fill_value_defined: unable to get creation fill value id");

  return retval.append (octave_int64 (fill_val_id));
}

// PKG_ADD: autoload ("__H5P_get_chunk__", "__H5P__.oct");
// PKG_DEL: autoload ("__H5P_get_chunk__", "__H5P__.oct", "remove");
DEFUN_DLD(__H5P_get_chunk__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {[@var{rank, @var{dims}] = } H5P.get_chunk (@var{plist_id})\n\
@seealso{H5D.get_crreate_plist}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.get_chunk");

  // Property list ID
  hid_t plist_id = get_h5_id (args, 0, "PLIST_ID", "H5P.get_chunk");

  hsize_t dims[1024];

  int rank = H5Pget_chunk (plist_id, 1024, dims);

  if (rank < 0)
    error ("H5P.get_chunk: unable to get chunk dims");

  Matrix odims (1, rank, 0.0);

  for (int ii = 0; ii < rank; ii++)
    odims(ii) = dims[ii];

  retval.append (octave_value (rank));
  retval.append (odims);

  return retval;
}

// PKG_ADD: autoload ("__H5P_get_layout__", "__H5P__.oct");
// PKG_DEL: autoload ("__H5P_get_layout__", "__H5P__.oct", "remove");
DEFUN_DLD(__H5P_get_layout__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{layout_id} = } H5P.get_layout (@var{plist_id})\n \
@seealso{H5D.get_create_plist}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5P.get_layout");

  // Property list ID
  hid_t plist_id = get_h5_id (args, 0, "PLIST_ID", "H5P.get_layout");

  H5D_layout_t layout_id = H5Pget_layout (plist_id);

  if (layout_id < 0)
    error ("H5P.get_layout: unable to get creation layout");

  return retval.append (octave_int64 (layout_id));
}


