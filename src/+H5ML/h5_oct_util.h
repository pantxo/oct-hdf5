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

#include <octave/octave.h>
// #include <octave/builtin-defun-decls.h>
#include <octave/parse.h>
#include <set>
#include <string>

std::map<std::string, hid_t> get_constants_map (void);

dim_vector
get_dim_vector (hid_t space_id)
{
  dim_vector dv;

  // Check for scalar and null dataspace
  H5S_class_t space_type = H5Sget_simple_extent_type (space_id);
  if (space_type == H5S_SCALAR)
    return dim_vector (1, 1);
  else if (space_type == H5S_NULL)
    return dim_vector (0, 0);

  int ndims = H5Sget_simple_extent_ndims (space_id);
  if (ndims < 0)
    error ("get_dim_vector: unable to get space dims");

  hsize_t dims[ndims];
  hsize_t maxdims[ndims];

  ndims = H5Sget_simple_extent_dims (space_id, dims, maxdims);

  if (ndims == 1)
    dv = dim_vector (static_cast<octave_idx_type> (dims[0]), 1);
  else if (ndims == 2)
    dv = dim_vector (static_cast<octave_idx_type> (dims[1]),
                     static_cast<octave_idx_type> (dims[0]));
  else if (ndims == 3)
    dv = dim_vector (static_cast<octave_idx_type> (dims[2]),
                     static_cast<octave_idx_type> (dims[1]),
                     static_cast<octave_idx_type> (dims[0]));
  else if (ndims == 4)
    dv = dim_vector (static_cast<octave_idx_type> (dims[3]),
                     static_cast<octave_idx_type> (dims[2]),
                     static_cast<octave_idx_type> (dims[1]),
                     static_cast<octave_idx_type> (dims[0]));
  else if (ndims == 5)
    dv = dim_vector (static_cast<octave_idx_type> (dims[4]),
                     static_cast<octave_idx_type> (dims[3]),
                     static_cast<octave_idx_type> (dims[2]),
                     static_cast<octave_idx_type> (dims[1]),
                     static_cast<octave_idx_type> (dims[0]));
  else if (ndims == 6)
    dv = dim_vector (static_cast<octave_idx_type> (dims[5]),
                     static_cast<octave_idx_type> (dims[4]),
                     static_cast<octave_idx_type> (dims[3]),
                     static_cast<octave_idx_type> (dims[2]),
                     static_cast<octave_idx_type> (dims[1]),
                     static_cast<octave_idx_type> (dims[0]));
  else
    error ("get_dim_vector: unhandled ndims");

  return dv;
}

hid_t get_h5_id (const octave_value_list& args, int argnum,
                 std::string argname, std::string caller,
                 bool maybe_string = true)
{
  std::map<std::string, hid_t> h5_oct_constants = get_constants_map ();
  hid_t id;

  static std::string msg1
    = "%s: %s must be a string or a scalar numeric identifier";
  static std::string msg2
    = "%s: %s must be a scalar numeric identifier";

  if (maybe_string && args(argnum).is_string ())
    {
      std::string name =  args(argnum).string_value ();

      if (h5_oct_constants.find (name) == h5_oct_constants.end ())
        error ("%s: unknown %s '%s'", caller.c_str (), argname.c_str (),
               name.c_str ());

      id = h5_oct_constants[name];
    }
  else if (args(argnum).is_scalar_type ())
    {
      std::string msg;

      if (maybe_string)
        id = args(argnum).xint64_scalar_value (msg1.c_str (), caller.c_str (),
                                               argname.c_str ());
      else
        id = args(argnum).xint64_scalar_value (msg2.c_str (), caller.c_str (),
                                               argname.c_str ());
    }
  else if (maybe_string)
    error (msg1.c_str (), caller.c_str (), argname.c_str ());
  else
    error (msg2.c_str (), caller.c_str (), argname.c_str ());

  return id;
}


// Callback data structure.

struct user_callback_data
{
  // User function to be called by the handler
  octave_value callback;

  // User data
  octave_value data;
};

herr_t
iter_handler (hid_t group_id, const char* name, const H5L_info_t* /*info*/,
              void* user_data)
{
  user_callback_data *ucdp = static_cast<user_callback_data *> (user_data);

  // Evaluate user supplied function
  herr_t status = 0;

  // Fdisp (ovl (std::string ("iter_handler receiving ")));
  // Fdisp (ucdp->data);
  try
    {
      octave_value_list result
        = octave::feval (ucdp->callback,
                         ovl (octave_int64 (group_id), std::string (name),
                              ucdp->data),
                         2);

      int nargout = result.length ();

      if (nargout != 2)
        error ("H5L.iterate: expect FCN to return exactly 2 arguments (%d returned", nargout);

      status = static_cast<herr_t> (result(0).int_value ());

      // Fdisp (ovl (std::string ("Changing value to ")));
      // Fdisp (result(1));
      ucdp->data = result(1);
    }
  catch (const octave::execution_exception& ee)
    {
      warning ("H5L.iterate: Operator function failed. %s",
               ee.message ().c_str ());

      status = -1;
    }

  return status;
}

herr_t
A_iter_handler (hid_t group_id, const char* name, const H5A_info_t* /*info*/,
              void* user_data)
{
  user_callback_data *ucdp = static_cast<user_callback_data *> (user_data);

  // Evaluate user supplied function
  herr_t status = 0;

  // Fdisp (ovl (std::string ("iter_handler receiving ")));
  // Fdisp (ucdp->data);
  try
    {
      octave_value_list result
        = octave::feval (ucdp->callback,
                         ovl (octave_int64 (group_id), std::string (name),
                              ucdp->data),
                         2);

      int nargout = result.length ();

      if (nargout != 2)
        error ("H5L.iterate: expect FCN to return exactly 2 arguments (%d returned", nargout);

      status = static_cast<herr_t> (result(0).int_value ());

      // Fdisp (ovl (std::string ("Changing value to ")));
      // Fdisp (result(1));
      ucdp->data = result(1);
    }
  catch (const octave::execution_exception& ee)
    {
      warning ("H5L.iterate: Operator function failed. %s",
               ee.message ().c_str ());

      status = -1;
    }

  return status;
}

std::map<std::string, hid_t>
get_constants_map (void)
{
  static std::map<std::string, hid_t>  h5_oct_constants =
    {
     //H5
     {"H5_INDEX_CRT_ORDER", H5_INDEX_CRT_ORDER},
     {"H5_INDEX_NAME", H5_INDEX_NAME},
     {"H5_ITER_ERROR", H5_ITER_ERROR},
     {"H5_ITER_CONT", H5_ITER_CONT},
     {"H5_ITER_DEC", H5_ITER_DEC},
     {"H5_ITER_INC", H5_ITER_INC},
     {"H5_ITER_NATIVE", H5_ITER_NATIVE},
     {"H5_ITER_STOP", H5_ITER_STOP},
     //H5D
     {"H5D_COMPACT", H5D_COMPACT},
     {"H5D_CONTIGUOUS", H5D_CONTIGUOUS},
     {"H5D_CHUNKED", H5D_CHUNKED},
     {"H5D_VIRTUAL", H5D_VIRTUAL},
     {"H5D_FILL_VALUE_UNDEFINED", H5D_FILL_VALUE_UNDEFINED},
     {"H5D_FILL_VALUE_DEFAULT", H5D_FILL_VALUE_DEFAULT},
     {"H5D_FILL_VALUE_USER_DEFINED", H5D_FILL_VALUE_USER_DEFINED},
     //H5E
     {"H5E_WALK_DOWNWARD", H5E_WALK_DOWNWARD},
     {"H5E_WALK_UPWARD", H5E_WALK_UPWARD},
     //H5F
     {"H5F_ACC_EXCL", H5F_ACC_EXCL},
     {"H5F_ACC_RDONLY", H5F_ACC_RDONLY},
     {"H5F_ACC_RDWR", H5F_ACC_RDWR},
     {"H5F_ACC_TRUNC", H5F_ACC_TRUNC},
     //H5O
     {"H5O_TYPE_UNKNOWN", H5O_TYPE_UNKNOWN},
     {"H5O_TYPE_GROUP", H5O_TYPE_GROUP},
     {"H5O_TYPE_DATASET", H5O_TYPE_DATASET},
     {"H5O_TYPE_NAMED_DATATYPE", H5O_TYPE_NAMED_DATATYPE},
     {"H5O_TYPE_NTYPES", H5O_TYPE_NTYPES},
     //H5P
     {"H5P_ATTRIBUTE_CREATE", H5P_ATTRIBUTE_CREATE},
     {"H5P_DATASET_ACCESS", H5P_DATASET_ACCESS},
     {"H5P_DATASET_CREATE", H5P_DATASET_CREATE},
     {"H5P_DATASET_XFER", H5P_DATASET_XFER},
     {"H5P_DATATYPE_CREATE", H5P_DATATYPE_CREATE},
     {"H5P_DATATYPE_ACCESS", H5P_DATATYPE_ACCESS},
     {"H5P_DEFAULT", H5P_DEFAULT},
     {"H5P_FILE_MOUNT", H5P_FILE_MOUNT},
     {"H5P_FILE_CREATE", H5P_FILE_CREATE},
     {"H5P_FILE_ACCESS", H5P_FILE_ACCESS},
     {"H5P_GROUP_CREATE", H5P_GROUP_CREATE},
     {"H5P_GROUP_ACCESS", H5P_GROUP_ACCESS},
     {"H5P_LINK_CREATE", H5P_LINK_CREATE},
     {"H5P_LINK_ACCESS", H5P_LINK_ACCESS},
     {"H5P_OBJECT_COPY", H5P_OBJECT_COPY},
     {"H5P_OBJECT_CREATE", H5P_OBJECT_CREATE},
     {"H5P_STRING_CREATE", H5P_STRING_CREATE},
     //H5R
     {"H5R_OBJECT", H5R_OBJECT},
     {"H5R_DATASET_REGION", H5R_DATASET_REGION},
     //H5S
     {"H5S_ALL", H5S_ALL},
     {"H5S_NULL", H5S_NULL},
     {"H5S_SCALAR", H5S_SCALAR},
     {"H5S_SIMPLE", H5S_SIMPLE},
     //H5T
     {"H5T_VARIABLE", H5T_VARIABLE},
     {"H5T_CSET_ASCII", H5T_CSET_ASCII},
     {"H5T_CSET_UTF8", H5T_CSET_UTF8},
     {"H5T_INTEGER", H5T_INTEGER},
     {"H5T_FLOAT", H5T_FLOAT},
     {"H5T_STRING", H5T_STRING},
     {"H5T_BITFIELD", H5T_BITFIELD},
     {"H5T_OPAQUE", H5T_OPAQUE},
     {"H5T_COMPOUND", H5T_COMPOUND},
     {"H5T_REFERENCE", H5T_REFERENCE},
     {"H5T_ENUM", H5T_ENUM},
     {"H5T_VLEN", H5T_VLEN},
     {"H5T_ARRAY", H5T_ARRAY},
     {"H5T_ORDER_BE", H5T_ORDER_BE},
     {"H5T_ORDER_LE", H5T_ORDER_LE},
     {"H5T_ORDER_BE", H5T_ORDER_VAX},
     {"H5T_ORDER_LE", H5T_ORDER_MIXED},
     {"H5T_ORDER_VAX", H5T_ORDER_NONE},
     /*
      * The IEEE floating point types in various byte orders.
      */
     {"H5T_IEEE_F32BE", H5T_IEEE_F32BE},
     {"H5T_IEEE_F32LE", H5T_IEEE_F32LE},
     {"H5T_IEEE_F64BE", H5T_IEEE_F64BE},
     {"H5T_IEEE_F64LE", H5T_IEEE_F64LE},
     /*
      * These are "standard" types.  For instance, signed (2's complement) and
      * unsigned integers of various sizes and byte orders.
      */
     {"H5T_STD_I8BE", H5T_STD_I8BE},
     {"H5T_STD_I8LE", H5T_STD_I8LE},
     {"H5T_STD_I16BE", H5T_STD_I16BE},
     {"H5T_STD_I16LE", H5T_STD_I16LE},
     {"H5T_STD_I32BE", H5T_STD_I32BE},
     {"H5T_STD_I32LE", H5T_STD_I32LE},
     {"H5T_STD_I64BE", H5T_STD_I64BE},
     {"H5T_STD_I64LE", H5T_STD_I64LE},
     {"H5T_STD_U8BE", H5T_STD_U8BE},
     {"H5T_STD_U8LE", H5T_STD_U8LE},
     {"H5T_STD_U16BE", H5T_STD_U16BE},
     {"H5T_STD_U16LE", H5T_STD_U16LE},
     {"H5T_STD_U32BE", H5T_STD_U32BE},
     {"H5T_STD_U32LE", H5T_STD_U32LE},
     {"H5T_STD_U64BE", H5T_STD_U64BE},
     {"H5T_STD_U64LE", H5T_STD_U64LE},
     {"H5T_STD_B8BE", H5T_STD_B8BE},
     {"H5T_STD_B8LE", H5T_STD_B8LE},
     {"H5T_STD_B16BE", H5T_STD_B16BE},
     {"H5T_STD_B16LE", H5T_STD_B16LE},
     {"H5T_STD_B32BE", H5T_STD_B32BE},
     {"H5T_STD_B32LE", H5T_STD_B32LE},
     {"H5T_STD_B64BE", H5T_STD_B64BE},
     {"H5T_STD_B64LE", H5T_STD_B64LE},
     {"H5T_STD_REF_OBJ", H5T_STD_REF_OBJ},
     /*
      * Types which are particular to Unix.
      */
     {"H5T_UNIX_D32BE", H5T_UNIX_D32BE},
     {"H5T_UNIX_D32LE", H5T_UNIX_D32LE},
     {"H5T_UNIX_D64BE", H5T_UNIX_D64BE},
     {"H5T_UNIX_D64LE", H5T_UNIX_D64LE},
     /*
      * Types particular to the C language.  String types use `bytes' instead
      * of `bits' as their size.
      */
     {"H5T_C_S1", H5T_C_S1},
     /*
      * Types particular to Fortran.
      */
     {"H5T_FORTRAN_S1", H5T_FORTRAN_S1},
     /*
      * These types are for Intel CPU's.  They are little endian with IEEE
      * floating point.
      */
     {"H5T_INTEL_I8", H5T_INTEL_I8},
     {"H5T_INTEL_I16", H5T_INTEL_I16},
     {"H5T_INTEL_I32", H5T_INTEL_I32},
     {"H5T_INTEL_I64", H5T_INTEL_I64},
     {"H5T_INTEL_U8", H5T_INTEL_U8},
     {"H5T_INTEL_U16", H5T_INTEL_U16},
     {"H5T_INTEL_U32", H5T_INTEL_U32},
     {"H5T_INTEL_U64", H5T_INTEL_U64},
     {"H5T_INTEL_B8", H5T_INTEL_B8},
     {"H5T_INTEL_B16", H5T_INTEL_B16},
     {"H5T_INTEL_B32", H5T_INTEL_B32},
     {"H5T_INTEL_B64", H5T_INTEL_B64},
     {"H5T_INTEL_F32", H5T_INTEL_F32},
     {"H5T_INTEL_F64", H5T_INTEL_F64},
     /*
      * These types are for DEC Alpha CPU's.  They are little endian with IEEE
      * floating point.
      */
     {"H5T_ALPHA_I8", H5T_ALPHA_I8},
     {"H5T_ALPHA_I16", H5T_ALPHA_I16},
     {"H5T_ALPHA_I32", H5T_ALPHA_I32},
     {"H5T_ALPHA_I64", H5T_ALPHA_I64},
     {"H5T_ALPHA_U8", H5T_ALPHA_U8},
     {"H5T_ALPHA_U16", H5T_ALPHA_U16},
     {"H5T_ALPHA_U32", H5T_ALPHA_U32},
     {"H5T_ALPHA_U64", H5T_ALPHA_U64},
     {"H5T_ALPHA_B8", H5T_ALPHA_B8},
     {"H5T_ALPHA_B16", H5T_ALPHA_B16},
     {"H5T_ALPHA_B32", H5T_ALPHA_B32},
     {"H5T_ALPHA_B64", H5T_ALPHA_B64},
     {"H5T_ALPHA_F32", H5T_ALPHA_F32},
     {"H5T_ALPHA_F64", H5T_ALPHA_F64},
     /*
      * These types are for MIPS cpu's commonly used in SGI systems. They are big
      * endian with IEEE floating point.
      */
     {"H5T_MIPS_I8", H5T_MIPS_I8},
     {"H5T_MIPS_I16", H5T_MIPS_I16},
     {"H5T_MIPS_I32", H5T_MIPS_I32},
     {"H5T_MIPS_I64", H5T_MIPS_I64},
     {"H5T_MIPS_U8", H5T_MIPS_U8},
     {"H5T_MIPS_U16", H5T_MIPS_U16},
     {"H5T_MIPS_U32", H5T_MIPS_U32},
     {"H5T_MIPS_U64", H5T_MIPS_U64},
     {"H5T_MIPS_B8", H5T_MIPS_B8},
     {"H5T_MIPS_B16", H5T_MIPS_B16},
     {"H5T_MIPS_B32", H5T_MIPS_B32},
     {"H5T_MIPS_B64", H5T_MIPS_B64},
     {"H5T_MIPS_F32", H5T_MIPS_F32},
     {"H5T_MIPS_F64", H5T_MIPS_F64},
     /*
      * The VAX floating point types (i.e. in VAX byte order)
      */
     {"H5T_VAX_F32", H5T_VAX_F32},
     {"H5T_VAX_F64", H5T_VAX_F64},
     /*
      * The predefined native types. These are the types detected by H5detect and
      * they violate the naming scheme a little.  Instead of a class name,
      * precision and byte order as the last component, they have a C-like type
      * name.  If the type begins with `U' then it is the unsigned version of the
      * integer type; other integer types are signed.  The type LLONG corresponds
      * to C's `long long' and LDOUBLE is `long double' (these types might be the
      * same as `LONG' and `DOUBLE' respectively).
      */
     {"H5T_NATIVE_CHAR", H5T_NATIVE_CHAR},
     {"H5T_NATIVE_SCHAR", H5T_NATIVE_SCHAR},
     {"H5T_NATIVE_UCHAR", H5T_NATIVE_UCHAR},
     {"H5T_NATIVE_SHORT", H5T_NATIVE_SHORT},
     {"H5T_NATIVE_USHORT", H5T_NATIVE_USHORT},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_LONG", H5T_NATIVE_LONG},
     {"H5T_NATIVE_ULONG", H5T_NATIVE_ULONG},
     {"H5T_NATIVE_LLONG", H5T_NATIVE_LLONG},
     {"H5T_NATIVE_ULLONG", H5T_NATIVE_ULLONG},
     {"H5T_NATIVE_FLOAT", H5T_NATIVE_FLOAT},
     {"H5T_NATIVE_DOUBLE", H5T_NATIVE_DOUBLE},
     //#if H5_SIZEOF_LONG_DOUBLE !=0
     //#define H5T_NATIVE_LDOUBLE	(H5OPEN H5T_NATIVE_LDOUBLE_g)
     //#endif
     {"H5T_NATIVE_B8", H5T_NATIVE_B8},
     {"H5T_NATIVE_B16", H5T_NATIVE_B16},
     {"H5T_NATIVE_B32", H5T_NATIVE_B32},
     {"H5T_NATIVE_B64", H5T_NATIVE_B64},
     {"H5T_NATIVE_OPAQUE", H5T_NATIVE_OPAQUE},
     {"H5T_NATIVE_HADDR", H5T_NATIVE_HADDR},
     {"H5T_NATIVE_HSIZE", H5T_NATIVE_HSIZE},
     {"H5T_NATIVE_HSSIZE", H5T_NATIVE_HSSIZE},
     {"H5T_NATIVE_HERR", H5T_NATIVE_HERR},
     {"H5T_NATIVE_HBOOL", H5T_NATIVE_HBOOL},
     /* C9x integer types */
     {"H5T_NATIVE_INT8", H5T_NATIVE_INT8},
     {"H5T_NATIVE_UINT8", H5T_NATIVE_UINT8},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT16", H5T_NATIVE_INT16},
     {"H5T_NATIVE_UINT16", H5T_NATIVE_UINT16},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT32", H5T_NATIVE_INT32},
     {"H5T_NATIVE_UINT32", H5T_NATIVE_UINT32},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT64", H5T_NATIVE_INT64},
     {"H5T_NATIVE_UINT64", H5T_NATIVE_UINT64},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT},
     {"H5T_NATIVE_INT", H5T_NATIVE_INT},
     {"H5T_NATIVE_UINT", H5T_NATIVE_UINT}
    };

  return h5_oct_constants;
}
