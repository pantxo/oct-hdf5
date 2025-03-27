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
// PKG_ADD: autoload ("__H5ML_compare_values__", "__H5ML__.oct");
// PKG_DEL: autoload ("__H5ML_compare_values__", "__H5ML__.oct", "remove");
DEFUN_DLD(__H5ML_compare_values__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{tf} =} H5ML.compare_values (@var{val1}, @var{val2})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5ML.compare_values");

  hid_t val1 = get_h5_id (args, 0, "VAL1", "H5ML.compare_values");
  hid_t val2 = get_h5_id (args, 1, "VAL2", "H5ML.compare_values");

  return retval.append (val1 == val2);
}

// PKG_ADD: autoload ("__H5ML_get_constant_names__", "__H5ML__.oct");
// PKG_DEL: autoload ("__H5ML_get_constant_names__", "__H5ML__.oct", "remove");
DEFUN_DLD(__H5ML_get_constant_names__, args, nargout, 
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

// PKG_ADD: autoload ("__H5ML_get_constant_value__", "__H5ML__.oct");
// PKG_DEL: autoload ("__H5ML_get_constant_value__", "__H5ML__.oct", "remove");
DEFUN_DLD(__H5ML_get_constant_value__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{retval} =} H5ML.get_constant_value (@var{name})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5ML.get_constant_value");

  std::string name = args(0).xstring_value ("H5ML.get_constant_value:NAME must be a string");

  std::map<std::string, hid_t> h5_oct_constants = get_constants_map ();

  auto it = h5_oct_constants.find (name);

  if (it != h5_oct_constants.end ())
    retval.append (octave_int64 (it->second));
  else
    retval.append (Matrix (0,1));

  return retval;
}


