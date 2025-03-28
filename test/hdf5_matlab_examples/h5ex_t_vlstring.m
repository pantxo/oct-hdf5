function h5ex_t_vlstring
  %**************************************************************************
  %   This example is adapted from the original file generously provided
  %   by a user to the HDFGroup examples.
  %  This example shows how to read and write variable-length
  %  string datatypes to a dataset.  The program first writes
  %  variable-length strings to a dataset with a dataspace of
  %  DIM0, then closes the file.  Next, it reopens the file,
  %  reads back the data, and outputs it to the screen.
  %
  %  This file is intended for use with HDF5 Library version 1.8
  %   EDIT: Also tested with version 1.10.
  %**************************************************************************

  fileName       = tempname ();
  DATASET        = 'DS1';
  DIM0           = 4;

  dims   = DIM0;
  wdata  = {'Parting'; 'is such'; 'sweet'; 'sorrow.'};

                    %
  %% Create a new file using the default properties.
  %
  file = H5F.create (fileName, 'H5F_ACC_TRUNC', 'H5P_DEFAULT', 'H5P_DEFAULT');

    %
    % Create file and memory datatypes.  For this example we will save
    % the strings as FORTRAN strings.
    %
  filetype = H5T.copy ('H5T_FORTRAN_S1');
  H5T.set_size (filetype,'H5T_VARIABLE');
  memtype = H5T.copy ('H5T_C_S1');
  H5T.set_size (memtype, 'H5T_VARIABLE');

      %
      % Create dataspace.  Setting maximum size to [] sets the maximum
      % size to be the current size.
      %
  space = H5S.create_simple (1,fliplr( dims), []);

     %
     % Create the dataset and write the variable-length string data to
     % it.
     %
  dset = H5D.create (file, DATASET, filetype, space, 'H5P_DEFAULT');
  H5D.write (dset, memtype, 'H5S_ALL', 'H5S_ALL', 'H5P_DEFAULT', wdata');

                                %
                                % Close and release resources.
                                %
  H5D.close (dset);
  H5S.close (space);
  H5T.close (filetype);
  H5T.close (memtype);
  H5F.close (file);

     %
  %% Now we begin the read section of this example.  Here we assume
  % the dataset has the same name and rank, but can have any size.
  %

                                %
                                % Open file and dataset.
                                %
  file = H5F.open (fileName, 'H5F_ACC_RDONLY', 'H5P_DEFAULT');
  dset = H5D.open (file, DATASET);

                                %
                                % Get the datatype.
                                %
  filetype = H5D.get_type (dset);

                  %
                  % Get dataspace and allocate memory for read buffer.
                  %
  space = H5D.get_space (dset);
  [~, dims, ~] = H5S.get_simple_extent_dims (space);
  dims=fliplr(dims);

                                %
                                % Create the memory datatype.
                                %
  memtype = H5T.copy ('H5T_C_S1');
  H5T.set_size (memtype, 'H5T_VARIABLE');
                                %
                                % Read the data.
                                %
  rdata=H5D.read (dset, memtype, 'H5S_ALL', 'H5S_ALL', 'H5P_DEFAULT');


                                %
                                % Close and release resources.
                                %
  H5D.close (dset);
  H5S.close (space);
  H5T.close (filetype);
  H5T.close (memtype);
  H5F.close (file);
  assert (wdata,rdata)
endfunction

