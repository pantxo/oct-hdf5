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
@seealso{}\n\
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
