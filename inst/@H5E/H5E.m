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
## @deftypefn {} {} H5E
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5E
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5E
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5E ()
      error ("H5E: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{cname} =} H5E.get_class_name (@var{class_id})
    ## @seealso{H5E.walk}
    ## @end deftypefn
    function cname = get_class_name (varargin)
      cname = __H5E_get_class_name__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{err_msg} =} H5E.get_major (@var{maj_num})
    ## @seealso{H5E.walk}
    ## @end deftypefn
    function err_msg = get_major (varargin)
      err_msg = __H5E_get_major__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{msg} =} H5E.get_msg (@var{msg_id})
    ## @seealso{H5E.walk}
    ## @end deftypefn
    function msg = get_msg (varargin)
      msg = __H5E_get_msg__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5E.oclear ()
    ## @seealso{H5E.walk}
    ## @end deftypefn
    function oclear (varargin)
      __H5E_oclear__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5E.set_auto (@var{tf})
    ## @seealso{H5E.walk}
    ## @end deftypefn
    function set_auto (varargin)
      __H5E_set_auto__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5E.walk (@var{direction}, @var{fcn})        
    ## @seealso{H5E.clear}                                           
    ## @end deftypefn
    function walk (varargin)
      __H5E_walk__ (varargin{:});
    endfunction

  endmethods

endclassdef
