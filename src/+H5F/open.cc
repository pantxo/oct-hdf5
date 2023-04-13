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

DEFUN_DLD(open, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{fid} =} H5F.open (@var{fname})\n\
@deftypefnx {} {@var{fid} =} H5F.open (@var{fname}, @var{flags})\n\
@deftypefnx {} {@var{fid} =} H5F.open (@var{fname}, @var{flags}, @var{fapl_id})\n\
@seealso{H5F.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin < 1)
    print_usage ("H5F.open");

  // File name
  std::string fname =  args(0).xstring_value ("FNAME must be a string");

  // File access flags
  int flags = H5F_ACC_RDONLY;

  if (nargin > 1)
    flags = get_h5_id (args, 1, "FLAGS", "H5F.create");

  // File acess property list
  hid_t fapl_id = H5P_DEFAULT;

  if (nargin > 2)
    fapl_id = get_h5_id (args, 2, "FAPL_ID", "H5F.create");

  hid_t file_id = H5Fopen (fname.c_str (), flags, fapl_id);

  if (file_id < 0)
    error ("H5F.open: unable to open file");

  return ovl (octave_int64 (file_id));
}
