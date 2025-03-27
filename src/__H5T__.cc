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
// PKG_ADD: autoload ("__H5T_array_create__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_array_create__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_array_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} =} H5T.array_create (@var{base_type_id}, @var{dims})\n\
@deftypefnx {} {@var{type_id} =} H5T.array_create (@var{base_type_id}, @var{rank}, @var{dims})\n\
\n\
Create a new array datatype object.\n\
\n\
@var{base_type_id} is the datatype of every element of the array, i.e.,\n\
of the number at each position in the array.\n\
\n\
@var{rank} is the number of dimensions and the size of each dimension is\n\
specified in the array @var{dims}.  The value of rank is currently limited\n\
to @qcode{'H5S_MAX_RANK'} and must be greater than 0 (zero).  All dimension\n\
sizes specified in @var{dims} must be greater than 0 (zero).\n\
\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2 && nargin != 3 && nargin != 4)
    print_usage ("H5T.array_create");

  if (nargin == 4)
    // SKIP last argument
    nargin = 3;

  // Type ID
  hid_t base_type_id = get_h5_id (args, 0, "BASE_TYPE_ID", "H5T.array_create");

  //Rank
  unsigned rank = 0;
  if (nargin == 3)
    {
      rank = static_cast<unsigned> (args(1).xuint64_value ("H5T.array_create: RANK must be a numeric positive scalar"));
      if (rank == 0)
        error ("H5T.array_create: RANK must be a numeric positive scalar");
    }

  // Dims
  uint64NDArray tmp_dims;
  if (nargin == 2)
    {
      tmp_dims = args(1).xuint64_array_value ("H5T.array_create: DIMS must be a vector of positive values");
      rank = static_cast<unsigned> (tmp_dims.dims ().ndims ());

    }
  else
    {
      tmp_dims = args(2).xuint64_array_value ("H5T.array_create: DIMS must be a vector of positive values");
      if (static_cast<unsigned> (tmp_dims.dims ().ndims ()) != rank)
        error ("H5T.array_create: inconsistent RANK and DIMS arguments");
    }

  hsize_t dims[rank];
  for (unsigned ii = 0; ii < rank; ii++)
    dims[ii] = static_cast<hsize_t> (tmp_dims(ii));

  hid_t type_id = H5Tarray_create (base_type_id, rank, dims);

  if (type_id < 0)
    error ("H5T.array_create: unable to create array data type");

  return ovl (octave_int64 (type_id));
}

// PKG_ADD: autoload ("__H5T_close__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_close__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_close__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5T.close (@var{type_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.close");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.close", false);

  if (H5Tclose (type_id) < 0)
    error ("H5T.close: unable to close type object");

  return retval;
}

// PKG_ADD: autoload ("__H5T_copy__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_copy__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_copy__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} =} H5T.copy (@var{type_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.copy");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.copy");

  hid_t copied_type_id = H5Tcopy (type_id);

  if (copied_type_id < 0)
    error ("H5T.copy: unable to copy data type");

  return ovl (octave_int64 (copied_type_id));
}

// PKG_ADD: autoload ("__H5T_create__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_create__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {@var{type_id}} H5T.create (@var{class}, @var{size})\n\
Create a new datatype of the specified class with the specified number \
of bytes.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{class} @tab @tab Class of datatype among @qcode{'H5T_COMPOUND'}, \
@qcode{'H5T_OPAQUE'}, @qcode{'H5T_ENUM'}, @qcode{'H5T_STRING'}\n\
 @item @var{size} @tab @tab Size, in bytes, of the datatype being created\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5T_CREATE}.\n\
\n\
@seealso{H5T.open,H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5T.create");

  H5T_class_t cls =
    static_cast<H5T_class_t> (get_h5_id (args, 0, "CLASS", "H5T.create"));

  size_t sz =
    args(1).xuint64_value ("H5T.create: SIZE must be a numeric scalar");

  hid_t type_id =  H5Tcreate (cls, sz);

  if (type_id < 0)
    error ("H5T.create: unable to create type");

  return ovl (octave_int64 (type_id));
}

/*
%!fail ("H5T.create ()", "Invalid call");
*/

// PKG_ADD: autoload ("__H5T_get_class__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_get_class__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_get_class__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{class_id} =} H5T.get_class (@var{type_id})\n\
Get the class of the data type specified by @var{type_id}.\n\
\n\
The output can be @qcode{'H5T_INTEGER'}, @qcode{'H5T_FLOAT'},\n\
@qcode{'H5T_STRING'}, @qcode{'H5T_BITFIELD'}, @qcode{'H5T_OPAQUE'},\n\
@qcode{'H5T_COMPOUND'}, @qcode{'H5T_REFERENCE'}, @qcode{'H5T_ENUM'},\n\
@qcode{'H5T_VLEN'}, or @qcode{'H5T_ARRAY'}.\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.get_class");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_class");

  H5T_class_t class_id = H5Tget_class (type_id);

  if (class_id == H5T_NO_CLASS)
    error ("H5T.get_class: unable to retrieve data type class");

  return ovl (octave_int64 (class_id));
}

// PKG_ADD: autoload ("__H5T_get_order__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_get_order__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_get_order__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{byte_order} =} H5T.get_order (@var{type_id})\n\
Get the byte order of the data type specified by @var{type_id}.\n\
\n\
The output can be @qcode{'H5T_ORDER_LE'}, @qcode{'H5T_ORDER_BE'},\n\
@qcode{'H5T_ORDER_VAX'}, @qcode{'H5T_ORDER_MIXED'},\n\
or @qcode{'H5T_ORDER_NONE'}.\n\
@seealso{H5T.set_order}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.copy");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_order");

  H5T_order_t byte_order = H5Tget_order (type_id);

  if (byte_order == H5T_ORDER_ERROR)
    error ("H5T.get_order: unable to retrieve data type byte order");

  return ovl (octave_int64 (byte_order));
}

// PKG_ADD: autoload ("__H5T_get_size__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_get_size__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_get_size__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{sz} =} H5T.get_size (@var{type_id})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5T.get_size");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_size");

  size_t sz = H5Tget_size (type_id);

  if (sz == 0)
    error ("H5T.get_qsize: unable to retrieve data type size");

  return ovl (octave_int64 (sz));
}

// PKG_ADD: autoload ("__H5T_insert__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_insert__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_insert__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {oct-hdf5} {} H5T.insert (@var{parent_id}, @var{name}, @var{offset}, @var{member_id})\n\
Adds a new member to a compound datatype.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{parent_id} @tab @tab Datatype identifier\n\
 @item @var{name} @tab @tab Name of the field to insert\n\
 @item @var{offset} @tab @tab Offset in memory structure of the field to insert\n\
 @item @var{member_id} @tab @tab Datatype identifier of the field to insert\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://support.hdfgroup.org/documentation/hdf5/latest/group___c_o_m_p_o_u_n_d.html}.\n\
\n\
@seealso{H5T.open,H5T.close}\n\
@end deftypefn")
{
  int nargin = args.length ();

  if (nargin != 4)
    print_usage ("H5T.insert");

  // parent_id
  hid_t parent_id = get_h5_id (args, 0, "PARENT_ID", "H5T.insert", false);

  // name
  std::string name = args(1).xstring_value ("H5T.insert: NAME must be a string");

  // offset
  size_t offset = args(2).xuint64_value ("H5T.insert: OFFSET must be a numeric");
  // member_id
  hid_t member_id = get_h5_id (args, 3, "MEMBER_ID", "H5T.insert");
  
  herr_t err =  H5Tinsert (parent_id, name.c_str (), offset, member_id);

  if (err < 0)
    error ("H5T.insert: unable to insert member");

  return ovl ();
}

/*
%!fail ("H5T.insert ()", "Invalid call");
*/

// PKG_ADD: autoload ("__H5T_set_cset__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_set_cset__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_set_cset__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5T.set_cset (@var{type_id}, @var{charset})\n\
@var{charset} must be @qcode{\"H5T_CSET_ASCII\"} \
or @qcode{\"H5T_CSET_UTF8\"}.\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5T.set_cset");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.set_cset", false);

  // Charset
  hid_t cset_id = get_h5_id (args, 1, "TYPE_ID", "H5T.set_cset", true);

  if (H5Tset_cset (type_id, static_cast<H5T_cset_t> (cset_id)) < 0)
    error ("H5T.set_cset: unable set specified charset");

  return retval;
}

// PKG_ADD: autoload ("__H5T_set_order__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_set_order__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_set_order__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5T.set_order (@var{type_id}, @var{byte_order})\n                       \
Set the byte order of the data type specified by @var{type_id}.\n\
\n\
The input @var{byte_order} can be @qcode{'H5T_ORDER_LE'},\n\
@qcode{'H5T_ORDER_BE'}, or @qcode{'H5T_ORDER_VAX'}.\n\
@seealso{H5T.get_order}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  std::map<std::string, hid_t> h5_oct_constants = get_constants_map ();

  if (nargin != 2)
    print_usage ("H5T.set_order");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.get_order");

  // Byte order
  H5T_order_t byte_order
    = static_cast<H5T_order_t> (get_h5_id (args, 0, "BYTE_ORDER",
                                           "H5T.set_order"));

  if (H5Tset_order (type_id, byte_order) < 1)
    error ("H5T.set_order: unable to change byte order of specified type");

  return retval;
}

// PKG_ADD: autoload ("__H5T_set_size__", "__H5T__.oct");
// PKG_DEL: autoload ("__H5T_set_size__", "__H5T__.oct", "remove");
DEFUN_DLD(__H5T_set_size__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5T.set_size (@var{type_id}, @var{size})\n\
@var{size} is the size in bytes of the type or @qcode{\"H5T_VARIABLE\"}.\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 2)
    print_usage ("H5T.set_size");

  // Type ID
  hid_t type_id = get_h5_id (args, 0, "TYPE_ID", "H5T.set_size", false);

  // Size
  size_t sz =  get_h5_id (args, 1, "SIZE", "H5T.set_size", true);

  if (H5Tset_size (type_id, sz) < 0)
    error ("H5T.set_size: unable set specified size");

  return retval;
}


