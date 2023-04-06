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

DEFUN_DLD(open, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {} {@var{dataset_id} =} H5D.open (@var{loc_id}, @var{name}, @var{dapl_id})\n\
Open the existing dataset specified by a location identifier and name,\n\
@var{loc_id} and @var{name}, respectively.\n\
\n\
@var{loc_id} may be a file,\n\
group, dataset, named datatype or attribute.  If an attribute, dataset,\n\
or named datatype is specified for @var{loc_id} then the dataset will be\n\
opened at the location where the attribute, dataset, or named datatype\n\
is attached.\n\
\n\
The dataset access property list, @var{dapl_id}, provides information\n\
regarding access to the dataset.\n\
\n\
To conserve and release resources, the dataset should be closed when access\n\
is no longer required.\n\
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
