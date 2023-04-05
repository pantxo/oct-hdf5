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

DEFUN_DLD(create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{ref} =} H5R.create (@var{loc_id}, @var{name}, @var{ref_type}, @var{space_id})\n\
Create the reference, @var{ref}, of the type specified in @var{ref_type},\n\
pointing to the object name located at @var{loc_id}.\n\
\n\
@var{ref_type} must be one of @qcode{'H5R_OBJECT'} or\n\
@qcode{'H5R_DATASET_REGION'}.\n\
\n\
The parameters @var{loc_id} and name are used to locate the object.\n\
\n\
The parameter @var{space_id} identifies the dataset region that a dataset\n\
region reference points to. This parameter is used only with dataset region\n\
references and should be set to -1 if the reference is an object reference,\n\
@qcode{'H5R_OBJECT'}.\n\
\n\
@seealso{H5R.open,H5R.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 4)
    print_usage ("H5R.create");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5R.create");

  std::string name
    = args(1).xstring_value ("H5R.create: NAME must be a string");

  H5R_type_t ref_type =  static_cast<H5R_type_t> (get_h5_id (args, 2, "REF_TYPE", "H5R.create"));

  hid_t space_id = get_h5_id (args, 3, "SPACE_ID", "H5R.create");

  if (ref_type == H5R_OBJECT)
    {
      hobj_ref_t ref;

      if (H5Rcreate (&ref, loc_id, name.c_str (), ref_type, space_id) < 0)
        error ("H5R.create: unable to create reference");

      retval.append (octave_int64 (ref));
    }
  else if (ref_type == H5R_DATASET_REGION)
    {
      hdset_reg_ref_t ref;

      if (H5Rcreate (&ref, loc_id, name.c_str (), ref_type, space_id) < 0)
        error ("H5R.create: unable to create reference");

      int nref = sizeof (haddr_t) + 4;

      int64NDArray out(dim_vector (1,nref));

      for (int ii = 0; ii < nref; ii++)
        out(ii) = ref[ii];

      retval.append (out);
    }
  else
    error ("H5R.create: unknown REF_TYPE");

  return retval;
}
