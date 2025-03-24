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
## @deftypefn {} {} H5R
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5R
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5R
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5R ()
      error ("H5R: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{ref} =} H5R.create (@var{loc_id}, @var{name}, @var{ref_type}, @var{space_id})
    ## Create the reference, @var{ref}, of the type specified in @var{ref_type},
    ## pointing to the object name located at @var{loc_id}.
    ## 
    ## @var{ref_type} must be one of @qcode{'H5R_OBJECT'} or
    ## @qcode{'H5R_DATASET_REGION'}.
    ## 
    ## The parameters @var{loc_id} and name are used to locate the object.
    ## 
    ## The parameter @var{space_id} identifies the dataset region that a dataset
    ## region reference points to. This parameter is used only with dataset region
    ## references and should be set to -1 if the reference is an object reference,
    ## @qcode{'H5R_OBJECT'}.
    ## 
    ## @seealso{H5R.open,H5R.close}
    ## @end deftypefn
    function ref = create (varargin)
      ref = __H5R_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{ref_obj_id} = } H5R.dereference (@var{obj_id}, @var{ref_type}, @var{ref})
    ## Given a reference, @var{ref}, to an object or a region in an object,
    ## open that object and return an identifier.
    ## 
    ## The parameter @var{obj_id} must be a valid identifier for the HDF5 file
    ## containing the referenced object or for any object in that HDF5 file.
    ## 
    ## The parameter @var{ref_type} specifies the reference type of the reference
    ## @var{ref}. @var{ref_type} may contain either of the following values:
    ## @qcode{'H5R_OBJECT'} or @qcode{'H5R_DATASET_REGION'}.
    ## 
    ## The object opened with this function should be closed when it is no longer
    ## needed so that resource leaks will not develop. Use the appropriate close
    ## function such as @code{H5O.close} or @code{H5D.close} for datasets.
    ## 
    ## @seealso{H5O.close}
    ## @end deftypefn
    function ref_obj_id = dereference (varargin)
      ref_obj_id = __H5R_dereference__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{obj_type} = } H5R.get_obj_type (@var{loc_id}, @var{ref_type}, @var{ref})
    ## 
    ## @seealso{H5O.close}
    ## @end deftypefn
    function obj_type = get_obj_type (varargin)
      obj_type = __H5R_get_obj_type__ (varargin{:});
    endfunction

  endmethods

endclassdef
