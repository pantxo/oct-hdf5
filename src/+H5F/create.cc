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
#include <hdf5/serial/hdf5.h>

#include "../util/h5_oct_util.h"

DEFUN_DLD(create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{fid} =} H5F.create (@var{fname})\n\
@deftypefnx {} {@var{fid} =} H5F.create (@var{fname}, @var{flags})\n\
@deftypefnx {} {@var{fid} =} H5F.create (@var{fname}, @var{flags}, @var{fcpl_id})\n\
@deftypefnx {} {@var{fid} =} H5F.create (@var{fname}, @var{flags}, @var{fcpl_id}, @var{fapl_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin < 1)
    print_usage ("H5F.create");

  // File name
  std::string fname =  args(0).xstring_value ("FNAME must be a string");

  // File access flags
  int flags = H5F_ACC_TRUNC;

  if (nargin > 1)
    flags = get_h5_id (args, 1, "FLAGS", "H5F.create");

  // File creation property list
  hid_t fcpl_id = H5P_DEFAULT;

  if (nargin > 2)
    fcpl_id = get_h5_id (args, 2, "FCPL_ID", "H5F.create");

  // File acess property list
  hid_t fapl_id = H5P_DEFAULT;

  if (nargin > 3)
    fapl_id = get_h5_id (args, 3, "FAPL_ID", "H5F.create");

  hid_t file_id = H5Fcreate (fname.c_str (), flags, fcpl_id, fapl_id);

  if (file_id < 0)
    error ("H5F.create: unable to create file");

  return ovl (octave_int64 (file_id));
}
