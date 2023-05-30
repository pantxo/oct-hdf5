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

DEFUN_DLD(get_create_plist, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{dcpl_id} = } H5D.get_create_plist (@var{dataset_id})\n\
Return an identifier for a copy of the dataset creation property list\n\
associated with the dataset specified by @var{dataset_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Dataset identifier\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_CREATE_PLIST}.\n\
\n\
@seealso{H5P.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.get_create_plist");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.get_create_plist");

  hid_t dcpl_id = H5Dget_create_plist (dataset_id);

  if (dcpl_id < 0)
    error ("H5D.get_create_plist: unable to retrieve creation property list");

  return retval.append (octave_int64 (dcpl_id));
}

/*
%!fail ("H5D.get_create_plist ()", "Invalid call");

%!fail ("H5E.set_auto (false);H5D.get_create_plist (-123456);H5E.set_auto (true);", "unable to retrieve creation property list");
*/
