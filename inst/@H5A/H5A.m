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
## @deftypefn {} {} H5A
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5A
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5A
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5A ()
      error ("H5A: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} { } H5A.close (@var{attr_id})
    ## Terminates access to the attribute specified by @var{attr_id}.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{attr_id} @tab @tab Attribute to release access to
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5A.open}
    ## @end deftypefn
    function close (varargin)
      __H5A_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id})
    ## @deftypefnx {oct-hdf5} {@var{attribute_id} =} H5A.create (@var{loc_id}, @var{attr_name}, @var{type_id}, @var{space_id}, @var{acpl_id}, @var{aapl_id},)
    ## Create an attribute, @var{attr_name}, which is attached to the object
    ## specified by the identifier @var{loc_id}.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{loc_id} @tab @tab Location or object identifier
    ##  @item @var{attr_name} @tab @tab Attribute name
    ##  @item @var{space_id} @tab @tab Attribute dataspace identifier
    ##  @item @var{acpl_id} @tab @tab Attribute creation property list identifier
    ##  @item @var{aapl_id} @tab @tab Attribute access property list identifier
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5A.open,H5A.close}
    ## @end deftypefn
    function attribute_id = create (varargin)
      attribute_id = __H5A_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {@var{attr_count} =} H5A.get_num_attrs (@var{loc_id})
    ## Return the number of attributes if successful; otherwise return a negative 
    ## value.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{loc_id} @tab @tab Identifier of a group, dataset, or named 
    ## datatype
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @end deftypefn
    function attr_count = get_num_attrs (varargin)
      attr_count = __H5A_get_num_attrs__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {@var{space_id} = } H5A.get_space (@var{attr_id})
    ## Retrieve a copy of the dataspace for an attribute @var{attr_id}.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{attr_id} @tab @tab Identifier of an attribute
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## The dataspace identifier returned from this function must be 
    ## released with H5S.close or resource leaks will develop.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5A.open}
    ## @end deftypefn
    function space_id = get_space (varargin)
      space_id = __H5A_get_space__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {@var{type_id} = } H5A.get_type (@var{attr_id})
    ## Return a datatype identifier if successful; otherwise returns a negative 
    ## value.
    ## 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{attr_id} @tab @tab Identifier of an attribute
    ##  @end multitable
    ## 
    ## 
    ## @strong{Description:}
    ## 
    ## The datatype identifier @var{type_id} returned from this function must
    ## be released with H5T.close or resource leaks will develop.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5T.close}
    ## @end deftypefn
    function type_id = get_type (varargin)
      type_id = __H5A_get_type__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5A.iterate (@var{loc_id}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in})
    ## Iterate over the attributes attached to the dataset or group specified
    ## with @var{loc_id}.
    ## 
    ## For each attribute, user-provided data, @var{op_data}, with additional
    ## information as defined below, is passed to a user-defined function, 
    ## @var{fcn}, which operates on that attribute.
    ## 
    ## The order of the iteration and the attributes iterated over are specified
    ## by three parameters: the index type, @var{idx_type}; the order in which
    ## the index is to be traversed, @var{order}; and the attribute’s position
    ## in the index, @var{idx_in}.
    ## 
    ## The type of index specified by @var{idx_type} can be one of the following:
    ## @table @asis
    ## @item @qcode{'H5_INDEX_NAME'}
    ## An alpha-numeric index by attribute name.
    ## @item @qcode{'H5_INDEX_CRT_ORDER'}
    ## An index by creation order.
    ## @end table
    ## 
    ## The order in which the index is to be traversed, as specified by @var{order},
    ## can be one of the following:
    ## @table @asis
    ## @item @qcode{'H5_ITER_INC'}
    ## Iteration is from beginning to end, i.e., a top-down iteration incrementing
    ## the index position at each step.
    ## @item @qcode{'H5_ITER_DEC'}
    ## Iteration starts at the end of the index, i.e., a bottom-up iteration
    ## decrementing the index position at each step.
    ## @item @qcode{'H5_ITER_NATIVE'}
    ## HDF5 iterates in the fastest-available order. No information is provided
    ## as to the order, but HDF5 ensures that each element in the index will be
    ## visited if the iteration completes successfully.
    ## @end table
    ## 
    ## The next attribute to be operated on is specified by @var{idx_in},
    ## a position in the index. The index of the last attibute operated on is
    ## returned as @var{idx_out}
    ## 
    ## For example, if @var{idx_type}, @var{order}, and @var{idx_in} are set to
    ## @qcode{'H5_INDEX_NAME'}, @qcode{'H5_ITER_INC'}, and @code{5}, respectively,
    ## the attribute in question is the fifth attribute from the beginning of the
    ## alpha-numeric index of attribute names.  If order were set to
    ## @qcode{'H5_ITER_DEC'}, it would be the fifth attribute from the end of
    ## the index.
    ## 
    ## The prototype for the @var{fcn} callback function is as follows:
    ## @group
    ## @code{[@var{status}, @var{op_data_out}] = op_fcn (@var{location_id},
    ## @var{attr_name}, @var{op_data_in})}
    ## @end group
    ## 
    ## The operation receives the location identifier @var{location_id} for the
    ## group or dataset being iterated over; the name of the current object
    ## dataset or attribute, @var{attr_name}; and the operator data passed into
    ## iterate function, @var{op_data_in}.
    ## 
    ## Valid return values @var{status} from an operator and the resulting
    ## H5A.iterate_by_name and @var{fcn} behavior are as follows:
    ## @table @asis
    ## @item @code{@var{status} == 0}
    ## Causes the iterator to continue, returning zero when all attributes have
    ## been processed.
    ## @item @code{@var{status} > 0}
    ## Causes the iterator to immediately return that positive value, indicating
    ## short-circuit success. The iterator can be restarted at the next attribute,
    ## as indicated by the return value @var{idx_out}.
    ## @item @code{@var{status} < 0}
    ## Causes the iterator to immediately return that value, indicating failure.
    ## The iterator can be restarted at the next attribute, as indicated by the
    ## return value of @var{idx_out}.
    ## @end table
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5A.iterate_by_name}
    ## @end deftypefn
    function [status, idx_out, opdata_out] = iterate (varargin)
      [status, idx_out, opdata_out] = __H5A_iterate__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {[@var{status}, @var{idx_out}, @var{opdata_out}] =} H5A.iterate_by_name (@var{loc_id}, @var{name}, @var{idx_type}, @var{order}, @var{idx_in},  @var{fcn}, @var{opdata_in}, @var{lapd_id})
    ## Iterate over the attributes attached to the dataset or group specified
    ## with @var{loc_id} and @var{obj_name}.
    ## 
    ## For each attribute, user-provided data, @var{op_data}, with additional
    ## information as defined below, is passed to a user-defined function, 
    ## @var{fcn}, which operates on that attribute.
    ## 
    ## If @var{loc_id} fully specifies the object to which these attributes are
    ## attached, @var{obj_name} should be '.' (a dot).
    ## 
    ## The order of the iteration and the attributes iterated over are specified
    ## by three parameters: the index type, @var{idx_type}; the order in which
    ## the index is to be traversed, @var{order}; and the attribute’s position
    ## in the index, @var{idx_in}.
    ## 
    ## The type of index specified by @var{idx_type} can be one of the following:
    ## @table @asis
    ## @item @qcode{'H5_INDEX_NAME'}
    ## An alpha-numeric index by attribute name.
    ## @item @qcode{'H5_INDEX_CRT_ORDER'}
    ## An index by creation order.
    ## @end table
    ## 
    ## The order in which the index is to be traversed, as specified by @var{order},
    ## can be one of the following:
    ## @table @asis
    ## @item @qcode{'H5_ITER_INC'}
    ## Iteration is from beginning to end, i.e., a top-down iteration incrementing
    ## the index position at each step.
    ## @item @qcode{'H5_ITER_DEC'}
    ## Iteration starts at the end of the index, i.e., a bottom-up iteration
    ## decrementing the index position at each step.
    ## @item @qcode{'H5_ITER_NATIVE'}
    ## HDF5 iterates in the fastest-available order. No information is provided
    ## as to the order, but HDF5 ensures that each element in the index will be
    ## visited if the iteration completes successfully.
    ## @end table
    ## 
    ## The next attribute to be operated on is specified by @var{idx_in},
    ## a position in the index. The index of the last attibute operated on is
    ## returned as @var{idx_out}
    ## 
    ## For example, if @var{idx_type}, @var{order}, and @var{idx_in} are set to
    ## @qcode{'H5_INDEX_NAME'}, @qcode{'H5_ITER_INC'}, and @code{5}, respectively,
    ## the attribute in question is the fifth attribute from the beginning of the
    ## alpha-numeric index of attribute names.  If order were set to
    ## @qcode{'H5_ITER_DEC'}, it would be the fifth attribute from the end of
    ## the index.
    ## 
    ## The prototype for the @var{fcn} callback function is as follows:
    ## @group
    ## @code{[@var{status}, @var{op_data_out}] = op_fcn (@var{location_id},
    ## @var{attr_name}, @var{op_data_in})}
    ## @end group
    ## 
    ## The operation receives the location identifier @var{location_id} for the
    ## group or dataset being iterated over; the name of the current object
    ## dataset or attribute, @var{attr_name}; and the operator data passed into
    ## iterate function, @var{op_data_in}.
    ## 
    ## Valid return values @var{status} from an operator and the resulting
    ## H5A.iterate_by_name and @var{fcn} behavior are as follows:
    ## @table @asis
    ## @item @code{@var{status} == 0}
    ## Causes the iterator to continue, returning zero when all attributes have
    ## been processed.
    ## @item @code{@var{status} > 0}
    ## Causes the iterator to immediately return that positive value, indicating
    ## short-circuit success. The iterator can be restarted at the next attribute,
    ## as indicated by the return value @var{idx_out}.
    ## @item @code{@var{status} < 0}
    ## Causes the iterator to immediately return that value, indicating failure.
    ## The iterator can be restarted at the next attribute, as indicated by the
    ## return value of @var{idx_out}.
    ## @end table
    ## 
    ## The link access property list, @var{lapl_id}, may provide information
    ## regarding the properties of links required to access the object,
    ## @var{obj_name}.  See 'Link Access Properties' in the H5P APIs.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## @seealso{H5A.iterate}
    ## @end deftypefn
    function [status, idx_out, opdata_out] = iterate_by_name (varargin)
      [status, idx_out, opdata_out] = __H5A_iterate_by_name__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {oct-hdf5} {@var{attr_id} =} H5A.open (@var{obj_id}, @var{name}, @var{aapl_id})
    ## Open an existing attribute, @var{name}, that is attached to an object
    ## specified by an object identifier, @var{obj_id}.
    ## 
    ## The attribute access property list, @var{aapl_id}, is currently unused
    ## and should be @qcode{'H5P_DEFAULT'}.
    ## 
    ## This function, must be called before an attribute can be accessed for any
    ## further purpose, including reading, writing, or any modification.
    ## 
    ## The attribute identifier @var{attr_id} returned by this function must be
    ## released with H5A.close or resource leaks will develop.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## @seealso{H5A.close}
    ## @end deftypefn
    function attr_id = open (varargin)
      attr_id = __H5A_open__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{attr_id} =} H5A.open_name (@var{loc_id}, @var{name})
    ## Open an attribute specified by its name, @var{name}, which is attached to
    ## the object specified with @var{loc_id}. 
    ## 
    ## The location object may be either a group, dataset, or named datatype,
    ## which may have any sort of attribute.
    ## 
    ## The attribute identifier @var{attr_id} returned by this function must be
    ## released with H5A.close or resource leaks will develop.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5A.close}
    ## @end deftypefn
    function attr_id = open_name (varargin)
      attr_id = __H5A_open_name__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{data} = } H5A.read (@var{attr_id})
    ## @deftypefnx {} {@var{data} = } H5A.read (@var{attr_id}, @var{mem_type_id})
    ## Read an attribute specified with @var{attr_id}.
    ## 
    ## Import data from dataset.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{attr_id} @tab @tab Location or attribute identifier
    ##  @item @var{mem_type_id} @tab @tab Target datatype (use @code{H5ML_DEFAULT} 
    ## for automatic conversion)
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## Only data with atomic HDF5 data types are handled. See @code{help H5D.read} 
    ## for details about automatic data type conversion.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5D.read}
    ## @end deftypefn
    function data = read (varargin)
      data = __H5A_read__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5A.write (@var{attribute_id}, @var{mem_type_id}, @var{data})
    ## Write an attribute, specified with @var{attribute_id}.
    ## 
    ## The attribute's memory datatype is specified with @var{mem_type_id}.
    ## The entire attribute is written to the file.
    ## 
    ## If @var{mem_type_id} is either a fixed-length or variable-length string,
    ## it is important to set the string length when defining the datatype.
    ## String datatypes are derived from @qcode{'H5T_C_S1'} (or
    ## @qcode{'H5T_FORTRAN_S1'} for Fortran), which defaults to 1 character
    ## in size. See @code{H5T.set_size}.
    ## 
    ## Datatype conversion takes place at the time of a read or write and is
    ## automatic.
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_a.html}.
    ## 
    ## @seealso{H5A.read}
    ## @end deftypefn
    function write (varargin)
      __H5A_write__ (varargin{:});
    endfunction

  endmethods

endclassdef
