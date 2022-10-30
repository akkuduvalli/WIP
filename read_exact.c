#include "mex.h"

#include "stdlib.h"
//#include "eGateHighSpeedPort.h"
//#include "eGateUtility.h"
#include "windows.h"
#include "time.h"
#include "matrix.h"


#define INVALID_WORKMODE                        (0)
#define WORKMODE_CONNECT                        (1)
#define WORKMODE_SETBUFFER                      (2)
#define WORKMODE_GETNUMBEROFCHANNEL             (3)
#define WORKMODE_GETCHANNELNAME                 (4)
#define WORKMODE_READDATA                       (5)
#define WORKMODE_CLOSE_CONNECTION               (6)
#define WORKMODE_GETSAMPLERATE                  (7)
#define WORKMODE_SLEEP                          (8)
#define WORKMODE_IDENTIFY_FIRST                 (10)
#define WORKMODE_IDENTIFY_NEXT                  (11)
#define WORKMODE_GETPOSTPROCESSBUFFERCOUNT      (12)
#define WORKMODE_GETPOSTPROCESSBUFFERINFO       (13)
#define WORKMODE_INIT_POSTPROCESSBUFFER         (14)
#define WORKMODE_SET_BACKTIME                   (15)

#define WORKMODE_DECODEFILE_SELECT              (101)
#define WORKMODE_DECODEFILE_READDATA            (102)


/*----------------- Data direction ID's ------------------------------------------------*/
#define DADI_INPUT				0   // = Input
#define DADI_OUTPT				1   // = Output
#define DADI_INOUT				2   // = Input/output
#define DADI_EMPTY				3   // = Empty
#define DADI_STATS				4	// = Statistic Channels
/*----------------- Connection Types ---------------------------------------------------*/
#define	HSP_ONLINE				1
#define	HSP_BUFFER				2
#define	HSP_ECONLOGGER			3
#define HSP_ARCHIVES			4
#define	HSP_FILES				5
#define HSP_DIAG				7
#define DLL_CONTROL				9
#define HSP_DIRECT				7
#define HSP_POSTPROCBUFFER		8
#define HSP_BUFFER0				100
#define HSP_BUFFER1				101
/*----------------- General Returncodes ------------------------------------------------*/
#define HSP_OK					0
#define HSP_ERROR				1
#define HSP_CONNECTION_ERROR	2
#define HSP_INIT_ERROR			3
#define HSP_LIMIT_ERROR			4
#define HSP_SYNC_CONF_ERROR		5
#define HSP_MULTYUSED_ERROR		6
#define HSP_INDEX_ERROR			7
#define HSP_FILE_ERROR			8
#define HSP_NOT_READY			9
/*----------------- ChannelInfo ID's ---------------------------------------------------*/
//String:
#define CHINFO_NAME				0	// = Channel name
#define CHINFO_UNIT				1	// = Unit (�C, m, kg,...)	
#define CHINFO_DADI				2	// = Data direction (Input, Output, Empty,..)
#define CHINFO_FORM				3	// = e.g.: %8.3
#define CHINFO_TYPE				4	// = FLOAT, DOUBLE,...
#define CHINFO_VART				33	// = Variable Type (AIN, AOU,..)
//integer:
#define CHINFO_INDI				5	// = Input access Index
#define CHINFO_INDO				6	// = Output access Index
#define CHINFO_INDX				7	// = Total access index
#define CHINFO_PREC				8	// = precision
#define CHINFO_FLEN				9	// = field length
#define CHINFO_RMIN				30	// = Range min
#define CHINFO_RMAX				31  // = Range max
#define CHINFO_MIND				32  // = Module Index
#define CHINFO_DTYI				34	// = Data Type Index




typedef int (__cdecl* MY_DLL_TYPE_INIT)(char*,int,int,int,int*,int*);
typedef int (__cdecl* MY_DLL_TYPE_INITBUFFER)(int,int,int);
typedef int (__cdecl* MY_DLL_TYPE_SETBACKTIME)(int,double);
typedef int (__cdecl* MY_DLL_TYPE_GETNUMBEROFCHANNELS)(int,int,int*);
typedef int (__cdecl* MY_DLL_TYPE_GETCHANNELINFOSTRING)(int,int,int,int,char*);
typedef int (__cdecl* MY_DLL_TYPE_STARTCONFIGURATION)();
typedef int (__cdecl* MY_DLL_TYPE_READBUFFERNEXTFRAME)(int,int);
typedef int (__cdecl* MY_DLL_TYPE_READBUFFERSINGLE)(int,int,int,double*);
typedef int (__cdecl* MY_DLL_TYPE_CLOSE)(int,int);
typedef int (__cdecl* MY_DLL_TYPE_GETSAMPLERATE)(int,int,int,double*,char);
typedef int (__cdecl* MY_DLL_TYPE_SLEEP)(int);
typedef int (__cdecl* MY_DLL_TYPE_DECODEFILESELECT)(int*, int*, char*); 
typedef int (__cdecl* MY_DLL_TYPE_LOADBUFFERDATA)(int, int*); 
typedef long (__cdecl* MY_DLL_TYPE_GETBUFFERFRAMESALL)(int, int); 
typedef int (__cdecl* MY_DLL_TYPE_REWIND)(int, int, int); 

typedef int (__cdecl* MY_DLL_TYPE_IDENTIFY_FIRST)(long,double,char*,char*,char*);
typedef int (__cdecl* MY_DLL_TYPE_IDENTIFY_NEXT)(char*,char*,char*);

typedef int (__cdecl* MY_DLL_TYPE_GETPOSTPROCESSBUFFERCOUNT)();
typedef int (__cdecl* MY_DLL_TYPE_GETPOSTPROCESSBUFFERINFO)(int,char*,mwSize,char*,mwSize);
typedef int (__cdecl* MY_DLL_TYPE_INIT_POSTPROCESSBUFFER)(char*,int*,int*);




void PrintUsage(){
    printf("This is the MexFunction read_exact.\n");
    printf("With the help of this MexFunction data can be read from a Q.series system.\n");
    printf("Usage:\n");
    printf("read_exact(workmode,...)\n");
    printf("workmode:\n");
    printf("\t1 - connect to hardware with specified ip address\n");
    printf("\t2 - set the buffer index to specify which buffer should be used\n");
    printf("\t3 - get number of channels of the selected buffer\n");
    printf("\t4 - get channel name\n");    
    printf("\t5 - read data\n");    
	printf("\t6 - close connection\n"); 
    printf("\t7 - get samplerate\n"); 
    printf("\t8 - sleep (ms)\n"); 
    printf("\t10 - Identify first - Use this Fuction (+ workmode 11) to scan the network for devices...\n"); 
    printf("\t11 - Identify next - Use this in addition to workmode 10 to scan the network for devices... \n"); 
    printf("\t12 - Get Number of available PostProcess buffers. Use this function in combination with workmode 13 \n"); 
    printf("\t13 - Read PostProcess Buffer information by index \n"); 
    printf("\t14 - Initialize connection to a PostProcess Buffer (GI.bench) \n"); 
    printf("\t15 - Set Backtime - use this function only in combination with workmode 14. \n"); 
    printf("\t101 - Open .DAT file\n"); 
    printf("\t102 - Read channel data from selected file \n"); 

    
   
	printf("\n");
	printf("Workmode 1: Connect to hardware (IP) \n");
	printf("Additional parameters:\n");
	printf("\tIP address of the hardware to connect\n");
	printf("\tdata source\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("Example: ret = read_exact(1,\"192.141.151.12\",2)\n");

	printf("\n");
	printf("Workmode 2: Set buffer index - use this workmode in combination with workmode 1.\n");
	printf("Additional parameters:\n");
	printf("\tBuffer index\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("Example: ret = read_exact(2,0)\n");

	printf("\n");
	printf("Workmode 3: Get number of channels\n");
	printf("Additional parameters:\n");
	printf("\tnone\n");
	printf("Return:\n");
	printf("\tError code\n");
	printf("\tNumber of channels\n");
    printf("Example: [ret,NumberOfChannels] = read_exact(3)\n");

	printf("\n");
	printf("Workmode 4: Get channel name\n");
	printf("Additional parameters:\n");
	printf("\tChannel index (zero based)\n");
	printf("Return:\n");
	printf("\tError code\n");
	printf("\tChannel name\n");
    printf("Example: [ret,ChannelName] = read_exact(4,0)\n");

	printf("\n");
	printf("Workmode 5: Read data\n");
	printf("Additional parameters:\n");
	printf("\tFirst set of data to read (0..no, 1..yes ->1 will force a re-init of the reading process)\n");
	printf("\tNumber of channels\n");
	printf("\tRow vector of channel indices to read\n");
	printf("Return:\n");
	printf("\tError code\n");
	printf("\tRow vector of read values\n");
    printf("Example: [ret,values] = read_exact(5,1,14,[0,2,5])\n");

	printf("\n");
	printf("Workmode 6: Close connection\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("Example: [ret] = read_exact(6)\n");

    printf("\n");
	printf("Workmode 10: Identify First - call this function once to start a network scan. continue with workmode 11.\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("\tAdapter Info\n");
    printf("\tDevice Info\n");
    printf("\tError Info\n");
    printf("Example: [ret, AdapterInfo, DeviceInfo, Error] = read_exact(10)\n");
    
    printf("\n");
	printf("Workmode 11: Identify Next - call this function as long as return code equals 0 to get all found devices in the network.\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("\tAdapter Info\n");
    printf("\tDevice Info\n");
    printf("\tError Info\n");
    printf("Example: [ret, AdapterInfo, DeviceInfo, Error] = read_exact(11)\n");
    
    printf("\n");
	printf("Workmode 12: Returns the number of available local PostProcess Buffers on the System.\n");
	printf("Return:\n");
	printf("\tBuffer Count\n");
    printf("Example: bufferCnt = read_exact(12)\n");
    
    printf("\n");
	printf("Workmode 13: Use this workmode to get PostProcess Buffer information in combination with workmode 12.\n");
    printf("Additional parameters:\n");
	printf("\tBuffer Index (loop for example over bufferCnt from workmode 12)\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("\tBuffer ID\n");
    printf("\tBuffer Name\n");
    printf("Example: [ret, bufferID, bufferName] = read_exact(13, index)\n");
    
    printf("\n");
	printf("Workmode 14: Initialize connection to a PostProcess Buffer.\n");
    printf("Additional parameters:\n");
	printf("\tBuffer ID (identify available buffer IDs using workmode 12 + 13)\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("Example: ret = read_exact(14, BufferID)\n");
    
    printf("\n");
	printf("Workmode 15: Set Backtime to get historical data from Postprocess buffer on the first call (in seconds). Default = 0 to get only the newest data.\n");
    printf("Additional parameters:\n");
	printf("\tBacktime in seconds\n");
	printf("Return:\n");
	printf("\tError code\n");
    printf("Example: ret = read_exact(15,0)\n");
}

void PrintUsage2(int Workmode)
{
    printf("Usage:\n");
    printf("read_exact(workmode,...)\n");
    
    if(Workmode == 101)
    {
            printf("\n");
            printf("Workmode 101: \"Open .DAT file\" \n");
            printf("\tAdditional parameters:\n");
            printf("\t\tFilename \n");
            printf("\tReturn:\n");
            printf("\t\tError code\n");
            printf("\tExample: ret = read_exact(101,'ExampleFile.dat')\n");
    }
    if(Workmode == 102)
    {
            printf("\n");
            printf("Workmode 102: \"Read Channeldata from File\" \n");
            printf("\tAdditional parameters:\n");
            printf("\t\tChannelIndex\n");
            printf("\tReturn:\n");
            printf("\t\tError code\n");
            printf("\t\tStruct of a channel X- and Y-Data\n");
            printf("\tExample: [ret, ChannelName] = read_exact(102,1)\n");
    }
}

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

    static int HCLIENT = -1;		//Sometimes different routines from one application need to access the same values
    static int HCONNECTION = -1;	//Up to 10 connections to the same or to different controllers can be initialized
									//Both identifiers shoule be initialized with -1 first!
    
    static bool  IsConnected = false;
    static bool  BufferInitialized = false;
									
	int ret=0;						//Return coede for the HighSpeedPort functions
	int time_ms;                    //sleep time in ms
    int channelIndex=-1;
	int channelCount=0;
    char channelInfo;
	double *rChannelCount;
    double *rSampleRate;
	int i=0;
	int workmode = INVALID_WORKMODE;
	int buffermode = HSP_BUFFER;
	int retries = 0;
    int frames = 0;

	double value=0;					

	int index= 0;
    int backtime= 0;
	char info[1024]="";
	char *ip;
    char *filename;
    int   buflen,status;
    int dims[2];
	double *rRet;
    
    //egateutility -> identify first/next:
	long	I,
			retsub;
	char	h_Error[1024]="";
	char	h_DeviceInfo[1024]="";
	char	h_AdapterInfo[1024]="";
	
	static HINSTANCE		HandleLIB = NULL;
	static MY_DLL_TYPE_INIT dll_init = NULL;
	static MY_DLL_TYPE_INITBUFFER dll_init_buffer = NULL;
    static MY_DLL_TYPE_SETBACKTIME dll_setbacktime = NULL;
	static MY_DLL_TYPE_GETNUMBEROFCHANNELS dll_get_number_of_channels = NULL;
	static MY_DLL_TYPE_GETCHANNELINFOSTRING dll_get_channel_info_string = NULL;
	static MY_DLL_TYPE_STARTCONFIGURATION dll_start_configuration = NULL;
	static MY_DLL_TYPE_READBUFFERNEXTFRAME dll_read_buffer_next_frame = NULL;
	static MY_DLL_TYPE_READBUFFERSINGLE dll_read_buffer_single = NULL;
	static MY_DLL_TYPE_CLOSE dll_close = NULL;	
    static MY_DLL_TYPE_GETSAMPLERATE dll_get_samplerate = NULL;
    static MY_DLL_TYPE_SLEEP dll_sleep = NULL;
    static MY_DLL_TYPE_DECODEFILESELECT dll_decodefile_select = NULL;
    static MY_DLL_TYPE_LOADBUFFERDATA dll_load_buffer_data = NULL;
    static MY_DLL_TYPE_GETBUFFERFRAMESALL dll_get_buffer_frames_all = NULL;
    static MY_DLL_TYPE_REWIND dll_rewind = NULL;
    static MY_DLL_TYPE_IDENTIFY_FIRST dll_identify_first = NULL;
	static MY_DLL_TYPE_IDENTIFY_NEXT dll_identify_next = NULL;
    static MY_DLL_TYPE_GETPOSTPROCESSBUFFERCOUNT dll_get_postprocessbuffer_count = NULL;
    static MY_DLL_TYPE_GETPOSTPROCESSBUFFERINFO dll_get_postprocessbuffer_info = NULL;
    static MY_DLL_TYPE_INIT_POSTPROCESSBUFFER dll_init_postprocessbuffer = NULL;
    
    
	if(HandleLIB == NULL){
	    //war noch nicht geladen->jetzt machen
    	HandleLIB = LoadLibrary("giutility.dll");
    }
	if (HandleLIB){
	    if(dll_init == NULL){
		    dll_init = (MY_DLL_TYPE_INIT)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_Init");
		}
	    if(dll_init_buffer == NULL){		
		dll_init_buffer = (MY_DLL_TYPE_INITBUFFER)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_InitBuffer");
        }
        if(dll_setbacktime == NULL){
        dll_setbacktime = (MY_DLL_TYPE_SETBACKTIME)GetProcAddress(HandleLIB,
                            "_CD_eGateHighSpeedPort_SetBackTime");  
        }
	    if(dll_get_number_of_channels == NULL){									
		dll_get_number_of_channels = (MY_DLL_TYPE_GETNUMBEROFCHANNELS)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_GetNumberOfChannels");
		}
	    if(dll_get_channel_info_string == NULL){									
		dll_get_channel_info_string = (MY_DLL_TYPE_GETCHANNELINFOSTRING)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_GetChannelInfo_String");
		}
	    if(dll_start_configuration == NULL){									
		dll_start_configuration = (MY_DLL_TYPE_STARTCONFIGURATION)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_StartConfiguration");
		}
	    if(dll_read_buffer_next_frame == NULL){									
		dll_read_buffer_next_frame = (MY_DLL_TYPE_READBUFFERNEXTFRAME)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_ReadBuffer_NextFrame");
		}
	    if(dll_read_buffer_single == NULL){									
		dll_read_buffer_single = (MY_DLL_TYPE_READBUFFERSINGLE)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_ReadBuffer_Single");
		}
	    if(dll_close == NULL){									
		dll_close = (MY_DLL_TYPE_CLOSE)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_Close");
        }
        if(dll_get_samplerate == NULL){									
		dll_get_samplerate = (MY_DLL_TYPE_GETSAMPLERATE)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_GetDeviceInfo");
        }
        if(dll_sleep == NULL){									
		dll_sleep = (MY_DLL_TYPE_SLEEP)GetProcAddress(HandleLIB,
									"_CD_eGateHighSpeedPort_SleepMS");
        }
        if(dll_decodefile_select == NULL){
        dll_decodefile_select = (MY_DLL_TYPE_DECODEFILESELECT)GetProcAddress(HandleLIB,
                    "_CD_eGateHighSpeedPort_DecodeFile_Select");
        }
        if(dll_load_buffer_data == NULL){
        dll_load_buffer_data = (MY_DLL_TYPE_LOADBUFFERDATA)GetProcAddress(HandleLIB,
                "_CD_eGateHighSpeedPort_LoadBufferData");
        }
        if(dll_get_buffer_frames_all == NULL){
        dll_get_buffer_frames_all = (MY_DLL_TYPE_GETBUFFERFRAMESALL)GetProcAddress(HandleLIB,
                "_CD_eGateHighSpeedPort_GetBufferFrames_All");
        }    
        if(dll_rewind == NULL){
        dll_rewind = (MY_DLL_TYPE_REWIND)GetProcAddress(HandleLIB,
                "_CD_eGateHighSpeedPort_Rewind");
        }
        if(dll_identify_first == NULL){
            dll_identify_first = (MY_DLL_TYPE_IDENTIFY_FIRST)GetProcAddress(HandleLIB,
                                    "_CDDLG_IdentifyDevices_First");
        }
        if(dll_identify_next == NULL){		
            dll_identify_next = (MY_DLL_TYPE_IDENTIFY_NEXT)GetProcAddress(HandleLIB,
                                    "_CDDLG_IdentifyDevices_Next");
        } 
        if(dll_get_postprocessbuffer_count == NULL){
           dll_get_postprocessbuffer_count =(MY_DLL_TYPE_GETPOSTPROCESSBUFFERCOUNT)GetProcAddress(HandleLIB,
                                    "_CD_eGateHighSpeedPort_GetPostProcessBufferCount");
        }
        if(dll_get_postprocessbuffer_info == NULL){
           dll_get_postprocessbuffer_info =(MY_DLL_TYPE_GETPOSTPROCESSBUFFERINFO)GetProcAddress(HandleLIB,
                                    "_CD_eGateHighSpeedPort_GetPostProcessBufferInfo");
        }
        if(dll_init_postprocessbuffer == NULL){
            dll_init_postprocessbuffer = (MY_DLL_TYPE_INIT_POSTPROCESSBUFFER)GetProcAddress(HandleLIB,
                    "_CD_eGateHighSpeedPort_Init_PostProcessBuffer");
        }
         
	}
	else{
		dll_init = NULL;
		dll_init_buffer = NULL;
        dll_setbacktime = NULL;
		dll_get_number_of_channels = NULL;
		dll_get_channel_info_string = NULL;
		dll_start_configuration = NULL;
		dll_read_buffer_next_frame = NULL;
		dll_read_buffer_single = NULL;
		dll_close = NULL;
        dll_get_samplerate = NULL;
        dll_sleep = NULL;
        dll_decodefile_select = NULL;
        dll_load_buffer_data = NULL;
        dll_get_buffer_frames_all = NULL;
        dll_rewind = NULL;
        dll_identify_first = NULL;
        dll_identify_next = NULL;
        dll_get_postprocessbuffer_count = NULL;
        dll_get_postprocessbuffer_info = NULL;
        dll_init_postprocessbuffer = NULL;
	}
    if((dll_init == NULL)||
            (dll_init_buffer == NULL)||
            (dll_get_number_of_channels == NULL)||
            (dll_get_channel_info_string == NULL)||
            (dll_read_buffer_next_frame == NULL)||
            (dll_read_buffer_single == NULL)||
            (dll_close == NULL)||
            (dll_get_samplerate == NULL)||
            (dll_sleep == NULL)||
            (dll_setbacktime == NULL)||
            (dll_decodefile_select == NULL)||
            (dll_load_buffer_data == NULL)||
            (dll_get_buffer_frames_all == NULL)||
            (dll_rewind == NULL) ||
            (dll_identify_first == NULL) ||
            (dll_identify_next == NULL) ||
            (dll_get_postprocessbuffer_count == NULL) ||
            (dll_get_postprocessbuffer_info == NULL) ||
            (dll_init_postprocessbuffer == NULL)
            ){
        //Dll nicht korrekt geladen
        mexErrMsgTxt("giutility.dll not loaded correctly. check if DLL is available! Terminate.\n");
    	dll_close(HCONNECTION, HCLIENT);
        HCONNECTION = -1;
        HCLIENT = -1;
    	return;
    }
	
    //Dll is now loaded -> check parameters, depending on work mode
    //Work modes:
    //1 - connect to hardware
    //2 - set buffer index
    //3 - get number of channels of the selected buffer and get channel info
    //4 - start configuration
    //5 - read data
    //7 - get sample rate
    //8 - sleep
    //10 - identify first
    //11 - identify next
    // 101 - decode file select
    // 102 - read channel data from file
    
    if(nrhs < 1){
        //error, no work mode specified
        PrintUsage();
        mexErrMsgTxt("No work mode specified\n");
        return;
    }

    if(mxIsChar(prhs[0]) || mxIsComplex(prhs[0]) || !(mxGetM(prhs[0]) == 1 && mxGetN(prhs[0]) == 1) ) {
        mexErrMsgTxt("Parameter 1 must be a noncomplex scalar integer.");
    }
    else{
        workmode = (int)*mxGetPr(prhs[0]);
    }
    
    
    
    
    //********************** Start  WORKMODE_CONNECT      **********************************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_CONNECT){
        //IP holen
        if(nrhs < 2){
            PrintUsage();
            mexErrMsgTxt("No ip address specified\n");
            return;
        }
        if(mxIsChar(prhs[1]) != 1){
            PrintUsage();
            mexErrMsgTxt("Parameter 2 must be a string.");
            return;
        }
        if(mxGetM(prhs[1]) != 1){
            mexErrMsgTxt("Parameter 2 must be a row vector.");
        }
        
        buflen = (mxGetM(prhs[1]) * mxGetN(prhs[1])) + 1;
        ip = mxCalloc(buflen, sizeof(char));
        status = mxGetString(prhs[1], ip, buflen);
        if(status != 0){
            mexWarnMsgTxt("Not enough space. String is truncated.");
        }
        //get transfer mode (HSP_BUFFER or HSP_LOGGER)
        if(nrhs < 3){
            PrintUsage();        
            mexErrMsgTxt("No transfer mode specified\n");
            return;
        }
        if(mxIsChar(prhs[2]) || mxIsComplex(prhs[2]) || !(mxGetM(prhs[2]) == 1 && mxGetN(prhs[2]) == 1) ) {
            mexErrMsgTxt("Paremeter 3 must be a noncomplex scalar integer.");
        }
        else{
            buffermode = (int)*mxGetPr(prhs[2]);
        }

		if(nlhs != 1){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }
        
        ret = 0;
        if(!IsConnected)
        {           
            ret = dll_init(                   ip,				//IP address of the controller as string
                                              5,				//Connection timeout in seconds as integer
                                              buffermode,		//Transfer mode as integer (see header file for constants)
                                              10,				//Sample reate: defines the interval for reading data between device and PC
                                              &HCLIENT,			//The function returns the ID's for the client and the connection
                                              &HCONNECTION);

            plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[0]);
            rRet[0] = (double)ret;

        }
        else
        {
            ret = HSP_OK;
            printf("Already Connected!\n");
            
            plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[0]);
            rRet[0] = (double)ret;
            return; 
        }
        

        
    	if(ret!=HSP_OK){
        	printf("Initialization not successful. Terminate.\n");
			//Hier kein close, da noch gar nichts offen war, bei aufruf crasht dll
        	//dll_close(HCONNECTION, HCLIENT);
        	return;
    	}
        

        IsConnected = true;
        printf("Successfully Connected!\n");

    	return;
    }
    //---Connect
									  
    //+++SetBuffer
    if(workmode == WORKMODE_SETBUFFER){
        //Bufferindex holen
        if(nrhs < 2){
            PrintUsage();
            mexErrMsgTxt("No buffer index specified\n");
            return;
        }
        if(mxIsChar(prhs[1]) || mxIsComplex(prhs[1]) || !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
          mexErrMsgTxt("Paremeter 2 must be a noncomplex scalar integer.");
        }
        else{
            index = (int)*mxGetPr(prhs[1]);
        }

		if(nlhs != 1){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }

        if(IsConnected)
        {           
            if(!BufferInitialized)
            {
                    ret = dll_init_buffer(HCONNECTION,  //Tells the DLL which connection sould be used
							index,          //The buffer index on the controller (can be up to 10)
							0);			    //AutoMode
            }
            if(BufferInitialized)
            {
                ret = HSP_OK;
                printf("Buffer already initialized!\n");
            }
        }

		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
	    rRet[0] = (double)ret;

    	if(ret!=HSP_OK){
             printf("Buffer initialization not successful. Ret %d. Terminate.\n",ret);
             IsConnected = false;
             BufferInitialized = false;
         	 dll_close(HCONNECTION, HCLIENT);
             HCONNECTION = -1;
             HCLIENT = -1;
        	 return;
    	}  
        
        
        if(!BufferInitialized)
        {
            ret = dll_setbacktime(HCONNECTION,0);
            if(ret == HSP_OK)
            {
                BufferInitialized = true;
                printf("Buffer Initialized!\n");
            }
        }      
        if(ret!=HSP_OK){
             printf("Set Backtime failed! Ret %d. Terminate.\n",ret);
             IsConnected = false;
             BufferInitialized = false;
             dll_close(HCONNECTION, HCLIENT);
             HCONNECTION = -1;
             HCLIENT = -1;
             return;
    	} 
        
    	return;
    }
    //---SetBuffer
	
    //+++GetNumberOfChannel
    if(workmode == WORKMODE_GETNUMBEROFCHANNEL){
		int channelCount=0;
        if(nlhs != 2){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }
    	//Read number of channels of the selected buffer
        
        if(IsConnected && BufferInitialized)
        {            
            ret = dll_get_number_of_channels(HCONNECTION,		//-"-
                                             DADI_INPUT,		//Only input channels
                                             &channelCount);	//Number of channels
        }
        else
        {
            printf("Buffer not initialized!\n",ret);
            ret = 1;
        }
		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
	    rRet[0] = (double)ret;

    	if(ret!=HSP_OK){
	        printf("Number of channels not retrieved. Ret %d. Terminate.\n",ret);
            IsConnected = false;
            BufferInitialized = false;
        	dll_close(HCONNECTION, HCLIENT);
            HCONNECTION = -1;
            HCLIENT = -1;
        	return;
	    }
	
		plhs[1] = mxCreateDoubleMatrix(1,1, mxREAL);
		rChannelCount = mxGetPr(plhs[1]);
	    rChannelCount[0] = (double)channelCount;

		return;
	}
	
	    //+++GetChannelInfo
    if(workmode == WORKMODE_GETCHANNELNAME){
		char *pXDataArrays = NULL;
		int channelindex=0;
        if(nlhs != 2){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }

		if(mxIsChar(prhs[1]) || mxIsComplex(prhs[1]) || !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
            mexErrMsgTxt("Paremeter 2 must be a noncomplex scalar integer.");
        }
        else{
            channelindex = (int)*mxGetPr(prhs[1]);
        }
	
		//Read the channel name
		ret =	dll_get_channel_info_string(HCONNECTION,
											 CHINFO_NAME,	//Type of info = name
											 DADI_INPUT,	//Input channels			
											 channelindex,				//channel index
											 info);			//info

		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)ret;
				
		buflen = strlen(info);
		pXDataArrays = mxCalloc(buflen, sizeof(char));
		plhs[1] = mxCreateString(info);
    	if(ret!=HSP_OK){
			printf("Channel info not retrieved. Terminate.\n");
            IsConnected = false;
            BufferInitialized = false;
        	dll_close(HCONNECTION, HCLIENT);
            HCONNECTION = -1;
            HCLIENT = -1;
        	return;
		}
		return;
	}

	if(workmode == WORKMODE_READDATA)
	{
		double *channels_indices;
		int    iNumChannels = 0;
		int    channelCount = 0;
		double *outputs;
		int    restart;
        
		//---GetChannelInfo	
		if(nrhs != 4){
			PrintUsage();
            mexErrMsgTxt("Not enough input arguments specified\n");
            return;
        }
		if(mxGetM(prhs[3]) != 1){
			PrintUsage();
            mexErrMsgTxt("Parameter 4 must be a row vector.\n");
		}
		
		if(mxIsChar(prhs[1]) || mxIsComplex(prhs[1]) || !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
			mexErrMsgTxt("Parameter 2 must be a noncomplex scalar integer.");
		}
        else{
			restart = (int)*mxGetPr(prhs[1]);
        }

		if(mxIsChar(prhs[2]) || mxIsComplex(prhs[2]) || !(mxGetM(prhs[2]) == 1 && mxGetN(prhs[2]) == 1) ) {
			mexErrMsgTxt("Parameter 3 must be a noncomplex scalar integer.");
		}
        else{
			channelCount = (int)*mxGetPr(prhs[2]);
        }
		if(nlhs != 2){
			PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
		}
		
		
		channels_indices = mxGetPr(prhs[3]);
		iNumChannels     = mxGetN(prhs[3]);

		plhs[1] = mxCreateDoubleMatrix(1,iNumChannels, mxREAL);
		outputs = mxGetPr(plhs[1]);
		
		for(i = 0; i < iNumChannels; ++i)
		{
			if((channels_indices[0]<0 || channels_indices[0]>=channelCount))
			{
	    		printf("Channel out of range. Terminate.\n");
                IsConnected = false;
                BufferInitialized = false;
				dll_close(HCONNECTION, HCLIENT);
                HCONNECTION = -1;
                HCLIENT = -1;
    			return;
			}
		}
		if(restart == 1)
		{
			dll_start_configuration();		//Start the internal threads
		}
	 
        
/*
         printf("read data \n");
*/
        
		ret = HSP_NOT_READY;
		retries = 0;
		
		while( ret != HSP_OK)
        {
			ret = dll_read_buffer_next_frame(HCONNECTION,HCLIENT);   //Step to the next frame

			if (ret != HSP_OK)
            {
                //printf("read buffer next channel ret = %d \n",ret);
				retries = retries + 1;
                dll_sleep(500);
				
				if(retries > 10)
				{
					printf("max. retries 10 reached! ret = %d  (read buffer next channel)\n",ret);
					return;
				}					
            }               
		}
		
        
		if(ret == HSP_OK)
		{
           
			for(i = 0; i < iNumChannels;++i)
			{
				ret = dll_read_buffer_single(HCONNECTION,HCLIENT,channels_indices[i],&value);  //Read values by channelIndex
				//This function would read the same value until "NextChannel" was called and returned HSP_OK
            
				//fill values to the output vector
                outputs[i] = value;
			}
		}
		else if(ret == HSP_NOT_READY){
			//Not ready, dll needs some time to read and decode data
            Sleep(1);
			printf("Not ready");
			plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
			rRet = mxGetPr(plhs[0]);
			rRet[0] = (double)ret;
		}
		else{
			plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
			rRet = mxGetPr(plhs[0]);
			rRet[0] = (double)ret;
			printf("Error occured while reading online values");
		}
		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)ret;

		return;
	}
	if(workmode == WORKMODE_CLOSE_CONNECTION){
		if(nlhs != 1){
			PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
		}

		if((HCLIENT != -1) && (HCONNECTION != -1)){
            IsConnected = false;
            BufferInitialized = false;
			dll_close(HCONNECTION, HCLIENT);
            HCONNECTION = -1;
            HCLIENT = -1;
		}
		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)0;
	}
    
    
    
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
     if(workmode == WORKMODE_GETSAMPLERATE){
    
       if(nlhs != 2){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }
    	//Read samplerate
    	ret = dll_get_samplerate(HCONNECTION,		//-"-
										 16,		//typeID=16 for samplerate
										 1,         //index
                                         &value  ,
                                         channelInfo);        

		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
	    rRet[0] = (double)ret;

    	if(ret!=HSP_OK){
	        printf("Sample Rate not retrieved. Ret %d. Terminate.\n",ret);
            IsConnected = false;
            BufferInitialized = false;
            dll_close(HCONNECTION, HCLIENT);
            HCONNECTION = -1;
            HCLIENT = -1;
        	
        	return;
	    }
	
        plhs[1] = mxCreateDoubleMatrix(1,1, mxREAL);
		rSampleRate = mxGetPr(plhs[1]);
	    rSampleRate[0] = (double)value;

		return;
        
    }
     //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   
    //%%%%%%%%SLEEP%%%%%%%%%%%%%%
    	if(workmode == WORKMODE_SLEEP){
        
            int time_ms = 0;
            
        
            if(nrhs != 2){
            PrintUsage();
            mexErrMsgTxt("Wrong number of input arguments. Use read_exact(8,time_ms)\n");
            return;
        }
        
        if(mxIsChar(prhs[1]) || mxIsComplex(prhs[1]) || !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
            mexErrMsgTxt("Paremeter 2 must be a noncomplex scalar integer.");
        }
        else{
            time_ms = (int)*mxGetPr(prhs[1]);
        }
            
            

        
    	ret = dll_sleep(time_ms
                );        

		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
	    rRet[0] = (double)ret;

    	if(ret!=HSP_OK){
	        printf("error in Sleep (read_exact.c). Ret %d. Terminate.\n",ret);
            IsConnected = false;
            BufferInitialized = false;
        	dll_close(HCONNECTION, HCLIENT);
            HCONNECTION = -1;
            HCLIENT = -1;
        	return;
	    }
        return;
    }
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    //**********************start WORKMODE_DECODEFILE_SELECT 101 ******************************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_DECODEFILE_SELECT)
    {
        
        if(nrhs != 2){
            PrintUsage2(101);
            mexErrMsgTxt("Wrong number of parameters\n");
            return;
        }
        if(mxIsChar(prhs[1]) != 1){
            //PrintUsage();
            mexErrMsgTxt("Parameter 2 must be a string.");
            return;
        }
        
     

        buflen = (mxGetM(prhs[1]) * mxGetN(prhs[1])) + 1;
        filename = mxCalloc(buflen, sizeof(char));
        status = mxGetString(prhs[1], filename, buflen);
        //if(status != 0){
        //    mexWarnMsgTxt("Not enough space. String is truncated.");}
        
        //printf("Load File: %s\n", filename); 
      
        //printf("1: hclient: %d, hconnection: %d\n",HCLIENT,HCONNECTION);  
        ret = dll_decodefile_select( &HCLIENT,				
				    				 &HCONNECTION,	
								    filename);
        //printf("2: hclient: %d, hconnection: %d\n",HCLIENT,HCONNECTION);
        
        plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
        rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)ret;
				
    	if(ret!=HSP_OK){
			printf("Could not init file. %s\n", filename);
        	return;
		}   
        if(ret == HSP_OK)
        {
            printf("File loaded successfully... %s\n", filename);
            BufferInitialized = true;
            IsConnected = true;
        }
            
		return;
        
    }
    //**********************end WORKMODE_DECODEFILE_SELECT    ******************************************************************
    //**************************************************************************************************************************
    
    
    
    
    //**********************start WORKMODE_DECODEFILE_READDATA******************************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_DECODEFILE_READDATA)
    {
        //DATA
        int channels_index = 0; //input parameter
        int channelCount = 0;   //number of channels in file
        int found = 0;
        int finished = 0;
        int frameindex = 0;     //actual read prosition
        long NumberOfFrames = 0;//number of frames in file
        double timestep = 0;       
        mxArray *YDataArray,*XDataArray; //data array
        double *YDataPtr, *XDataPtr;     //pointer to data array
        int i;
        const char *fieldnames[2]; //This will hold field names.

        if(nrhs != 2)
        {
			PrintUsage2(WORKMODE_DECODEFILE_READDATA);
            mexErrMsgTxt("Not enough input arguments specified\n");
            return;
        }
        if(nlhs != 2){
			PrintUsage2(WORKMODE_DECODEFILE_READDATA);
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
		}
        if(mxIsChar(prhs[1]) || 
                    mxIsComplex(prhs[1]) || 
                    !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
			mexErrMsgTxt("Parameter 2 must be a noncomplex scalar integer.");
		}
        else{
			channels_index = (int)*mxGetPr(prhs[1]);
        }
            
        //read number of channels in file
        ret = dll_get_number_of_channels(HCONNECTION,		//-"-
                             DADI_INPUT,		//Only input channels
                             &channelCount);	//Number of channels
            
        if(ret != HSP_OK)
        {
            plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[0]);
            rRet[0] = (double)ret;
            plhs[1] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[1]);
            rRet[1] = 0;
        }
        
        //printf("Number of channels in file: %i\n", channelCount);
        
        if(channels_index < 0 || channels_index >channelCount)
        {
            plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[0]);
            rRet[0] = (double)ret;
            plhs[1] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[1]);
            rRet[1] = 1;
            mexErrMsgTxt("Invalid Channel Index!");
        }
        
        //read sample rate
        ret = dll_get_samplerate(HCONNECTION,		//-"-
										 16,		//typeID=16 for samplerate
										 1,         //index
                                         &value  ,
                                         channelInfo); 
        
        if(value > 0)
        {
            timestep = 1 / value;
        }
        else
        {
            timestep = 1;
        }
        //printf("Samle Rate: %f - Timestep: %f\n",value,timestep);
        
        //read total number of frames in file
        NumberOfFrames = dll_get_buffer_frames_all(HCONNECTION, HCLIENT);
        //printf("Number of frames in file: %i\n", NumberOfFrames);
        
          
        if(NumberOfFrames == 0)
        {
                plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
                rRet = mxGetPr(plhs[0]);
                rRet[0] = (double)ret;
                plhs[1] = mxCreateDoubleMatrix(1,1, mxREAL);
                rRet = mxGetPr(plhs[1]);
                rRet[1] = 1;
                mexErrMsgTxt("No Frames in file...\n");
        }
        else
        {

                //Create mxArray data structures to hold the data
                //to be assigned for the structure.
                YDataArray  = mxCreateDoubleMatrix(NumberOfFrames,1, mxREAL);
                XDataArray  = mxCreateDoubleMatrix(NumberOfFrames,1, mxREAL);

                YDataPtr    = mxGetPr(YDataArray);
                XDataPtr    = mxGetPr(XDataArray);

                //printf("matrix allocated\n");

                XDataPtr[1,frameindex] = 0; //erster zeitstempel

                ret = dll_load_buffer_data(HCONNECTION, &frames);
              
                while(ret == HSP_OK)
                {
                    //printf("load data...%d \n", frames);
                    value = 0;

                      for(i = 0; i < frames; i++)
                      {
                          //read channel value                          
                          ret = dll_read_buffer_single(HCONNECTION,HCLIENT,channels_index,&value);
                          if(ret != HSP_OK)
                          {                                      
                              printf("error at read buffer single...CHind: %d, returncode: %i\n", channels_index, ret);
                              found = 1;
                              break;
                          }
                          
                          //write value in channel on current position
                          YDataPtr[1,frameindex] = value;

                          //write new timestamp in the next row
                          XDataPtr[1,frameindex+1] = XDataPtr[1, frameindex] + timestep;
                          frameindex++;
                          /*
                         if(frameindex == NumberOfFrames-10)
                         {
                             printf("number of frames reached... \n");
                             break;
                         }
                          */

                          //load next frame
                          ret = dll_read_buffer_next_frame(HCONNECTION,HCLIENT);

                          if(ret != HSP_OK)               
                          {  
                                //printf("error at next_frame... \n");
                                break;
                          }


                      }

                    if(found == 1)
                    {
                        break;
                    }
                    //load next data -> if no data available: ret = 1
                    ret = dll_load_buffer_data(HCONNECTION, &frames);
                    if (ret != HSP_OK)
                    {
                        finished = 1;
                    }

                }
              
                if(finished == 1)
                {
                    //file reading finished successfully
                    ret = 0;
                }

                plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
                rRet = mxGetPr(plhs[0]);
                rRet[0] = (double)ret;

                dll_rewind(HCONNECTION, HCLIENT, NumberOfFrames);

                //printf("rewind ret = %i \n",ret);
                //dll_close(HCONNECTION, HCLIENT);
                //HCONNECTION = -1;
                //HCLIENT = -1;
        	            
                //Assign field names
                fieldnames[0] = (char*)mxMalloc(20);
                fieldnames[1] = (char*)mxMalloc(20);

                memcpy((void *)fieldnames[0],"XData",sizeof("XData"));
                memcpy((void *)fieldnames[1],"YData",sizeof("YData"));

                //Allocate memory for the structure
                plhs[1] = mxCreateStructMatrix(1,1,2,fieldnames);

                //Deallocate memory for the fieldnames
                mxFree((void *) fieldnames[0] );
                mxFree((void *) fieldnames[1] );

                //Assign the field values
                mxSetFieldByNumber(plhs[1],0,1, YDataArray);
                mxSetFieldByNumber(plhs[1],0,0, XDataArray);
                //NOTE: mxSetFieldByNumber(..) will automatically take care
                //      of allocating required memory for the fields.
        }

       
        return;
	
    }  
    //**********************WORKMODE_DECODEFILE_READDATA************************************************************************
    //**************************************************************************************************************************

    
    
    //********************** Start  WORKMODE_IDENTIFY_FIRST*********************************************************************
    //**************************************************************************************************************************
    
    if(workmode == WORKMODE_IDENTIFY_FIRST)
    {        
        char *pMyStrings = NULL;

        if(nlhs != 4){ //4 output arguments? AdapterInfo, DeviceInfo, Error
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified! Use for example: [ret, AdapterInfo, DeviceInfo, Error] = read_utility(1); \n");
            return;
        }
        //example answer:
        //SID:3	OAN:Q.station XT	OVN:GANTNER instruments	SAN:Q.station XT	SVN:GANTNER instruments	LOC:Sync2	MKC:175010	MID:175-10-6381921-0#	SNR:770692	ASK:DYNAMIC	IPA:192.168.5.155	SNM:255.255.255.0	GWA:192.168.5.254	MAA:00:30:d6:17:d1:32	UID:0df75cca-3072-11e9-9dab-6451065705eb	EXTSID:0	EXTAPPVER:V2.14 B04 2019-02-15 	EXTETHSTATIPA:192.168.1.18	

        
		//Identify Devices
		ret = dll_identify_first(0, //UseExtCommand
											 1,	//ScanTime
											 h_AdapterInfo,		
											 h_DeviceInfo,
											 h_Error);
 
		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)ret;
				
		buflen = strlen(h_AdapterInfo);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[1] = mxCreateString(h_AdapterInfo);
        
        buflen = strlen(h_DeviceInfo);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[2] = mxCreateString(h_DeviceInfo);
        
        buflen = strlen(h_Error);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[3] = mxCreateString(h_Error);
        
        
        if(ret!=0){
			printf("Error in read_exact.c - identify device first. Terminate.\n");
        	return;
		}
        
        return;
        		

    }
    //********************** End  WORKMODE_IDENTIFY_FIRST***********************************************************************
    //**************************************************************************************************************************
    
    
    
    //********************** Start  WORKMODE_IDENTIFY_Next *********************************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_IDENTIFY_NEXT)
    {
        char *pMyStrings = NULL;

        if(nlhs != 4){ //4 output arguments? AdapterInfo, DeviceInfo, Error
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified! Use for example: [ret, AdapterInfo, DeviceInfo, Error] = read_utility(2); \n");
            return;
        }

		//Identify Devices
		ret =	dll_identify_next(	 h_AdapterInfo,		
									 h_DeviceInfo,
									 h_Error);

        
		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)ret;
				
		buflen = strlen(h_AdapterInfo);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[1] = mxCreateString(h_AdapterInfo);
        
        buflen = strlen(h_DeviceInfo);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[2] = mxCreateString(h_DeviceInfo);
        
        buflen = strlen(h_Error);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[3] = mxCreateString(h_Error);
        
        
        
        
    	if(ret!=0){

            //program needs to loop over identify_next until the function returns = 1
            //-> avoid throwing an error from read_exact.c here...
			//printf("Error in read_exact.c - identify device next. Terminate.\n");

        	return;
		}
        
		return; 
    }
    //********************** End  WORKMODE_IDENTIFY_NEXT************************************************************************
    //**************************************************************************************************************************
    
    //********************** Start  WORKMODE_GETPOSTPROCESSBUFFERCOUNT *********************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_GETPOSTPROCESSBUFFERCOUNT)
    {

        ret = dll_get_postprocessbuffer_count();
               
        plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
	    rRet[0] = (double)ret;
        
        return;
    }
    
    //********************** End  WORKMODE_GETPOSTPROCESSBUFFERCOUNT ***********************************************************
    //**************************************************************************************************************************
    
    
    //********************** Start  WORKMODE_GETPOSTPROCESSBUFFERINFO *********************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_GETPOSTPROCESSBUFFERINFO)
    {
        
        char	h_BufferID[1024]="";
        char	h_BufferName[1024]="";
        char    *pMyStrings = NULL;
        mwSize bufsize = 1024;
        
        if(nlhs != 3){ //3 output arguments? ret, bufferID, bufferName
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified! Use for example: [ret, bufferID, bufferName] = read_utility(13, i); \n");
            return;
        }
          
        //Bufferindex holen
        if(nrhs < 2){
            PrintUsage();
            mexErrMsgTxt("No buffer index specified\n");
            return;
        }
        if(mxIsChar(prhs[1]) || mxIsComplex(prhs[1]) || !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
          mexErrMsgTxt("Paremeter 2 must be a noncomplex scalar integer.");
        }
        else{
            index = (int)*mxGetPr(prhs[1]);
        }
        
       
        
         ret = dll_get_postprocessbuffer_info(index, 
                h_BufferID,
                bufsize,
                h_BufferName,
                bufsize);
               
        
        plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
		rRet[0] = (double)ret;
				
		buflen = strlen(h_BufferID);
		pMyStrings = mxCalloc(buflen, sizeof(char));
 		plhs[1] = mxCreateString(h_BufferID);
        
        buflen = strlen(h_BufferName);
		pMyStrings = mxCalloc(buflen, sizeof(char));
		plhs[2] = mxCreateString(h_BufferName);  

        
        return;
    }
    
    //********************** End  WORKMODE_GETPOSTPROCESSBUFFERINFO ************************************************************
    //**************************************************************************************************************************
 
    
    
    
    
    
    
    //********************** Start  WORKMODE_INIT_POSTPROCESSBUFFER ************************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_INIT_POSTPROCESSBUFFER)
    {
                //BufferID holen
        if(nrhs < 1){
            PrintUsage();
            mexErrMsgTxt("No BufferID specified\n");
            return;
        }
        if(mxIsChar(prhs[1]) != 1){
            PrintUsage();
            mexErrMsgTxt("Parameter 2 must be a string.");
            return;
        }
        if(mxGetM(prhs[1]) != 1){
            mexErrMsgTxt("Parameter 2 must be a row vector.");
        }
        
        buflen = (mxGetM(prhs[1]) * mxGetN(prhs[1])) + 1;
        ip = mxCalloc(buflen, sizeof(char));
        status = mxGetString(prhs[1], ip, buflen);
        if(status != 0){
            mexWarnMsgTxt("Not enough space. String is truncated.");
        }


		if(nlhs != 1){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }
        
        if(!IsConnected)
        {
            
            ret = dll_init_postprocessbuffer( ip,				//IP address of the controller as string
                                              &HCLIENT,			//The function returns the ID's for the client and the connection
                                              &HCONNECTION);

            plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[0]);
            rRet[0] = (double)ret;
        }
        else
        {
            printf("Already connected!\n");
            ret = HSP_OK;
            plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
            rRet = mxGetPr(plhs[0]);
            rRet[0] = (double)ret;
        }
								    	  
    	if(ret!=HSP_OK){
        	printf("Initialization of PostProcess Buffer not successful. Terminate.\n");
			//Hier kein close, da noch gar nichts offen war, bei aufruf crasht dll
        	//dll_close(HCONNECTION, HCLIENT);
        	return;
    	}
        
        BufferInitialized = true;
        IsConnected = true;
        
    	return;
    }
    //********************** End  WORKMODE_INIT_POSTPROCESSBUFFER **************************************************************
    //**************************************************************************************************************************
  
    
    
    
    
    
    
    
    
    
    
    
    
    //********************** Start  WORKMODE_SET_BACKTIME **********************************************************************
    //**************************************************************************************************************************
    if(workmode == WORKMODE_SET_BACKTIME)
    {
             //backtime holen
        if(nrhs < 2){
            PrintUsage();
            mexErrMsgTxt("No Backtime specified\n");
            return;
        }
        if(mxIsChar(prhs[1]) || mxIsComplex(prhs[1]) || !(mxGetM(prhs[1]) == 1 && mxGetN(prhs[1]) == 1) ) {
          mexErrMsgTxt("Paremeter 2 must be a noncomplex scalar integer.");
        }
        else{
            backtime = (int)*mxGetPr(prhs[1]);
        }

		if(nlhs != 1){
            PrintUsage();
            mexErrMsgTxt("Not enough output arguments specified\n");
            return;
        }

        ret = dll_setbacktime(HCONNECTION,backtime);
        

		plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
		rRet = mxGetPr(plhs[0]);
	    rRet[0] = (double)ret;

    	if(ret!=HSP_OK){
             printf("Set Backtime not successful. Ret %d. Terminate.\n",ret);
             IsConnected = false;
             BufferInitialized = false;
         	 dll_close(HCONNECTION, HCLIENT);
             HCONNECTION = -1;
             HCLIENT = -1;
        	 return;
    	}   
        
        return;
    }
    
    
    //********************** End  WORKMODE_SET_BACKTIME ************************************************************************
    //**************************************************************************************************************************
    
    
    return;
}