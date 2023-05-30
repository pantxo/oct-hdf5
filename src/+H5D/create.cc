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

DEFUN_DLD(create, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{dataset_id} =} H5D.create (@var{loc_id}, @var{name}, @var{dtype_id}, @var{space_id}, @var{lcpl_id})\n\
@deftypefnx {} {@var{dataset_id} =} H5D.create (@var{loc_id}, @var{name}, @var{dtype_id}, @var{space_id}, @var{lcpl_id}, @var{dcpl_id}, @var{dapl_id})\n\
Create a dataset, @var{name}, which is attached to the object\n\
specified by the identifier @var{loc_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Location or object identifier\n\
 @item @var{name} @tab @tab Dataset name\n\
 @item @var{dtype_id} @tab @tab Datatype identifier\n\
 @item @var{space_id} @tab @tab Dataspace identifier\n\
 @item @var{lcpl_id} @tab @tab Link creation property list identifier\n\
 @item @var{dcpl_id} @tab @tab Dataset creation property list identifier\n\
 @item @var{dapl_id} @tab @tab Dataset access property list identifier\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_CREATE2}.\n\
\n\
@seealso{H5D.open,H5D.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 5 && nargin != 7)
    print_usage ("H5D.create");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5D.create");

  std::string name
    = args(1).xstring_value ("H5D.create: NAME must be a string");

  hid_t dtype_id = get_h5_id (args, 2, "DTYPE_ID", "H5D.create");

  hid_t space_id = get_h5_id (args, 3, "SPACE_ID", "H5D.create");

  hid_t lcpl_id = get_h5_id (args, 4, "LCPL_ID", "H5D.create");

  hid_t dcpl_id = H5P_DEFAULT;
  if (nargin == 7)
    dcpl_id = get_h5_id (args, 5, "DCPL_ID", "H5D.create");

  hid_t dapl_id = H5P_DEFAULT;
  if (nargin == 7)
    dapl_id = get_h5_id (args, 6, "DAPL_ID", "H5D.create");

  hid_t dataset_id =  H5Dcreate (loc_id, name.c_str (), dtype_id, space_id,
                                 lcpl_id, dcpl_id, dapl_id);

  if (dataset_id < 0)
    error ("H5D.create: unable to create dataset");

  return ovl (octave_int64 (dataset_id));
}

/*
%!fail ("H5D.create ()", "Invalid call")

%!fail ("H5D.create (1, 2)", "Invalid call")

%!fail ("H5D.create (1, 2, 3, 4, 5, 6, 7, 8)", "Invalid call")
*/
