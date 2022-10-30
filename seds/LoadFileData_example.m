%% load file data
read_exact(101,'backup___0_2018-10-02_05-11-00_000000.dat') %RawData___0_2018-06-19_12-19-09_575000 backup___0_2018-10-02_05-11-00_000000 doesnotworkqgate
%read number of channels
[ret, numChannels] = read_exact(3)

%create struct for all channels
channelstruct = struct();

for i=1:numChannels
    %read channel name
    [ret,strChannel] = read_exact(4,i-1);

    %read channel data
    [ret,data] = read_exact(102,i-1);
    
    %write to struct -> remove invalid characters from string!
    channelstruct.(regexprep(strChannel, {'#' '/' ' '}, '_')) = data;
end


%close file
ret = read_exact(6);


%example plot for channel data -> use available channelname!! (sine3
%A101_2_AI2_bridge, ...)

plot(channelstruct.sine3.XData,channelstruct.sine3.YData)