## Copyright (C) 2023 Pantxo DIRIBARNE PD218333
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
## @deftypefn {} {} write_mat73 (@var{fname}, @var{var1}, @var{var2}, @dots{})
## @deftypefnx {} {} write_mat73 (@var{fname}, @qcode{"-append"), @var{var1}, @var{var2}, @dots{})
## Write variables to an HDF5 file @var{fname}, formated according to Matlab's
## v7.3 format.
## Variables @var{xxx} must be assigned in the calling scope and their
## name is used in the output file.
## @seealso{read_mat73}
## @end deftypefn

function retval = write_mat73 (fname, varargin)

  append = false;

  if (! ischar (fname) || numel (varargin) == 0)
    error ("write_mat73: FNAME must be a string")
  else
    ## Handle "-append" option
    if (strcmp (varargin{1}, "-append"))
      varargin(1)  = [];
      append = true;
      varidx++;
    endif

    if (numel (varargin) == 0)
      print_usage ()
    endif

    ## Check we have a name
    varnames = varargin;
    for ii = 1:numel (varargin)
      varnames{ii} = inputname (ii+1);
      if (isempty (varnames{ii}))
        error ("write_mat73: variable %d is not assigned", ii);
      endif
    endfor
  endif

  ## Try writing to file as a check for permissions
  fid = fopen (fname, "w");
  if (fid < 0)
    error ("write_mat73: unable to open '%s' for writting", fname)
  endif
  fclose (fid);

  if (! append)
    unlink (fname);
  endif;

  unwind_protect

    file = [];
    obj_id = [];
    attr_id = [];

    H5E.set_auto (false);

    ## Prepare file
    try
      if (! append)
        file = H5F.create (fname);
      else
        file = H5F.open (fname, "H5F_ACC_RDWR", "H5P_DEFAULT");
      endif
    catch
      rethrow_h5error ()
    end_try_catch


    for ii = 1:numel (varargin)
      varname = varnames{ii};
      varinfo = evalin ("caller", sprintf ("whos ('%s')", varname));
      write_var (file, varname, varargin{ii}, varinfo, file);
    endfor

  unwind_protect_cleanup
    if (! isempty (file))
      H5F.close (file);
    endif

    H5E.set_auto (true);
  end_unwind_protect

endfunction

function rethrow_h5error ()
  H5E.walk ("H5E_WALK_UPWARD", @error_walker);
endfunction

function status = error_walker (n, s)
  msg = [lasterr() " (" s.desc ")"];
  error (msg);
  status = -1;
endfunction

function write_var (parent_id, varname, var, varinfo, file, h5path = "")
  persistent has_refs = false;
  space_id = [];
  obj_id = [];
  
  empty_var = isempty (var);
  
  unwind_protect
    switch varinfo.class
      case {"int8","int16","int32","int64", ...
            "uint8","uint16","uint32","uint64"}
        if (! empty_var)
          rnk = ndims (var);
          sz = fliplr (size (var));
          type_id = ["H5T_NATIVE_" upper(varinfo.class)];
        else
          rnk = 1;
          sz = ndims (var);
          type_id = "H5T_NATIVE_UINT64";
          var = uint64 (size (var));
        endif

        space_id = H5S.create_simple (rnk, sz, sz);

        obj_id = H5D.create (parent_id, varname, type_id, space_id, ...
                             "H5P_DEFAULT");

        ## Write attributes
        write_matlab_class (obj_id, varinfo.class);
        if (empty_var)
          write_matlab_empty (obj_id);
        endif

        ## Write data
        H5D.write (obj_id, "H5ML_DEFAULT", "H5S_ALL", "H5S_ALL", ...
                   "H5P_DEFAULT", var);
        
      case {"double", "single"}
        issingle = strcmp (varinfo.class, "single");

        if (! empty_var)
          rnk = ndims (var);
          sz = fliplr (size (var));
          if (! issingle)
            type_id = "H5T_NATIVE_DOUBLE";
            tsize = 8;
          else
            type_id = "H5T_NATIVE_FLOAT";
            tsize = 4;
          endif

          if (varinfo.complex)
            sub_type_id = type_id;
            type_id = H5T.create ("H5T_COMPOUND", 2 * tsize);
            H5T.insert (type_id, "real", 0, sub_type_id);
            H5T.insert (type_id, "imag", tsize, sub_type_id);
          endif

        else
          rnk = 1;
          sz = ndims (var);
          type_id = "H5T_NATIVE_UINT64";
          var = uint64 (size (var));
        endif

        space_id = H5S.create_simple (rnk, sz, sz);

        obj_id = H5D.create (parent_id, varname, type_id, space_id, ...
                             "H5P_DEFAULT");

        ## Write attributes
        write_matlab_class (obj_id, varinfo.class);
        if (empty_var)
          write_matlab_empty (obj_id);
        endif

        ## Write data
        if (varinfo.complex)
          tmpvar = var;
          clear var;
          var.real = real (tmpvar);
          var.imag = imag (tmpvar);
          
          H5T.close (type_id);
        endif

        H5D.write (obj_id, "H5ML_DEFAULT", "H5S_ALL", "H5S_ALL", ...
                   "H5P_DEFAULT", var);          
        
      case "struct"
        disp ("writing struct")
      case "cell"        
        if (has_refs)
          refs_id = H5G.open (file, "/#refs#", "H5P_DEFAULT");
        else
          refs_id = H5G.create (file, "/#refs#");
          has_refs = true;
        endif
        
        unwind_protect            
          if (! empty_var)
            ## Create a dataset to be referenced for each element
            ref_names = cell (size (var));
            for ii = 1:numel (var)
              tmp = var{ii};
              name = next_ref_name ();
              ref_names{ii} = name;
              ref_info = whos ("tmp");
              write_var (refs_id, name, tmp, ref_info, file, ["/#refs#/" name]);
            endfor
            
            rnk = ndims (var);
            sz = fliplr (size (var));
            type_id = "H5T_STD_REF_OBJ";
          else
            rnk = 1;
            sz = ndims (var);
            type_id = "H5T_NATIVE_UINT64";
            var = uint64 (size (var));
          endif

          space_id = H5S.create_simple (rnk, sz, sz);

          obj_id = H5D.create (parent_id, varname, type_id, space_id, ...
                               "H5P_DEFAULT");

          refs = zeros (size (var), "uint64");
          for ii = 1:numel (ref_names)
            refs(ii) = H5R.create (refs_id, ref_names{ii}, 'H5R_OBJECT', -1);
          endfor
          H5D.write (obj_id, 'H5T_STD_REF_OBJ', 'H5S_ALL', 'H5S_ALL', ...
                     'H5P_DEFAULT', refs);
          
          ## Write attributes
          write_matlab_class (obj_id, varinfo.class);
          if (empty_var)
            write_matlab_empty (obj_id);
          endif
        unwind_protect_cleanup
          try
            H5G.close (refs_id);
          catch
          end_try_catch 
        end_unwind_protect
        
      case "char"
        if (! empty_var)
          rnk = ndims (var);
          sz = fliplr (size (var));
          type_id = "H5T_NATIVE_UINT16";
        else
          rnk = 1;
          sz = ndims (var);
          type_id = "H5T_NATIVE_UINT64";
          var = uint64 (size (var));
        endif

        space_id = H5S.create_simple (rnk, sz, sz);

        obj_id = H5D.create (parent_id, varname, type_id, space_id, ...
                             "H5P_DEFAULT");

        ## Write attributes
        write_matlab_class (obj_id, varinfo.class);
        write_int_decode (obj_id, 2);
        if (empty_var)
          write_matlab_empty (obj_id);
        endif

        ## Write data a utf-16
        sz = size (var);
        order = H5T.get_order (type_id);
        is_le = order == H5ML.get_constant_value ("H5T_ORDER_LE");
        if (is_le)
          var = typecast (unicode2native (var(:)', "utf-16le"), "uint16");
        else
          var = typecast (unicode2native (var(:)', "utf-16be"), "uint16");
        endif
        if (rows (var)>1)
          ## FIXME: This won't work if any character is non-ascii because
          ## sz is not the number of actual characters but the number of bytes
          ## (eventually padded) to represent glyphs
          var = reshape (var, sz);
        endif
        H5D.write (obj_id, "H5ML_DEFAULT", "H5S_ALL", "H5S_ALL", ...
                   "H5P_DEFAULT", var);
    endswitch

    ## Write H5PATH
    if (! isempty (h5path))
      write_h5path (obj_id, h5path);
    endif
  unwind_protect_cleanup
    if (! isempty (space_id))
      H5S.close (space_id);
    endif

    if (! isempty (obj_id))
      H5O.close (obj_id);
    endif
  end_unwind_protect
endfunction

function write_matlab_class (parent_id, cls)
  ## Create type id
  type_id = H5T.copy ("H5T_C_S1");
  H5T.set_size (type_id, numel (cls));
  H5T.set_cset (type_id, "H5T_CSET_ASCII");

  ## Create scalar space id
  space_id = H5S.create ("H5S_SCALAR");

  try
    attr_id = H5A.create (parent_id, "MATLAB_class", type_id, ...
                          space_id, "H5P_DEFAULT", "H5P_DEFAULT");
  catch
    rethrow_h5error ()
  end_try_catch

  try
    H5A.write (attr_id, "H5ML_DEFAULT", cls);
    H5A.close (attr_id);
  catch
    rethrow_h5error ()
  end_try_catch

  H5S.close (space_id);
  H5T.close (type_id);
endfunction

function write_h5path (parent_id, pth)
  ## Create type id
  type_id = H5T.copy ("H5T_C_S1");
  H5T.set_size (type_id, numel (pth));
  H5T.set_cset (type_id, "H5T_CSET_ASCII");

  ## Create scalar space id
  space_id = H5S.create ("H5S_SCALAR");

  try
    attr_id = H5A.create (parent_id, "H5PATH", type_id, ...
                          space_id, "H5P_DEFAULT", "H5P_DEFAULT");
  catch
    rethrow_h5error ()
  end_try_catch
  
  try
    H5A.write (attr_id, "H5ML_DEFAULT", pth);
    H5A.close (attr_id);
  catch
    rethrow_h5error ()
  end_try_catch
  
  H5S.close (space_id);
  H5T.close (type_id);
endfunction

function write_int_decode (parent_id, val)
  ## Create type id
  type_id = "H5T_NATIVE_INT32";

  ## Create scalar space id
  space_id = H5S.create ("H5S_SCALAR");

  try
    attr_id = H5A.create (parent_id, "MATLAB_int_decode", type_id, ...
                          space_id, "H5P_DEFAULT", "H5P_DEFAULT");
  catch
    rethrow_h5error ()
  end_try_catch
  
  try
    H5A.write (attr_id, "H5ML_DEFAULT", val);
    H5A.close (attr_id);
  catch
    rethrow_h5error ()
  end_try_catch
  
  H5S.close (space_id);
endfunction

function write_matlab_empty (parent_id)
  ## Create type id
  type_id = H5T.copy ("H5T_NATIVE_UINT8");

  ## Create scalar space id
  space_id = H5S.create ("H5S_SCALAR");

  try
    attr_id = H5A.create (parent_id, "MATLAB_empty", "H5T_NATIVE_UINT8", ...
                          space_id, "H5P_DEFAULT", "H5P_DEFAULT");
  catch
    rethrow_h5error ()
  end_try_catch

  try
    H5A.write (attr_id, "H5ML_DEFAULT", uint8 (1));
    H5A.close (attr_id);
  catch
    rethrow_h5error ()
  end_try_catch
  
  H5S.close (space_id);
  H5T.close (type_id);
endfunction

function name = next_ref_name ()
  persistent pidx = 0;
  persistent alphabet = char ([97:122]);
  name = "";
  idx = pidx;
  while idx >= 0
    n = mod (idx, 26);
    name = [alphabet(n+1) name];
    idx = floor (idx / 26)-1;
  endwhile
  pidx++;
endfunction

  
