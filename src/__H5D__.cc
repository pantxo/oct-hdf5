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
// PKG_ADD: autoload ("__H5D_close__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_close__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_close__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} { } H5D.close (@var{dataset_id})\n\
End access to a dataset specified by @var{dataset_id} and releases resources\n\
used by it.\n\
\n\
Further use of the dataset identifier is illegal in calls to\n\
the dataset API.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Dataset to release access to\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_CLOSE}.\n\
\n\
@seealso{H5D.open,H5D.create}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.close");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.close", false);

  if (H5Dclose (dataset_id) < 0)
    error ("H5D.close: unable to close dataset");

  return retval;
}

/*
%!fail ("H5D.close ()", "Invalid call")

%!fail ("H5D.close (1, 2)", "Invalid call")

%!fail ("H5D.close (-123456)", "unable to close dataset")
*/

// PKG_ADD: autoload ("__H5D_create__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_create__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{dataset_id} =} H5D.create (@var{loc_id}, @var{name}, @var{dtype_id}, @var{space_id}, @var{lcpl_id})\n\
@deftypefnx {} {@var{dataset_id} =} H5D.create (@var{loc_id}, @var{name}, @var{dtype_id}, @var{space_id}, @var{lcpl_id}, @var{dcpl_id}, @var{dapl_id})\n\
Create a dataset, @var{name}, which is attached to the object\n\
specified by the identifier @var{loc_id}.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{loc_id} @tab @tab Location or object identifier\n\
 @item @var{name} @tab @tab Dataset name\n\
 @item @var{dtype_id} @tab @tab Datatype identifier\n\
 @item @var{space_id} @tab @tab Dataspace identifier\n\
 @item @var{lcpl_id} @tab @tab Link creation property list identifier\n\
 @item @var{dcpl_id} @tab @tab Dataset creation property list identifier\n\
 @item @var{dapl_id} @tab @tab Dataset access property list identifier\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_CREATE2}.\n\
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

/*
%!fail ("H5D.create ()", "Invalid call")

%!fail ("H5D.create (1, 2)", "Invalid call")

%!fail ("H5D.create (1, 2, 3, 4, 5, 6, 7, 8)", "Invalid call")
*/

// PKG_ADD: autoload ("__H5D_get_create_plist__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_get_create_plist__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_get_create_plist__, args, nargout, 
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

// PKG_ADD: autoload ("__H5D_get_space__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_get_space__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_get_space__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{space_id} = } H5D.get_space (@var{dataset_id})\n\
Make a copy of the dataspace of the dataset specified by @var{dataset_id}.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Identifier of a dataset\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
The dataspace identifier returned from this function must be \
released with H5S.close or resource leaks will develop.\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_SPACE}.\n\
\n\
@seealso{H5S.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.get_space");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.get_space", false);

  hid_t space_id = H5Dget_space (dataset_id);

  if (space_id < 0)
    error ("H5D.get_space: unable to retrieve data space");

  return retval.append (octave_int64 (space_id));
}

/*
%!fail ("H5D.get_space ()", "Invalid call");

%!fail ("H5D.get_space ('toto')", "DATASET_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5D.get_space (1); H5E.set_auto (true)", "unable to retrieve data space");
*/

// PKG_ADD: autoload ("__H5D_get_type__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_get_type__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_get_type__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{type_id} = } H5D.get_type (@var{dataset_id})\n\
Return an identifier for a copy of the datatype for a dataset.\n\
\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Identifier of a dataset\n\
 @end multitable\n\
\n\
\n\
@strong{Description:}\n\
\n\
The datatype identifier @var{type_id} returned from this function must\n\
be released with H5T.close or resource leaks will develop.\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_TYPE}.\n\
\n\
@seealso{H5T.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1)
    print_usage ("H5D.get_type");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.get_type", false);

  hid_t type_id = H5Dget_type (dataset_id);

  if (type_id < 0)
    error ("H5D.get_type: unable retrieve data type");

  return retval.append (octave_int64 (type_id));
}

/*
%!fail ("H5D.get_type ()", "Invalid call");

%!fail ("H5D.get_type ('toto')", "DATASET_ID must be a scalar numeric identifier");

%!fail ("H5E.set_auto (false); H5A.get_type (-12345); H5E.set_auto (true)", "unable to retrieve data type");
*/

// PKG_ADD: autoload ("__H5D_open__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_open__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_open__, args, nargout, 
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
 @end multitable\n\
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

// PKG_ADD: autoload ("__H5D_read__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_read__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_read__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{data} = } H5D.read (@var{dataset_id})\n\
@deftypefnx {} {@var{data} = } H5D.read (@var{dataset_id}, @var{mem_type_id}, @var{mem_space_id}, @var{file_space_id}, @var{xfer_plist_id})\n\
Import data from dataset.\n\
\n\
@strong{Parameters:}\n\
 @multitable @columnfractions 0.33 0.02 0.65\n\
 @item @var{dataset_id} @tab @tab Location or dataset identifier\n\
 @item @var{mem_type_id} @tab @tab Target datatype (use @code{H5ML_DEFAULT} \
for automatic conversion)\n\
 @item @var{mem_space_id} @tab @tab Imported data dataspace identifier or \
@code{H5S_ALL}\n\
 @item @var{file_space_id} @tab @tab Original data dataspace identifier or \
@code{H5S_ALL}\n\
 @item @var{xfer_plist_id} @tab @tab Transfer property list identifier or \
@code{H5P_DEFAULT}\n\
 @end multitable\n\
\n\
@strong{Description:}\n\
\n\
Only data with atomic HDF5 data types are handled. Unless @var{mem_type_id} \
is specified or different from 'H5ML_DEFAULT', automatic conversion to \
Octave representation is as follows:\n\
@table @asis\n\
@item H5T_INTEGER\n\
Corresponding integer type, e.g. H5T_STD_I8BE in file is converted \
to H5T_NATIVE_INT8 in memory and returned as a @code{int8} class array.\n\
@item H5T_FLOAT\n\
Corresponding floating point type, e.g. H5T_IEEE_F32LE in file is converted \
to H5T_NATIVE_FLOAT in memory and returned as a @code{single} class array.\n\
@item H5T_STRING\n\
Returned as a char array.\n\
@item H5T_REFERENCE\n\
Returned as a @code{int64} class array containing the reference \
identifier(s).\n\
See @code{H5T.dereference} for how to retrieve the actual object identifier.\n\
@item H5T_COMPOUND\n\
Returned as a struct with fields corresponding to field names in the compound \
data type.\n\
@item H5T_VLEN\n\
Only H5T_STRING base types are handled. Returned as a char array.\n\
@end table\n\
\n\
See original function at \
@url{https://portal.hdfgroup.org/display/HDF5/H5D_READ}.\n\
\n\
@seealso{H5D.open,H5A.read,H5T.dereference}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 1 && nargin != 5)
    print_usage ("H5D.read");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.read", false);

  // Data buffer memory type ID
  hid_t mem_type_id = H5Dget_type (dataset_id);

  if (nargin > 1
      && (! args(1).is_string ()
          || args(1).string_value () != "H5ML_DEFAULT"))
        mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5D.read");

  // Data buffer space ID
  hid_t mem_space_id = H5S_ALL;

  if (nargin > 1)
    mem_space_id = get_h5_id (args, 2, "MEM_SPACE_ID", "H5D.read");

  // File space ID
  hid_t file_space_id = H5S_ALL;

  if (nargin > 1)
    file_space_id = get_h5_id (args, 3, "FILE_SPACE_ID", "H5D.read");

  // Transfer plist ID
  hid_t xfer_plist_id = H5P_DEFAULT;

  if (nargin > 1)
    xfer_plist_id = get_h5_id (args, 4, "XFER_PLIST_ID", "H5D.read");

  // Get output dimensions
  dim_vector dv;
  if (mem_space_id != H5S_ALL)
    error ("H5D.read: only H5S_ALL is implemented for MEM_SPACE_ID");

  if (file_space_id == H5S_ALL)
    file_space_id = H5Dget_space (dataset_id);

  dv = get_dim_vector (file_space_id);

  if (dv.ndims () == 0)
    retval = ovl (Matrix ());
  else
    retval.append (__h5_read__ ("H5D.read", dv, dataset_id, mem_type_id,
                                mem_space_id, file_space_id, xfer_plist_id));

  return retval;
}


/*
%!test
%! fail ("H5D.read ()", "Invalid call");

%!test
%! fail ("H5D.read (1, 2, 3)", "Invalid call");

%!test
%! fail ("H5D.read (1, 2, 3, 4, 5, 6)", "Invalid call");

%!test
%! h5ex_d_rdwr ('int8')

%!test
%! h5ex_d_rdwr ('int16')

%!test
%! h5ex_d_rdwr ('int32')

%!test
%! h5ex_d_rdwr ('int64')

%!test
%! h5ex_d_rdwr ('uint8')

%!test
%! h5ex_d_rdwr ('uint16')

%!test
%! h5ex_d_rdwr ('uint32')

%!test
%! h5ex_d_rdwr ('uint64')

%!test
%! h5ex_d_rdwr ('single')

%!test
%! h5ex_d_rdwr ('double')

%!test
%! h5ex_t_vlstring ()

%!test
%! h5ex_t_string ()

*/

// PKG_ADD: autoload ("__H5D_write__", "__H5D__.oct");
// PKG_DEL: autoload ("__H5D_write__", "__H5D__.oct", "remove");
DEFUN_DLD(__H5D_write__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {} H5D.write (@var{dataset_id}, @var{mem_type_id}, @var{mem_space_id}, @var{file_space_id}, @var{xfer_plist_id}, @var{data})\n\
@seealso{}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 6)
    print_usage ("H5D.write");

  // Dataset ID
  hid_t dataset_id = get_h5_id (args, 0, "DATASET_ID", "H5D.write", false);

  // Data buffer memory type ID
  hid_t mem_type_id;

  if (args(1).is_string () && args(1).string_value () == "H5ML_DEFAULT")
    mem_type_id = -1234;
  else
    mem_type_id = get_h5_id (args, 1, "MEM_TYPE_ID", "H5D.write");

  // Data buffer space selection ID
  hid_t mem_space_id = get_h5_id (args, 2, "MEM_SPACE_ID", "H5D.write");

  // File space selection ID
  hid_t file_space_id = get_h5_id (args, 3, "FILE_SPACE_ID", "H5D.write");

  // Transfer plist ID
  hid_t xfer_plist_id = get_h5_id (args, 4, "XFER_PLIST_ID", "H5D.write");

  // Write data
  __h5write__ ("H5D.write", args(5), dataset_id, mem_type_id, mem_space_id,
               file_space_id, xfer_plist_id);

  return retval;
}

/*
%!test
% data = 122 + 245.56i;
% fname = tempname ();
% fid = H5F.create (tempname (), 'H5F_ACC_TRUNC', 'H5P_DEFAULT', 'H5P_DEFAULT');
% typeid = H5T.copy ('H5T_IEEE_F64LE');
% sizeid = H5S.create_simple(ndims (data), fliplr (size (data)),
%                            fliplr (size (data)));
% elemsize = H5T.get_size (typeid);
% memtype = H5T.create ('H5T_COMPOUND', elemsize * 2);
% H5T.insert (memtype, 'Real', 0, typeid);
% H5T.insert (memtype, 'Imag', elemsize, typeid);
% oid = H5D.create (fid, '/a', memtype, sizeid, 'H5P_DEFAULT');
% data = struct ('Real', real (data), 'Imag', imag (data));
% H5D.write (oid, 'H5ML_DEFAULT', 'H5S_ALL', 'H5S_ALL', 'H5P_DEFAULT', data);
% H5T.close (memtype);
% H5T.close (typeid);
% H5D.close(oid);
% H5F.close(fid);
% ## We went so far, it's ok
% delete (fname);
% assert (true)
*/
