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

  file = H5F.open (fname);

  infobuf = H5O.get_info_by_name (file, path, "H5P_DEFAULT");

  obj_id = [];
  attr_id = [];

  unwind_protect


    switch (infobuf.type)

      case H5ML.get_constant_value ("H5O_TYPE_GROUP")
        obj_id = H5G.open (file, path, "H5P_DEFAULT");
        attr_id = H5A.open (obj_id, attr, "H5P_DEFAULT");
        data = H5A.read (attr_id);

      case H5ML.get_constant_value ("H5O_TYPE_DATASET")
        obj_id = H5D.open (file, path, "H5P_DEFAULT");
        attr_id = H5A.open (obj_id, attr, "H5P_DEFAULT");
        data = H5A.read (attr_id);

      otherwise
        error ("h5readatt: unhandled object type")

    endswitch

  unwind_protect_cleanup

    if (! isempty (attr_id))
      H5A.close (attr_id);
    endif

    if (! isempty (obj_id))
      H5O.close (obj_id);
    endif

    H5F.close (file);

  end_unwind_protect

endfunction
