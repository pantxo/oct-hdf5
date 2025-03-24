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
## @deftypefn {} {} H5L
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5L
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5L
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5L ()
      error ("H5L: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5L.iterate (@var{group_id}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in})
    ## @seealso{}
    ## @end deftypefn
    function [status, idx_out, opdata_out] = iterate (varargin)
      [status, idx_out, opdata_out] = __H5L_iterate__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5L.iterateiterate_by_name (@var{group_id}, @var{name}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in}, @var{lapl_id})
    ## @seealso{}
    ## @end deftypefn
    function [status, idx_out, opdata_out] = iterate_by_name (varargin)
      [status, idx_out, opdata_out] = __H5L_iterate_by_name__ (varargin{:});
    endfunction

  endmethods

endclassdef
