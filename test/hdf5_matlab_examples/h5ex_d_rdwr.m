function h5ex_d_rdwr (dtype)
%**************************************************************************
%   This example is adapted from the original file generously provided
%   by a user to the HDFGroup examples.
%
%   This example shows how to read and write data to a
%   dataset.  The program first writes various types of data to a dataset
%   with dataspace dimensions of DIM0xDIM1, then closes the
%   file.  Next, it reopens the file, reads back the data, and
%   compares with the original data.
%
%   This file is intended for use with HDF5 Library version 1.8
%   EDIT: Also tested with version 1.10.
%**************************************************************************

  FILE    = 'h5ex_d_rdwr.h5';
  DATASET = 'DS1';
  DIM0    = 4;
  DIM1    = 7;

  dims = [DIM0, DIM1];

  types = {'int8', 'int16', 'int32', 'int64', ...
           'uint8', 'uint16', 'uint32', 'uint64', ...
           'single', 'double'};
  h5memtypes = {'H5T_NATIVE_INT8', 'H5T_NATIVE_INT16', ...
                'H5T_NATIVE_INT32', 'H5T_NATIVE_INT64', ...
                'H5T_NATIVE_UINT8', 'H5T_NATIVE_UINT16', ...
                'H5T_NATIVE_UINT32', 'H5T_NATIVE_UINT64', ...
                'H5T_NATIVE_FLOAT', 'H5T_NATIVE_DOUBLE'};
  h5filetypes = {'H5T_STD_I8LE', 'H5T_STD_I16BE', ...
                'H5T_MIPS_I32', 'H5T_ALPHA_I64', ...
                'H5T_MIPS_U8', 'H5T_ALPHA_U16', ...
                'H5T_STD_U32BE', 'H5T_STD_U64LE', ...
                'H5T_ALPHA_F32', 'H5T_INTEL_F64'};

  t = strcmp (dtype, types);
  %%
  %% Initialize data.
  %%
  wdata = zeros(dims,types{t});
  for i = 1:DIM0
    for j = 1:DIM1
      ii=i-1;
      jj=j-1;
      wdata(i,j) = ii * jj - jj;
    end
  end

  %%
  %% Create a new file using the default properties.
  %%
  file = H5F.create (FILE,'H5F_ACC_TRUNC','H5P_DEFAULT','H5P_DEFAULT');

  %%
  %% Create dataspace.  Setting maximum size to [] sets the maximum
  %% size to be the current size.
  %%
  space = H5S.create_simple (2, fliplr(dims), []);

  %%
  %% Create the dataset.  We will use all default properties for this
  %% example.
  %%
  dset = H5D.create (file, DATASET,h5filetypes{t},space,'H5P_DEFAULT');

  %%
  %% Write the data to the dataset.
  %%
  H5D.write (dset,h5memtypes{t},'H5S_ALL','H5S_ALL','H5P_DEFAULT', wdata);

  %%
  %% Close and release resources.
  %%
  H5D.close(dset);
  H5S.close(space);
  H5F.close(file);

  %%
  %% Now we begin the read section of this example.
  %%

  %%
  %% Open file and dataset using the default properties.
  %%
  file = H5F.open(FILE,'H5F_ACC_RDONLY','H5P_DEFAULT');
  dset = H5D.open(file, DATASET);

  %%
  %% Read the data using the default properties.
  %%
  rdata = H5D.read (dset,h5memtypes{t},'H5S_ALL','H5S_ALL','H5P_DEFAULT');

  %%
  %% Close and release resources.
  %%
  H5D.close (dset);
  H5F.close (file);

  %% Check data are identical and delete file
  assert (wdata, rdata)
  delete (FILE)

endfunction
