## Copyright (C) 2021-2025 Pantxo Diribarne
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
## @deftypefn {} {} H5P
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5P
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5P
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5P ()
      error ("H5P: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5P.close (@var{plist})
    ## @seealso{}
    ## @end deftypefn
    function close (varargin)
      __H5P_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{plist_id} =} H5P.create (@var{class_id})
    ## @seealso{}
    ## @end deftypefn
    function plist_id = create (varargin)
      plist_id = __H5P_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{fill_val_id} = } H5P.fill_value_defined (@var{plist_id}) 
    ## @seealso{H5D.get_create_plist}
    ## @end deftypefn
    function fill_val_id = fill_value_defined (varargin)
      fill_val_id = __H5P_fill_value_defined__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {[@var{rank}, @var{dims}] = } H5P.get_chunk (@var{plist_id})
    ## @seealso{H5D.get_crreate_plist}
    ## @end deftypefn
    function [rank, dims] = get_chunk (varargin)
      [rank, dims] = __H5P_get_chunk__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{layout_id} = } H5P.get_layout (@var{plist_id}) 
    ## @seealso{H5D.get_create_plist}
    ## @end deftypefn
    function layout_id = get_layout (varargin)
      layout_id = __H5P_get_layout__ (varargin{:});
    endfunction

  endmethods

endclassdef
