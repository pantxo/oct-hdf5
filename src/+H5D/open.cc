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
@deftypefn {} {@var{dataset_id} =} H5D.open (@var{loc_id}, @var{name}, @var{dapl_id})\n\
Open the existing dataset specified by a location identifier and name,\n\
@var{loc_id} and @var{name}, respectively.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Location identifier; may be a file, group, \
named datatype, dataset, or attribute\n\
 @item @var{name} @tab @tab Dataset name\n\
 @item @var{dapl_id} @tab @tab Dataset access property list\n\
\n\
\n\
@strong{Description:}\n\
\n\
The dataset identifier @var{dataset_id} returned from this function must\n\
be released with H5D.close or resource leaks will develop.\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_OPEN}.\n\
\n\
@seealso{H5D.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2 && nargin != 3)
    print_usage ("H5D.open");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5D.open");

  std::string name = args(1).xstring_value ("H5D.open: NAME must be a string");

  // Dataset access plist
  hid_t dapl_id = H5P_DEFAULT;
  if (nargin == 3)
    dapl_id = get_h5_id (args, 2, "DAPL_ID", "H5D.open");

  hid_t dataset_id =  H5Dopen (loc_id, name.c_str (), dapl_id);

  if (dataset_id < 0)
    error ("H5D.open: unable open dataset");

  return ovl (octave_int64 (dataset_id));
}

/*
%!fail ("H5D.open ()", "Invalid call");

%!fail ("H5D.open (123456789, 1, 1)", "NAME must be a string")

%!fail ("H5D.open (123456789, 'toto', 'toto')", "unknown DAPL_ID 'toto'")
*/
