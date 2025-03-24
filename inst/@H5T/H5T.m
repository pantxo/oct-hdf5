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
## @deftypefn {} {} H5T
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5T
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5T
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5T ()
      error ("H5T: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{type_id} =} H5T.array_create (@var{base_type_id}, @var{dims})
    ## @deftypefnx {} {@var{type_id} =} H5T.array_create (@var{base_type_id}, @var{rank}, @var{dims})
    ## 
    ## Create a new array datatype object.
    ## 
    ## @var{base_type_id} is the datatype of every element of the array, i.e.,
    ## of the number at each position in the array.
    ## 
    ## @var{rank} is the number of dimensions and the size of each dimension is
    ## specified in the array @var{dims}.  The value of rank is currently limited
    ## to @qcode{'H5S_MAX_RANK'} and must be greater than 0 (zero).  All dimension
    ## sizes specified in @var{dims} must be greater than 0 (zero).
    ## 
    ## @seealso{H5T.close}
    ## @end deftypefn
    function type_id = array_create (varargin)
      type_id = __H5T_array_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5T.close (@var{type_id})
    ## @seealso{}
    ## @end deftypefn
    function close (varargin)
      __H5T_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{type_id} =} H5T.copy (@var{type_id})
    ## @seealso{}
    ## @end deftypefn
    function type_id = copy (varargin)
      type_id = __H5T_copy__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {@var{type_id}} H5T.create (@var{class}, @var{size})
    ## Create a new datatype of the specified class with the specified number 
    ## of bytes.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{class} @tab @tab Class of datatype among @qcode{'H5T_COMPOUND'}, 
    ## @qcode{'H5T_OPAQUE'}, @qcode{'H5T_ENUM'}, @qcode{'H5T_STRING'}
    ##  @item @var{size} @tab @tab Size, in bytes, of the datatype being created
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5T_CREATE}.
    ## 
    ## @seealso{H5T.open,H5T.close}
    ## @end deftypefn
    function type_id = create (varargin)
      type_id = __H5T_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{class_id} =} H5T.get_class (@var{type_id})
    ## Get the class of the data type specified by @var{type_id}.
    ## 
    ## The output can be @qcode{'H5T_INTEGER'}, @qcode{'H5T_FLOAT'},
    ## @qcode{'H5T_STRING'}, @qcode{'H5T_BITFIELD'}, @qcode{'H5T_OPAQUE'},
    ## @qcode{'H5T_COMPOUND'}, @qcode{'H5T_REFERENCE'}, @qcode{'H5T_ENUM'},
    ## @qcode{'H5T_VLEN'}, or @qcode{'H5T_ARRAY'}.
    ## @seealso{}
    ## @end deftypefn
    function class_id = get_class (varargin)
      class_id = __H5T_get_class__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{byte_order} =} H5T.get_order (@var{type_id})
    ## Get the byte order of the data type specified by @var{type_id}.
    ## 
    ## The output can be @qcode{'H5T_ORDER_LE'}, @qcode{'H5T_ORDER_BE'},
    ## @qcode{'H5T_ORDER_VAX'}, @qcode{'H5T_ORDER_MIXED'},
    ## or @qcode{'H5T_ORDER_NONE'}.
    ## @seealso{H5T.set_order}
    ## @end deftypefn
    function byte_order = get_order (varargin)
      byte_order = __H5T_get_order__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{sz} =} H5T.get_size (@var{type_id})
    ## @seealso{}
    ## @end deftypefn
    function sz = get_size (varargin)
      sz = __H5T_get_size__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {} H5T.insert (@var{parent_id}, @var{name}, @var{offset}, @var{member_id})
    ## Adds a new member to a compound datatype.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{parent_id} @tab @tab Datatype identifier
    ##  @item @var{name} @tab @tab Name of the field to insert
    ##  @item @var{offset} @tab @tab Offset in memory structure of the field to insert
    ##  @item @var{member_id} @tab @tab Datatype identifier of the field to insert
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___c_o_m_p_o_u_n_d.html}.
    ## 
    ## @seealso{H5T.open,H5T.close}
    ## @end deftypefn
    function insert (varargin)
      __H5T_insert__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5T.set_cset (@var{type_id}, @var{charset})
    ## @var{charset} must be @qcode{\"H5T_CSET_ASCII\"} 
    ## or @qcode{\"H5T_CSET_UTF8\"}.
    ## @seealso{}
    ## @end deftypefn
    function set_cset (varargin)
      __H5T_set_cset__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5T.set_order (@var{type_id}, @var{byte_order})                       
    ## Set the byte order of the data type specified by @var{type_id}.
    ## 
    ## The input @var{byte_order} can be @qcode{'H5T_ORDER_LE'},
    ## @qcode{'H5T_ORDER_BE'}, or @qcode{'H5T_ORDER_VAX'}.
    ## @seealso{H5T.get_order}
    ## @end deftypefn
    function set_order (varargin)
      __H5T_set_order__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5T.set_size (@var{type_id}, @var{size})
    ## @var{size} is the size in bytes of the type or @qcode{\"H5T_VARIABLE\"}.
    ## @seealso{}
    ## @end deftypefn
    function set_size (varargin)
      __H5T_set_size__ (varargin{:});
    endfunction

  endmethods

endclassdef
