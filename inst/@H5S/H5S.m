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
## @deftypefn {} {} H5S
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5S
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5S
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5S ()
      error ("H5S: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5S.close (@var{space_id})
    ## @seealso{}
    ## @end deftypefn
    function close (varargin)
      __H5S_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{space_id} =} create (@var{type})
    ## 
    ## Create a new dataspace of a particular type.
    ## 
    ## Currently supported types are as follows: @qcode{'H5S_SCALAR'},
    ## @qcode{'H5S_SIMPLE'}, and @qcode{'H5S_NULL'}. Further dataspace types
    ## may be added later.
    ## 
    ## A scalar dataspace, @qcode{'H5S_SCALAR'}, has a single element,
    ## though that element may be of a complex datatype, such as a compound
    ## or array datatype. By convention, the rank of a scalar dataspace is
    ## always 0 (zero); think of it geometrically as a single, dimensionless point,
    ## though that point can be complex.
    ## 
    ## A simple dataspace, @var{H5S_SIMPLE}, consists of a regular array
    ## of elements.
    ## 
    ## A null dataspace, @var{H5S_NULL}, has no data elements.
    ## 
    ## The dataspace identifier returned by this function can be released
    ## with @code{H5S.close} so that resource leaks will not occur.
    ## @seealso{H5S.close}
    ## @end deftypefn
    function space_id = create (varargin)
      space_id = __H5S_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{space_id} =} create_simple (@var{rank}, @var{dims}, @var{max_dims}) 
    ## Create a new simple dataspace and open it for access, returning a dataspace 
    ## identifier.
    ## 
    ## @var{rank} is the number of dimensions used in the dataspace.
    ## 
    ## @var{dims} is a one-dimensional array of size @var{rank} specifying the 
    ## size of each dimension of the dataset. maximum_dims is an array of the same 
    ## size specifying the upper limit on the size of each dimension.
    ## 
    ## Any element of @var{dims} can be 0 (zero). Note that no data can be written to 
    ## a dataset if the size of any dimension of its current dataspace is 0. This is 
    ## sometimes a useful initial state for a dataset.
    ## 
    ## @var{max_dims} may be an empty matrix, in which case the upper limit is the 
    ## same as @var{dims}. Otherwise, no element of @var{max_dims} should be smaller 
    ## than the corresponding element of @var{dims}.
    ## 
    ## @var{max_dims} may also be \"H5S_UNLIMITED\", in which case all dimensions 
    ## are unlimited.
    ## 
    ## Any dataset with an unlimited dimension must also be chunked. 
    ## See @code{H5P.set_chunk}. Similarly, a dataset must be chunked if @var{dims} 
    ## does not equal @var{max_dims}.
    ## 
    ## The dataspace identifier returned from this function must be released with 
    ## @code{H5S.close} or resource leaks will occur.
    ## @seealso{H5S.close}
    ## @end deftypefn
    function space_id = create_simple (varargin)
      space_id = __H5S_create_simple__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {[@var{ndims}, @var{dims}, @var{maxdims}] = } H5S.get_simple_extent_dims (@var{space_id})
    ## @seealso{}
    ## @end deftypefn
    function [ndims, dims, maxdims] = get_simple_extent_dims (varargin)
      [ndims, dims, maxdims] = __H5S_get_simple_extent_dims__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{space_type} =} H5S.get_simple_extent_type (@var{space_id})
    ## n
    ## 
    ## Determine the current class @var{space_type} of a dataspace @var{space_id}.
    ## 
    ## @seealso{H5S.get_simple_extent_dims}
    ## @end deftypefn
    function space_type = get_simple_extent_type (varargin)
      space_type = __H5S_get_simple_extent_type__ (varargin{:});
    endfunction

  endmethods

endclassdef
