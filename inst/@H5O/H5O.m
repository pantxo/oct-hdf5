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
## @deftypefn {} {} H5O
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5O
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5O
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5O ()
      error ("H5O: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5O.close (@var{object_id})
    ## Closes the object, dataset, or named datatype specified by @var{object_id}.
    ## 
    ## This function is the companion to @code{H5O.open}, and has the same
    ## effect as calling @code{H5G.close}, @code{H5D.close}, or @code{H5T.close}.
    ## 
    ## @code{H5O.close} is not used to close a dataspace, attribute, property list,
    ## or file.
    ## 
    ## @seealso{H5O.open}
    ## @end deftypefn
    function close (varargin)
      __H5O_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{info_struct} =} H5O.get_info_by_name (@var{loc_id}, @var{name}, @var{lapl_id})
    ## The @var{info_struct} structure contains fields:
    ## fileno;    /* File number that object is located in 
    ## addr;      /* Object address in file 
    ## type;      /* Basic object type (group, dataset, etc.) 
    ## rc;        /* Reference count of object    
    ## atime;     /* Access time            
    ## mtime;     /* Modification time      
    ## ctime;     /* Change time            
    ## btime;     /* Birth time         
    ## num_attrs; /* # of attributes attached to object 
    ## 
    ## @seealso{}
    ## @end deftypefn
    function info_struct = get_info_by_name (varargin)
      info_struct = __H5O_get_info_by_name__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{dataset_id} =} H5O.open (@var{loc_id}, @var{name}, @var{lapl_id})
    ## Open a group, dataset, or committed (named) datatype specified by a location, 
    ## @var{loc_id}, and a path name, @var{name}
    ## 
    ## @seealso{H5O.close}
    ## @end deftypefn
    function dataset_id = open (varargin)
      dataset_id = __H5O_open__ (varargin{:});
    endfunction

  endmethods

endclassdef
