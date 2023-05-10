% Copyright 2010-2013 Christopher C. Hulbert. All rights reserved.
%
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are met:
%
%    1. Redistributions of source code must retain the above copyright notice,
%       this list of conditions and the following disclaimer.
%
%    2. Redistributions in binary form must reproduce the above copyright
%       notice, this list of conditions and the following disclaimer in the
%       documentation and/or other materials provided with the distribution.
%
% THIS SOFTWARE IS PROVIDED BY CHRISTOPHER C. HULBERT ``AS IS'' AND ANY EXPRESS
% OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
% OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
% EVENT SHALL CHRISTOPHER C. HULBERT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
% INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
% (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
% LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
% ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
% (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
% SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

function val = matio_test_cases (name)
  idx = [1:12 21:25 28:39 48:65 69:77 80:81 90:93];

  switch name
    case "var1"
      val = reshape(1:20,4,5);
    case "var2"
      val = reshape(single(1:20),4,5);
    case "var3"
      val = reshape(int64(1:20),4,5);
    case "var4"
      val = reshape(uint64(1:20),4,5);
    case "var5"
      val = reshape(int32(1:20),4,5);
    case "var6"
      val = reshape(uint32(1:20),4,5);
    case "var7"
      val = reshape(int16(1:20),4,5);
    case "var8"
      val = reshape(uint16(1:20),4,5);
    case "var9"
      val = reshape(int8(1:20),4,5);
    case "var10"
      val = reshape(uint8(1:20),4,5);
    case "var11"
      val = reshape(complex(1:20,21:40),4,5);
    case "var12"
      val = reshape(single(complex(1:20,21:40)),4,5);
    case "var21"
      val = sparse(diag(1:5));
    case "var22"
      val = sparse(diag(complex(1:5,6:10)));
    case "var23"
      val = [];
    case "var24"
      val = ['abcdefghijklmnopqrstuvwxyz';
         'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
         '1234567890!@#$%^&*()-_=+`~';
         '[{]}\|;:''",<.>/?          '];

      %% Structure Variables
    case "var25"
      val = struct();
    case "var26"
      val = repmat(struct('field1',[],'field2',[]),0,1);
    case "var27"
      val(1).field1 = zeros(0,1);
      val(1).field2 = repmat(' ',0,1);
      val(2).field1 = repmat(struct,0,1);
      val(2).field2 = repmat({zeros(0,0)},0,1);
    case "var28"
      val = [struct('field1',1,'field2',reshape(2:13,3,4));
             struct('field1',14,'field2',reshape(15:26,3,4))];
    case "var29"
      val = [struct('field1',single(1),'field2',reshape(single(2:13),3,4));
             struct('field1',single(14),'field2', reshape(single(15:26),3,4))];
    case "var30"
      val = [struct('field1',int64(1),'field2',reshape(int64(2:13),3,4));
             struct('field1',int64(14),'field2', reshape(int64(15:26),3,4))];
    case "var31"
      val = [struct('field1',uint64(1),'field2',reshape(uint64(2:13),3,4));
             struct('field1',uint64(14),'field2',
                    reshape(uint64(15:26),3,4))];
    case "var32"
      val = [struct('field1',int32(1),'field2',reshape(int32(2:13),3,4));
             struct('field1',int32(14),'field2',
                    reshape(int32(15:26),3,4))];
    case "var33"
      val = [struct('field1',uint32(1),'field2',reshape(uint32(2:13),3,4));
             struct('field1',uint32(14),'field2',
                    reshape(uint32(15:26),3,4))];
    case "var34"
      val = [struct('field1',int16(1),'field2',reshape(int16(2:13),3,4));
             struct('field1',int16(14),'field2',
                    reshape(int16(15:26),3,4))];
    case "var35"
      val = [struct('field1',uint16(1),'field2',reshape(uint16(2:13),3,4));
             struct('field1',uint16(14),'field2',
                    reshape(uint16(15:26),3,4))];
    case "var36"
      val = [struct('field1',int8(1),'field2',reshape(int8(2:13),3,4));
             struct('field1',int8(14),'field2',
                    reshape(int8(15:26),3,4))];
    case "var37"
      val = [struct('field1',uint8(1),'field2',reshape(uint8(2:13),3,4));
             struct('field1',uint8(14),'field2',
                    reshape(uint8(15:26),3,4))];
    case "var38"
      val = [struct('field1',1+51*j,'field2',reshape((2:13)+(52:63)*j,3,4));
             struct('field1',14+64*j,'field2',
                    reshape((15:26)+(65:76)*j,3,4))];
    case "var39"
      val = [struct('field1',single(1+51*j),...
                    'field2',reshape(single((2:13)+(52:63)*j),3,4));
             struct('field1',single(14+64*j),...
                    'field2',reshape(single((15:26)+(65:76)*j),3,4))];
    case "var48"
      val = struct('field1',sparse(triu(reshape(1:20,4,5))),...
                   'field2',sparse(triu(reshape(1:20,4,5))'));
    case "var49"
      val = struct('field1',sparse(triu(reshape((1:20)+j*(21:40),4,5))),...
                   'field2',sparse(triu(reshape((1:20)+j*(21:40),4,5))'));
    case "var50"
      val = [struct('field1','abcdefghijklmnopqrstuvwxyz',...;
                    'field2','ABCDEFGHIJKLMNOPQRSTUVWXYZ');
             struct('field1','1234567890!@#$%^&*()-_=+`~',...
                    'field2','[{]}\|;:''",<.>/?          ')];

      %% Cell-Array Variables
    case "var51"
      val = {};
    case "var52"
      val = {[] single([]) int64([]) uint64([]) int32([]) uint32([]) ...
             int16([]) uint16([]) int8([]) uint8([])};
    case "var53"
      val = {[1 2;3 4] [5 6 7;8 9 10] [11 12 13 14;15 16 17 18];
             [19 20;21 22] [23 24;25 26;27 28] [29 30;31 32;33 34;35 36]};
    case "var54"
      val = {single([1 2;3 4]) single([5 6 7;8 9 10]) ...
             single([11 12 13 14;15 16 17 18]); single([19 20;21 22]) ...
             single([23 24;25 26;27 28]) ...
             single([29 30;31 32;33 34;35 36])};
    case "var55"
      val = {int64([1 2;3 4]) int64([5 6 7;8 9 10]) ...
             int64([11 12 13 14;15 16 17 18]); int64([19 20;21 22]) ...
             int64([23 24;25 26;27 28]) ...
             int64([29 30;31 32;33 34;35 36])};
    case "var56"
      val = {uint64([1 2;3 4]) uint64([5 6 7;8 9 10]) ...
             uint64([11 12 13 14;15 16 17 18]); uint64([19 20;21 22]) ...
             uint64([23 24;25 26;27 28]) uint64([29 30;31 32;33 34;35 36])};
    case "var57"
      val = {int32([1 2;3 4]) int32([5 6 7;8 9 10]) ...
         int32([11 12 13 14;15 16 17 18]); int32([19 20;21 22]) ...
         int32([23 24;25 26;27 28]) int32([29 30;31 32;33 34;35 36])};
    case "var58"
      val = {uint32([1 2;3 4]) uint32([5 6 7;8 9 10]) ...
             uint32([11 12 13 14;15 16 17 18]); uint32([19 20;21 22]) ...
             uint32([23 24;25 26;27 28]) uint32([29 30;31 32;33 34;35 36])};
    case "var59"
      val = {int16([1 2;3 4]) int16([5 6 7;8 9 10]) ...
             int16([11 12 13 14;15 16 17 18]); int16([19 20;21 22]) ...
             int16([23 24;25 26;27 28]) int16([29 30;31 32;33 34;35 36])};
    case "var60"
      val = {uint16([1 2;3 4]) uint16([5 6 7;8 9 10]) ...
             uint16([11 12 13 14;15 16 17 18]); uint16([19 20;21 22]) ...
             uint16([23 24;25 26;27 28]) uint16([29 30;31 32;33 34;35 36])};
    case "var61"
      val = {int8([1 2;3 4]) int8([5 6 7;8 9 10]) ...
             int8([11 12 13 14;15 16 17 18]); int8([19 20;21 22]) ...
             int8([23 24;25 26;27 28]) int8([29 30;31 32;33 34;35 36])};
    case "var62"
      val = {uint8([1 2;3 4]) uint8([5 6 7;8 9 10]) ...
             uint8([11 12 13 14;15 16 17 18]); uint8([19 20;21 22]) ...
             uint8([23 24;25 26;27 28]) uint8([29 30;31 32;33 34;35 36])};

    case "var63"
      val = {sparse(triu(reshape(1:20,4,5))) sparse(triu(reshape(1:20,4,5))')};

    case "var64"
      val = {sparse(triu(reshape((1:20)+j*(21:40),4,5)));
             sparse(triu(reshape((1:20)+j*(21:40),4,5))')};
    case "var65"
      val = {'abcdefghijklmnopqrstuvwxyz' '1234567890!@#$%^&*()-_=+`~';
             'ABCDEFGHIJKLMNOPQRSTUVWXYZ' '[{]}\|;:''",<.>/?          '};
    case "var66"
      var25 = struct();
      var26 = repmat(struct('field1',[],'field2',[]),0,1);
      var27(1).field1 = zeros(0,1);
      var27(1).field2 = repmat(' ',0,1);
      var27(2).field1 = repmat(struct,0,1);
      var27(2).field2 = repmat({zeros(0,0)},0,1);
      val = {var25 var26 var27};
    case "var69"
      val = {struct('field1','abcdefghijklmnopqrstuvwxyz',...;
                    'field2','ABCDEFGHIJKLMNOPQRSTUVWXYZ');
             struct('field1','1234567890!@#$%^&*()-_=+`~',...
                    'field2','[{]}\|;:''",<.>/?          ')};
    case "var70"
      val = reshape(1:32*32*32,32,32,32);
    case "var71"
      val = reshape(single(1:32*32*32),32,32,32);
    case "var72"
      val = reshape(int64(1:32*32*32),32,32,32);
    case "var73"
      val = reshape(uint64(1:32*32*32),32,32,32);
    case "var74"
      val = reshape(int32(1:32*32*32),32,32,32);
    case "var75"
      val = reshape(uint32(1:32*32*32),32,32,32);
    case "var76"
      val = reshape(int16(1:32*32*32),32,32,32);
    case "var77"
      val = reshape(uint16(1:32*32*32),32,32,32);
    case "var80"
      val = reshape((1:2:2*32^3) + j*(2:2:2*32^3),32,32,32);
    case "var81"
      val = reshape(single((1:2:2*32^3) + j*(2:2:2*32^3)),32,32,32);
    case "var90"
      val = tril(true(5));
    case "var91"
      val = [struct('field1',logical(mod(reshape(0:19,4,5),2)),...
                    'field2',~mod(reshape(0:19,4,5),2));...
             struct('field1',tril(true(5)),'field2',triu(true(5)))];
    case "var92"
      val = {logical(mod(reshape(0:19,4,5),2));~mod(reshape(0:19,4,5),2);...
             tril(true(5));triu(true(5))};
    case "var93"
      val = cell(2,3);
  endswitch

  fname = 'matio_test_cases_uncompressed_hdf_le.mat';

  v73 = read_mat73 (fname, name);

  assert (v73.(name), val)
endfunction
