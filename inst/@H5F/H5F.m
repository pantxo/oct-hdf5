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
## @deftypefn {} {} H5F
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5F
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5F
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5F ()
      error ("H5F: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5F.close (@var{file_id})
    ## Terminates access to an HDF5 file.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{file_id} @tab @tab File identifier
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_f.html}.
    ## 
    ## @seealso{H5F.open}
    ## @end deftypefn
    function close (varargin)
      __H5F_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{fid} =} H5F.create (@var{fname})
    ## @deftypefnx {} {@var{fid} =} H5F.create (@var{fname}, @var{flags})
    ## @deftypefnx {} {@var{fid} =} H5F.create (@var{fname}, @var{flags}, @var{fcpl_id})
    ## @deftypefnx {} {@var{fid} =} H5F.create (@var{fname}, @var{flags}, @var{fcpl_id}, @var{fapl_id})
    ## Creates an HDF5 file.
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{fname} @tab @tab Name of the file to create
    ##  @item @var{flags} @tab @tab File access flags. One of @{@qcode{"H5F_ACC_TRUNC"} | @qcode{"H5F_ACC_EXCL"}@}. The default is @qcode{"H5F_ACC_TRUNC"}.
    ##  @item @var{fcpl_id} @tab @tab File creation property list identifier.  The default is @qcode{"H5P_DEFAULT"}.
    ##  @item @var{fapl_id} @tab @tab File access property list identifier.  The default is @qcode{"H5P_DEFAULT"}.
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_f.html}.
    ## 
    ## @seealso{H5F.open, H5F.close}
    ## @end deftypefn
    function fid = create (varargin)
      fid = __H5F_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{fid} =} H5F.open (@var{fname})
    ## @deftypefnx {} {@var{fid} =} H5F.open (@var{fname}, @var{flags})
    ## @deftypefnx {} {@var{fid} =} H5F.open (@var{fname}, @var{flags}, @var{fapl_id})
    ## Opens an existing HDF5 file. 
    ## 
    ## @strong{Parameters:}
    ##  @multitable @columnfractions 0.33 0.02 0.65
    ##  @item @var{fname} @tab @tab Name of the file to be opened 
    ##  @item @var{flags} @tab @tab File access flags. One of @{@qcode{"H5F_ACC_RDWR"} | @qcode{"H5F_ACC_RDONLY"} | @qcode{"H5F_ACC_RDONLY|H5F_ACC_SWMR_READ"} | @qcode{"H5F_ACC_RDWR|H5F_ACC_SWMR_WRITE"}@}. The default is @qcode{"H5F_ACC_RDONLY"}.
    ##  @item @var{fapl_id} @tab @tab File access property list identifier. The default is @qcode{"H5P_DEFAULT"}.
    ##  @end multitable
    ## 
    ## @strong{Description:}
    ## 
    ## See original function at 
    ## @url{https://support.hdfgroup.org/documentation/hdf5/latest/group___h5_f.html}.
    ## 
    ## @seealso{H5F.close}
    ## @end deftypefn
    function fid = open (varargin)
      fid = __H5F_open__ (varargin{:});
    endfunction

  endmethods

endclassdef
