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

DEFUN_DLD(get_obj_type, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{obj_type} = } H5R.get_obj_type (@var{loc_id}, @var{ref_type}, @var{ref})\n\
\n\
@seealso{H5O.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5R.get_obj_type");

  // Object ID
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5R.get_obj_type", false);

  // Ref type
  H5R_type_t ref_type
    = static_cast<H5R_type_t> (get_h5_id (args, 1,
                                          "REF_TYPE", "H5R.get_obj_type"));

  // Ref
  hobj_ref_t ref
    = static_cast<hobj_ref_t> (args(2).xint64_scalar_value ("H5R.get_obj_type: REF must be a reference"));


  H5O_type_t obj_type;

  if (H5Rget_obj_type2 (loc_id, ref_type, &ref, &obj_type) < 0)
    error ("H5R.get_obj_type: unable to retrieve object type");

  return retval.append (octave_int64 (obj_type));
}
