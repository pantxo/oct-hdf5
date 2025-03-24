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
## @deftypefn {} {} H5I
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5I
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5I
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5I ()
      error ("H5I: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{info_struct} =} H5I.get_name (@var{obj_id})
    ## Retrieve a name for the object identified by @var{obj_id}.
    ## 
    ## @seealso{}
    ## @end deftypefn
    function info_struct = get_name (varargin)
      info_struct = __H5I_get_name__ (varargin{:});
    endfunction

  endmethods

endclassdef
