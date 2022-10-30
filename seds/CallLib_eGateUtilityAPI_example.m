clc
clear all
close all

if libisloaded('lib1')
    unloadlibrary lib1
end

A = loadlibrary('giutility', 'eGateUtility.h', 'alias', 'lib1');


file = 'RawData___0_2018-06-19_12-19-09_575000.dat';
desfile = 'RawData.txt';
errorstr = libpointer('cstring','000000000000000000000000000000000000000000000000000000000000000000000000000000000000');

ret = calllib('lib1', 'CDDLG_DecodeBufferFile', file, [], desfile, errorstr);

if ret ~= 0
    errorstr.value
end 

file = 'qgatedata.dat';
desfile = 'qgatedata.txt';
errorstr = libpointer('cstring','000000000000000000000000000000000000000000000000000000000000000000000000000000000000');

ret = calllib('lib1', 'CDDLG_DecodeBufferFile', file, [], desfile, errorstr);

if ret ~= 0
    errorstr.value
end 



if libisloaded('lib1')
    unloadlibrary lib1
end


