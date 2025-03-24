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
## @deftypefn {} {} H5G
## Collection of pure static methods to manipulate hdf5 files.
## @seealso{}
## @end deftypefn
classdef H5G
  methods
    ## -*- texinfo -*-
    ## @deftypefn {} {} H5G
    ## Dummy class constructor.
    ## @seealso{}
    ## @end deftypefn
    function obj = H5G ()
      error ("H5G: directly call static functions");
    endfunction
  endmethods

  methods (Static = true)

    ## -*- texinfo -*-
    ## @deftypefn {} {} H5G.close (@var{group_id})
    ## Release resources used by a group which was opened by @code{H5G.create}
    ## or @code{H5G_OPEN}.
    ## 
    ## After closing a group, the group_id cannot be used again.
    ## 
    ## Failure to release a group with this call will result in resource leaks.
    ## 
    ## @seealso{H5G.open}
    ## @end deftypefn
    function close (varargin)
      __H5G_close__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{group_id} =} H5G.create (@var{loc_id}, @var{name})
    ## @deftypefnx {} {@var{group_id} =} H5G.create (@var{loc_id}, @var{name}, @var{lcpl_id}, @var{gcpl_id}, @var{gapl_id})
    ## Create a new group in a file.
    ## After a group has been created, links to datasets and to other groups can
    ## be added.
    ## 
    ## The @var{loc_id} and @var{name} parameters specify where the group is
    ## located. @var{loc_id} may be a file, group, dataset, named datatype or
    ## attribute in the file.  If an attribute, dataset, or named datatype is
    ## specified for @var{loc_id} then the group will be created at the location
    ## where the attribute, dataset, or named datatype is attached.
    ## @var{name} is the link to the group; @var{name} may be either an absolute
    ## path in the file (the links from the root group to the new group) or a
    ## relative path from @var{loc_id} (the link(s) from the group specified by
    ## @var{loc_id} to the new group).
    ## 
    ## @var{lcpl_id}, @var{gcpl_id}, and @var{gapl_id} (optional) are property list
    ## identifiers. These property lists govern how the link to the group is
    ## created, how the group is created, and how the group can be accessed in
    ## the future, respectively. The default is @qcode{'H5P_DEFAULT'}.
    ## Currently, there are no APIs for the group access property list;
    ## use @qcode{'H5P_DEFAULT'}.
    ## 
    ## To conserve and release resources, the group should be closed when access
    ## is no longer required.
    ## 
    ## @seealso{H5G.open,H5G.close}
    ## @end deftypefn
    function group_id = create (varargin)
      group_id = __H5G_create__ (varargin{:});
    endfunction

    ## -*- texinfo -*-
    ## @deftypefn {} {@var{group_id} =} H5G.open (@var{loc_id}, @var{name}, @var{gapl_id})
    ## Open an existing group, @var{name}, at the location specified by
    ##  @var{loc_id}.
    ## 
    ## @var{gapl_id} is the group access property list.
    ## 
    ## H5G.open returns a group identifier for the group that was opened.
    ## This group identifier should be released by calling @code{H5G.close} when
    ## it is no longer needed.
    ## 
    ## @seealso{H5G.close}
    ## @end deftypefn
    function group_id = open (varargin)
      group_id = __H5G_open__ (varargin{:});
    endfunction

  endmethods

endclassdef
