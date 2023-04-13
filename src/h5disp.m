## Copyright (C) 2023 Vipul Cariappa
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
## @deftypefn  {} {} h5disp @var{filename}
## @deftypefnx  {} {} h5disp @var{filename}, @var{loc}
## @deftypefnx  {} {} h5disp @var{filename}, @var{loc}, @var{mode}
## Displays the metadata and structure of hdf5 file given by @var{filename}
## 
## @var{loc} can be used to specify the location, default is "/"
## 
## There are two @var{mode} "min" and "simple", default is "simple".
## Use "min" to get minimised output
## 
## @end deftypefn

## Author: Vipul Cariappa <vipulcariappa@gmail.com>
## Created: 2023-04-05

function h5disp (filename, loc = "/", mode = "simple")
  if (nargin < 1)
    print_usage ();
  endif

  if (strcmp(loc, "/") ~= 1)
    error ("Use of different group is not yet implemented")
  endif
  
  if (strcmp(mode, "simple") ~= 1)
    error ("Only default (simple) mode is implemented")
  endif

  unwind_protect
    H5E.set_auto (false);
    file = H5F.open (filename, "H5F_ACC_RDONLY", "H5P_DEFAULT");

    ## Get object info for the root group
    infobuf = H5O.get_info_by_name (file, "/", "H5P_DEFAULT");

    oddata = get_oddata (0, 0, infobuf.addr);

    fprintf ("Group '/'\n");
    [status, li, od_out] = H5L.iterate (file,
                                        "H5_INDEX_NAME",
                                        "H5_ITER_NATIVE",
                                        0,
                                        @op_func,
                                        oddata);

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

function od_out = disp_group (grp_id, name, od_in, infobuf)
  od_out = get_oddata (od_in.recurs + 1, od_in, infobuf.addr);

  spaces = blanks (2 * (od_in.recurs + 1));

  fprintf ("%sGroup '%s'\n", spaces, name);

  attr_count = H5A.get_num_attrs (grp_id);
  if (attr_count > 0)
    disp_attrs (grp_id, od_out);
  endif

  od_out = H5L.iterate_by_name (grp_id, 
                                name,
                                "H5_INDEX_NAME",
                                "H5_ITER_NATIVE",
                                0,
                                @op_func,
                                od_out,
                                "H5P_DEFAULT");

endfunction

function od_out = disp_dataset (grp_id, name, od_in, infobuf)
  od_out = get_oddata (od_in.recurs + 1, od_in, infobuf.addr);

  spaces = blanks (2 * (od_in.recurs + 1));

  dset_id = H5D.open (grp_id, name, "H5P_DEFAULT");

  fprintf ("%sDataSet '%s'\n", spaces, name);

  # display attribute info
  attr_count = H5A.get_num_attrs (dset_id);
  if (attr_count > 0)
    disp_attrs (dset_id, od_out);
  endif

  space_id = H5D.get_space (dset_id);
  type_id = H5D.get_type (dset_id);

  type_str = H5LT.dtype_to_text (type_id);
  fprintf ("%s Datatype: %s\n", spaces, type_str);

  switch H5S.get_simple_extent_type (space_id)
    case 0 # scalar
      fprintf ("%s Size: Scalar\n", spaces);
    
    case 1 # simple
      [ndim, dims, maxdims] = H5S.get_simple_extent_dims (space_id);
      
      for i = 1:ndim
        if (i == 1)
          fprintf ("%s Size: %d", spaces, dims(i));
        else
          fprintf ("x%d", dims(i));
        endif
      endfor
      
      fprintf("\n");
      
      for i = 1:ndim
        if (i == 1)
          fprintf ("%s MaxSize: %d", spaces, maxdims(i));
        else
          fprintf ("x%d", maxdims(i));
        endif
      endfor
      
      fprintf("\n");

    case 2 # null
      fprintf ("%s Size: Null\n", spaces);
  endswitch

  H5D.close (dset_id);
endfunction

function od_out = disp_attrs (loc_id, od_in)
  spaces = blanks (2 * (od_in.recurs + 1));
  
  fprintf ("%sAttributes:\n", spaces);

  od = get_oddata (od_in.recurs + 1, od_in, od_in.addr);

  [status, idx, od_out] = H5A.iterate (loc_id,
                                        "H5_INDEX_NAME",
                                        "H5_ITER_NATIVE",
                                        0,
                                        @op_attr_func,
                                        od);
endfunction

function [status, od_out] = op_func (loc_id, name, od_in)
  infobuf = H5O.get_info_by_name (loc_id, name, "H5P_DEFAULT");
  
  if (infobuf.type == 0) # GROUP
    od_out = disp_group (loc_id, name, od_in, infobuf);
  elseif (infobuf.type == 1) # DATASET
    od_out = disp_dataset (loc_id, name, od_in, infobuf);
  else
  endif

  od_out = od_in;
  status = 0;
endfunction

function [status, od_out] = op_attr_func (loc_id, attr_name, od_in)
  spaces = blanks (2 * (od_in.recurs + 1));

  fprintf ("%s%s: ", spaces, attr_name);

  attr_id = H5A.open(loc_id, attr_name, "H5P_DEFAULT");
  attr = H5A.read(attr_id);

  disp (attr);

  od_out = od_in;
  status = 0;
endfunction

function status = error_walker (n, s)
  disp (s.desc)
  status = -1;
endfunction

function oddata = get_oddata (recurs, prev, addr)
  oddata = struct ("recurs", recurs, "prev", prev, "addr", addr);
endfunction
