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

*/

#include "h5_data_util.h"

octave_value
__h5_read__ (const std::string& caller, dim_vector dv, hid_t object_id,
             hid_t mem_type_id, hid_t mem_space_id,
             hid_t file_space_id, hid_t xfer_plist_id,
             hid_t field_type_id)
{
  octave_value retval;

  // Decide which hdf5 function to call based on caller name
  int read_fcn;

  if (caller == "H5D.read")
    read_fcn = 0;
  else // H5A.read
    read_fcn = 1;

  // Get type info
  octave_scalar_map info_struct;

  if (field_type_id != H5_INDEX_UNKNOWN)
    {
      if (! dtype_to_struct (field_type_id, info_struct))
        error ("%s: unable to determine type", caller.c_str ());
    }
  else if (! dtype_to_struct (mem_type_id, info_struct))
    error ("%s: unable to determine type", caller.c_str ());

  std::string cls = info_struct.getfield ("Class").string_value ();

  if (cls == "H5T_INTEGER")
    {
      std::string typ = info_struct.getfield ("Type").string_value ();
      if (typ.find ("I8") != std::string::npos)
        {
          int8NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("U8") != std::string::npos)
        {
          uint8NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("I16") != std::string::npos)
        {
          int16NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("U16") != std::string::npos)
        {
          uint16NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("I32") != std::string::npos)
        {
          int32NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("U32") != std::string::npos)
        {
          uint32NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("I64") != std::string::npos)
        {
          int64NDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("U64") != std::string::npos)
        {
          uint64NDArray data (dv);
          H5READ ();
        }
      else
        error ("H5D.read: unexpected mem type");
    }
  else if (cls == "H5T_FLOAT")
    {
      std::string typ = info_struct.getfield ("Type").string_value ();
      if (typ.find ("F32") != std::string::npos)
        {
          FloatNDArray data (dv);
          H5READ ();
        }
      else if (typ.find ("F64") != std::string::npos)
        {
          NDArray data (dv);
          H5READ ();
        }
      else
        error ("H5D.read: unexpected mem type");
    }
  else if (cls == "H5T_STRING")
    {
      // FIXME: This section is messy (at least). Separate variable and fixed
      // length sections for readability and see if this can be simplified.
      std::string typ = info_struct.getfield ("Type").scalar_map_value ()
        .getfield ("CharacterType").string_value ();

      if (typ != "H5T_C_S1")
        error ("H5D.read: unhandled string type %s", typ.c_str ());

      int slen = H5Tget_size (mem_type_id);

      octave_idx_type ndims = dv.ndims ();
      octave_idx_type nstrings = 0;
      if (ndims == 0)
        return octave_value (std::string ());
      else
        {
          nstrings = dv(--ndims);

          while (ndims > 0)
            nstrings *= dv(--ndims);
        }

      herr_t status;

      // H5Dread expects a char** with memory allocated for dimensions only
      // for vlstrings . In this case HDF5 allocates the necessary
      // memory for the actual char internally so we finally have to reclaim
      // owwnership of the pointer and free it ourselves.
      char **rdata;
      rdata = (char **)malloc (nstrings * sizeof (char *));

      // Check if this is a vl string
      octave_value sz = info_struct.getfield ("Type").scalar_map_value ()
        .getfield ("Length");
      bool is_vlstring = sz.is_string ()
        && sz.string_value ().compare ("H5T_VARIABLE") == 0;

      if (! is_vlstring)
        {
          rdata[0] = (char *)malloc ((slen+1) * nstrings * sizeof (char));
          for (int ii = 1; ii < nstrings; ii++)
            rdata[ii] = rdata[0] + ii * (slen + 1);
          mem_type_id = H5Tcopy (H5T_C_S1);
          H5Tset_size (mem_type_id, slen + 1);
        }
      else if (read_fcn == 0)
        mem_space_id = H5Dget_space (object_id);
      else
        mem_space_id = H5Aget_space (object_id);

      if (read_fcn == 0)
        if (is_vlstring)
          status = H5Dread (object_id, mem_type_id, mem_space_id,
                            file_space_id, xfer_plist_id, rdata);
        else
          status = H5Dread (object_id, mem_type_id, mem_space_id,
                            file_space_id, xfer_plist_id, rdata[0]);
      else
        if (is_vlstring)
          status = H5Aread (object_id, mem_type_id, rdata);
        else
          status = H5Aread (object_id, mem_type_id, rdata[0]);

      if (nstrings == 1)
        retval = octave_value (std::string (rdata[0]));
      else if (is_vlstring)
        {
          Cell cell_str (dv);

          for (int ii = 0; ii < nstrings; ii++)
            cell_str(ii) = octave_value (std::string (rdata[ii]));

          retval = octave_value (cell_str);
        }
      else
        {
          dim_vector dv2(nstrings, std::strlen (rdata[0]));
          char fill_val[] = " ";
          charMatrix cm (dv2, *fill_val);
          for (int ii = 0; ii < nstrings; ii++)
            cm.insert (rdata[ii], ii, 0);

          if (slen == 1)
            retval = octave_value (cm.reshape (dv));
          else
            retval = octave_value (cm);
        }

      // Cleanup
      if (is_vlstring)
        {
          H5Dvlen_reclaim (mem_type_id, mem_space_id, H5P_DEFAULT, rdata);
          H5Sclose (mem_space_id);
        }
      else
        H5Tclose (mem_type_id);

      free (rdata);

      if (status < 0)
        error ("%s: unable to read string data", caller.c_str ());
    }
  else if (cls == "H5T_REFERENCE")
    {
      int64NDArray data (dv);
      H5READ ();
    }
  else if (cls == "H5T_COMPOUND")
    {
      octave_scalar_map data;
      size_t nfields = H5Tget_nmembers (mem_type_id);

      for (size_t ii = 0; ii < nfields; ii++)
        {
          char *name = H5Tget_member_name (mem_type_id, ii);

          field_type_id  = H5Tget_member_type (mem_type_id, ii);

          size_t sz = H5Tget_size (field_type_id);

          hid_t type_id = H5Tcreate (H5T_COMPOUND, sz);

          H5Tinsert (type_id, name, 0, field_type_id);

          octave_value val = __h5_read__ (caller, dv, object_id,
                                          type_id, mem_space_id,
                                          file_space_id, xfer_plist_id,
                                          field_type_id);

          data.assign (std::string (name), val);

          H5Tclose(type_id);
        }

      retval = octave_value (data);
    }
  else if (cls == "H5T_VLEN")
    {
      if (dv.ndims () > 0)
        {
          hid_t base_type_id = H5Tget_super (mem_type_id);
          dtype_to_struct (base_type_id, info_struct);

          if (info_struct.getfield ("Class").string_value () != "H5T_STRING")
            error ("%s: unhandled type %s", caller.c_str (), cls.c_str ());

          hvl_t *rdata = (hvl_t *)malloc(dv.ndims () * sizeof(hvl_t));

          herr_t status;
          if (read_fcn == 0)
            status = H5Dread (object_id, mem_type_id, mem_space_id,
                              file_space_id, xfer_plist_id, rdata);
          else
            status = H5Aread (object_id, mem_type_id, rdata);

          if (status)
            error ("%s: unable to retrieve vlen data", caller.c_str ());

          if (dv.ndims () > 1)
            {
              Cell data (dv);

              for (int ii = 0; ii < dv.ndims (); ii++)
                data(ii) = std::string (static_cast<char*> (rdata[ii].p));

              retval = octave_value (data);
            }
          else
            retval = std::string (static_cast<char*> (rdata[0].p));
          
          status = H5Dvlen_reclaim (mem_type_id, mem_space_id, H5P_DEFAULT,
                                    rdata);
          free(rdata);
        }
      else
        retval = octave_value ();
    }
  else
    error ("%s: unhandled type %s", caller.c_str (), cls.c_str ());

  return retval;
}

void
__h5write__ (const std::string& caller, const octave_value& ov,
             hid_t object_id, hid_t mem_type_id, hid_t mem_space_id,
             hid_t file_space_id, hid_t xfer_plist_id, hid_t field_type_id)
{
  // Decide which hdf5 function to call based on caller name
  int wrt_fcn;

  if (caller == "H5D.write")
    wrt_fcn = 0;
  else // H5A.write
    wrt_fcn = 1;

  herr_t status = -1;

  if (ov.iscomplex ())
    error ("H5D.write: complex data are currently not handled");

  if (mem_space_id != H5S_ALL)
    {
      warning ("H5D.write: substituting MEM_SPACE_ID with 'H5S_ALL'");
      mem_space_id = H5S_ALL;
    }

  bool auto_type = (mem_type_id == -1234);

  if (auto_type)
    {
      // Match Dataset/Attribute datatype
      if (wrt_fcn == 0)
        mem_type_id = H5Dget_type (object_id);
      else
        mem_type_id = H5Aget_type (object_id);
    }

  hid_t sub_type_id = mem_type_id;
  if (field_type_id != H5_INDEX_UNKNOWN)
    sub_type_id = field_type_id;

  if (H5Tequal (sub_type_id, H5T_NATIVE_DOUBLE) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id, ov.array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.array_value ().data ());

    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_FLOAT) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.float_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.float_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_INT8) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.int8_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.int8_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_INT16) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.int16_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.int16_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_INT32) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.int32_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.int32_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_INT64) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.int64_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.int64_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_UINT8) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.uint8_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.uint8_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_UINT16) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.uint16_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.uint16_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_UINT32) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.uint32_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.uint32_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_NATIVE_UINT64) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.uint64_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.uint64_array_value ().data ());
    }
  else if (H5Tequal (sub_type_id, H5T_STD_REF_OBJ) > 0)
    {
      if (wrt_fcn == 0)
        status = H5Dwrite (object_id, mem_type_id, mem_space_id, file_space_id,
                           xfer_plist_id,
                           ov.int64_array_value ().data ());
      else
        status = H5Awrite (object_id, mem_type_id,
                           ov.int64_array_value ().data ());
    }
  else if (H5Tget_class (sub_type_id) == H5T_STRING)
    {
      htri_t is_vlstr  = H5Tis_variable_str (sub_type_id);

      if (is_vlstr)
        {
          char **str_array;
          if (ov.is_string ())
            {
              str_array = (char**) malloc (sizeof (char*));
              str_array[0] = (char *) malloc (ov.numel () + 1);
              std::strcpy (str_array[0], ov.string_value ().c_str ());
            }
          else if (ov.iscellstr ())
            {
              Cell cellstr = ov.cell_value ();
              octave_idx_type nstr = cellstr.numel ();
              str_array = (char**) malloc (nstr * sizeof (char*));

              for (int ii = 0; ii < nstr; ii++)
                {
                  // For compatibility with ML, transform vertical char arrays
                  // into row strings
                  charMatrix cm = cellstr(ii).char_matrix_value ();
                  octave_idx_type nchar = cm.numel ();
                  cm = cm.reshape (1, nchar);

                  str_array[ii] = (char *) malloc ((nchar + 1)
                                                   * sizeof (char*));

                  if (ii < nstr-1)
                    str_array[ii+1] = str_array[ii] + ii * (nchar + 1);

                  std::strcpy (str_array[ii], cm.row_as_string (0).c_str ());
                }
            }
          else
            error ("%s: expect a single line char array "
                   "or cell array of strings for H5T_VARIABLE type",
                   caller.c_str ());

          if (wrt_fcn == 0)
            status = H5Dwrite (object_id, mem_type_id, mem_space_id,
                               file_space_id, xfer_plist_id,
                               str_array);
          else
            status = H5Awrite (object_id, mem_type_id,
                               str_array);

          free (str_array);
        }
      else
        {
          charMatrix cm = ov.xchar_matrix_value ("%s: expecting char array for fixed length strings", caller.c_str ());

          if (wrt_fcn == 0)
            status = H5Dwrite (object_id, mem_type_id, mem_space_id,
                               file_space_id, xfer_plist_id,
                               cm.data ());
          else
            status = H5Awrite (object_id, mem_type_id,
                               cm.data ());
        }
    }
  else if (H5Tget_class (sub_type_id) == H5T_COMPOUND)
    {
      // Expect a scalar input structure
      octave_scalar_map data =
        ov.xscalar_map_value ("H5D.write: expecting a scalar structure "
                              "for compound data type");

      size_t nfields = H5Tget_nmembers (sub_type_id);

      for (size_t ii = 0; ii < nfields; ii++)
        {
          char *name = H5Tget_member_name (sub_type_id, ii);
          if (data.isfield (name))
            {
              field_type_id = H5Tget_member_type (sub_type_id, ii);

              size_t sz = H5Tget_size (field_type_id);

              hid_t type_id = H5Tcreate (H5T_COMPOUND, sz);

              H5Tinsert (type_id, name, 0, field_type_id);

              const octave_value val = data.getfield (name);

              __h5write__ (caller, val, object_id, type_id,
                           mem_space_id, file_space_id, xfer_plist_id,
                           field_type_id);
              // We went that far, all is ok.
              status = 0;
              H5Tclose (field_type_id);
              H5Tclose (type_id);
            }

          H5free_memory (name);
        }
    }
  if (auto_type)
    H5Tclose (mem_type_id);

  if (status < 0)
    error ("%s: unable to write data (status %d)", caller.c_str (), status);
}
