clc
clear all
close all
 
if libisloaded('HighSpeedPortDLL')
    unloadlibrary HighSpeedPortDLL
end
 
%% Load DLL
A = loadlibrary('giutility','eGateHighSpeedPort.h','alias','HighSpeedPortDLL');
 
 
%% INIT Connection
host        = '192.168.5.24';
hclient     = libpointer('int32Ptr',-1);
hconnection = libpointer('int32Ptr',-1);
timeout     = 10;
conType     = 1; %online
 
ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_Init',host,timeout,conType,10,hclient,hconnection);
 

%% READ Device Info
location = 10;
Address = 11;
SerialNumber=15;

try
    %use this call for INT infos!
    p = libpointer('cstring'); %nullpointer
    info = libpointer('doublePtr',0); %pointer to double value
 
    ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetDeviceInfo',int32(hconnection.Value),int32(SerialNumber),0,info,p)  
    serial = info.Value
 
    %use this call for string infos (-> header file changed to void pointer instead of char*!
    p = libpointer('cstring','000000000000000000000000000000000000000000000000000000');       
 
    ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetDeviceInfo',int32(hconnection.Value),int32(location),0,info,p);
    loc = p.Value
 
    ret = calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_GetDeviceInfo',int32(hconnection.Value),int32(Address),0,info,p);
    adr = p.Value
 
 
 
catch
    disp('error at function "GetDeviceInfo" -> disconnect')
    calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_Close',hconnection.Value,hclient.Value);
    unloadlibrary HighSpeedPortDLL
    return
end



 
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
 
%% read data
 
valuePtr = libpointer('doublePtr',0);
ChannelIndex = 6 %channel index to read...
 
loops = 10;
cnt = 0;

                                         
while calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_ReadOnline_Single',int32(hconnection.Value),ChannelIndex,valuePtr) == 0
 
    fprintf('Value = %i \r',valuePtr.Value)   
    
    if cnt > loops
        break
    end
    pause(1);
    cnt = cnt + 1;
end
 
 
 
%% Close connection
fprintf('Done...');
calllib('HighSpeedPortDLL','CD_eGateHighSpeedPort_Close',hconnection.Value,hclient.Value);
unloadlibrary HighSpeedPortDLL