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
#include "../+H5ML/h5_data_util.h"


DEFUN_DLD(write, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {} H5D.write (@var{dataset_id}, @var{mem_type_id}, @var{mem_space_id}, @var{file_space_id}, @var{xfer_plist_id}, @var{data})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 6)
    print_usage ("H5D.write");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.write", false);

  // Data buffer memory type ID
  hid_t mem_type_id;

  if (args(1).is_string () && args(1).string_value () == "H5ML_DEFAULT")
    mem_type_id = -1234;
  else
    mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5D.write");

  // Data buffer space selection ID
  hid_t mem_space_id = get_h5_id (args, 2, "MEM_SPACE_ID", "H5D.write");

  // File space selection ID
  hid_t file_space_id = get_h5_id (args, 3, "FILE_SPACE_ID", "H5D.write");

  // Transfer plist ID
  hid_t xfer_plist_id = get_h5_id (args, 4, "XFER_PLIST_ID", "H5D.write");

  // Write data
  __h5write__ ("H5D.write", args(5), dataset_id, mem_type_id, mem_space_id,
               file_space_id, xfer_plist_id);

  return retval;
}
