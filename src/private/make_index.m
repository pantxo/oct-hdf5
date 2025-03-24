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
## @deftypefn {} {} make_index (@var{basedir})
## Write package index.
## @seealso{}
## @end deftypefn

function make_index (basedir)
  fname = fullfile (basedir, '..', "INDEX");
  dirs = {"H5A", "H5D", "H5E", "H5F", "H5G", "H5I", "H5L", "H5LT", ...
          "H5ML", "H5O", "H5P", "H5R", "H5S", "H5T"};

  hl_fun = {"h5info", "h5readatt", "read_mat73"};

  try
    delete (fname);
  catch
  end_try_catch

  fid = fopen (fname, "w+");
  fprintf (fid, "oct-hdf5 >> HDF5 functions\n");
  fprintf (fid, "High Level Functions\n");
  for ii = 1:numel (hl_fun)
    fprintf (fid, "  %s\n", hl_fun{ii});
  endfor

  addpath (fullfile (basedir, "..", "inst"));
  for ii = 1:numel (dirs)
    fprintf (fid, "%s\n", dirs{ii});
    fcn_list = methods (dirs{ii})(2:end);
    for jj = 1:numel (fcn_list)
      fprintf (fid, "  %s.%s\n", dirs{ii}, fcn_list{jj});
    endfor
  endfor

  fclose (fid);
endfunction
