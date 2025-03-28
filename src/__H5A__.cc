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
#include "./util/h5_data_util.h"

// PKG_ADD: autoload ("__H5A_close__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_close__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_close__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} { } H5A.close (@var{attr_id})\n\
Terminates access to the attribute specified by @var{attr_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Attribute to release access to\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.open}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.close");

  // Attribute ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.close", false);

  if (H5Aclose (attr_id) < 0)
    error ("H5A.close: unable to close attribute");

  return retval;
}

/*
%!test
%! fail ("H5A.close ()", "Invalid call");

%!test
%! fail ("H5E.set_auto (false); H5A.close (123456789); H5E.set_auto (true)", "unable to close attribute");
*/

// PKG_ADD: autoload ("__H5A_create__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_create__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id})\n\
@deftypefnx {oct-hdf5} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id}, @var{aapl_id},)\n\
Create an attribute, @var{attr_name}, which is attached to the object\n\
specified by the identifier @var{loc_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Location or object identifier\n\
 @item @var{attr_name} @tab @tab Attribute name\n\
 @item @var{space_id} @tab @tab Attribute dataspace identifier\n\
 @item @var{acpl_id} @tab @tab Attribute creation property list identifier\n\
 @item @var{aapl_id} @tab @tab Attribute access property list identifier\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.open,H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin < 4 || nargin > 6)
    print_usage ("H5A.create");

  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.create", false);

  std::string attr_name
    = args(1).xstring_value ("H5A.create: ATTR_NAME must be a string");

  hid_t type_id = get_h5_id (args, 2, "TYPE_ID", "H5A.create");

  hid_t space_id = get_h5_id (args, 3, "SPACE_ID", "H5A.create");

  hid_t acpl_id = H5P_DEFAULT;
  if (nargin > 4)
    acpl_id = get_h5_id (args, 4, "ACPL_ID", "H5A.create");

  hid_t aapl_id = H5P_DEFAULT;
  if (nargin > 5)
    aapl_id = get_h5_id (args, 5, "AAPL_ID", "H5A.create");

  hid_t attribute_id =  H5Acreate2 (loc_id, attr_name.c_str (), type_id,
                                   space_id, acpl_id, aapl_id);

  if (attribute_id < 0)
    error ("H5A.create: unable to create attribute");

  return ovl (octave_int64 (attribute_id));
}

/*
%!fail ("H5A.create ()", "Invalid call");

%!fail ("H5A.create (123456789, 1, 1, 1, 1)", "ATTR_NAME must be a string");

%!fail ("H5E.set_auto (false); H5A.create (123456789, 'att', 1, 1, 1); H5E.set_auto (true)", "unable to create attribute");

%!fail ("H5A.create (123456789, 'att', 'blob', 1, 1)", "unknown TYPE_ID 'blob'");
*/

// PKG_ADD: autoload ("__H5A_get_num_attrs__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_get_num_attrs__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_get_num_attrs__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{attr_count} =} H5A.get_num_attrs (@var{loc_id})\n\
Return the number of attributes if successful; otherwise return a negative \
value.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Identifier of a group, dataset, or named \
datatype\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_num_attrs");

  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.get_num_attrs", false);

  int attr_count = H5Aget_num_attrs (loc_id);

  if (attr_count < 0)
    error ("H5A.get_num_attr: unable determine number of attributes");

  return ovl (octave_int64 (attr_count));
}


/*
%!test
%! fail ("H5A.get_num_attrs ()", "Invalid call");

%!test
%! fail ("H5A.get_num_attrs ('toto')", "LOC_ID must be a scalar numeric identifier");

%!test
%! fail ("H5E.set_auto (false); H5A.get_num_attrs (1); H5E.set_auto (true)", "unable determine number of attributes");
*/

// PKG_ADD: autoload ("__H5A_get_space__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_get_space__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_get_space__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{space_id} = } H5A.get_space (@var{attr_id})\n\
Retrieve a copy of the dataspace for an attribute @var{attr_id}.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Identifier of an attribute\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
The dataspace identifier returned from this function must be \
released with H5S.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.open}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_space");

  // Attribue ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.get_space", false);

  hid_t space_id = H5Aget_space (attr_id);

  if (space_id < 0)
    error ("H5A.get_space: unable to retrieve data space");

  return retval.append (octave_int64 (space_id));
}

/*
%!fail ("H5A.get_space ()", "Invalid call");

%!fail ("H5A.get_space ('toto')", "ATTR_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5A.get_space (1); H5E.set_auto (true)", "unable to retrieve data space");
*/

// PKG_ADD: autoload ("__H5A_get_type__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_get_type__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_get_type__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{type_id} = } H5A.get_type (@var{attr_id})\n\
Return a datatype identifier if successful; otherwise returns a negative \
value.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Identifier of an attribute\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
The datatype identifier @var{type_id} returned from this function must\n\
be released with H5T.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5A.get_type");

  // Attribue ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.get_type", false);

  hid_t type_id = H5Aget_type (attr_id);

  if (type_id < 0)
    error ("H5A.get_type: unable to retrieve data type");

  return retval.append (octave_int64 (type_id));
}

/*
%!fail ("H5A.get_type ()", "Invalid call");

%!fail ("H5A.get_type ('toto')", "ATTR_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5A.get_type (1); H5E.set_auto (true)", "unable to retrieve data type");
*/

// PKG_ADD: autoload ("__H5A_iterate__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_iterate__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_iterate__, args, nargout, 
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

// PKG_ADD: autoload ("__H5A_iterate_by_name__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_iterate_by_name__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_iterate_by_name__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5A.iterate_by_name (@var{loc_id}, @var{name}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in}, @var{lapd_id})\n\
Iterate over the attributes attached to the dataset or group specified\n\
with @var{loc_id} and @var{obj_name}.\n\
\n\
For each attribute, user-provided data, @var{op_data}, with additional\n\
information as defined below, is passed to a user-defined function, \n\
@var{fcn}, which operates on that attribute.\n\
\n\
If @var{loc_id} fully specifies the object to which these attributes are\n\
attached, @var{obj_name} should be '.' (a dot).\n\
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
The link access property list, @var{lapl_id}, may provide information\n\
regarding the properties of links required to access the object,\n\
@var{obj_name}.  See 'Link Access Properties' in the H5P APIs.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
@seealso{H5A.iterate}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 8)
    print_usage ("H5A.iterate_by_name");

  // Group ID
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.iterate_by_name");

  // Name
  std::string name = args(1).xstring_value ("H5A.iterate_by_name: NAME must be a string");

  // Index type
  H5_index_t idx_type
    = static_cast<H5_index_t> (get_h5_id (args, 2, "IDX_TYPE", "H5A.iterate_by_name"));

  // Order
  H5_iter_order_t order
    = static_cast<H5_iter_order_t> (get_h5_id (args, 3, "ORDER",
                                               "H5A.iterate_by_name"));

  // Iteration index
  hsize_t idx_in
    = static_cast<hsize_t> (get_h5_id (args, 4, "ORDER", "H5A.iterate_by_name"), false);

  // Iteration handler callback
  if (! args(5).is_function_handle ())
    error ("H5A.iterate_by_name: FCN must be a function handle");

  user_callback_data ucd;
  ucd.callback = args(5);
  ucd.data = args(6);

  // Link access plist ID
  hid_t lapd_id = get_h5_id (args, 7, "LAPD_ID", "H5A.iterate_by_name");

  herr_t status = H5Aiterate_by_name (loc_id, name.c_str (), idx_type,
                                      order, &idx_in, A_iter_handler,
                                      &ucd, lapd_id);

  retval.append (octave_value (status));
  retval.append (octave_value (idx_in));
  retval.append (ucd.data);

  return retval;
}


/*
%!test
%! fail ("H5A.iterate_by_name ()", "Invalid call");

%!test
%! fail ("H5A.iterate_by_name (123456789, 1, 1, 1, 1, 1, 1, 1)", "NAME must be a string")

%!test
%! fail ("H5A.iterate_by_name (123456789, 'toto', 1, 1, 1, 1, 1, 1)", "FCN must be a function handle")
*/

// PKG_ADD: autoload ("__H5A_open__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_open__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_open__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{attr_id} =} H5A.open (@var{obj_id}, @var{name}, @var{aapl_id})\n\
Open an existing attribute, @var{name}, that is attached to an object\n\
specified by an object identifier, @var{obj_id}.\n\
\n\
The attribute access property list, @var{aapl_id}, is currently unused\n\
and should be @qcode{'H5P_DEFAULT'}.\n\
\n\
This function, must be called before an attribute can be accessed for any\n\
further purpose, including reading, writing, or any modification.\n\
\n\
The attribute identifier @var{attr_id} returned by this function must be\n\
released with H5A.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
@seealso{H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5A.open");

  // Parse arguments
  hid_t obj_id = get_h5_id (args, 0, "OBJ_ID", "H5A.open");

  std::string name = args(1).xstring_value ("H5A.open: NAME must be a string");

  hid_t aapl_id = get_h5_id (args, 2, "AAPL_ID", "H5A.open");

  hid_t attr_id =  H5Aopen (obj_id, name.c_str (), aapl_id);

  if (attr_id < 0)
    error ("H5A.open: unable open attribute");

  return ovl (octave_int64 (attr_id));
}

/*
%!fail ("H5A.open ()", "Invalid call");

%!fail ("H5A.open (123456789, 1, 1)", "NAME must be a string")

%!fail ("H5A.open (123456789, 'toto', 'toto')", "unknown AAPL_ID 'toto'")
*/

// PKG_ADD: autoload ("__H5A_open_name__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_open_name__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_open_name__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{attr_id} =} H5A.open_name (@var{loc_id}, @var{name})\n\
Open an attribute specified by its name, @var{name}, which is attached to\n\
the object specified with @var{loc_id}. \n\
\n\
The location object may be either a group, dataset, or named datatype,\n\
which may have any sort of attribute.\n\
\n\
The attribute identifier @var{attr_id} returned by this function must be\n\
released with H5A.close or resource leaks will develop.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5A.open_name");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5A.open_name");

  std::string name = args(1).xstring_value ("H5A.open_name: NAME must be a string");

  hid_t attr_id =  H5Aopen_name (loc_id, name.c_str ());

  if (attr_id < 0)
    error ("H5A.open_name: unable open attribute");

  return ovl (octave_int64 (attr_id));
}

// PKG_ADD: autoload ("__H5A_read__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_read__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_read__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{data} = } H5A.read (@var{attr_id})\n\
@deftypefnx {} {@var{data} = } H5A.read (@var{attr_id}, @var{mem_type_id})\n\
Read an attribute specified with @var{attr_id}.\n\
\n\
Import data from dataset.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{attr_id} @tab @tab Location or attribute identifier\n\
 @item @var{mem_type_id} @tab @tab Target datatype (use @code{H5ML_DEFAULT} \
for automatic conversion)\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
Only data with atomic HDF5 data types are handled. See @code{help H5D.read} \
for details about automatic data type conversion.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5D.read}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1 && nargin != 2)
    print_usage ("H5A.read");

  // Attribute ID
  hid_t attr_id = get_h5_id (args, 0, "ATTR_ID", "H5A.read", false);

  // Data buffer memory type ID
  hid_t mem_type_id = H5Aget_type (attr_id);

  if (nargin > 1
      && (! args(1).is_string ()
          || args(1).string_value () != "H5ML_DEFAULT"))
        mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5A.read");

  // Get output dimensions
  dim_vector dv;
  hid_t space_id = H5Aget_space (attr_id);
  dv = get_dim_vector (space_id);

  if (dv.ndims () == 0)
    retval = ovl (Matrix ());
  else
    retval.append (__h5_read__ ("H5A.read", dv, attr_id, mem_type_id));

  return retval;
}


/*
%!test
%! fail ("H5A.read ()", "Invalid call");

%!test
%! fail ("H5A.read (1, 2, 3)", "Invalid call");

%!test
%! fail ("H5E.set_auto (false); data = H5A.read (123456789, 'toto'); H5E.set_auto (true)", "unknown MEM_TYPE_ID 'toto'");
*/

// PKG_ADD: autoload ("__H5A_write__", "__H5A__.oct");
// PKG_DEL: autoload ("__H5A_write__", "__H5A__.oct", "remove");
DEFUN_DLD(__H5A_write__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {} H5A.write (@var{attribute_id}, @var{mem_type_id}, @var{data})\n\
Write an attribute, specified with @var{attribute_id}.\n\
\n\
The attribute's memory datatype is specified with @var{mem_type_id}.\n\
The entire attribute is written to the file.\n\
\n\
If @var{mem_type_id} is either a fixed-length or variable-length string,\n\
it is important to set the string length when defining the datatype.\n\
String datatypes are derived from @qcode{'H5T_C_S1'} (or\n\
@qcode{'H5T_FORTRAN_S1'} for Fortran), which defaults to 1 character\n\
in size. See @code{H5T.set_size}.\n\
\n\
Datatype conversion takes place at the time of a read or write and is\n\
automatic.\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.\n\
\n\
@seealso{H5A.read}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5A.write");

  // Attribute ID
  hid_t attribute_id = get_h5_id (args, 0, "ATTRIBUTE_ID", "H5A.write", false);

  // Data buffer memory type ID
  hid_t mem_type_id;

  if (args(1).is_string () && args(1).string_value () == "H5ML_DEFAULT")
    mem_type_id = -1234;
  else
    mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5A.write");

  // Write data
  __h5write__ ("H5A.write", args(2), attribute_id, mem_type_id);

  return retval;
}

/*

%!test
%! h5ex_t_stringatt ()

%!test
%! h5ex_t_vlstringatt ()

*/
