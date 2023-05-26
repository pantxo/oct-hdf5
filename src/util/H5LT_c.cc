/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// dtype_to_struct is an adaptation of the original H5LTdtype_to_text function.

#include "H5LT_c.h"

bool
dtype_to_struct (hid_t dtype, octave_scalar_map& s)
{
  std::size_t size = H5Tget_size(dtype);
  s.assign ("Name", "");
  s.assign ("Size", size);

  H5T_class_t tcls = H5Tget_class (dtype);
  std::string h5type_string;

  if (tcls < 0)
    return false;

  switch (tcls)
    {
    case H5T_INTEGER:
    case H5T_BITFIELD:
      {
        s.assign ("Class", "H5T_INTEGER");

        if (H5Tequal(dtype, H5T_STD_I8BE))
          h5type_string = "H5T_STD_I8BE";
        else if (H5Tequal(dtype, H5T_STD_I8LE))
          h5type_string = "H5T_STD_I8LE";
        else if (H5Tequal(dtype, H5T_STD_I16BE))
          h5type_string = "H5T_STD_I16BE";
        else if (H5Tequal(dtype, H5T_STD_I16LE))
          h5type_string = "H5T_STD_I16LE";
        else if (H5Tequal(dtype, H5T_STD_I32BE))
          h5type_string = "H5T_STD_I32BE";
        else if (H5Tequal(dtype, H5T_STD_I32LE))
          h5type_string = "H5T_STD_I32LE";
        else if (H5Tequal(dtype, H5T_STD_I64BE))
          h5type_string = "H5T_STD_I64BE";
        else if (H5Tequal(dtype, H5T_STD_I64LE))
          h5type_string = "H5T_STD_I64LE";
        else if (H5Tequal(dtype, H5T_STD_U8BE))
          h5type_string = "H5T_STD_U8BE";
        else if (H5Tequal(dtype, H5T_STD_U8LE))
          h5type_string = "H5T_STD_U8LE";
        else if (H5Tequal(dtype, H5T_STD_U16BE))
          h5type_string = "H5T_STD_U16BE";
        else if (H5Tequal(dtype, H5T_STD_U16LE))
          h5type_string = "H5T_STD_U16LE";
        else if (H5Tequal(dtype, H5T_STD_U32BE))
          h5type_string = "H5T_STD_U32BE";
        else if (H5Tequal(dtype, H5T_STD_U32LE))
          h5type_string = "H5T_STD_U32LE";
        else if (H5Tequal(dtype, H5T_STD_U64BE))
          h5type_string = "H5T_STD_U64BE";
        else if (H5Tequal(dtype, H5T_STD_U64LE))
          h5type_string = "H5T_STD_U64LE";
        else if (H5Tequal(dtype, H5T_NATIVE_SCHAR))
          h5type_string = "H5T_NATIVE_SCHAR";
        else if (H5Tequal(dtype, H5T_NATIVE_UCHAR))
          h5type_string = "H5T_NATIVE_UCHAR";
        else if (H5Tequal(dtype, H5T_NATIVE_SHORT))
          h5type_string = "H5T_NATIVE_SHORT";
        else if (H5Tequal(dtype, H5T_NATIVE_USHORT))
          h5type_string = "H5T_NATIVE_USHORT";
        else if (H5Tequal(dtype, H5T_NATIVE_INT))
          h5type_string = "H5T_NATIVE_INT";
        else if (H5Tequal(dtype, H5T_NATIVE_UINT))
          h5type_string = "H5T_NATIVE_UINT";
        else if (H5Tequal(dtype, H5T_NATIVE_LONG))
          h5type_string = "H5T_NATIVE_LONG";
        else if (H5Tequal(dtype, H5T_NATIVE_ULONG))
          h5type_string = "H5T_NATIVE_ULONG";
        else if (H5Tequal(dtype, H5T_NATIVE_LLONG))
          h5type_string = "H5T_NATIVE_LLONG";
        else if (H5Tequal(dtype, H5T_NATIVE_ULLONG))
          h5type_string = "H5T_NATIVE_ULLONG";
        else
          h5type_string = "undefined integer";

        s.assign ("Type", h5type_string);
        return true;
      }
    case H5T_FLOAT:
      {
        s.assign ("Class", "H5T_FLOAT");

        if (H5Tequal(dtype, H5T_IEEE_F32BE))
          h5type_string = "H5T_IEEE_F32BE";
        else if (H5Tequal(dtype, H5T_IEEE_F32LE))
          h5type_string = "H5T_IEEE_F32LE";
        else if (H5Tequal(dtype, H5T_IEEE_F64BE))
          h5type_string = "H5T_IEEE_F64BE";
        else if (H5Tequal(dtype, H5T_IEEE_F64LE))
          h5type_string = "H5T_IEEE_F64LE";
        else if (H5Tequal(dtype, H5T_NATIVE_FLOAT))
          h5type_string = "H5T_NATIVE_FLOAT";
        else if (H5Tequal(dtype, H5T_NATIVE_DOUBLE))
          h5type_string = "H5T_NATIVE_DOUBLE";
        // #if H5_SIZEOF_LONG_DOUBLE !=0
        //             } else if (H5Tequal(dtype, H5T_NATIVE_LDOUBLE)) {
        //                 HDsnprintf(dt_str, *slen, "H5T_NATIVE_LDOUBLE");
        // #endif
        else
          h5type_string = "undefined float";

        s.assign ("Type", h5type_string);
        return true;
      }
    case H5T_STRING:
      {
        s.assign ("Class", "H5T_STRING");

        h5type_string = "H5T_STRING";
        octave_scalar_map type_map;

        /* Make a copy of type in memory in case when DTYPE is on disk, the size
         * will be bigger than in memory.  This makes it easier to compare
         * types in memory. */
        hid_t tmp_type = H5Tcopy (dtype);
        if(tmp_type < 0)
          return false;

        size = H5Tget_size(tmp_type);
        if (size ==0)
          return false;

        H5T_str_t str_pad = H5Tget_strpad (tmp_type);
        if (str_pad < 0)
          return false;


        H5T_cset_t cset  = H5Tget_cset (tmp_type);
        if(cset < 0)
          return false;

        htri_t is_vlstr  = H5Tis_variable_str (tmp_type);
        if (is_vlstr < 0)
          return false;


        if (is_vlstr)
          type_map.assign ("Length", "H5T_VARIABLE");
        else
          type_map.assign ("Length", size);

        if (str_pad == H5T_STR_NULLTERM)
          type_map.assign ("Padding", "H5T_STR_NULLTERM");
        else if (str_pad == H5T_STR_NULLPAD)
          type_map.assign ("Padding", "H5T_STR_NULLPAD");
        else if (str_pad == H5T_STR_SPACEPAD)
          type_map.assign ("Padding", "H5T_STR_SPACEPAD");
        else
          type_map.assign ("Padding", "H5T_STR_ERROR");

        if (cset == H5T_CSET_ASCII)
          type_map.assign ("CharacterSet", "H5T_CSET_ASCII");
        else if (cset == H5T_CSET_UTF8)
          type_map.assign ("CharacterSet", "H5T_CSET_UTF8");
        else
          type_map.assign ("CharacterSet", "Unknown");

        /* Reproduce a base C type string */
        hid_t str_type = H5Tcopy (H5T_C_S1);

        if(str_type < 0)
          return false;

        if(is_vlstr)
          {
            if (H5Tset_size (str_type, H5T_VARIABLE) < 0)
              return false;
          }
        else if (H5Tset_size (str_type, size) < 0)
          return false;

        if(H5Tset_cset(str_type, cset) < 0)
          return false;

        if(H5Tset_strpad(str_type, str_pad) < 0)
          return false;

        if (H5Tequal (tmp_type, str_type))
          {
            type_map.assign ("CharacterType", "H5T_C_S1");
            s.assign ("Type", type_map);
            return true;
          }

        // Check differing endianness
        H5T_order_t order  = H5Tget_order (tmp_type);
        if(order < 0)
          return false;

        if (order == H5T_ORDER_LE)
          {
            if(H5Tset_order(str_type, H5T_ORDER_LE) < 0)
              return false;
          }
        else if (order == H5T_ORDER_BE)
          if(H5Tset_order(str_type, H5T_ORDER_BE) < 0)
            return false;

        if (H5Tequal (tmp_type, str_type))
          {
            type_map.assign ("CharacterType", "H5T_C_S1");
            s.assign ("Type", type_map);
            return true;
          }

        /* If not equal to C variable-length string, check Fortran type.
         * Actually H5Tequal can't tell difference between H5T_C_S1 and
         * H5T_FORTRAN_S1
         * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

        if (H5Tclose (str_type) < 0)
          return false;
        if ((str_type = H5Tcopy (H5T_FORTRAN_S1)) < 0)
          return false;
        if (H5Tset_cset (str_type, cset) < 0)
          return false;
        if (H5Tset_size (str_type, size) < 0)
          return false;
        if (H5Tset_strpad (str_type, str_pad) < 0)
          return false;


        if (H5Tequal (tmp_type, str_type))
          {
            type_map.assign ("CharacterType", "H5T_FORTRAN_S1");
            s.assign ("Type", type_map);
            return true;
          }

        /* Change the endianness and see if they're equal. */
        if((order = H5Tget_order(tmp_type)) < 0)
          return false;

        if (order==H5T_ORDER_LE)
          {
            if(H5Tset_order(str_type, H5T_ORDER_LE) < 0)
              return false;
          }
        else if (order == H5T_ORDER_BE)
          if (H5Tset_order(str_type, H5T_ORDER_BE) < 0)
              return false;

        if (H5Tequal (tmp_type, str_type))
          {
            type_map.assign ("CharacterType", "H5T_FORTRAN_S1");
            s.assign ("Type", type_map);
            return true;
          }

        /*  No match */
        if (H5Tequal (tmp_type, str_type))
          return false;
      }
    case H5T_OPAQUE:
      s.assign ("Class", "H5T_OPAQUE");
      break;
    case H5T_COMPOUND:
      s.assign ("Class", "H5T_COMPOUND");
      break;
    case H5T_REFERENCE:
      s.assign ("Class", "H5T_REFERENCE");
      s.assign ("Type", "H5R_OBJECT");
      break;
    case H5T_ENUM:
      s.assign ("Class", "H5T_ENUM");
      break;
    case H5T_VLEN:
      s.assign ("Class", "H5T_VLEN");
      break;
    case H5T_ARRAY:
      s.assign ("Class", "H5T_ARRAY");
      break;
    case H5T_NCLASSES:
      s.assign ("Class", "H5T_NCLASSES");
      break;
    case H5T_TIME:
      s.assign ("Class", "H5T_TIME");
      break;
    case H5T_NO_CLASS:
      s.assign ("Class", "H5T_NO_CLASS");
    }
  return true;
}
