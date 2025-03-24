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
## @deftypefn {} {} H5LT
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5LT
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5LT
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5LT ()
      error ("H5LT: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{str} = } H5LT.dtype_to_struct (@var{type_id})
    ## @seealso{}
    ## @end deftypefn
    function str = dtype_to_struct (varargin)
      str = __H5LT_dtype_to_struct__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{str} = } H5LT.dtype_to_text (@var{type_id})
    ## @seealso{}
    ## @end deftypefn
    function str = dtype_to_text (varargin)
      str = __H5LT_dtype_to_text__ (varargin{:});
    endfunction

  endmethods

endclassdef
