%% load file data
%{
read_exact(101,'HF_SEQUENCE__0_2019-01-26_16-42-53_752000.dat') %RawData___0_2018-06-19_12-19-09_575000 backup___0_2018-10-02_05-11-00_000000 doesnotworkqgate
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
%}
% at this point, dat file data in 1x1 struct channelstruct
fields = fieldnames(channelstruct)
sztwo = size(fields)
sz = sztwo(1,1)
tiledlayout(8,4)
for x = 32:64
    % plot for channel data -> use available channelname!! (sine3
    %A101_2_AI2_bridge, ...)
    nexttile
    xl = "Timestamp";
    yl = fields{x,1};
    plot(channelstruct.(xl).XData,channelstruct.(yl).YData, 'LineWidth',2)
    grid on
    set(gca, 'FontSize', 12, 'FontName', 'Times')
    %xlabel(append('$',xl,'$'),'Interpreter','latex');
    %ylabel(append('$',yl,'$'),'Interpreter','latex');
    %xlabel(xl)
    ylabel(yl)
end
