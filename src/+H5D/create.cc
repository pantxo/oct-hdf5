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
Create a new dataset named @var{name} at the location specified by\n\
@var{loc_id}.\n\
\n\
The function associates constant and initial persistent properties with\n\
that dataset, including @var{dtype_id}, the datatype of each data element\n\
as stored in the file; @var{space_id}, the dataspace of the dataset;\n\
and other initial properties as defined in the dataset creation property\n\
and access property lists, @var{dcpl_id} and @var{dapl_id}, respectively.\n\
Once created, the dataset is opened for access.\n\
\n\
@var{loc_id} may be a file, group, dataset, named datatype or attribute.\n\
If an attribute, dataset, or named datatype is specified for @var{loc_id}\n\
then the dataset will be created at the location where the attribute,\n\
dataset, or named datatype is attached.  @var{name} may be either an\n\
absolute path in the file or a relative path from @var{loc_id} naming the\n\
dataset.\n\
\n\
If @var{dtype_id} is either a fixed-length or variable-length string,\n\
it is important to set the string length when defining the datatype.\n\
String datatypes are derived from @qcode{'H5T_C_S1'}\n\
(or @qcode{'H5T_FORTRAN_S1'} for Fortran codes), which defaults to 1\n\
character in size. See @qcode{H5T.set_size} function.\n\
\n\
If @var{dtype_id} is a committed datatype, and if the file location\n\
associated with the committed datatype is different from the file location\n\
where the dataset will be created, the datatype is copied and converted\n\
to a transient type.\n\
\n\
The link creation property list, @var{lcpl_id}, governs creation of the\n\
link(s) by which the new dataset is accessed and the creation of any\n\
intermediate groups that may be missing.\n\
\n\
The datatype and dataspace properties and the dataset creation and access\n\
property lists are attached to the dataset, so the caller may derive new\n\
datatypes, dataspaces, and creation and access properties from the old ones\n\
and reuse them in calls to create additional datasets.\n\
\n\
Once created, the dataset is ready to receive raw data. Immediately\n\
attempting to read raw data from the dataset will probably return the\n\
fill value.\n\
\n\
To conserve and release resources, the dataset should be closed when access\n\
is no longer required.\n\
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
