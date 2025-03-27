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

// PKG_ADD: autoload ("__H5R_create__", "__H5R__.oct");
// PKG_DEL: autoload ("__H5R_create__", "__H5R__.oct", "remove");
DEFUN_DLD(__H5R_create__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{ref} =} H5R.create (@var{loc_id}, @var{name}, @var{ref_type}, @var{space_id})\n\
Create the reference, @var{ref}, of the type specified in @var{ref_type},\n\
pointing to the object name located at @var{loc_id}.\n\
\n\
@var{ref_type} must be one of @qcode{'H5R_OBJECT'} or\n\
@qcode{'H5R_DATASET_REGION'}.\n\
\n\
The parameters @var{loc_id} and name are used to locate the object.\n\
\n\
The parameter @var{space_id} identifies the dataset region that a dataset\n\
region reference points to. This parameter is used only with dataset region\n\
references and should be set to -1 if the reference is an object reference,\n\
@qcode{'H5R_OBJECT'}.\n\
\n\
@seealso{H5R.open,H5R.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 4)
    print_usage ("H5R.create");

  // Parse arguments
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5R.create");

  std::string name
    = args(1).xstring_value ("H5R.create: NAME must be a string");

  H5R_type_t ref_type =  static_cast<H5R_type_t> (get_h5_id (args, 2, "REF_TYPE", "H5R.create"));

  hid_t space_id = get_h5_id (args, 3, "SPACE_ID", "H5R.create");

  if (ref_type == H5R_OBJECT)
    {
      hobj_ref_t ref;

      if (H5Rcreate (&ref, loc_id, name.c_str (), ref_type, space_id) < 0)
        error ("H5R.create: unable to create reference");

      retval.append (octave_int64 (ref));
    }
  else if (ref_type == H5R_DATASET_REGION)
    {
      hdset_reg_ref_t ref;

      if (H5Rcreate (&ref, loc_id, name.c_str (), ref_type, space_id) < 0)
        error ("H5R.create: unable to create reference");

      int nref = sizeof (haddr_t) + 4;

      int64NDArray out(dim_vector (1,nref));

      for (int ii = 0; ii < nref; ii++)
        out(ii) = ref[ii];

      retval.append (out);
    }
  else
    error ("H5R.create: unknown REF_TYPE");

  return retval;
}

// PKG_ADD: autoload ("__H5R_dereference__", "__H5R__.oct");
// PKG_DEL: autoload ("__H5R_dereference__", "__H5R__.oct", "remove");
DEFUN_DLD(__H5R_dereference__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{ref_obj_id} = } H5R.dereference (@var{obj_id}, @var{ref_type}, @var{ref})\n\
Given a reference, @var{ref}, to an object or a region in an object,\n\
open that object and return an identifier.\n\
\n\
The parameter @var{obj_id} must be a valid identifier for the HDF5 file\n\
containing the referenced object or for any object in that HDF5 file.\n\
\n\
The parameter @var{ref_type} specifies the reference type of the reference\n\
@var{ref}. @var{ref_type} may contain either of the following values:\n\
@qcode{'H5R_OBJECT'} or @qcode{'H5R_DATASET_REGION'}.\n\
\n\
The object opened with this function should be closed when it is no longer\n\
needed so that resource leaks will not develop. Use the appropriate close\n\
function such as @code{H5O.close} or @code{H5D.close} for datasets.\n\
\n\
@seealso{H5O.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  bool is_1_10_syntax = nargin == 4;

  if (nargin != 3 && nargin != 4)
    print_usage ("H5R.dereference");

  size_t argnum = 0;

  // Object ID
  hid_t obj_id = get_h5_id (args, argnum++, "OBJ_ID", "H5R.dereference", false);

  // Object access property list identifier
  hid_t oapl_id = H5P_DEFAULT;
  if (is_1_10_syntax)
    {
      oapl_id = static_cast<H5R_type_t> (get_h5_id (args, argnum++,
                                                    "OAPL_ID",
                                                    "H5R.dereference"));
    }

  // Ref type
  H5R_type_t ref_type
    = static_cast<H5R_type_t> (get_h5_id (args, argnum++,
                                          "REF_TYPE", "H5R.dereference"));

  // Ref
  int64NDArray ref = args(argnum).xint64_array_value ("H5R.dereference: "
                                                      "REF must be a "
                                                      "reference");

  hid_t out_ref = H5Rdereference2 (obj_id, oapl_id,ref_type,
                                   ref.data ());

  return retval.append (octave_int64 (out_ref));
}

// PKG_ADD: autoload ("__H5R_get_obj_type__", "__H5R__.oct");
// PKG_DEL: autoload ("__H5R_get_obj_type__", "__H5R__.oct", "remove");
DEFUN_DLD(__H5R_get_obj_type__, args, nargout, 
"-*- texinfo -*-\n\
@deftypefn {} {@var{obj_type} = } H5R.get_obj_type (@var{loc_id}, @var{ref_type}, @var{ref})\n\
\n\
@seealso{H5O.close}\n\
@end deftypefn")
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin != 3)
    print_usage ("H5R.get_obj_type");

  // Object ID
  hid_t loc_id = get_h5_id (args, 0, "LOC_ID", "H5R.get_obj_type", false);

  // Ref type
  H5R_type_t ref_type
    = static_cast<H5R_type_t> (get_h5_id (args, 1,
                                          "REF_TYPE", "H5R.get_obj_type"));

  // Ref
  hobj_ref_t ref
    = static_cast<hobj_ref_t> (args(2).xint64_scalar_value ("H5R.get_obj_type: REF must be a reference"));


  H5O_type_t obj_type;

  if (H5Rget_obj_type2 (loc_id, ref_type, &ref, &obj_type) < 0)
    error ("H5R.get_obj_type: unable to retrieve object type");

  return retval.append (octave_int64 (obj_type));
}


