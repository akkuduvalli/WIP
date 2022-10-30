


//////////////////////////////////////////////////////////////////////////////////////////
/*----------------- Global Constants ---------------------------------------------------*/

#define COMPILE_DEMO_DLL_OFF

//////////////////////////////////////////////////////////////////////////////////////////
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
#define HSP_EXLIB_MISSING		10
#define HSP_NOT_CONNECTED		11
#define HSP_NO_FILE				12
#define HSP_CORE_ERROR			13
#define HSP_POINTER_INVALID		14
#define HSP_NOT_IMPLEMENTED		15
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

/*----------------- DeviceInfo ID's ------------------------------------------------*/
//String
#define DEVICE_LOCATION			10
#define DEVICE_ADDRESS			11
#define DEVICE_TYPENAME			12
#define DEVICE_TYPECODE			14
#define DEVICE_VERSION			13
#define DEVICE_SERIALNR			15
//integer
#define DEVICE_SAMPLERATE		16
#define DEVICE_MODULECOUNT		17
#define DEVICE_CHANNELCOUNT		18
#define	DEVICE_MID				50
#define DEVICE_BUFFERCOUNT		51
#define	DEVICE_LOGGERCOUNT		52
#define DEVICE_TSTYPE			53
/*----------------- SlaveModuleInfo ID's ----------------------------------------------*/
//String
#define MODULE_TYPE				19
#define MODULE_TYPECODE			20
#define MODULE_Location			21
//integer
#define MODULE_UARTINDEX		22
#define MODULE_ADDRESS			23
#define MODULE_VARCOUNT			24
/*----------------- StorageInfo ID's ---------------------------------------------------*/
#define STORE_FILECOUNT			25
#define STORE_SECONDS			26
/*------------------ Buffer ID's -------------------------------------------------------*/
#define BUFFER_MAXFRAMES		27
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
//...
/*----------------- Statistic info Types -----------------------------------------------*/
#define STAT_CONNECTED			0
#define	STAT_STACKSIZE			1
#define	STAT_DECODETIME			2
/*----------------- Diagnostic Types ---------------------------------------------------*/
#define DIAG_CONTROLLER			0
#define DIAG_INTERFACE			1
#define DIAG_TRANSPORT			2
#define DIAG_VARIABLE			3
#define DIAG_ITEMCOUNT			4
/*----------------- Data storage Types -------------------------------------------------*/
#define STOR_MDF				0
#define STOR_CSV				1
/*----------------- Timestamp Types ----------------------------------------------------*/
#define TSTYPE_NO				0
#define	TSTYPE_COUNTER			1
#define TSTYPE_TIMEOLE2			2
#define TSTYPE_DCSYSTEMTIME		3
/*----------------- File Types ---------------------------------------------------------*/
#define FILE_DIR_ALL				0
#define FILE_FLASHAPPLICATION		1
#define FILE_FLASHDATA				2
#define FILE_USBDATA				3
#define	FILE_VIRTUALSTATE			4
#define	FILE_VIRTUALONLINEBUFFER	5
#define	FILE_VIRTUALCIRCLEBUFFER	6
#define FILE_VIRTUALARCHIVE			7
#define FILE_VIRTUALLOGGER			8
#define	FILE_IDENTIFY_BY_PATH		10
/*----------------- File Locations -----------------------------------------------------*/
#define LOC_LOCALE				0
#define LOC_CONTROLLER			1
/*----------------- Data Types ---------------------------------------------------------*/
#define DATY_NO					0
#define DATY_BOOL				1
#define DATY_SINT8				2
#define DATY_USINT8				3
#define DATY_SINT16				4
#define DAYT_USINT16			5
#define DATY_SINT32				6
#define	DATY_USINT32			7
#define DATY_FLOAT				8
#define DATY_SET8				9
#define DATY_SET16				10
#define DATY_SET32				11
#define DATY_DOUBLE				12
#define DATY_SINT64				13
#define DATY_USINT64			14
#define DATY_SET64				15
/*----------------- Callback Types -----------------------------------------------------*/
#define CALL_CONTROL			0
#define CALL_ERROR				1
#define CALL_DIAG				2
#define CALL_DSPDATA			3
#define CALL_FREADY				4
#define CALL_DEBUG				5
/*----------------- Remote Control Types -----------------------------------------------*/
#define REMOTE_START			0
#define REMOTE_STOP				1
#define REMOTE_END				2
/*----------------- System Limits ------------------------------------------------------*/
#ifdef COMPILE_DEMO_DLL_ON
	#define MAXCONNECTIONS		1
	#define MAXCLIENTS			1
#else
	#define MAXCONNECTIONS		20		// = Max connections for this DLL
	#define MAXCLIENTS			100		// = Max clients for one connection
#endif

#define MAXADDRESSLENGTH		100
#define	MAXDEBUGMESSAGELEN		1024


//typedef void (CALLBACK *messageProc)(int32_t ,double ,char[MAXDEBUGMESSAGELEN]);

int _CD_eGateHighSpeedPort_TEST(int test, int command);


//////////////////////////////////////////////////////////////////////////////////////////
/*------------- Global Control ---------------------------------------------------------*/
/*																						*/
/*	Description:																		*/
/*																						*/
/*		Following functions provide possibilities for global setup						*/
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////	
/*--------------------------------------------------------------------------------------*/
/************		Initialize connection												
																					
		Initialize the Ethernet HighSpeedPort Connection to a Gantner-Instruments		
	    Controller. 																																			
 	IN:
			hostName		 ...  the ip address of the controller

			timeout			 ...  the connection timeout in seconds

			mode			 ...  the communication mode (see constants "Connection Types")
								  
								  If HSP_ONLINE: this function initializes the complete 
								  communication.

								  If HSP_BUFFER or HSP_LOGGER: 
								  eGateHighSpeedPort_InitBuffer is needed to select the 
								  buffer index before data transfer.

								  Other Communication types will only open the Port but 
								  not initialize anything.

			sampleRate       ...  The sample rate for reading single or buffered data 
								  from the controller.
								  
								  HSP_ONLINE: up to 1000Hz (check System Health!)
								  HSP_BUFFER: 2Hz default (otherwise check System Health!)
	OUT:
	
			client Instance	  ... If several tasks of the application uses the same connection,
								  some DLL functions need the client instance 
								  for synchronisation.

			connectionInstance .. handle that identifies/selects a connection 

   RETURN:	General Returncodes
*/

int  _CD_eGateHighSpeedPort_Init(const char* hostName,
											  int timeOutSec, 
											  int mode,
											  int sampleRateHz, 
											  int* clientInstance, 
											  int* connectionInstance);

											  
/*--------------------------------------------------------------------------------------*/
/************		Get BostProcess buffer count
 *
	RETURN:
		Number of available post process buffers
*/
int _CD_eGateHighSpeedPort_GetPostProcessBufferCount();


/*--------------------------------------------------------------------------------------*/
/************		Get info about PostProcess buffer
	Returns basic information about one post process buffer

	IN:
		bufferIndex			...	buffer index
		bufferIDLen			... maximum length of destination buffer for bufferID
		bufferNameLen		... maximum length of destination buffer for bufferName

	OUT:
		bufferID			... unique buffer id (needed to connect to it)
		bufferName			... friendly buffer name

	RETURN:
		General return codes

	ATTENTION:
		GetPostProcessBufferCount has to be called before!!
		Not thread save! This means that a call to GetPostProcessBufferCount() from another Thread
		could change buffer enumeration while reading single buffers!
*/

int  _CD_eGateHighSpeedPort_GetPostProcessBufferInfo(int 	bufferIndex,
																			 char* 		bufferID,
																			 int 	bufferIDLen,
																			 char* 		bufferName,
																			 int 	bufferNameLen);

/*--------------------------------------------------------------------------------------*/
/************		Initialize connection to PostProcess buffer
*/

int  _CD_eGateHighSpeedPort_Init_PostProcessBuffer(const char* BufferID,
														int* clientInstance,
														int* connectionInstance);

/*--------------------------------------------------------------------------------------*/
/************		Read and decode file
This function initializes any UDBF file as it was a connection.
Common buffer functions can be used to access the data

IN:

fileName			...	source file name

OUT:

client Instance		... If several tasks of the application uses the same connection,
					    some DLL functions need the client instance for synchronisation.

connectionInstance	... handle that identifies/selects a connection

RETURN:	General Returncodes
*/

int  _CD_eGateHighSpeedPort_DecodeFile_Select(int* clientInstance,
																	  int* connectionInstance,
																	  const char* fileName);

/*--------------------------------------------------------------------------------------*/
/************		enable/disable auto sync mode
  
	Buffered connections support automatic synchronisation of data streams.

	If different connections have hardware synchronized controllers, data streaming will 
	start only with similar timestamps.

	Following sync settings should be used on the controller:

			- Qsync			... direct synchronisation between Q-series controller
			- IRIG/AFNOR	... synchronisation by external Clocks on DCF77 or GPS
			- SNTP			... synchronisation over Ethernet
			- NMEA			... synchronisation over GPS


  1	=	enabled
  0	=	disabled
*/

 
int 	_CD_eGateHighSpeedPort_SetAutoSyncMode(int connectionInstance,
																   int enable);

				
/*--------------------------------------------------------------------------------------*/
/************		Start/Stop auto mode
  
	This functions are used to start configured mechanisms like reading buffer, synchronisation		
	or data storage.

	The SyncStartConfiguration() function starts only the dedicated connection and not all auto 
	connections as StartConfiguration()

*/

 
int 	_CD_eGateHighSpeedPort_StartConfiguration();


 
int 	_CD_eGateHighSpeedPort_SyncStartConfiguration(int connectionIndex);


 
int 	_CD_eGateHighSpeedPort_StopConfiguration();



/*--------------------------------------------------------------------------------------*/
/************		Configure sample rate

		Modifies the sample rate at runtime.
		This sample rate only defines the interval for reading data from the controller
		to the pc.
		Due to ethernet is not deterministic, this will not be an exact timing.
		It only helps to influence the rate how fast data is copied between Controller and PC.
		The exact measurement rate of the controller has to be configured with test.commander!

	IN: 
		
		connectionInstance	...	to select the correct connection

		sampleRate			... sampleRate(Hz)

	RETURN:	General Returncodes	
*/
 
int  _CD_eGateHighSpeedPort_SetSampleRate(int connectionInstance, 
															  int sampleRateHz);

/*--------------------------------------------------------------------------------------*/
/************		Read sample rate													

		Read the sample rate as configured at "Init" or "Configure sample rate".
		

	IN:

		connectionInstance	...	to select the correct connection

	OUT:

		sampleRate			... sampleRate(Hz)

	RETURN:	General Returncodes	
		
*/
 
int  _CD_eGateHighSpeedPort_GetSampleRate(int connectionInstance, 
															  int *sampleRateHz);

/*--------------------------------------------------------------------------------------*/
/************		Configure Receive Timeout											


	At eGateHighSpeedPort_init(..) connection timeout and receive timeout are similar.
	This function configures the timeout for receiving data.
	The "winsock.h" function "select()" is used to generate the timeout 
	-> no blocking while timeout

	IN:

		connectionInstance	...	to select the correct connection

		timeout				... receive timeout in seconds

	RETURN:	General Returncodes	

*/
 
int  _CD_eGateHighSpeedPort_SetReceiveTimeout(int connectionInstance, 
																  int timeOut);

/*--------------------------------------------------------------------------------------*/
/************		Read Receive Timeout												

	Reads the timeout configured with "_CD_eGateHighSpeedPort_SetReceiveTimeout"

	IN:

		connectionInstance	...	to select the correct connection
		
	OUT:

		timeout				... receive timeout in seconds

  RETURN: General Returncodes	

*/
 
int  _CD_eGateHighSpeedPort_GetReceiveTimeout(int connectionInstance, 
																  int *timeOut);

/*--------------------------------------------------------------------------------------*/
/************		Close connection													

	Closes an opened connection and terminates its worker threads.

	IN:

		connectionInstance	...	to select the correct connection

		clientInstance		... to select the correct client

	RETURN: General Returncodes

*/
 
int  _CD_eGateHighSpeedPort_Close(int connectionInstance,
													  int clientInstance);

//////////////////////////////////////////////////////////////////////////////////////////
/*------------- Config Data ------------------------------------------------------------*/
/*																						*/
/*	Description:																		*/
/*																						*/
/*		Following functions provide config information	from a specific connection		*/
/*		The connection has to be initialized first										*/	
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------*/
/************		Get number of channels		

	Reads the number of channels of a specific connection and a specific data direction

	ATTENTION: Buffered connections will not show any output channels,
			   although they are configured on the device!!
			

	IN:

		connectionInstance	...	to select the correct connection

		directionID			... to select the channel direction:

								DADI_INPUT	-> Input channels
								DADI_OUTPT	-> Output channels
								DADI_INOUT	-> Input or output channels

	OUT:

		ChannelCount			Number of channels

	RETURN: General Returncodes
*/
 
int  _CD_eGateHighSpeedPort_GetNumberOfChannels(int ConnectionInstance,
															 	 	 	int directionID,
																		int *ChannelCount);

/*--------------------------------------------------------------------------------------*/
/************		Get device info											


	Can be used to get different system info's from a initialized connection.

	IN:

		connectionInstance	...	to select the correct connection

		typeID				...	to select the requested type:

								DEVICE_LOCATION		...	reads the device location to channelInfo[]
								DEVICE_ADDRESS		...	reads the ip Address to channelInfo[]
								DEVICE_TYPE			... reads the module type to channelInfo[]
								DEVICE_VERSION		...	reads the firmware version to channelInfo[]
								DEVICE_TYPECODE		...	reads the MK-Code to channelInfo[]
								DEVICE_SERIALNR		...	reads the serial number to channelInfo[]

								DEVICE_SAMPLERATE	... reads the sample rate to info
								DEVICE_MODULECOUNT	... reads the number of slave modules to info
								DEVICE_CHANNELCOUNT	... reads the number of channels to info

	OUT:

		info				... device info as integer as selected with typeID

		channelInfo			... device info as string as selected with typeID

	RETURN: General Returncodes
*/
 
int  _CD_eGateHighSpeedPort_GetDeviceInfo(int ConnectionInstance,
															  int typeID,
															  int Index,
															  double *info,
															  void *channelInfo);

/*--------------------------------------------------------------------------------------*/
/************		Get channel info - string											

	Reads channel specific text based info's by an type ID, the channel Index and direction.

	Use "_CD_eGateHighSpeedPort_GetNumberOfChannels" first to get the number of channels for the 
	desired data direction.

	Then read any neccessary info to the chanels by indexing within a loop.
	The channel order is strictly conform to the system configuration.

	The same DirectionID as for "_CD_eGateHighSpeedPort_GetNumberOfChannels" has to be used!!

	IN:

		connectionInstance	...	to select the correct connection

		typeID				...	type of info
		
								CHINFO_NAME	-> Channel name
								CHINFO_UNIT	-> Unit (°C, m, kg,...)	
								CHINFO_DADI	-> Data direction (Input, Output, Empty,..)
								CHINFO_FORM	-> Data type
								CHINFO_TYPE	-> Channel Type (analog, digital,..)

		directionID			...	similar to "_CD_eGateHighSpeedPort_GetNumberOfChannels"

		channelIndex		... to access the correct channel from the list

		channelInfo			... desired string based channel info

	OUT:

		channelInfo			...	channel info as string

	RETURN: General Returncodes

*/
 
int  _CD_eGateHighSpeedPort_GetChannelInfo_String(int ConnectionInstance,
															   	   	      int typeID,
																		  int directionID,
																		  int channelIndex,
																		  void *channelInfo);

/*--------------------------------------------------------------------------------------*/
/************		Get channel info - int
Reads channel specific text based info's by an type ID, the channel Index and direction.

	Use "_CD_eGateHighSpeedPort_GetNumberOfChannels" first to get the number of channels for the 
	desired data direction.

	Then read any neccessary info to the chanels by indexing within a loop.
	The channel order is strictly conform to the system configuration.

	The same DirectionID as for "_CD_eGateHighSpeedPort_GetNumberOfChannels" has to be used!!

	IN:

		connectionInstance	...	to select the correct connection

		typeID				...	type of info
		
								CHINFO_INDI		-> Input access Index
								CHINFO_INDO		-> Output access Index
								CHINFO_INDX		-> Total access index
								CHINFO_PREC		-> precision
								CHINFO_FLEN		-> field length

		directionID			...	similar to "_CD_eGateHighSpeedPort_GetNumberOfChannels"

		channelIndex		... to access the correct channel from the list

		channelInfo			... desired numeric channel info

	OUT:

		channelInfo			...	channel info as integer

	RETURN: General Returncodes

*/										
 
int  _CD_eGateHighSpeedPort_GetChannelInfo_Int(int ConnectionInstance,
																   int typeID,
																   int directionID,
																   int channelIndex,
																   int *ChannelInfo);

//////////////////////////////////////////////////////////////////////////////////////////
/*------------- Online Communication ---------------------------------------------------*/
/*																						*/
/*	Description:																		*/
/*																						*/
/*		Following functions provide communication possibilities for online data.		*/
/*		The connection has to be initialized first and config data functions can be		*/
/*		used to read some channel informations first.									*/
/*																						*/
/*		The cyclic data transmission between controller and PC is done by the DLL.		*/
/*		The DLL only provides buffers containing double values,							*/
/*		which are already decoded.														*/
/*		Following functions provide read/write access to this DLL Buffers.				*/
/*																						*/
/*	Performance:																		*/
/*																						*/
/*		The cycle time for updating online values is defined by sampleRate at			*/
/*		initialisation.																	*/
/*		The timing is not very exact and the cycle time can be about 100Hz max			*/
/*																						*/
/*		It is recommended to use online data transfer functions for:					*/
/*																						*/
/*			- check values (e.g. to trigger buffer communications)						*/
/*			- slow controlling applications												*/
/*			- monitor static, or non high dynamic values								*/
/*			- write output channels														*/
/*																						*/
/*		For high dynamic values use buffer functions instead							*/
/*																						*/
/*		For fast controlling applications use DistributorPort functions instead			*/
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------*/
/************		Read online single channel											

	Read a single double value from a specific channel on the connection, selected 
	with connectionIndex.

	All channels(analoge, digital / floating point, integer, boolean,..)

	IN:

		connectionInstance	...	to select the correct connection

		channelIndex		... to access the correct channel from the list
								Here, always the total index is neccessary!!

								-> Use "eGateHighSpeedPort_GetChannelInfo_Int" to to convert 
								any IN, OUT or INOUT index to the correct total index;

	OUT:

		value				... the actual value of this channel converted to double

	RETURN: General Returncodes
*/
 
int _CD_eGateHighSpeedPort_ReadOnline_Single(int connectionIndex,
										 int channelIndex,
										 double* value );

/*--------------------------------------------------------------------------------------*/
/************		Read online frame

Reads a complete online frame to be stored internally and accessed with ReadOnline_Single 

IN:

connectionInstance	...	to select the correct connection

RETURN: General Returncodes
*/

int _CD_eGateHighSpeedPort_ReadOnline_Frame(int connectionIndex);


/*--------------------------------------------------------------------------------------*/
/************		Read online frame to double array

	Reads a complete or part of an online frame to a double array.
	No worker threads are needed, every call initiates TCP/IP communication.

	"eGateHighSpeedPort_Init" has to be used first!

	IN:

		connectionInstance	...	to select the correct connection

		arrayLength			... Number of elements in "valueArray"
								If "valueArray" is smaller than "arrayLength" this will cause a segfault!!!

		startIndex			... index of first variable to be read
								(this will be the first value in valueArray)

		channelCount		... Number of channels to be read starting from "startIndex"^.
								If channelCount is larger than arrayLength, only arrayLength channels will be read.
								Is channelCount is -1 or larger than the real number of channels, all channels will be read.
	OUT:

		valueArray			... Pointer to a double array with at least "ArrayLength" elements
								contains double converted values.

	RETURN: General Returncodes
*/

int _CD_eGateHighSpeedPort_ReadOnline_FrameToDoubleArray(int connectionIndex, 
													 double *valueArray,
													 int arrayLength,
													 int startIndex,
													 int channelCount);


/*--------------------------------------------------------------------------------------*/
/************		Read online multiple channels										

	Not supported


EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_CD 
_CD_eGateHighSpeedPort_ReadOnline_Window(int connectionIndex,
										 int startIndex,
										 int number,
										 double* values[]);
EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_SC 
SC_eGateHighSpeedPort_ReadOnline_Window(int connectionIndex,
										int startIndex,
										int number,
										double* values[]);*/
/*--------------------------------------------------------------------------------------*/
/************		Write online single channel	
										
	Write a single double value to a specific channel on the connection, selected 
	with connectionIndex.

	All channels(analoge, digital / floating point, integer, boolean,..)

	ATTENTION: All channels can be written one by one. They will be stored in the DLL output buffer
			   until "eGateHighSpeedPort_WriteOnline_ReleaseOutputData" is called for this connection.

	IN:

		connectionInstance	...	to select the correct connection

		channelIndex		... to access the correct channel from the list
								Here, always the total index is neccessary!!

								-> Use "eGateHighSpeedPort_GetChannelInfo_Int" to to convert 
								any IN, OUT or INOUT index to the correct total index;

		value				... the new value for this channel as double
								(will be converted to the correct data type on the device)

	RETURN: General Returncodes
*/
 
int _CD_eGateHighSpeedPort_WriteOnline_Single(int connectionIndex,
										  int channelIndex, 
										  double value);

/*--------------------------------------------------------------------------------------*/
/************		Write online single channel	Immediate
										
	Write a single double value to a specific channel on the connection, selected 
	with connectionIndex immeadiately.

	All channels(analoge, digital / floating point, integer, boolean,..)

	IN:

		connectionInstance	...	to select the correct connection

		channelIndex		... to access the correct channel from the list
								Here, always the total index is neccessary!!

								-> Use "eGateHighSpeedPort_GetChannelInfo_int" to to convert 
								any IN, OUT or INOUT index to the correct total index;

		value				... the new value for this channel as double
								(will be converted to the correct data type on the device)

	RETURN: General Returncodes
*/
 
int  _CD_eGateHighSpeedPort_WriteOnline_Single_Immediate(int instance,
															int channelIndex,
															double value);

/*--------------------------------------------------------------------------------------*/
/************		Release output value										

	Releases all bufered output values.
	This ensures that all channels are written simultaniously.

	IN:

		connectionInstance	...	to select the correct connection


	RETURN: General Returncodes
*/
 
int _CD_eGateHighSpeedPort_WriteOnline_ReleaseOutputData(int connectionInstance);

/*--------------------------------------------------------------------------------------*/
/************		Write online multiple channels										

Not supported

*/
 
int _CD_eGateHighSpeedPort_WriteOnline_Window(int connectionIndex,
										  int startIndex,
										  int number,
										  double* values[]);

/*--------------------------------------------------------------------------------------*/
/************		Set client state
 	IN:
			connection Index ...  identifies the connection

			client Index	 ...  identifies the client

			state			 ...  set "1" if reading finished.
								  DLL will set this state to zero if new data is 
								  available.
							
   RETURN:	General Returncodes	

EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_CD 
_CD_eGateHighSpeedPort_SetClientState(int connectionIndex,
									  int clientIndex,
									  int state);
EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_SC 
SC_eGateHighSpeedPort_SetClientState(int connectionIndex,
									 int clientIndex,
									 int state);*/
/*--------------------------------------------------------------------------------------*/
/************		Get client state
 	IN:
			connection Index ...  identifies the connection

			client Index	 ...  identifies the client

	RETURN: 
			Requested client state

EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_CD 
_CD_eGateHighSpeedPort_GetClientState(int connectionIndex,
									  int clientIndex);
EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_SC 
SC_eGateHighSpeedPort_GetClientState(int connectionIndex,
									 int clientIndex);*/
//////////////////////////////////////////////////////////////////////////////////////////
/*------------- Buffered Communication -------------------------------------------------*/
/*																						*/
/*	Description:																		*/
/*																						*/
/*		Following functions provide communication possibilities for buffered data.		*/
/*		Connection and buffer has to be initialized first and config data functions 	*/
/*		can beused to read some channel information's first.							*/
/*																						*/
/*		For initialisation, Communication type "HSP_BUFFER" or "HSP_ECONLOGGER" has		*/
/*		to be used:																		*/
/*																						*/
/*		HSP_BUFFER ..... read data from the internal circle buffer						*/
/*		HSP_ECONLOGGER . read data from a e.con dataLogger 								*/
/*																						*/
/*		The cyclic data transmission between controller and PC is done by the DLL.		*/
/*		The DLL only provides buffers containing double values,							*/
/*		which are already decoded.														*/
/*		Following functions provide read access to this DLL Buffers.					*/
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------*/
/************		Initialize Buffer
												    
		Initializes a buffered connection with a specified circular buffer or test.con
		dataLogger on a supported controller.

		Communication Type must be HSP_BUFFER or HSP_LOGGER.

		eGateHighSpeedPort_Init() has to be used first!!
	    																		
 	IN:

		connection Instance	...	to select the correct connection

		buffer Index		... the index of a CircleBuffer or test.con Logger.
								Check configuration if the correct buffer type is supported
								and configured correctly!

	RETURN:	General Returncodes	
*/

 
int  _CD_eGateHighSpeedPort_InitBuffer(int connectionInstance,
														   int bufferIndex,
														   int autoRun);

/*--------------------------------------------------------------------------------------*/
/************			Set BackTime													

		Is used to set a BackTime Manually for one Ethernet Request.

		Backtime: defines how many seconds of buffer data should be read at one request.

	IN:

		connection Instance	...	to select the correct connection

		BackTime			... >0 : complete buffer will be read
								<=0: the next request will contain the last "BackTime" seconds
									 or the complete buffer if less than "BackTime" seconds 
									 are stored

	RETURN:	General Returncodes	

*/
 
int  _CD_eGateHighSpeedPort_SetBackTime(int connectionInstance,
															double BackTime);

/*--------------------------------------------------------------------------------------*/
/************	Rewind

	Moves the current frame pointer back.

	IN:
		connectionInstance	...	to select the correct connection
		clientInstance		... to select the correct client
		rewindFrames		... number of frames to rewind

	RETURN:
		General return codes

	ATTENTION:
		This function can only be used with PostProcessBuffer or file connections!!
*/

int  _CD_eGateHighSpeedPort_Rewind(int 	connectionInstance,
														   int 	clientInstance,
														   int 	rewindFrames);

/*--------------------------------------------------------------------------------------*/
/************		Clear Buffer Frames												

	Same as "eGateHighSpeedPort_SetBackTime"

*/
 
int _CD_eGateHighSpeedPort_ReadBuffer_Clear(int ConnectionInstance,
										double timeLeft);


/*--------------------------------------------------------------------------------------*/
/************		Get Buffer Frames																								

	Returns the number of available (read and decoded) data frames.
	The dll contains a FIFO with a fixed length -> data has to be read out continously
	with all clients, otherwise the data transfer will be paused 
	and the circle buffer may overun!

	IN:

		connectionInstance	...	to select the correct connection

		clientInstance		... to select the correct client
	
	RETURN:	General Returncodes	
*/
 
int _CD_eGateHighSpeedPort_GetBufferFrames(int ConnectionInstance,
									   int ClientInstance);


/*--------------------------------------------------------------------------------------*/
/************		Get Buffer Frames All

Returns the number of all data frames of the datasource.


IN:
connectionInstance	...	to select the correct connection
clientInstance		... to select the correct client

RETURN:
number of total frames in selected datasource
*/

long _CD_eGateHighSpeedPort_GetBufferFrames_All(int ConnectionInstance,
											int ClientInstance);


/*--------------------------------------------------------------------------------------*/
/************		Load Buffer Data

Returns the number of available (read and decoded) data frames.
The dll contains a FIFO with a fixed length -> data has to be read out continously
with all clients, otherwise the data transfer will be paused
and the circle buffer may overun!

IN:

connectionInstance	...	to select the correct connection

clientInstance		... to select the correct client

Out:

frames				... number of available data frames

RETURN:	General Returncodes
*/

int _CD_eGateHighSpeedPort_LoadBufferData(int connectionInstance, int *frames);

/*--------------------------------------------------------------------------------------*/
/************		Next Frame														

	Used to step to the next frame.
	As long as this function isn't called for all clients,
	"eGateHighSpeedPort_ReadBuffer_Single" will not point to the next frame.

  IN:

		connectionInstance	...	to select the correct connection

		clientInstance		... to select the correct client

  OUT:

  RETURN:	General Returncodes	
*/
 
int _CD_eGateHighSpeedPort_ReadBuffer_NextFrame(int ConnectionInstance,
												 int ClientInstance);



#define _CD_eGateHighSpeedPort_ReadBuffer_NextChannel _CD_eGateHighSpeedPort_ReadBuffer_NextFrame
#define SC_eGateHighSpeedPort_ReadBuffer_NextChannel SC_eGateHighSpeedPort_ReadBuffer_NextFrame
/*--------------------------------------------------------------------------------------*/
/************		Read buffered single channel - convert to double										

	Used to read the value of a specified channel from the actual frame and converts it to double.

	IN:
		connectionInstance	...	to select the correct connection

		clientInstance		... to select the correct client

		channel index		... to select the channel index (total index)
								use "eGateHighSpeedPort_GetChannelInfo_int" to convert
								channel index if necessary.

	OUT:

		value				... value converted to double


	RETURN:	General Returncodes	
*/

 
int _CD_eGateHighSpeedPort_ReadBuffer_Single(int ConnectionInstance,
										 int ClientInstance,
									     int ChannelIndex,
										 double* value);


/*--------------------------------------------------------------------------------------*/
/************		Read buffered single channel - raw data type

Used to read the value of a specified channel from the actual frame to buffer.

IN:
connectionInstance	...	to select the correct connection

clientInstance		... to select the correct client

channel index		... to select the channel index (total index)
use "eGateHighSpeedPort_GetChannelInfo_int" to convert
channel index if necessary.

OUT:

value				... raw value


RETURN:	General Returncodes


EGATEHIGHSPEEDPORT_API
int CALLINGCONVENTION_CD _CD_eGateHighSpeedPort_ReadBuffer_Single_RawType(int ConnectionInstance,
																			  int ClientInstance,
																			  int ChannelIndex,
																			  char* value,
																			  size_t valueSize);

EGATEHIGHSPEEDPORT_API
int CALLINGCONVENTION_SC SC_eGateHighSpeedPort_ReadBuffer_Single_RawType(int ConnectionInstance,
																			 int ClientInstance,
																			 int ChannelIndex,
																			 char* value,
																			 size_t valueSize);*/
/*--------------------------------------------------------------------------------------*/
/************		Read buffered multiple channels										

	Not supported


EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_CD 
_CD_eGateHighSpeedPort_ReadBuffer_Window(int connectionInstance);
EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_SC 
SC_eGateHighSpeedPort_ReadBuffer_Window(int clientInstance);*/

/*--------------------------------------------------------------------------------------*/
/************		Read buffer to double array									

	Reads buffered data to a double array.
	No worker threads are needed, every call initiates TCP/IP communication and data decoding.

	"eGateHighSpeedPort_Init" and "eGateHighSpeedPort_InitBuffer" has to be used first!

  	IN:
		connectionInstance	...	To select the correct connection


		arrayLength			... Number of elements in "valueArray"
								If "valueArray" is smaller than "arrayLength" this will cause a segfault!!!

		fillArray			... With fillArray set to "1" this call will block until "arrayLength/channelCount"
								frames are received!

	OUT:

		valueArray			... Pointer to a double array with at least "ArrayLength" elements
								Contains double converted values.


		receivedFrames		... Number of frames in valueArray after processing
								(frame = 1 sample over all channels)

		channelCount		... Number of channels in one frame
								(can also be read with "getNumberOfChannels")

		complete			...	Indicates if one TCP/IP request was completely decoded


	RETURN:	General Returncodes	

*/
 
int _CD_eGateHighSpeedPort_ReadBufferToDoubleArray(int ConnectionInstance,
											double *valueArray,
											int arrayLength,
											int fillArray,
											int *receivedFrames,
											int*channelCount,
											int *complete);

 
int _CD_eGateHighSpeedPort_ReadBufferToDoubleArray_StraightTimestamp(int ConnectionInstance,
																double *valueArray,
																int arrayLength,
																int fillArray,
																int *receivedFrames,
																int	*channelCount,
																int *complete);

//////////////////////////////////////////////////////////////////////////////////////////
/*------------- DLL Diagnostic ---------------------------------------------------------*/
/*																						*/
/*	Description:																		*/
/*																						*/
/*		Following functions provide diagnostic actions and information's				*/
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////	
/*--------------------------------------------------------------------------------------*/
/************		DLL Version/Date													*/
 
void  _CD_eGateHighSpeedPort_IdentDLL(char *dllVersion, 
												          char *dllDate);

/*--------------------------------------------------------------------------------------*/
/************		Toggle debug mode													

	In debug mode, this DLL will generate a log file.


EGATEHIGHSPEEDPORT_API 
void CALLINGCONVENTION_CD _CD_eGateHighSpeedPort_ToggleDebugMode(void);
EGATEHIGHSPEEDPORT_API 
void CALLINGCONVENTION_SC SC_eGateHighSpeedPort_ToggleDebugMode(void);*/
/*--------------------------------------------------------------------------------------*/
/************		Explain error message												

	Is used to get error information in plain text if any error-returncode is thrown

	IN:
		connectionInstance	...	to select the correct connection

	OUT:

		error message		... plain text error message


	RETURN:	General Returncodes	

*/
 
int  _CD_eGateHighSpeedPort_ExplainError(int connectionInstance, 
															 char errorMessage[]);


/*--------------------------------------------------------------------------------------*/
/************		Get debug message												

	Is used to get error information in plain text if any error-returncode is thrown

	IN:
		connectionInstance	...	to select the correct connection

	OUT:

		debug message		... plain text error message


	RETURN:	General Returncodes	

*/
 
int  _CD_eGateHighSpeedPort_GetDebugMessage(char errorMessage[]);


/*--------------------------------------------------------------------------------------*/
/************		Get debug message	
EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_CD _CD_eGateHighSpeedPort_WriteDebugMessage(char errorMessage[]);
EGATEHIGHSPEEDPORT_API 
int CALLINGCONVENTION_SC SC_eGateHighSpeedPort_WriteDebugMessage(char errorMessage[]);*/
/*--------------------------------------------------------------------------------------*/
/************		Read Connection State										
																					
	If a connection is broken (e.g. ethernet disconnect or module restart) the dll 
	will try to establish the connection agin as int as the connection isn't terminated.

	This function can be used to indicate the actual connection state.
	
	  IN:

		connectionInstance	...	to select the correct connection

	RETURN:

		1 ....  connection open
		0 ....	connection closed
*/
 
int  _CD_eGateHighSpeedPort_Connected(int connectionInstance);

/*--------------------------------------------------------------------------------------*/
/************		Get Diagnostic														


	Provides system diagnostic info's
	
	Diag info's are only up to date when diagLevel==DIAG_CONTROLLER was used before!
	If errorCount != 0 then other diagLevels can be checked for errors.

	IN:

		connectionInstance	...	to select the correct connection

		diagLevel			... to request the desired info

		index				... request the info from a specified index


	OUT:	

		cycleCount			... number of communication cycles for the requested item (should be)

		errorCount			... diagLevel==DIAG_CONTROLLER:

									the number of errors over the whole system.
									Use this diagLevel to refresh diag data!

								diagLevel==DIAG_intERFACE:

									the number of error on a specifc interface
									(internal, UART1, UART2,...)

								diagLevel==DIAG_TRANSPORT:

									the number of error on a specifc transport
									(system variables, virtual variables, Slave1, Slave2,..)

								diagLevel==DIAG_VARIABLE:

									not used

								diagLevel==DIAG_ITEMCOUNT:

									the number of items for the defined level

	RETURN:	General Returncodes	
*/

int  _CD_eGateHighSpeedPort_Diagnostic(int connectionInstance,
														   int diagLevel,
														   int index,
														   int *cycleCount,
														   int *errorCount);

/*--------------------------------------------------------------------------------------*/
/************		Register Callback Proc												*/
/*
EGATEHIGHSPEEDPORT_API
void CALLINGCONVENTION_CD _CD_eGateHighSpeedPort_RegisterCallback(int connectionInstance, 
															     int callbackType,
																 messageProc callFunc);
EGATEHIGHSPEEDPORT_API
void CALLINGCONVENTION_SC SC_eGateHighSpeedPort_RegisterCallback(int connectionInstance, 
																 int callbackType,
																 messageProc callFunc);
																 */
/*--------------------------------------------------------------------------------------*/
/************		Remote control command												

	This function is part of the inter process communication mechanism.

	It can be used to send commands to every process that uses the DLL
*/

void  _CD_eGateHighSpeedPort_RemoteControl(int controlID);


//////////////////////////////////////////////////////////////////////////////////////////
/*------------- File transfer functions ------------------------------------------------*/
/*																						*/
/*	Description:																		*/
/*																						*/
/*		Following functions provide file transfer - and decode functions				*/
/*																						*/
/*		Files can only be read or deleted completely, but not written or modified.		*/
/*		Configuration handling has to be done via FTP									*/
/*																						*/ 
/*		Files can either be copied to a drive or decoded online without storing to a	*/
/*		file.																			*/
/*		After decoding data from local files or a file stream from the controller,		*/	
/*		the values are accessible in the same way as reading buffer values.				*/																
/*		(eGateHighSpeedPort_ReadBuffer_NextFrame, eGateHighSpeedPort_ReadBuffer_Single)	*/
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////	
/*--------------------------------------------------------------------------------------*/
/************		Number of files														

	Used to read the number of files on the device.
	A file TypeID is also neccessary to control the desired file type

	This function will also store file specific info's which can be read with
	"eGateHighSpeedPort_GetFileInfo".

	IN:
		connectionInstance	... to select the correct connection

		fileTypeID			... to define the file type on e.gate, Q.gate, Q.pac 
								(use FILE_DIR_ALL to get all Files or 
								FILE_IDENTIFY_BY_PATH to specify a sub path on e.g. Q.Station)

		filePath			... sub path on the device

	OUT:

		fileCount			... number of files regarding file type

	RETURN:	General Returncodes	
*/

int  _CD_eGateHighSpeedPort_GetFileCount(int connectionInstance, 
																int	 fileTypeID,
																const char  *filePath,
																int*	 fileCount);

/*--------------------------------------------------------------------------------------*/
/************		Get file info														

	Used to read name, size and time of a specified file.

	IN:
		connectionInstance	... to select the correct connection

		fileIndex			... index to select a certain file

	OUT:

		fileName			... fileName on device

		fileNameLen			... capacity of fileName buffer

		fileIdent			... file identification, needed to access file on device

		fileIdentLen		... capacity of fileIdent buffer

		size				... size of file

		OLETime				...	days since 01.01.1900 00:00:00
								(use eGate_OLETime2TimeStruct to convert)

								ATTENTION: 
								this time is not absolutely synchronous to the timestamp!
								
	RETURN:	General Returncodes	
*/

int  _CD_eGateHighSpeedPort_GetFileInfo(int connectionInstance, 
																int fileIndex,
																char *fileName,
																int fileNameLen,
																char* fileIdent,
																int fileIdentLen,
																int* size,
																double* OLETime);

/*--------------------------------------------------------------------------------------*/
/************		Copy file															

	Used to copy a file on device to a local path

	IN:
		connectionInstance	... to select the correct connection

		fileIdent			... file identification as received with "_CD_eGateHighSpeedPort_GetFileInfo()"

		savePath			... existing path + file name

	RETURN:	General Returncodes	
*/

int  _CD_eGateHighSpeedPort_CopyFile(int connectionInstance, 
														 const char *fileIdent,
														 const char *savePath);

/*--------------------------------------------------------------------------------------*/
/************		Delete file															

	Used to delete a file on device

	IN:
		connectionInstance	... to select the correct connection

		fileIdent			... file identification as received with "_CD_eGateHighSpeedPort_GetFileInfo()"

	RETURN:	General Returncodes
*/

int  _CD_eGateHighSpeedPort_DeleteFile(int connectionInstance, 
														   const char *fileIdent);

//////////////////////////////////////////////////////////////////////////////////////////
/*------------- Data Storage functions -------------------------------------------------*/
/*																						*/
/*	This dll suports automatic data storage functions:									*/
/*	Any connection can be linked to a data storage mechanism that will run in an own	*/
/*	thread.																				*/
/*																						*/
/*	At the moment, data can be stored to MDF or ASCII format							*/
/*																						*/
//////////////////////////////////////////////////////////////////////////////////////////	
/************		Initialize MDF data storage												

		Initializes a new thread for data storage to MDF format.

		Only data from one connection can be stored to a MDF file.
		If several connections shoule be stored to a single file, 
		synchronisation has to be done first.

	IN:

		connection Instance ... to select the data source

		fileName			...	defines the format of the filename

		maxSamples			... defines the length of the file in samples (<0 if no limit)

		maxSeconds			... defines the length of the file in seconds (<0 if no limit)

  RETURN:

		General Returncodes

*/
 
int 	_CD_eGateHighSpeedPort_InitMDFStorage(int connectionInstance,
																	const char* fileNameFormat,
																	const char* text,
																	const char* name,
																	const char* department,
																	const char* project,
																	const char* subject,
																	double maxSamples,
																	double maxSeconds,
																	double totalSamples,
																	double totalSeconds);

/*--------------------------------------------------------------------------------------*/
/************		Add MDF channel

  This function selects a channel from the connection to be stored in the MDF file.
  10 Groups are possible with 1024 channels for each group as maximum.

	IN:

		connection Instance ... to select the data source
*/

 
int 	_CD_eGateHighSpeedPort_ConfigureMDFChannel(int connectionInstance,
																	   int MDFGroupIndex,
																	   int MDFChannelIndex,
																	   int HWChannelIndex,
																	   int isTimeChannel,
																	   const char *unit,
																	   float factor,
																	   float offset);

/*--------------------------------------------------------------------------------------*/
/************		Modify MDF Timestamp
	
	No Info available

*/
 
int 	_CD_eGateHighSpeedPort_ModifyMDFTimestamp(int connectionInstance,
																	 int TSType);



/*--------------------------------------------------------------------------------------*/
/************		Initialize ASCII data storage												

		Initializes a new thread for data storage to any ASCII format.

		Only data from one connection can be stored to a ASCII file.
		If several connections shoule be stored to a single file, 
		synchronisation has to be done first.

	IN:

		connection Instance ... to select the data source

		fileName			...	defines the format of the filename

		maxSamples			... defines the length of the file in samples (<0 if no limit)

		maxSeconds			... defines the length of the file in seconds (<0 if no limit)

  RETURN:

		General Returncodes

*/
 
int 	_CD_eGateHighSpeedPort_InitASCIIStorage(int connectionInstance,
																		const char* fileNameFormat,
																		const char* fileHeaderText,
																		const char* channelSepChar,
																		const char* frameSepChar,
																		double maxSamples,
																		double maxSeconds,
																		double totalSamples,
																		double totalSeconds);


/*--------------------------------------------------------------------------------------*/
/************		Add ASCII save channel

  This function selects a channel from the connection to be stored in the ASCII file.
  10 Groups are possible with 1024 channels for each group as maximum.

	IN:

		connection Instance	... to select the data source

		HWChannelIndex		... real channel index on device

		numFract			... number of fractal positions 

								if <1 -> as configured on the module
*/

 
int 	_CD_eGateHighSpeedPort_AddASCIIChannel(int connectionInstance,
																   int HWChannelIndex,
																   int numFract);


/*------------- Helper -------------------------------------------------------*/
 
double  _CD_eGate_TimeStruct2OLETime(int year, 
														 int month, 
														 int day, 
														 int hour, 
														 int minute,
														 int second, 
														 double belowSeconds);

 
double  _CD_eGate_OLETime2TimeStruct(double OLETime,
														 int *year, 
														 int *month, 
														 int *day, 
														 int *hour, 
														 int *minute,
														 int *second, 
														 double *belowSeconds);


/*--------------------------------------------------------------------------------------*/
/************		Sleep MS											

		Can be used to sleep

	IN:

		time_msec			 ... time to sleep in milli seconds

  RETURN:

		General Returncodes

*/

int  _CD_eGateHighSpeedPort_SleepMS(int time_msec);


