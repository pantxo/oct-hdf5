## Copyright (C) 2021 Pantxo Diribarne
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
## @deftypefn {} {@var{val} =} h5readatt (@var{fname}, @var{loc}, @var{attr})
## Read data associated to a group or dataset attribute @var{attr}, at location
## @var{loc}.
##
## The location @var{loc} must be a string indicating the path to the
## related group or dataset.
## @example
## ## Read attribute "color" in dataset "triangle" located at the root of an hdf5 file
## color = h5readatt ("some_file.h5", "/triangle", "color");
## @end example
## @seealso{h5info}
## @end deftypefn

function data = h5readatt (fname, path, attr)

  if (nargin != 3)
    print_usage ()
  elseif (! exist (fname, "file"))
    error ("h5readatt: FNAME must be an existing file name")
  endif

  unwind_protect

    file = [];
    obj_id = [];
    attr_id = [];

    H5E.set_auto (false);
    try
      file = H5F.open (fname, "H5F_ACC_RDONLY", "H5P_DEFAULT");
    catch
      rethrow_h5error ()
    end_try_catch

    try
      obj_id = H5O.open (file, path, "H5P_DEFAULT");
    catch
      rethrow_h5error ()
    end_try_catch

    try
      attr_id = H5A.open (obj_id, attr, "H5P_DEFAULT");
    catch
      rethrow_h5error ()
    end_try_catch

    try
      data = H5A.read (attr_id);
    catch
      rethrow_h5error ()
    end_try_catch

  unwind_protect_cleanup

    if (! isempty (file))
      H5F.close (file);
    endif

    if (! isempty (obj_id))
      H5O.close (obj_id);
    endif

    if (! isempty (attr_id))
      H5A.close (attr_id);
    endif

    ## Restore previous error printing
    H5E.set_auto (true);
  end_unwind_protect

endfunction

function rethrow_h5error ()
  H5E.walk ("H5E_WALK_UPWARD", @error_walker);
endfunction

function status = error_walker (n, s)
  msg = [lasterr() " (" s.desc ")"];
  error (msg);
  status = -1;
endfunction

%!fail ('h5readatt ()', 'Invalid call')

%!fail ("h5readatt ('__some_non_existing_file__', 'some_location', 'some_att')", "FNAME must be an existing file name")

%!fail ("h5readatt (file_in_loadpath ('base_types_mat73.mat'), 'some_location', 'some_att')", "object 'some_location' doesn't exist")

%!fail ("h5readatt (file_in_loadpath ('base_types_mat73.mat'), '/char_vector', 'some_att')", "can't locate attribute")

%!test
%! assert (h5readatt (file_in_loadpath ('base_types_mat73.mat'), '/char_vector', 'MATLAB_class'), 'char')
