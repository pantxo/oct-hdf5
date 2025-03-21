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

DEFUN_DLD(iterate, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5A.iterate (@var{loc_id}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in})\n\
Iterate over the attributes attached to the dataset or group specified\n\
with @var{loc_id}.\n\
\n\
For each attribute, user-provided data, @var{op_data}, with additional\n\
information as defined below, is passed to a user-defined function, \n\
@var{fcn}, which operates on that attribute.\n\
\n\
The order of the iteration and the attributes iterated over are specified\n\
by three parameters: the index type, @var{idx_type}; the order in which\n\
the index is to be traversed, @var{order}; and the attribute’s position\n\
in the index, @var{idx_in}.\n\
\n\
The type of index specified by @var{idx_type} can be one of the following:\n\
@table @asis\n\
@item @qcode{'H5_INDEX_NAME'}\n\
An alpha-numeric index by attribute name.\n\
@item @qcode{'H5_INDEX_CRT_ORDER'}\n\
An index by creation order.\n\
@end table\n\
\n\
The order in which the index is to be traversed, as specified by @var{order},\n\
can be one of the following:\n\
@table @asis\n\
@item @qcode{'H5_ITER_INC'}\n\
Iteration is from beginning to end, i.e., a top-down iteration incrementing\n\
the index position at each step.\n\
@item @qcode{'H5_ITER_DEC'}\n\
Iteration starts at the end of the index, i.e., a bottom-up iteration\n\
decrementing the index position at each step.\n\
@item @qcode{'H5_ITER_NATIVE'}\n\
HDF5 iterates in the fastest-available order. No information is provided\n\
as to the order, but HDF5 ensures that each element in the index will be\n\
visited if the iteration completes successfully.\n\
@end table\n\
\n\
The next attribute to be operated on is specified by @var{idx_in},\n\
a position in the index. The index of the last attibute operated on is\n\
returned as @var{idx_out}\n\
\n\
For example, if @var{idx_type}, @var{order}, and @var{idx_in} are set to\n\
@qcode{'H5_INDEX_NAME'}, @qcode{'H5_ITER_INC'}, and @code{5}, respectively,\n\
the attribute in question is the fifth attribute from the beginning of the\n\
alpha-numeric index of attribute names.  If order were set to\n\
@qcode{'H5_ITER_DEC'}, it would be the fifth attribute from the end of\n\
the index.\n\
\n\
The prototype for the @var{fcn} callback function is as follows:\n\
@group\n\
@code{[@var{status}, @var{op_data_out}] = op_fcn (@var{location_id},\n\
@var{attr_name}, @var{op_data_in})}\n\
@end group\n\
\n\
The operation receives the location identifier @var{location_id} for the\n\
group or dataset being iterated over; the name of the current object\n\
dataset or attribute, @var{attr_name}; and the operator data passed into\n\
iterate function, @var{op_data_in}.\n\
\n\
Valid return values @var{status} from an operator and the resulting\n\
H5A.iterate_by_name and @var{fcn} behavior are as follows:\n\
@table @asis\n\
@item @code{@var{status} == 0}\n\
Causes the iterator to continue, returning zero when all attributes have\n\
been processed.\n\
@item @code{@var{status} > 0}\n\
Causes the iterator to immediately return that positive value, indicating\n\
short-circuit success. The iterator can be restarted at the next attribute,\n\
as indicated by the return value @var{idx_out}.\n\
@item @code{@var{status} < 0}\n\
Causes the iterator to immediately return that value, indicating failure.\n\
The iterator can be restarted at the next attribute, as indicated by the\n\
return value of @var{idx_out}.\n\
@end table\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.iterate_by_name}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 6)
    print_usage ("H5A.iterate");

  // Group ID
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.iterate");

  // Index type
  H5_index_t idx_type
    = static_cast<H5_index_t> (get_h5_id (args, 1, "IDX_TYPE", "H5A.iterate"));

  // Order
  H5_iter_order_t order
    = static_cast<H5_iter_order_t> (get_h5_id (args, 2, "ORDER",
                                               "H5A.iterate"));

  // Iteration index
  hsize_t idx_in
    = static_cast<hsize_t> (get_h5_id (args, 3, "ORDER", "H5A.iterate"), false);

  // Iteration handler callback
  if (! args(4).is_function_handle ())
    error ("H5A.iterate: FCN must be a function handle");

  user_callback_data ucd;
  ucd.callback = args(4);
  ucd.data = args(5);

  herr_t status = H5Aiterate (loc_id, idx_type, order, &idx_in, A_iter_handler,
                              &ucd);

  retval.append (octave_value (status));
  retval.append (octave_value (idx_in));
  retval.append (ucd.data);

  return retval;
}

/*
%!test
%! fail ("H5A.iterate ()", "Invalid call");

%!test
%! fail ("H5A.iterate (123456789, 1, 1, 1, 1, 1)", "FCN must be a function handle")
*/
