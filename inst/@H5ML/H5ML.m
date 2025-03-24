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
## @deftypefn {} {} H5ML
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5ML
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5ML
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5ML ()
      error ("H5ML: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{tf} =} H5ML.compare_values (@var{val1}, @var{val2})
    ## @seealso{}
    ## @end deftypefn
    function tf = compare_values (varargin)
      tf = __H5ML_compare_values__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{names} =} H5ML.get_constant_names ()
    ## @seealso{}
    ## @end deftypefn
    function names = get_constant_names (varargin)
      names = __H5ML_get_constant_names__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{retval} =} H5ML.get_constant_value (@var{name})
    ## @seealso{}
    ## @end deftypefn
    function retval = get_constant_value (varargin)
      retval = __H5ML_get_constant_value__ (varargin{:});
    endfunction

  endmethods

endclassdef
