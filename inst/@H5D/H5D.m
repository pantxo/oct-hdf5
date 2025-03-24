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
## @deftypefn {} {} H5D
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5D
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5D
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5D ()
      error ("H5D: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} { } H5D.close (@var{dataset_id})
    ## End access to a dataset specified by @var{dataset_id} and releases resources
    ## used by it.
    ## 
    ## Further use of the dataset identifier is illegal in calls to
    ## the dataset API.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{dataset_id} @tab @tab Dataset to release access to
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_CLOSE}.
    ## 
    ## @seealso{H5D.open,H5D.create}
    ## @end deftypefn
    function close (varargin)
      __H5D_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{dataset_id} =} H5D.create (@var{loc_id}, @var{name}, @var{dtype_id}, @var{space_id}, @var{lcpl_id})
    ## @deftypefnx {} {@var{dataset_id} =} H5D.create (@var{loc_id}, @var{name}, @var{dtype_id}, @var{space_id}, @var{lcpl_id}, @var{dcpl_id}, @var{dapl_id})
    ## Create a dataset, @var{name}, which is attached to the object
    ## specified by the identifier @var{loc_id}.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{loc_id} @tab @tab Location or object identifier
    ##  @item @var{name} @tab @tab Dataset name
    ##  @item @var{dtype_id} @tab @tab Datatype identifier
    ##  @item @var{space_id} @tab @tab Dataspace identifier
    ##  @item @var{lcpl_id} @tab @tab Link creation property list identifier
    ##  @item @var{dcpl_id} @tab @tab Dataset creation property list identifier
    ##  @item @var{dapl_id} @tab @tab Dataset access property list identifier
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_CREATE2}.
    ## 
    ## @seealso{H5D.open,H5D.close}
    ## @end deftypefn
    function dataset_id = create (varargin)
      dataset_id = __H5D_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{dcpl_id} = } H5D.get_create_plist (@var{dataset_id})
    ## Return an identifier for a copy of the dataset creation property list
    ## associated with the dataset specified by @var{dataset_id}.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{dataset_id} @tab @tab Dataset identifier
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_CREATE_PLIST}.
    ## 
    ## @seealso{H5P.close}
    ## @end deftypefn
    function dcpl_id = get_create_plist (varargin)
      dcpl_id = __H5D_get_create_plist__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{space_id} = } H5D.get_space (@var{dataset_id})
    ## Make a copy of the dataspace of the dataset specified by @var{dataset_id}.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{dataset_id} @tab @tab Identifier of a dataset
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## The dataspace identifier returned from this function must be 
    ## released with H5S.close or resource leaks will develop.
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_SPACE}.
    ## 
    ## @seealso{H5S.close}
    ## @end deftypefn
    function space_id = get_space (varargin)
      space_id = __H5D_get_space__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{type_id} = } H5D.get_type (@var{dataset_id})
    ## Return an identifier for a copy of the datatype for a dataset.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{dataset_id} @tab @tab Identifier of a dataset
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## The datatype identifier @var{type_id} returned from this function must
    ## be released with H5T.close or resource leaks will develop.
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_GET_TYPE}.
    ## 
    ## @seealso{H5T.close}
    ## @end deftypefn
    function type_id = get_type (varargin)
      type_id = __H5D_get_type__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{dataset_id} =} H5D.open (@var{loc_id}, @var{name}, @var{dapl_id})
    ## Open the existing dataset specified by a location identifier and name,
    ## @var{loc_id} and @var{name}, respectively.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{loc_id} @tab @tab Location identifier; may be a file, group, 
    ## named datatype, dataset, or attribute
    ##  @item @var{name} @tab @tab Dataset name
    ##  @item @var{dapl_id} @tab @tab Dataset access property list
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## The dataset identifier @var{dataset_id} returned from this function must
    ## be released with H5D.close or resource leaks will develop.
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_OPEN}.
    ## 
    ## @seealso{H5D.close}
    ## @end deftypefn
    function dataset_id = open (varargin)
      dataset_id = __H5D_open__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{data} = } H5D.read (@var{dataset_id})
    ## @deftypefnx {} {@var{data} = } H5D.read (@var{dataset_id}, @var{mem_type_id}, @var{mem_space_id}, @var{file_space_id}, @var{xfer_plist_id})
    ## Import data from dataset.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{dataset_id} @tab @tab Location or dataset identifier
    ##  @item @var{mem_type_id} @tab @tab Target datatype (use @code{H5ML_DEFAULT} 
    ## for automatic conversion)
    ##  @item @var{mem_space_id} @tab @tab Imported data dataspace identifier or 
    ## @code{H5S_ALL}
    ##  @item @var{file_space_id} @tab @tab Original data dataspace identifier or 
    ## @code{H5S_ALL}
    ##  @item @var{xfer_plist_id} @tab @tab Transfer property list identifier or 
    ## @code{H5P_DEFAULT}
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## Only data with atomic HDF5 data types are handled. Unless @var{mem_type_id} 
    ## is specified or different from 'H5ML_DEFAULT', automatic conversion to 
    ## Octave representation is as follows:
    ## @table @asis
    ## @item H5T_INTEGER
    ## Corresponding integer type, e.g. H5T_STD_I8BE in file is converted 
    ## to H5T_NATIVE_INT8 in memory and returned as a @code{int8} class array.
    ## @item H5T_FLOAT
    ## Corresponding floating point type, e.g. H5T_IEEE_F32LE in file is converted 
    ## to H5T_NATIVE_FLOAT in memory and returned as a @code{single} class array.
    ## @item H5T_STRING
    ## Returned as a char array.
    ## @item H5T_REFERENCE
    ## Returned as a @code{int64} class array containing the reference 
    ## identifier(s).
    ## See @code{H5T.dereference} for how to retrieve the actual object identifier.
    ## @item H5T_COMPOUND
    ## Returned as a struct with fields corresponding to field names in the compound 
    ## data type.
    ## @item H5T_VLEN
    ## Only H5T_STRING base types are handled. Returned as a char array.
    ## @end table
    ## 
    ## See original function at 
    ## @url{https://portal.hdfgroup.org/display/HDF5/H5D_READ}.
    ## 
    ## @seealso{H5D.open,H5A.read,H5T.dereference}
    ## @end deftypefn
    function data = read (varargin)
      data = __H5D_read__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5D.write (@var{dataset_id}, @var{mem_type_id}, @var{mem_space_id}, @var{file_space_id}, @var{xfer_plist_id}, @var{data})
    ## @seealso{}
    ## @end deftypefn
    function write (varargin)
      __H5D_write__ (varargin{:});
    endfunction

  endmethods

endclassdef
