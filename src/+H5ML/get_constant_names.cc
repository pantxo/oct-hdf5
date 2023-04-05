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

#include "h5_oct_util.h"

DEFUN_DLD(get_constant_names, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{names} =} H5ML.get_constant_names ()\n\
@seealso{}\n\
@end deftypefn")
{
  int nargin = args.length ();

  if (nargin != 0)
    print_usage ("H5ML.get_constant_names");

  std::map<std::string, hid_t> h5_oct_constants = get_constants_map ();

  size_t nel = h5_oct_constants.size ();

  static Cell names (dim_vector (1, nel));

  if (! names.isempty ())
    {
      octave_idx_type ii = 0;
      for (auto it = h5_oct_constants.begin (); it != h5_oct_constants.end ();
           it ++)
        names(ii++) = it->first;
    }

  return ovl (names);
}
