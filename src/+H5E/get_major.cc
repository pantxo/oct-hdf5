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

DEFUN_DLD(get_major, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{err_msg} =} H5E.get_major (@var{maj_num})\n\
@seealso{H5E.walk}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5E.get_major");

  H5E_major_t maj_num =
    static_cast<H5E_major_t> (args(0).xuint64_value ("H5E.get_major: MAJ_NUM  must an unsigned integer"));


  char* str = H5Eget_major (maj_num);

  if (str)
    {
      retval.append (std::string (str));
      std::free (str);
    }
  else
    retval.append (std::string ());

  return retval;
}
