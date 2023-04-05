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

function rdata = read_mat73 (fname, path = "")

  if (! ischar (path))
    error ("read_mat73: PATH must be a variable name or empty string")
  endif

  path = ["/" path];

  try
    s = h5info (fname, path);
  catch
    error ("read_mat73: reading datasets. %s", lasterr ())
  end_try_catch


  file = H5F.open (fname);

  if (strcmp (path, "/"))
    dsets = s.Datasets;
  else
    dsets = s;
    dsets.Name = dsets.Name(2:end);

    isgroup = isfield (s, "Groups");

    if (isgroup)
      error ("read_mat73: groups are unhandled")
    endif
  endif

  file = H5F.open (fname);

  rdata = struct ();

  unwind_protect
    for ii = 1:numel (dsets)
      if (! any (strcmp ({dsets(ii).Attributes.Name}, "MATLAB_class")))
        error ("read_mat73: variable doesn't seem to be Matlab v7.3")
      endif

      dset_name = dsets(ii).Name;

      dset = H5D.open (file, dset_name, "H5P_DEFAULT");

      try
        ## printf ("Reading %s of type %s:", dset_name, dsets(ii).Datatype.Class)
        tmp = H5D.read (dset, "H5ML_DEFAULT", "H5S_ALL", "H5S_ALL", ...
                        "H5P_DEFAULT");

        if (strcmp (dsets(ii).Datatype.Class, "H5T_REFERENCE"))
          refs = tmp;
          tmp = {};
          for jj = 1:numel (refs)
            ref = refs(jj);

            dset_ref = H5R.dereference (dset, "H5R_OBJECT", refs(jj));

            try
              tmp = [tmp, H5D.read(dset_ref, "H5ML_DEFAULT", "H5S_ALL", ...
                                   "H5S_ALL", "H5P_DEFAULT")];
            catch ee
              H5D.close (dset_ref);
              rethrow (ee)
            end_try_catch

          endfor

          tmp = reshape (tmp, size (refs));
        endif

        ## printf (" retval is of type %s\n", class (tmp));
        rdata.(dset_name) = reinterpret (tmp, dset);
      catch
        H5D.close (dset);
      end_try_catch
    endfor
  unwind_protect_cleanup
    H5F.close (file);
  end_unwind_protect

endfunction

function val = reinterpret (val, dset)

  attr_id = H5A.open (dset, "MATLAB_class", "H5P_DEFAULT");
  cls = H5A.read (attr_id);

  iscplx = isstruct (val) && all (isfield (val, {"real", "imag"}));

  switch cls
    case {"double", "single", ...
          "int8", "int16", "int32", "int64",...
          "uint8", "uint16", "uint32", "uint64"}
      if (iscplx)
        val = complex (val.real, val.imag);
      endif
    otherwise
      warning ("read_mat73: unhandled class %s, returning data asis", ...
               cls)
  endswitch

endfunction
