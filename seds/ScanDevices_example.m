function [ OAN, LOC, IPA ] = ScanDevices_example()
%SCANDEVICES Summary of this function goes here

%% info:
% 1 line from scan response looks for example: 
% SID:3	OAN:Q.station XT	OVN:GANTNER instruments	SAN:Q.station XT	SVN:GANTNER instruments	LOC:Sync2	MKC:175010	MID:175-10-6381921-0#	SNR:770692	ASK:DYNAMIC	IPA:192.168.5.155	SNM:255.255.255.0	GWA:192.168.5.254	MAA:00:30:d6:17:d1:32	UID:0df75cca-3072-11e9-9dab-6451065705eb	EXTSID:0	EXTAPPVER:V2.14 B04 2019-02-15 	EXTETHSTATIPA:192.168.1.18	

%pick the desired information in the loop!

%% identify devices

%first call -> read_exact(10)
[retsub, AdapterInfo, DeviceInfo, Error] = read_exact(10);


h = waitbar(0.5,'Please wait...');
steps = 30;

j = 0;
while(retsub == 0)  %as long as retsub returns 0
    j = j+1;

    waitbar(j / steps)


    info = regexpi(DeviceInfo, '\t','split')';
    length_info = size(info,1);


    i=0;

    for i=1:length_info

        if findstr(info{i},'OAN:') == 1    
            OAN(j,:) = cellstr(info{i}(5:size(info{i},2)));
        end

        if findstr(info{i},'LOC:') == 1    
            LOC(j,:) = cellstr(info{i}(5:size(info{i},2)));
        end

        if findstr(info{i},'IPA:') == 1    
            IPA(j,:) = cellstr(info{i}(5:size(info{i},2)));
        end

    end

    %next calls -> read_exact(11)
    [retsub, AdapterInfo, DeviceInfo, Error] = read_exact(11);


end

close(h) 

end