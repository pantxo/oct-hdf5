## Copyright (C) 2023 Pantxo Diribarne
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {@var{out_struct} =} read_mat73_var (@var{fname})
## @deftypefnx {} {@var{out_struct} =} read_mat73_var (@var{fname}, @var{varname})
## Read variables from a HDF5 files formated according to Matlab's
## v7.3 format and return them as a struct.
##
## If an argument @var{varname} is provided, read only the specified variable,
## otherwise return all datasets at the root of the file.
##
## @seealso{H5D.read}
## @end deftypefn

function rdata = read_mat73 (fname, varnames = "__all__")

  if (! ischar (fname))
    error ("read_mat73: FNAME must be a string")
  elseif (! ischar (varnames) && ! iscellstr (varnames))
    print_usage ();
  endif

  fname = file_in_loadpath (fname);
  if (isempty (fname) || ! exist (fname, "file"))
    error ("read_mat73: FNAME must be a valid file name")
  endif

  ## Disable automatic display of hdf5 library errors
  H5E.set_auto (false);

  unwind_protect
    ## Open file
    try
      file_id = H5F.open (fname, "H5F_ACC_RDONLY", "H5P_DEFAULT");

    catch
      error ("read_mat73: unbale to open '%s'.", fname)

      ## Walk the error stack to print the last error from hdf5
      H5E.walk ("H5E_WALK_UPWARD", @error_walker);
    end_try_catch

    ## List available variables in file and check the requested ones existvar
    if (strcmp (varnames, "__all__"))
      varnames = {};
    elseif (! iscellstr (varnames))
      varnames = {varnames};
    endif

    ## Read variables
    rdata = read_vars (file_id, varnames);

    if (any (varnames))
      for ii = 1:numel (varnames)
        if (! isfield (rdata, (varnames{ii})))
          error ("read_mat73: can't find variable '%s' in file '%s'", ...
                 varnames{ii}, fname)
        endif
      endfor
    endif

  unwind_protect_cleanup
    H5F.close (file_id);

    ## Restore previous error printing
    H5E.set_auto (true);

  end_unwind_protect
endfunction


function status = error_walker (n, s)
  disp (s.desc)
  status = -1;
endfunction

function rdata = read_vars (obj_id, varnames, as_dset = false)

  if (isempty (varnames))
    ## Read every variable
    varnames = get_named_variables (obj_id);
  endif

  rdata = struct ([varnames(:).'; cell(size (varnames(:).'))]{:});

  for ii = 1:numel (varnames)

    name = varnames{ii};

    obj = H5O.open (obj_id, name, "H5P_DEFAULT");
    unwind_protect
      if (! as_dset)
        rdata.(name) = get_object_data (obj);
      else
        rdata.(name) = read_dataset (obj);
      endif

    unwind_protect_cleanup
      H5O.close (obj);
    end_unwind_protect

  endfor

endfunction

function vars = get_named_variables (obj_id)
  [status, ii, vars] = H5L.iterate_by_name (obj_id, "/", "H5_INDEX_NAME", ...
                                            "H5_ITER_NATIVE", 0, @var_func, ...
                                            {}, "H5P_DEFAULT");
endfunction

function [status, data_out] = var_func (obj_id, name, data_in)
  obj = H5O.open (obj_id, name, "H5P_DEFAULT");
  cls = var_class (obj);

  data_out = data_in;

  if (! isempty (cls))
    data_out = [data_out name];
  endif

  H5O.close (obj);

  status = 0;
endfunction

function val = read_dataset (obj_id)

  type_id = H5D.get_type (obj_id);
  dtype = H5LT.dtype_to_struct (type_id);
  H5T.close (type_id);

  val = H5D.read(obj_id, "H5ML_DEFAULT", "H5S_ALL", ...
                 "H5S_ALL", "H5P_DEFAULT");

  if (strcmp (dtype.Class, "H5T_REFERENCE"))
    refs = val;
    tmp = {};
    for jj = 1:numel (refs)

      obj_id = H5R.dereference (obj_id, "H5R_OBJECT", refs(jj));
      try
        tmp{jj} = get_object_data (obj_id);
      catch ee
        H5O.close (obj_id);
        rethrow (ee)
      end_try_catch

    endfor

    val = reshape (tmp, size (refs));
  endif
endfunction

function tf = is_sparse (id)

  tf = false;

  try
    attr_id = H5A.open (id, "MATLAB_sparse", "H5P_DEFAULT");
    tf = true;
    H5A.close (attr_id)
  catch
  end_try_catch

endfunction

function tf = is_empty (id)

  tf = false;

  try
    attr_id = H5A.open (id, "MATLAB_empty", "H5P_DEFAULT");
    tf = true;
    H5A.close (attr_id)
  catch
  end_try_catch

endfunction

function cls = var_class (id)

  try
    attr_id = H5A.open (id, "MATLAB_class", "H5P_DEFAULT");
    cls = H5A.read (attr_id);
    H5A.close (attr_id)
  catch
    cls = "";
  end_try_catch

endfunction

function ic = jc2ic (jc)
  nc = numel (jc) - 1;
  ic = zeros (jc(end),1);

  for jj = 1:nc
    ic((jc(jj)+1):(jc(jj+1))) = jj;
  endfor

endfunction

function val = get_object_data (obj_id)

  empty = is_empty (obj_id);

  cls = var_class (obj_id);

  switch cls
    case {"int8", "int16", "int32", "int64",...
          "uint8", "uint16", "uint32", "uint64"}

      val = read_dataset (obj_id);

      if (empty)
        val = zeros (val, cls);
      endif

    case {"double", "single"}

      if (! is_sparse (obj_id))
        val = read_dataset (obj_id);

        ## complex
        if (isstruct (val) && all (isfield (val, {"real", "imag"})))
          val = complex (val.real, val.imag);
        endif

        if (empty)
          val = zeros (val, cls);
        endif

      else
        val = read_vars (obj_id, {"data", "ir", "jc"}, true);

        ## complex
        if (isstruct (val.data) && all (isfield (val.data, {"real", "imag"})))
          val.data = complex (val.data.real, val.data.imag);
        endif

        ## Convert CSR format to row idx
        val = sparse (val.ir + 1, jc2ic (val.jc), val.data);

      endif

    case "char"

      val = read_dataset (obj_id);

      if (strcmp (class (val), "uint8"))
        ## ASCII
        val = char (val);
      else
        ## UTF-16
        type_id = H5D.get_type (obj_id);
        order = H5T.get_order (type_id);
        is_le = order == H5ML.get_constant_value ("H5T_ORDER_LE");
        H5T.close (type_id);
        tmpval = '';
        for ii = 1:rows (val)
          if (is_le)
            tmp = native2unicode(typecast (val(ii,:), "uint8"), ...
                                 "utf-16le");
          else
            tmp = native2unicode(typecast (val(ii,:), "uint8"), ...
                                 "utf-16be");
          endif
          tmpval = [tmpval; tmp];
        endfor
        val = tmpval;
      endif

      if (empty)
        val = '';
      endif

    case "logical"

      val = read_dataset (obj_id);

      if (empty)
        val = zeros (val, cls);
      else
        val = logical (val);
      endif

    case "struct"

      val = struct ();

      if (! empty)
        attr_id = H5A.open (obj_id, "MATLAB_fields", "H5P_DEFAULT");
        fields = H5A.read (attr_id);
        H5A.close (attr_id)

        try
          obj = H5O.open (obj_id, fields{1}, "H5P_DEFAULT");
          cls = var_class (obj);
          H5O.close (obj);
        catch
        end_try_catch

        if (isempty (cls))
          ## Struct arrays store data in datasets with no named class
          tmp = read_vars (obj_id, fields, true);

          args = {};
          for ii = 1:numel (fields)
            args{end+1} = fields{ii};
            args{end+1} = tmp.(fields{ii});
          endfor

          try
            val = struct (args{:});
          catch
            val = tmp;
          end_try_catch
        else
          val = read_vars (obj_id, fields);
        endif
      endif
    case "canonical empty"

      val = read_dataset (obj_id);
      val = resize ([], val);

    case "cell"

      val = read_dataset (obj_id);

      if (empty)
        val = resize ({}, val);
      endif

    otherwise
      warning ("read_mat73: unhandled class %s, returning data asis", ...
               cls)
  endswitch

endfunction

%!test
%! v7 = load ('base_types_mat7.mat', 'char_empty');
%! v73 = read_mat73 ('base_types_mat73.mat', 'char_empty');
%! assert (v7.char_empty, v73.char_empty)

%!test
%! v7 = load ('base_types_mat7.mat', 'char_vector');
%! v73 = read_mat73 ('base_types_mat73.mat', 'char_vector');
%! assert (v7.char_vector, v73.char_vector)

%!test
%! v7 = load ('base_types_mat7.mat', 'char_matrix');
%! v73 = read_mat73 ('base_types_mat73.mat', 'char_matrix');
%! assert (v7.char_matrix, v73.char_matrix)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_int8');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_int8');
%! assert (v7.empty_int8, v73.empty_int8)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_int16');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_int16');
%! assert (v7.empty_int16, v73.empty_int16)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_int32');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_int32');
%! assert (v7.empty_int32, v73.empty_int32)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_int64');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_int64');
%! assert (v7.empty_int64, v73.empty_int64)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_uint8');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_uint8');
%! assert (v7.empty_uint8, v73.empty_uint8)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_uint16');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_uint16');
%! assert (v7.empty_uint16, v73.empty_uint16)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_uint32');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_uint32');
%! assert (v7.empty_uint32, v73.empty_uint32)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_uint64');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_uint64');
%! assert (v7.empty_uint64, v73.empty_uint64)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_single');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_single');
%! assert (v7.empty_single, v73.empty_single)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_double');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_double');
%! assert (v7.empty_double, v73.empty_double)

%!test
%! v7 = load ('base_types_mat7.mat', 'empty_logical');
%! v73 = read_mat73 ('base_types_mat73.mat', 'empty_logical');
%! assert (v7.empty_logical, v73.empty_logical)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_int8');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_int8');
%! assert (v7.scalar_int8, v73.scalar_int8)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_int16');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_int16');
%! assert (v7.scalar_int16, v73.scalar_int16)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_int32');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_int32');
%! assert (v7.scalar_int32, v73.scalar_int32)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_int64');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_int64');
%! assert (v7.scalar_int64, v73.scalar_int64)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_uint8');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_uint8');
%! assert (v7.scalar_uint8, v73.scalar_uint8)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_uint16');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_uint16');
%! assert (v7.scalar_uint16, v73.scalar_uint16)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_uint32');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_uint32');
%! assert (v7.scalar_uint32, v73.scalar_uint32)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_uint64');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_uint64');
%! assert (v7.scalar_uint64, v73.scalar_uint64)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_double');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_double');
%! assert (v7.scalar_double, v73.scalar_double)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_single');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_single');
%! assert (v7.scalar_single, v73.scalar_single)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_logical');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_logical');
%! assert (v7.scalar_logical, v73.scalar_logical)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_int8');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_int8');
%! assert (v7.ndim_int8, v73.ndim_int8)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_int16');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_int16');
%! assert (v7.ndim_int16, v73.ndim_int16)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_int32');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_int32');
%! assert (v7.ndim_int32, v73.ndim_int32)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_int64');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_int64');
%! assert (v7.ndim_int64, v73.ndim_int64)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_uint8');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_uint8');
%! assert (v7.ndim_uint8, v73.ndim_uint8)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_uint16');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_uint16');
%! assert (v7.ndim_uint16, v73.ndim_uint16)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_uint32');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_uint32');
%! assert (v7.ndim_uint32, v73.ndim_uint32)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_uint64');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_uint64');
%! assert (v7.ndim_uint64, v73.ndim_uint64)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_double');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_double');
%! assert (v7.ndim_double, v73.ndim_double)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_single');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_single');
%! assert (v7.ndim_single, v73.ndim_single)

%!test
%! v7 = load ('base_types_mat7.mat', 'ndim_logical');
%! v73 = read_mat73 ('base_types_mat73.mat', 'ndim_logical');
%! assert (v7.ndim_logical, v73.ndim_logical)

%!test
%! v7 = load ('base_types_mat7.mat', 'sparse_double');
%! v73 = read_mat73 ('base_types_mat73.mat', 'sparse_double');
%! assert (v7.sparse_double, v73.sparse_double)

%!test
%! v7 = load ('base_types_mat7.mat', 'cplx_scalar_double');
%! v73 = read_mat73 ('base_types_mat73.mat', 'cplx_scalar_double');
%! assert (v7.cplx_scalar_double, v73.cplx_scalar_double)

%!test
%! v7 = load ('base_types_mat7.mat', 'cplx_scalar_single');
%! v73 = read_mat73 ('base_types_mat73.mat', 'cplx_scalar_single');
%! assert (v7.cplx_scalar_single, v73.cplx_scalar_single)

%!test
%! v7 = load ('base_types_mat7.mat', 'cplx_ndim_double');
%! v73 = read_mat73 ('base_types_mat73.mat', 'cplx_ndim_double');
%! assert (v7.cplx_ndim_double, v73.cplx_ndim_double)

%!test
%! v7 = load ('base_types_mat7.mat', 'cplx_ndim_single');
%! v73 = read_mat73 ('base_types_mat73.mat', 'cplx_ndim_single');
%! assert (v7.cplx_ndim_single, v73.cplx_ndim_single)

%!test
%! v7 = load ('base_types_mat7.mat', 'cell_any');
%! v73 = read_mat73 ('base_types_mat73.mat', 'cell_any');
%! assert (v7.cell_any, v73.cell_any)

%!test
%! v7 = load ('base_types_mat7.mat', 'cell_str');
%! v73 = read_mat73 ('base_types_mat73.mat', 'cell_str');
%! assert (v7.cell_str, v73.cell_str)

%!test
%! v7 = load ('base_types_mat7.mat', 'scalar_struct');
%! v73 = read_mat73 ('base_types_mat73.mat', 'scalar_struct');
%! assert (v7.scalar_struct.field1, v73.scalar_struct.field1)
%! assert (v7.scalar_struct.field2.field1, v73.scalar_struct.field2.field1)

%!test
%! v7 = load ('base_types_mat7.mat', 'struct_array');
%! v73 = read_mat73 ('base_types_mat73.mat', 'struct_array');
%! assert (v7.struct_array, v73.struct_array)

%% Test hability to read matio test file
%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var1');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var2');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var3');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var4');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var5');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var6');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var7');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var8');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var9');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var10');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var11');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var12');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var21');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var22');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var23');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var24');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var25');

%!xtest
%! matio_test_cases ('var26');

%!xtest
%! matio_test_cases ('var27');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var28');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var29');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var30');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var31');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var32');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var33');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var34');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var35');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var36');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var37');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var38');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var39');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var48');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var49');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var50');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var51');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var52');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var53');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var54');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var55');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var56');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var57');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var58');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var59');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var60');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var61');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var62');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var63');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var64');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var65');

%!xtest
%! matio_test_cases ('var66');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var69');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var70');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var71');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var72');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var73');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var74');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var75');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var76');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var77');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var80');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var81');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var90');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var91');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var92');

%!testif ; exist (file_in_loadpath ("matio_test_cases_uncompressed_hdf_le.mat"), "file")
%! matio_test_cases ('var93');
