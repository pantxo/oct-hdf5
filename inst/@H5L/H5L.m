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
    ## Iterates over links in a group, with user callback routine, according
    ## to the order within an index.
    ##
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{group_id} @tab @tab Group identifier
    ##  @item @var{idx_type} @tab @tab Index type. One of @{@qcode{"H5_INDEX_NAME"} | @qcode{"H5_INDEX_CRT_ORDER"}@}
    ##  @item @var{order} @tab @tab Iteration order. One of @{@qcode{"H5_ITER_INC"} | @qcode{"H5_ITER_DEC"} | @qcode{"H5_ITER_NATIVE"}@}
    ##  @item @var{idx_in} @tab @tab Iteration index to allow continuing a previous iteration
    ##  @item @var{fcn} @tab @tab Callback function
    ##  @item @var{opdata_in} @tab @tab Callback data
    ##  @end multitable
    ##
    ##
    ## @strong{Return values:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{status} @tab @tab Negative value if the iteration were interrupted
    ##  @item @var{idx_out} @tab @tab Final iteration index
    ##  @item @var{opdata_out} @tab @tab Returned callback data
    ##  @end multitable
    ##
    ## 
    ## @strong{Description:}
    ##
    ## The user callback function signature must be:
    ## @example
    ## @code{[@var{status}, @var{opdata_out}] = fcn (@var{loc_id}, @var{name}, @var{opdata_in})}
    ## @end example
    ## It receives and identifier for the currently visited object @var{loc_id},
    ## its name @var{name} and the data @var{opdata_in} as returned by the
    ## previous iteration.
    ## The function can return a negative @var{status} to instruct
    ## @code{H5L.iterate} to stop iterations. Gathered data are returned in
    ## @var{opdata_out}.
    ##
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___t_r_a_v.html}.
    ## 
    ## @seealso{H5L.iterate_by_name}
    ## @end deftypefn
    function [status, idx_out, opdata_out] = iterate (varargin)
      [status, idx_out, opdata_out] = __H5L_iterate__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5L.iterateiterate_by_name (@var{group_id}, @var{name}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in}, @var{lapl_id})
    ## Iterates over links in a named group, with user callback routine,
    ## according
    ## to the order within an index.
    ##
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{group_id} @tab @tab Group identifier
    ##  @item @var{name} @tab @tab Group name
    ##  @item @var{idx_type} @tab @tab Index type. One of @{@qcode{"H5_INDEX_NAME"} | @qcode{"H5_INDEX_CRT_ORDER"}@}
    ##  @item @var{order} @tab @tab Iteration order. One of @{@qcode{"H5_ITER_INC"} | @qcode{"H5_ITER_DEC"} | @qcode{"H5_ITER_NATIVE"}@}
    ##  @item @var{idx_in} @tab @tab Iteration index to allow continuing a previous iteration
    ##  @item @var{fcn} @tab @tab Callback function
    ##  @item @var{opdata_in} @tab @tab Callback data
    ##  @item @var{lapl_id} @tab @tab Link access property list identifier.
    ##  @end multitable
    ##
    ##
    ## @strong{Return values:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{status} @tab @tab Negative value if the iteration were interrupted
    ##  @item @var{idx_out} @tab @tab Final iteration index
    ##  @item @var{opdata_out} @tab @tab Returned callback data
    ##  @end multitable
    ##
    ## 
    ## @strong{Description:}
    ##
    ## The user callback function signature must be:
    ## @example
    ## @code{[@var{status}, @var{opdata_out}] = fcn (@var{loc_id}, @var{name}, @var{opdata_in})}
    ## @end example
    ## It receives and identifier for the currently visited object @var{loc_id},
    ## its name @var{name} and the data @var{opdata_in} as returned by the
    ## previous iteration.
    ## The function can return a negative @var{status} to instruct
    ## @code{H5L.iterate} to stop iterations. Gathered data are returned in
    ## @var{opdata_out}.
    ##
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___t_r_a_v.html}.
    ## 
    ## @seealso{H5L.iterate_by_name}
    ## @end deftypefn
    function [status, idx_out, opdata_out] = iterate_by_name (varargin)
      [status, idx_out, opdata_out] = __H5L_iterate_by_name__ (varargin{:});
    endfunction

  endmethods

endclassdef
