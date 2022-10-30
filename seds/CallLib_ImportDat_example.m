clc
clear all
close all
 
if libisloaded('HighSpeedPortDLL')
    unloadlibrary HighSpeedPortDLL
end
 
%% Load DLL
A = loadlibrary('giutility','eGateHighSpeedPort.h','alias','HighSpeedPortDLL');
 
 
%% INIT Connection
file = 'M:\Eigene Dateien\MATLAB\LoadDLL\buffer.dat';
hclient     = libpointer('int32Ptr',-1);
hconnection = libpointer('int32Ptr',-1);
 
ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_DecodeFile_Select',hclient,hconnection,file);
 
 
%% READ Channel Count
datadirection_input = 0; %0 = input
channelCount = libpointer('int32Ptr',0);
if ret == 0
    try
        ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetNumberOfChannels',int32(hconnection.Value),int32(datadirection_input),channelCount);
        fprintf('channelcount = %i \r',channelCount.Value)    
    catch
    end
end
 
%% READ Device Info
SampleRate=16;  %info in file only supported by q.station fw >= 1.11
SerialNumber=15;
 
 
try
    %use this call for INT infos!
    p = libpointer('cstring'); %nullpointer
    info = libpointer('doublePtr',0); %pointer to double value
 
    ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetDeviceInfo',int32(hconnection.Value),int32(SerialNumber),0,info,p);  
    serial = info.Value;
 
    ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetDeviceInfo',int32(hconnection.Value),int32(SampleRate),0,info,p);
    srate = info.Value
 
 
 
catch
    disp('error at function "GetDeviceInfo" -> disconnect')
    calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_Close',hconnection.Value,hclient.Value);
    unloadlibrary HighSpeedPortDLL
    return
end
 
 
%% List all channels
 
ChInfoName = 0;
%use this call for string infos (-> header file changed to void pointer instead of char*!
p = libpointer('cstring','000000000000000000000000000000000000000000000000000000');  
 
channelstrings = {};
if ret == 0
    for i = 1:channelCount.Value
        ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetChannelInfo_String',int32(hconnection.Value),int32(ChInfoName),int32(datadirection_input),i-1,p);
        channelstrings = [channelstrings p.Value];
    end
end
 
channelstrings
 
%% load data
 
valuePtr = libpointer('doublePtr',0);
Frames = libpointer('int32Ptr',0);
info = libpointer('doublePtr',0); %pointer to double value
p = libpointer('cstring'); %nullpointer
 
precision = 8;
ChannelIndex = 5
 
channel = zeros(100000,1);
 
NrOfFrames = 0;
 
while calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_LoadBufferData',int32(hconnection.Value),Frames) == 0
 
    for i = 1:Frames.value  % 0 = timestamp
        NrOfFrames = NrOfFrames + 1;
 
        ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetDeviceInfo',int32(hconnection.Value),int32(precision),ChannelIndex,info,p);
 
        if info.Value > 0 %convert to double with correct precision automatically
            calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_ReadBuffer_Single',int32(hconnection.Value),int32(hclient.Value),ChannelIndex,valuePtr);
            channel(i,1) = valuePtr.Value; 
        else %read raw type
            %TO DO
        end
 
        if calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_ReadBuffer_NextFrame',int32(hconnection.Value),int32(hclient.Value)) ~= 0
            break;
        end
 
    end  
 
end
 
NrOfFrames
plot(channel(1:NrOfFrames,1))
legend(channelstrings(ChannelIndex+1));
 
 
 
%% Close connection
fprintf('Done...');
unloadlibrary HighSpeedPortDLL