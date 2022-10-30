function varargout = HSP_ReadBuffer(varargin)

global IP;
global AllData;
global numChannels;
global ChannelIndex;
global StopReadingData;
global SampleRate;
global size_AllData;
global StartSave;
global ChannelNames;



% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @HSP_ReadBuffer_OpeningFcn, ...
                   'gui_OutputFcn',  @HSP_ReadBuffer_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before HSP_ReadBuffer is made visible.
function HSP_ReadBuffer_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to HSP_ReadBuffer (see VARARGIN)

% Choose default command line output for HSP_ReadBuffer
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes HSP_ReadBuffer wait for user response (see UIRESUME)
% uiwait(handles.HSP_ReadBufferfig);


% --- Outputs from this function are returned to the command line.
function varargout = HSP_ReadBuffer_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function editIPAddress_Callback(hObject, eventdata, handles)
% hObject    handle to editIPAddress (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editIPAddress as text
%        str2double(get(hObject,'String')) returns contents of editIPAddress as a double



% --- Executes during object creation, after setting all properties.
function editIPAddress_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editIPAddress (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbConnect.
function pbConnect_Callback(hObject, eventdata, handles)
% hObject    handle to pbConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global IP;
global iConnectionMode;
global bufferIndex;
global numChannels;
global ChannelIndex;
global SampleRate;



%set ChannelIndex to default 0
ChannelIndex = [0 0];

%% Connect to Device (IP-Address) or Postprocess Buffer (ID)
if get(handles.radiobutton_posprocessbuffer,'Value') == 1
    BufferID = get(handles.editIPAddress,'String');
    
    % use Workmode = 14
    ret = read_exact(14, BufferID);
    if ret == 0
        set(handles.MessageBox,'String','Connected');
    end
    
else
    %% Connect to IP 
    IP = get(handles.editIPAddress,'String');
    bufferIndex = str2num(get(handles.editBufferIndex,'String'));
    iConnect = get(handles.popupMenu,'Value');
    if iConnect == 1
        %HSP_BUFFER   
        iConnectionMode = 2;
    end
    if iConnect == 2
        %HSP_ECONLOGGER    
        iConnectionMode = 3;
    end

    ret = read_exact(1,IP,iConnectionMode);

    if ret == 0
        set(handles.MessageBox,'String','Connected');
        % initialize buffer
        ret = read_exact(2,bufferIndex);
        if ret ~= 0
            set(handles.MessageBox,'String','Invalid Buffer Index');
        end
    else
        set(handles.MessageBox,'String','Connection failed. Wrong IP?');
    end
end

%% Kanalliste füllen  
if ret == 0
[ret, numChannels] = read_exact(3);

    if ret == 0
        %Kanalnamen holen
        Channels=[];
        for i=1:numChannels
            [ret,strChannel] = read_exact(4,i-1);
            if ret ~= 0
                break;
            end
            Channels = strvcat(Channels,strChannel);
        end
        if ret == 0
            set(handles.ListOfChannels,'String',Channels);     
        else
            msgbox('An error occured while reading the channel names from the controller: Error ',num2str(ret));
        end
    else
        msgbox('An error occured while reading the number of channels from the controller: Error ',num2str(ret))
    end

end


%% SampleRate auslesen
if ret == 0
[ret, SampleRate]= read_exact(7);
if ret == 0
    set(handles.sampleRateTextbox,'String',num2str(SampleRate));     
else
    msgbox('An error occured while reading Controller Sample Rate');
end
end

% --------------------------------------------------------------------
function Untitled_1_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pbDisconnect.
function pbDisconnect_Callback(hObject, eventdata, handles)
% hObject    handle to pbDisconnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global StopReadingData;    
StopReadingData = 1;

ret = read_exact(6);
   
if ret == 0
    set(handles.MessageBox,'String','Disconnected');
    set(handles.sampleRateTextbox,'String','');
else
    msgbox('An error occured while disconnecting from the controller: Error ',num2str(ret));
end


% --- Executes on selection change in ListOfChannels.
function ListOfChannels_Callback(hObject, eventdata, handles)
% hObject    handle to ListOfChannels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns ListOfChannels contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ListOfChannels
global ChannelIndex;
global ChannelNames;
ChannelIndex = [0 get(hObject,'Value')-1];
ChannelNames = get(hObject,'String');


% --- Executes during object creation, after setting all properties.
function ListOfChannels_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ListOfChannels (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbStartCommunication.
function pbStartCommunication_Callback(hObject, eventdata, handles)
% hObject    handle to pbStartCommunication (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global AllData;
global numChannels;
global ChannelIndex;
global ChannelNames;
global channels_name;
global StopReadingData;
global PlotData;
global bufferSize;
global SampleRate;
global size_AllData;
global size_PlotData;
global StartSave;
global buflen;
global index;
global savedata;


if get(handles.radiobutton_posprocessbuffer,'Value') == 1
    ret = read_exact(15,0);
end


StartSave = 0;
buflen = 0;
index = 0;
ChannelIndex = [0 get(handles.ListOfChannels,'Value')-1];
ChannelNames =  get(handles.ListOfChannels,'String');

size_AllData = round(SampleRate/3);

bufferSize = round(get(handles.slider,'Value'));
set(handles.sliderValue,'String',num2str(bufferSize));

StopReadingData = 0;
iIsFirstData = 1;

PlotData = zeros(100000,numChannels); %allocate array
AllData  = zeros(size_AllData,numChannels);  %allocate array AllData

size_PlotData = size(PlotData,1);
str = get(handles.MessageBox,'String');

%% Read Buffer + Plot
if strcmp(str ,'Connected')

    while(StopReadingData == 0)

        for i=1:1:size_AllData

             %read values from 1 frame   
            [ret, data] = read_exact(5,iIsFirstData,numChannels,ChannelIndex);
            iIsFirstData = 0;
            AllData(i,1:size(ChannelIndex,2)) = data;


                if((StopReadingData == 1)||(ret ~= 0))
                     break;    
                end


                if StartSave == 1
                     buflen = buflen + 1;
                end

        end
        
        pause(0.1)
        

        if(ret ~= 0)
            StopReadingData = 1;
        end
        
        PlotData = circshift(PlotData, -(i-1));
        PlotData(size_PlotData-(i-1)+1 : size_PlotData ,1:size(AllData,2)) = AllData(1:(i-1),:);        

        
% -------PLOT---------------------        
plot(handles.axes1,PlotData((size_PlotData-bufferSize+1):size_PlotData,2:size(ChannelIndex,2)));
channels_name = ChannelNames(ChannelIndex(2:size(ChannelIndex,2))+1,:);
legend('-DynamicLegend',[channels_name]);
% --------------------------------
          
          if StartSave == 1
              
              if StopReadingData == 0 && buflen > 60000 
                  index = index + 1;
                  savedata = PlotData((size_PlotData-buflen+1):size_PlotData,1:size(ChannelIndex,2));
                  name = strcat('test',num2str(index));
                  xlswrite(name,savedata);
                  buflen = 0;            
              end
          end
          
          if (StartSave == 0 && buflen ~= 0);
                index = index + 1;
                savedata = PlotData((size_PlotData-buflen+1):size_PlotData,1:size(ChannelIndex,2));
                name = strcat('test',num2str(index));
                xlswrite(name,savedata);
                buflen = 0; 
          end
          
    end
else
    msgbox('Not Connected to Controller')
end




% --- Executes on button press in pbStopCommunication.
function pbStopCommunication_Callback(hObject, eventdata, handles)
% hObject    handle to pbStopCommunication (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global StopReadingData;
global StartSave;

StopReadingData = 1;
StartSave = 0;


% --- Executes during object creation, after setting all properties.
function MessageBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to MessageBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called



% --- Executes during object creation, after setting all properties.
function popupMenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function editBufferIndex_Callback(hObject, eventdata, handles)
% hObject    handle to editBufferIndex (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editBufferIndex as text
%        str2double(get(hObject,'String')) returns contents of editBufferIndex as a double


% --- Executes during object creation, after setting all properties.
function editBufferIndex_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editBufferIndex (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --------------------------------------------------------------------
function Untitled_2_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pbCloseWindow.
function pbCloseWindow_Callback(hObject, eventdata, handles)
% hObject    handle to pbCloseWindow (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global StopReadingData;

ret = read_exact(6);

if ret == 0
   StopReadingData = 1;
   delete(handles.HSP_ReadBufferfig)
   close all
else
    msgbox('An error occured while disconnecting from the controller: Error ',num2str(ret));
end
 
 


% --- Executes on slider movement.
function slider_Callback(hObject, eventdata, handles)
% hObject    handle to slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global bufferSize
bufferSize = round(get(hObject,'Value'));
set(handles.sliderValue,'String',num2str(bufferSize));

% --- Executes during object creation, after setting all properties.
function slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on selection change in Locations.
function Locations_Callback(hObject, eventdata, handles)
% hObject    handle to Locations (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns Locations contents as cell array
%        contents{get(hObject,'Value')} returns selected item from Locations


% --- Executes during object creation, after setting all properties.
function Locations_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Locations (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function axes1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes1


% --- Executes on button press in pbSave.
function pbSave_Callback(hObject, eventdata, handles)
% hObject    handle to pbSave (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global StartSave;

StartSave = 1;


% --- Executes on button press in pbStopSave.
function pbStopSave_Callback(hObject, eventdata, handles)
% hObject    handle to pbStopSave (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global StartSave;

StartSave = 0;


% --- Executes on selection change in popupMenu.
function popupMenu_Callback(hObject, eventdata, handles)
% hObject    handle to popupMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupMenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupMenu


% --- Executes on button press in radiobutton_device.
function radiobutton_device_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_device (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_device

toggleConnectionType(~get(hObject,'Value'), handles); 

% --- Executes on button press in radiobutton_posprocessbuffer.
function radiobutton_posprocessbuffer_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_posprocessbuffer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_posprocessbuffer

toggleConnectionType(get(hObject,'Value'), handles); 


function toggleConnectionType(OnOff, handles)
if OnOff == 1
    set(handles.radiobutton_posprocessbuffer,'Value',1);
    set(handles.radiobutton_device,'Value',0);
    set(handles.text1,'String','Buffer ID:');
    set(handles.editIPAddress,'String','');
    set(handles.text5,'Visible','off');
    set(handles.editBufferIndex,'Visible','off');    
else
    set(handles.radiobutton_posprocessbuffer,'Value',0);
    set(handles.radiobutton_device,'Value',1);
    set(handles.text1,'String','IP-Address:');
    set(handles.editIPAddress,'String','');
    set(handles.text5,'Visible','on');
    set(handles.editBufferIndex,'Visible','on');  
end


% --- Executes on button press in btnScanNetwork.
function btnScanNetwork_Callback(hObject, eventdata, handles)
% hObject    handle to btnScanNetwork (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[ OAN, LOC, IPA ] = ScanDevices_example;

devices = [OAN LOC IPA]
% --- Executes on button press in btnScanPostprocessBuffers.
function btnScanPostprocessBuffers_Callback(hObject, eventdata, handles)
% hObject    handle to btnScanPostprocessBuffers (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
try    
    [ BufferIDs, BufferNames ] = ScanPostprocessBuffers_example;
    buffers = [BufferNames BufferIDs]
catch
    
end
