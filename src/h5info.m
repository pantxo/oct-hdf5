## Copyright (C) 2021 Pantxo Diribarne
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
## @deftypefn {} {@var{s} =} h5info (@var{fname}, @var{path})
##
## @seealso{}
## @end deftypefn

## Author: Pantxo Diribarne <pantxo@dibona>
## Created: 2021-05-09

function s = h5info (fname, obj_name = "/")

  unwind_protect
    H5E.set_auto (false);
    file = H5F.open (fname, "H5F_ACC_RDONLY", "H5P_DEFAULT");

    debug_print (false);

    ## Begin iteration.
    debug_print ("Objects in root group:\n");

    ## Get object info for the root group
    infobuf = H5O.get_info_by_name (file, obj_name, "H5P_DEFAULT");

    [status, s] = op_func (file, obj_name, group_struct ("__g__", []));

    ## Remove fake container group
    if (! isempty (s.Groups))
      s = s.Groups;
    else
      s = s.Datasets;
    endif
  unwind_protect_cleanup
    ## Close and release resources.
    if (exist ("file", "var"))
      H5F.close (file);
    endif

    ## Walk the error stack to print the last error from hdf5
    H5E.walk ("H5E_WALK_UPWARD", @error_walker);

    ## Restore previous error printing
    H5E.set_auto (false);
  end_unwind_protect
endfunction

function status = error_walker (n, s)
  disp (s.desc)
  status = -1;
endfunction

function str = class_id_to_string (class_id)
  switch class_id
    case H5ML.get_constant_value ('H5T_INTEGER')
      str = 'H5T_INTEGER';
    case H5ML.get_constant_value ('H5T_FLOAT')
      str = 'H5T_FLOAT';
    case H5ML.get_constant_value ('H5T_STRING')
      str = 'H5T_STRING';
    case H5ML.get_constant_value ('H5T_BITFIELD')
      str = 'H5T_BITFIELD';
    case H5ML.get_constant_value ('H5T_OPAQUE')
      str = 'H5T_OPAQUE';
    case H5ML.get_constant_value ('H5T_COMPOUND')
      str = 'H5T_COMPOUND';
    case H5ML.get_constant_value ('H5T_REFERENCE')
      str = 'H5T_REFERENCE';
    case H5ML.get_constant_value ('H5T_ENUM')
      str = 'H5T_ENUM';
    case H5ML.get_constant_value ('H5T_VLEN')
      str = 'H5T_VLEN';
    case H5ML.get_constant_value ('H5T_ARRAY')
      str = 'H5T_ARRAY';
    otherwise
      error ("class_id_to_string: unknown datatype class %d", class_id)
  endswitch
endfunction

function debug_print (varargin)

  persistent debug = true;

  if (numel (varargin) && islogical (varargin{1}))
    debug = varargin{1};
  elseif (debug)
    printf (varargin{:})
  endif

endfunction

function s = group_struct (name, addr = 0)
  s = struct ("Name", name, "Groups", [], "Datasets", [], "Datatypes", [], ...
              "Links", [], "Attributes", [], "addr", addr);
endfunction

function s = dataset_struct (name)
  s = struct ("Name", name, "Datatype", datatype_struct (""), ...
              "Dataspace", dataspace_struct (), ...
              "ChunkSize", [], "FillValue", 0, "Filters", [], ...
              "Attributes", []);
endfunction

function s = attribute_struct (name)
  s = struct ("Name", name, "Datatype", datatype_struct (""), ...
              "Dataspace", dataspace_struct (), "Value", []);
endfunction

function s = datatype_struct (name)
  s = struct ("Name", name, "Class", "", "Type", "", "Size", 0, ...
              "Attributes", []);
endfunction

function s = dataspace_struct ()
  s = struct ("Size", [], "MaxSize", [], "Type", "simple");
endfunction

function status = group_check (parent_addr, target_addr)
  status = 0;

  if (any (parent_addr == target_addr))
    status = 1; # addr match
  endif
endfunction

function [status, od_out] = attr_op_func (loc_id, name, od_in)

  attr_id = H5A.open (loc_id, name, "H5P_DEFAULT");

  as = attribute_struct (name);

  ## Type info
  type_id = H5A.get_type (attr_id);
  as.Datatype = H5LT.dtype_to_struct (type_id);
  H5T.close (type_id);

  ## Space info
  space_id = H5A.get_space (attr_id);
  [n, dims, maxdims] = H5S.get_simple_extent_dims (space_id);
  space_type = H5S.get_simple_extent_type (space_id);

  switch (space_type)
    case H5ML.get_constant_value ("H5S_SIMPLE")
      as.Dataspace.Size = dims;
      as.Dataspace.MaxSize = maxdims;
    case H5ML.get_constant_value ("H5S_NULL")
      as.Dataspace.Size = dims;
      as.Dataspace.MaxSize = maxdims;
      as.Dataspace.Type = "null";
    case H5ML.get_constant_value ("H5S_SCALAR")
      as.Dataspace.Size = 1;
      as.Dataspace.MaxSize = 1;
      as.Dataspace.Type = "scalar";
  endswitch

  H5S.close (space_id);

  if (isempty (od_in))
    od_in = as;
  else
    od_in(end+1) = as;
  endif

  status = 0;
  od_out = od_in;
endfunction

function [status, od_out] = op_func (loc_id, name, od_in)

  status = 0;

  spaces = 2*(numel (od_in.addr) + 1);
  debug_print ("%*s", spaces, "");

  infobuf = H5O.get_info_by_name (loc_id, name, "H5P_DEFAULT");

  switch (infobuf.type)
    case H5ML.get_constant_value ("H5O_TYPE_GROUP")
      debug_print ("  Group: %s {\n", name);

      ## Iterate through the links of this group
      if (group_check (od_in.addr, infobuf.addr))
        debug_print ("%*s  Warning: Loop detected!\n", spaces, "");
      else
        child_od = group_struct (name, [od_in.addr infobuf.addr]);
        [status, ii, cod_out] = H5L.iterate_by_name (loc_id, ...
                                                     name, ...
                                                     "H5_INDEX_NAME", ...
                                                     "H5_ITER_NATIVE", ...
                                                     0, ...
                                                     @op_func,...
                                                     child_od, ...
                                                     "H5P_DEFAULT");


        ## Attributes
        [status, ii, att_out] = H5A.iterate_by_name (loc_id, name, ...
                                                     "H5_INDEX_NAME", ...
                                                     "H5_ITER_NATIVE", 0,
                                                     @attr_op_func, [],...
                                                     "H5P_DEFAULT");

        cod_out.Attributes = att_out;

        od_in.Groups(end+1) = cod_out;
      endif

      debug_print ("%*s}\n", spaces, "");

    case H5ML.get_constant_value ("H5O_TYPE_DATASET")

      debug_print ("  Dataset: %s\n", name);

      ds = dataset_struct (name);

      ## Type info
      dset_id = H5D.open (loc_id, name, "H5P_DEFAULT");
      type_id = H5D.get_type (dset_id);

      class_id = H5T.get_class (type_id);
      ## ds.Datatype.Class = class_id_to_string (class_id);
      ## ds.Datatype.Type = H5LT.dtype_to_text (type_id);
      ## ds.Datatype.Size = H5T.get_size (type_id);
      ds.Datatype = H5LT.dtype_to_struct (type_id);

      H5T.close (type_id);

      ## Space info
      space_id = H5D.get_space (dset_id);
      [n, dims, maxdims] = H5S.get_simple_extent_dims (space_id);

      space_type = H5S.get_simple_extent_type (space_id);

      switch (space_type)
        case H5ML.get_constant_value ("H5S_SIMPLE")
          ds.Dataspace.Size = dims;
          ds.Dataspace.MaxSize = maxdims;
        case H5ML.get_constant_value ("H5S_NULL")
          ds.Dataspace.Size = dims;
          ds.Dataspace.MaxSize = maxdims;
          ds.Dataspace.Type = "null";
        case H5ML.get_constant_value ("H5S_SCALAR")
          ds.Dataspace.Size = 1;
          ds.Dataspace.MaxSize = 1;
          ds.Dataspace.Type = "scalar";
      endswitch

      H5S.close (space_id);

      ## Chunk size
      dcpl_id = H5D.get_create_plist (dset_id);
      chunk_dims = [];
      if (H5P.get_layout (dcpl_id) == H5ML.get_constant_value ("H5D_CHUNKED"))
        [~,chunk_dims] = H5P.get_chunk (dcpl_id);
      endif

      ds.ChunkSize = chunk_dims;

      ## Fill value
      fill_id = H5P.fill_value_defined (dcpl_id);
      if (fill_id == H5ML.get_constant_value ("H5D_FILL_VALUE_USER_DEFINED"))
        ## FIXME: implement this
      endif

      H5P.close (dcpl_id);
      H5D.close (dset_id);

      ## Attributes
      [status, ii, att_out] = H5A.iterate_by_name (loc_id, name, ...
                                                   "H5_INDEX_NAME", ...
                                                   "H5_ITER_NATIVE", 0,
                                                   @attr_op_func, [], ...
                                                   "H5P_DEFAULT");
      ds.Attributes = att_out;

      od_in.Datasets(end+1) = ds;

    case H5ML.get_constant_value ("H5O_TYPE_NAMED_DATATYPE")
      debug_print ("  Datatype: %s\n", name);

    otherwise
      debug_print ( "  Unknown: %s\n", name);
  endswitch

  od_out = od_in;
endfunction