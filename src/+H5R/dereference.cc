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

#include "../util/h5_oct_util.h"

DEFUN_DLD(dereference, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{ref_obj_id} = } H5R.dereference (@var{obj_id}, @var{ref_type}, @var{ref})\n\
Given a reference, @var{ref}, to an object or a region in an object,\n\
open that object and return an identifier.\n\
\n\
The parameter @var{obj_id} must be a valid identifier for the HDF5 file\n\
containing the referenced object or for any object in that HDF5 file.\n\
\n\
The parameter @var{ref_type} specifies the reference type of the reference\n\
@var{ref}. @var{ref_type} may contain either of the following values:\n\
@qcode{'H5R_OBJECT'} or @qcode{'H5R_DATASET_REGION'}.\n\
\n\
The object opened with this function should be closed when it is no longer\n\
needed so that resource leaks will not develop. Use the appropriate close\n\
function such as @code{H5O.close} or @code{H5D.close} for datasets.\n\
\n\
@seealso{H5O.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  bool is_1_10_syntax = nargin == 4;

  if (nargin != 3 && nargin != 4)
    print_usage ("H5R.dereference");

  size_t argnum = 0;

  // Object ID
  hid_t obj_id = get_h5_id (args, argnum++, "OBJ_ID", "H5R.dereference", false);

  // Object access property list identifier
  hid_t oapl_id = H5P_DEFAULT;
  if (is_1_10_syntax)
    {
      oapl_id = static_cast<H5R_type_t> (get_h5_id (args, argnum++,
                                                    "OAPL_ID",
                                                    "H5R.dereference"));
    }

  // Ref type
  H5R_type_t ref_type
    = static_cast<H5R_type_t> (get_h5_id (args, argnum++,
                                          "REF_TYPE", "H5R.dereference"));

  // Ref
  int64NDArray ref = args(argnum).xint64_array_value ("H5R.dereference: "
                                                      "REF must be a "
                                                      "reference");

  hid_t out_ref = H5Rdereference2 (obj_id, oapl_id,ref_type,
                                   ref.data ());

  return retval.append (octave_int64 (out_ref));
}
