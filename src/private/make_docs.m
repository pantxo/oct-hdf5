## Copyright (C) 2025 Pantxo Diribarne
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
## @deftypefn {} {} make_docs (@var{basedir})
## Make package documentation.
## @seealso{}
## @end deftypefn

function make_docs (basedir)

  docdir = fullfile (basedir, "docs");
  if (! exist (docdir, "dir"))
    mkdir (docdir);
  else
    warning ("off", "Octave:delete:no-such-file", "local")
    ## Delete previously present files
    delete (fullfile (docdir, "*"))
  endif

  ## We need pkg-octave-doc package
  try
    pkg load pkg-octave-doc
  catch
    pkg install -forge pkg-octave-doc
    disp ("* Installing pkg-octave-doc")
    pkg load pkg-octave-doc
  end_try_catch
  
  ## We need to install the latest oct-hdf5
  try
    warning ("off")
    pkg uninstall oct-hdf5
  catch
  end_try_catch
  disp ("* Installing oct-hdf5")
  pkg ("install", fullfile (basedir, "oct-hdf5.tar.gz"))

  ## Now build the doc
  olddir = cd (docdir);
  disp ("* Building the doc")
  package_texi2html ("oct-hdf5");
  
  cd (olddir);
endfunction
