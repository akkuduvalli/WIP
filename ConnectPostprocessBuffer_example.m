
clc

[BufferIDs, BufferNames] = ScanPostprocessBuffers_example;

fprintf('%i buffers found \n',size(BufferIDs,1))

if (size(BufferIDs,1) > 0)
    
    BufferIDs
    
    for j = 1:size(BufferIDs,1)
        fprintf('\n -> Connecting to buffer %i, ID %s \n \n',j,BufferIDs(j,:)); 

        %use workmode 14 to connect to postprocess buffer:
        ret = read_exact(14, BufferIDs(j,:));

        numChannels = 0;
        if(ret == 0)
           [ret, numChannels] = read_exact(3);
           fprintf('Found %i channels in buffer %i\n',numChannels, j); 
           Channels = [];
           for i = 1:numChannels
               [ret, strChannel] = read_exact(4,i-1);
               if ret ~= 0
                   break;
               end
               Channels = strvcat(Channels,strChannel);
           end
        end

        if ret ~= 0
           fprintf('Error connecting to buffer, ID %s',BufferIDs(1,:));
        end

        Channels

        fprintf('Disconnect... \n \n ');
        ret = read_exact(6); 
    end  
end

