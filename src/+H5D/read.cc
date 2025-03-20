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
#include <hdf5.h>

#include "../util/h5_oct_util.h"
#include "../util/h5_data_util.h"

DEFUN_DLD(read, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{data} = } H5D.read (@var{dataset_id})\n\
@deftypefnx {} {@var{data} = } H5D.read (@var{dataset_id}, @var{mem_type_id}, @var{mem_space_id}, @var{file_space_id}, @var{xfer_plist_id})\n\
Import data from dataset.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Location or dataset identifier\n\
 @item @var{mem_type_id} @tab @tab Target datatype (use @code{H5ML_DEFAULT} \
for automatic conversion)\n\
 @item @var{mem_space_id} @tab @tab Imported data dataspace identifier or \
@code{H5S_ALL}\n\
 @item @var{file_space_id} @tab @tab Original data dataspace identifier or \
@code{H5S_ALL}\n\
 @item @var{xfer_plist_id} @tab @tab Transfer property list identifier or \
@code{H5P_DEFAULT}\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
Only data with atomic HDF5 data types are handled. Unless @var{mem_type_id} \
is specified or different from 'H5ML_DEFAULT', automatic conversion to \
Octave representation is as follows:\n\
@table @asis\n\
@item H5T_INTEGER\n\
Corresponding integer type, e.g. H5T_STD_I8BE in file is converted \
to H5T_NATIVE_INT8 in memory and returned as a @code{int8} class array.\n\
@item H5T_FLOAT\n\
Corresponding floating point type, e.g. H5T_IEEE_F32LE in file is converted \
to H5T_NATIVE_FLOAT in memory and returned as a @code{single} class array.\n\
@item H5T_STRING\n\
Returned as a char array.\n\
@item H5T_REFERENCE\n\
Returned as a @code{int64} class array containing the reference \
identifier(s).\n\
See @code{H5T.dereference} for how to retrieve the actual object identifier.\n\
@item H5T_COMPOUND\n\
Returned as a struct with fields corresponding to field names in the compound \
data type.\n\
@item H5T_VLEN\n\
Only H5T_STRING base types are handled. Returned as a char array.\n\
@end table\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_READ}.\n\
\n\
@seealso{H5D.open,H5A.read,H5T.dereference}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1 && nargin != 5)
    print_usage ("H5D.read");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.read", false);

  // Data buffer memory type ID
  hid_t mem_type_id = H5Dget_type (dataset_id);

  if (nargin > 1
      && (! args(1).is_string ()
          || args(1).string_value () != "H5ML_DEFAULT"))
        mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5D.read");

  // Data buffer space ID
  hid_t mem_space_id = H5S_ALL;

  if (nargin > 1)
    mem_space_id = get_h5_id (args, 2, "MEM_SPACE_ID", "H5D.read");

  // File space ID
  hid_t file_space_id = H5S_ALL;

  if (nargin > 1)
    file_space_id = get_h5_id (args, 3, "FILE_SPACE_ID", "H5D.read");

  // Transfer plist ID
  hid_t xfer_plist_id = H5P_DEFAULT;

  if (nargin > 1)
    xfer_plist_id = get_h5_id (args, 4, "XFER_PLIST_ID", "H5D.read");

  // Get output dimensions
  dim_vector dv;
  if (mem_space_id != H5S_ALL)
    error ("H5D.read: only H5S_ALL is implemented for MEM_SPACE_ID");

  if (file_space_id == H5S_ALL)
    file_space_id = H5Dget_space (dataset_id);

  dv = get_dim_vector (file_space_id);

  if (dv.ndims () == 0)
    retval = ovl (Matrix ());
  else
    retval.append (__h5_read__ ("H5D.read", dv, dataset_id, mem_type_id,
                                mem_space_id, file_space_id, xfer_plist_id));

  return retval;
}


/*
%!test
%! fail ("H5D.read ()", "Invalid call");

%!test
%! fail ("H5D.read (1, 2, 3)", "Invalid call");

%!test
%! fail ("H5D.read (1, 2, 3, 4, 5, 6)", "Invalid call");

%!test
%! h5ex_d_rdwr ('int8')

%!test
%! h5ex_d_rdwr ('int16')

%!test
%! h5ex_d_rdwr ('int32')

%!test
%! h5ex_d_rdwr ('int64')

%!test
%! h5ex_d_rdwr ('uint8')

%!test
%! h5ex_d_rdwr ('uint16')

%!test
%! h5ex_d_rdwr ('uint32')

%!test
%! h5ex_d_rdwr ('uint64')

%!test
%! h5ex_d_rdwr ('single')

%!test
%! h5ex_d_rdwr ('double')

%!test
%! f = H5F.open (file_in_loadpath ('h5ex_t_vlstring.h5'));
%! d = H5D.open (f, "/DS1");
%! t = H5D.read (d);
%! H5D.close (d);
%! H5F.close (f);
%! assert (t, {'Parting';'is such';'sweet';'sorrow.'})

*/
