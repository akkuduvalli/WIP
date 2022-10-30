function [ BufferIDs, BufferNames ] = ScanPostprocessBuffers_example()
%SCANPostProcessBuffers Summary of this function goes here



%% scan for postprocess buffers on system

try
    %first call -> read buffer count - workmode "12"
    bufferCnt = read_exact(12);

    if bufferCnt > 0

        %loop over all buffers and read buffer infos -> workmode "13"
        for i = 0:(bufferCnt-1)
            [ret, bufferID, bufferName] = read_exact(13, i);
            BufferIDs(i+1,:) = cellstr(bufferID);
            BufferNames(i+1,:) = cellstr(bufferName);
        end


    else
         fprintf('no buffers found!')   
    end

catch
    fprintf('exception occured!')   
end


