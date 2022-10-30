/*#########################################################################*/
/*
   WARNING: Do not add to, delete from, or otherwise modify the contents
            of this header file !!!!!!!!!!!!!!!!!!!!!!!!!

   COPYRIGHT    : (C) 2004 by Gantner instruments, all rights reserved !
*/
/*.........................................................................*/
/*
   PROJECT      : eGateUtility.dll interface
   FILE NAME    : eGateUtility.pas

   COMPILER     : MSVC++ 6.0
   SYSTEM       : Windows95 / 98 / ME / NT / 2K / XP

   DATE         : 2004-01-21
   AUTHOR       : Stefan Stocker

*/
/*.........................................................................*/
/*
   COMMENT:
     C-Header-File to use with DLL.

     ATTENTION: eGateUtility.LIB can be build in the following way:
                  - Create a eGateUtility.DEF file, where all exported DLL-
                     functions listed under keyword "EXPORTS" (to get all
   			         exported functions you can use VC++ build in
  			         "dumpbin" program, calling commandline
  			         "dumpbin /EXPORTS ismbus32.dll >eGateUtility.fct");
  		          - Create library eGateUtility.LIB by means of VC++ build in
  		             "lib" program, calling commandline
  			         "lib /MACHINE:IX86 /DEF:eGateUtility.DEF";
*/
/*.........................................................................*/
/*
   ATTENTION: All strings need to have a terminating 0 at the end !!!!
*/
/*
   CHANGES:
             2004-01-21    Implementation;
             2005-02-01    Function "DeleteFile" added;
             2005-02-12    Function "GetProperty" and "SetProperty" added;
             2005-03-01    Function "GetPingTime" added;
             2005-08-23    Function "DecodeBufferFileEx" added;
             2008-01-08    Property @IDENT_ADD_RESPONSE_TIME_TYPE,
                            @IDENT_CONTROL_RESPONSE_TIMEOUT_S,
                            @IDENT_SCAN_TIME_FACTOR_PPP and
                            @IDENT_NO_ADD_DEVICES_FOUND_COUNT_TO_TERM_SCAN added;
                           Property @FTP_USE_PASSIVE_MODE and
                            @FTP_USE_PING_BEFORE_CONNECT;
                           HighspeedPortTCPIP-functions added;
             2008-10-06    Function "LZSSCompressUncompressLocalFile" and
                            "CalculateCRCOfLocalFile" added;
                           @Function "GetActualDirectory_First", filename can contain
                            searchmask with wildcards;

*/
/*#########################################################################*/

#ifdef __cplusplus
extern  "C" {
#endif

/*#########################################################################*/
/* Definitions                                                             */
/*#########################################################################*/

#define PRE_DECL                       __declspec( dllimport )
#define ALL_DECL                       _cdecl

// CONSTANTS ******************************************************************
#define  eGUt_DLL_Name							"eGateUtility.dll"

//  General Returncodes:
#define  eGUt_GENRET_OK                         0
#define  eGUt_GENRET_FAIL                       1


/*#########################################################################*/
/* Function prototypes                                                     */
/*#########################################################################*/


/******************************************************************************/
/*
  Outputs DLL-Identification.
  OUT:
	DLLDescription  : DLLDescription          (reserve >100 bytes);
	DLLVersion      : DLLVersion              (reserve >100 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_IdentDLL(
						char *DLLDescription,
						char *DLLVersion);

/******************************************************************************/
/*
  For internal use only.
*/

PRE_DECL long ALL_DECL _CDDLG_ToggleDebugMode(
						long State);

/******************************************************************************/
/*
  Enables setting of different properties.
  IN:
    Name             : Property name
    ValueStr         : Value string according given name
                        (decimal separator is fixed '.')
  OUT:
    ErrorStr         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes

  Available property names:
    ----- Section - @FTP ---------------------------------------------------------------------------
      REMARK: Only available if connection is opened and always set to default,
                if "OpenConnection" is called !!!
      '@FTP_TRANSFERTIMEOUT_S' ...
          timeout for read/write - in [s]
          default: timeout, set with "OpenConnection"
      '@FTP_READFILE_LIST_MAX_POLL_TIME_S' ...
          max. poll time at "ReadFile" for reading list and checking if file is existing before reading file - in [s]
          default: 1.0, set with "OpenConnection"
      '@FTP_READFILE_GET_MAX_POLL_TIME_S' ...
          max. poll time at "ReadFile" for reading file - in [s]
          default: timeout, set with "OpenConnection"
      '@FTP_USE_PASSIVE_MODE' ...
          0 ... OFF (ACTIVE MODE): Client opens random data port and tells number with PORT-command to server.
          1 ... ON (PASSIVE MODE): Client sends PASV command, server opens data port and sends number back to client => often needed for servers behind NAT or FIREWALLS
          default: 0
      '@FTP_USE_PING_BEFORE_CONNECT' ...
          0 ... OFF: No ping is done before connect.
          1 ... ON:  A successful ping is needed before it tries to connect to server.
          default: 1
    ----- Section - @IDENT -------------------------------------------------------------------------
      '@IDENT_ADD_RESPONSE_TIME_TYPE' ...
          DeviceInfo at IdentifyDevices_xxx contains also "RTI:<ResponseTime [s]>[TAB]" at beginning of each item;
          0 ... Off
          1 ... Request response time
          2 ... Full time
          default: 0
      '@IDENT_CONTROL_RESPONSE_TIMEOUT_S' ...
          Timeout, waiting for response for ident controlling commands - in [s]
          default: 0.1
      '@IDENT_SCAN_TIME_FACTOR_PPP' ...
          Scan time multiply factor in case of PPP adapter is detected
          default: 3.0
      '@IDENT_NO_ADD_DEVICES_FOUND_COUNT_TO_TERM_SCAN' ...
          Count of "no add. devices found" to terminat scan loop
          default: 1
*/

PRE_DECL long ALL_DECL _CDDLG_SetProperty(
						char   *Name,
						char   *ValueStr,
						char   *ErrorStr);

/******************************************************************************/
/*
  Enables getting of different properties.
  IN:
    Name             : Identification name
  OUT:
    ValueStr         : Value string according given name (reserve > 100 bytes)
                        (decimal separator is fixed '.')
    ErrorStr         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes

  Refer to function "SetProperty" for available property names !!!

*/

PRE_DECL long ALL_DECL _CDDLG_GetProperty(
						char   *Name,
						char   *ValueStr,
						char   *ErrorStr);

/******************************************************************************/
/*
  If a 'NIL' will be set and for default, a default routine will be called.
  Attention: Procedure must be compiled as 'FAR' and '_cdecl'.
	Defined procedure have to be in memory when called from DLL.
  Defines Procedure-Pointer to be called to monitor progress.
  A Double-Number will be transfered (0-100%) as argument.
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_SetProgressProc(
						void *Pnt);

/******************************************************************************/
/*
  Opens connection to a defined server.
  IN:
	ServerName       : server name or TCP/IP address
	Username         : username
	Password         : password
	Timeout          : timeout [s]
  OUT:
	ErrorStr         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_OpenConnection(
						char   *ServerName,
						char   *Username,
						char   *Password,
						double Timeout,
						char   *ErrorStr);

/******************************************************************************/
/*
  Closes connection when opened.
  ATTENTION: Be sure that this function is called before application is terminated,
			 so that proper closing of connection can happen !!!
  OUT:
	ErrorStr         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_CloseConnection(
						char   *ErrorStr);

/******************************************************************************/
/*
  Initiates directory listing iteration. Iteration runs up to return code is not OK.
  IN:
	FileName         : search filemask, containing wildcards;
  OUT:
	FileName         : filename (reserve >100 bytes);
	FileSize         : filesize in bytes
	ErrorStr         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_GetActualDirectory_First(
						char   *FileName,
						long   *FileSize,
						char   *ErrorStr);

/******************************************************************************/
/*
  Follows "first" command to get remaining listing items. Iteration runs up to return code is not OK.
  OUT:
	FileName         : filename (reserve >100 bytes);
	FileSize         : filesize in bytes
	ErrorStr         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_GetActualDirectory_Next(
						char   *FileName,
						long   *FileSize,
						char   *ErrorStr);

/******************************************************************************/
/*
  Reads one specified file to destination.
  Supports progress function !!!
  IN:
	SourceFileName                   : source filename;
	DestinationFullFileName          : destination filename with path;
	AppendToExistingDestinationFile  : if <> 0 than appending to an existing destination file;
  OUT:
	ErrorStr                         : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_ReadFile(
						char   *SourceFileName,
						char   *DestinationFullFileName,
						long   AppendToExistingDestinationFile,
						char   *ErrorStr);

/******************************************************************************/
/*
  Writes one specified file to destination.
  Supports progress function !!!
  IN:
	SourceFullFileName         : source filename with path;
	DestinationFileName        : destination filename;
  OUT:
	ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_WriteFile(
						char   *SourceFullFileName,
						char   *DestinationFileName,
						char   *ErrorStr);

/******************************************************************************/
/*
  Deletes one specified file at destination.
  IN:
	FileName                   : filename;
  OUT:
	ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_DeleteFile(
						char   *FileName,
						char   *ErrorStr);

/******************************************************************************/
/*
  Decodes a binary buffer file to a destination ASCII file. If header- and data-file
  is merged together, use only one of "SourcexxxxxxFullFilename" arguments and leave
  the other empty.
  Supports progress function !!!
  IN:
	SourceHeaderFullFilename   : source filename of header with path;
	SourceDataFullFilename     : source filename of data with path;
	DestinationFullFilename    : destination filename with path;
  OUT:
	ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
	General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_DecodeBufferFile(
						char   *SourceHeaderFullFilename,
						char   *SourceDataFullFilename,
						char   *DestinationFullFilename,
						char   *ErrorStr);

/******************************************************************************/
/*
  Like DecodeBufferFile but with possibility to define decoding.
  IN:
    SourceHeaderFullFilename   : source filename of header with path;
    SourceDataFullFilename     : source filename of data with path;
    DestinationFullFilename    : destination filename with path;
    DecodeKind                 : defines decoding
                                 0 .. Time info of each frame is calculated as:
                                       dActTime*dActTimeToSecondFactor (like DecodeBufferFile does)
                                 1 .. Time info of each frame is calculated as:
                                       no modifications; like stored in buffer file
                                 2 .. Time info of each frame is calculated as:
                                       StartTime*StartTimeToDayFactor + dActTime*dActTimeToSecondFactor/86400 (OLE2)
                                 3 .. Time info of each frame is calculated as:
                                       StartTime*StartTimeToDayFactor + dActTime*dActTimeToSecondFactor/86400 (DATETIMESTRING)
    AddArgument                : Additional argument, to control behaviour
                                 0 .. nothing
  OUT:
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_DecodeBufferFileEx(
						char   *SourceHeaderFullFilename,
						char   *SourceDataFullFilename,
						char   *DestinationFullFilename,
						long	DecodeKind,
						long	AddArgument,
						char   *ErrorStr);

/******************************************************************************/
/*
  Initiates info retrieving of connected devices iteration.
  Iteration runs up to return code is not OK.
  Supports progress function !!!
  IN:
    UseExtCommand              : if <> 0 than extended command is used
    ScanTime                   : duration of scan time for one network adapter [s]
  OUT:
    AdapterInfo                : info about used adapter; consists of: <AdapterType>[TAB]<AdapterName>
                                  (reserve >1024 bytes);
    DeviceInfo                 : complete info string as described in "e.bloxx System Guide" -
                                  section "Broadcastport ASCII/UDP" (reserve >1024 bytes)
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_IdentifyDevices_First(
						long   UseExtCommand,
						double ScanTime,
						char   *AdapterInfo,
						char   *DeviceInfo,
						char   *ErrorStr);

/******************************************************************************/
/*
  Follows "first" command to get remaining device info items.
  Iteration runs up to return code is not OK.
  OUT:
    AdapterInfo                : info about used adapter; consists of: <AdapterType>[TAB]<AdapterName>
                                  (reserve >1024 bytes);
    DeviceInfo                 : complete info string as described in "e.bloxx System Guide" -
                                  section "Broadcastport ASCII/UDP" (reserve >1024 bytes)
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_IdentifyDevices_Next(
						char   *AdapterInfo,
						char   *DeviceInfo,
						char   *ErrorStr);

/******************************************************************************/
/*
  Forces device to visually identify itself.
  IN:
    MACAddress                 : valid MAC Address;
    Timeout                    : duration of max. waiting for answer of device [s];
  OUT:
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_IdentifyVisual(
						char   *MACAddress,
						double Timeout,
						char   *ErrorStr);

/******************************************************************************/
/*
  Used to build up and send a broadcast command, as described in "e.bloxx System Guide" -
  section "Broadcastport ASCII/UDP".
  Control character (CR,LF and TAB) can be inputted like '\r', '\n' and '\t'.
  IN:
    MACAddress                 : valid MAC Address or EMPTY for real broadcast;
    BaseCommand                : the command body with NO control characters;
    AdditionalData             : to extent command if needed.
    TimeoutOrWaittime          : duration of waiting for answer of device or in case of
                                  real broadcast the waittime [s];
  OUT:
    ResponseStr                : response data of device(s). In average, such a command
                                  responds with at about 30-40 bytes per device, so maybe
                                  a good approach is to reserve 50*DeviceCount bytes to start with;
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_BroadcastCommand(
						char   *MACAddress,
						char   *BaseCommand,
						char   *AdditionalData,
						double TimeoutOrWaittime,
						char   *ResponseStr,
						char   *ErrorStr);

/******************************************************************************/
/*
  Used to ping a server.
  IN:
    ServerName                 : server name or TCP/IP address;
    Timeout                    : Waittime [s];
  OUT:
    ResponseTime               : response time [s];
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_GetPingTime(
						char   *ServerName,
						double Timeout,
						double *ResponseTime,
						char   *ErrorStr);

/******************************************************************************/
/*
  Opens HighspeedPortTCPIP-socket to a server.
  IN:
    ServerName                 : server name or TCP/IP address;
    Timeout                    : Waittime [s];
  OUT:
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_OpenHighspeedPortTCPIP(
						char   *IPAddress,
						double Timeout,
						char   *ErrorStr);

/******************************************************************************/
/*
  Closes HighspeedPortTCPIP-socket to a server.
  OUT:
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/

PRE_DECL long ALL_DECL _CDDLG_CloseHighspeedPortTCPIP(
						char   *ErrorStr);

/******************************************************************************/
/*
  Performs HighspeedPortTCPIP communication to a server.
  IN:
    Command                    : command according Highspeedport protocol;
    WriteData                  : data to write according Highspeedport protocol;
    WriteLength                : amount of data to write;
    WriteOffset                : offset to write to according Highspeedport protocol;
    Timeout                    : Waittime [s];
    UseDynamicLength           : if <> 0, ReadLength has not to be set at calling the function;
    ReadLength                 : if UseDynamicLength = 0, this amount of data are waited for;
  OUT:
    ReadData                   : received data;
    ReadLength                 : contains amount of received data;
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/
PRE_DECL long ALL_DECL _CDDLG_CommunicateHighspeedPortTCPIP(
						long   Command,
						char   *WriteData,
						long   WriteLength,
						long   WriteOffset,
						char   *ReadData,
						long   *ReadLength,
						double Timeout,
						long   UseDynamicLength,
						char   *ErrorStr);

/******************************************************************************/
/*
  Calculates CRC of a file.
  IN:
    FullFilename               : filename of data with path;
    BitWidth                   : Depth of CRC; supported: 16 / 32
  OUT:
    CRC                        : calculated value;
    ErrorStr                   : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/
PRE_DECL long ALL_DECL _CDDLG_CalculateCRCOfLocalFile(
						char	*FullFilename,
						long	BitWidth,
						long	*CRC,
						char	*ErrorStr);

/******************************************************************************/
/*
  (Un)Compresses file according LZSS algorithm.
  IN:
    SourceFullFilename            : filename of source with path;
    DestinationFullFilename       : filename of destination with path;
    IsCompress                    : if <> 0 => compress;
  OUT:
    DestinationToSourceSizeRatio  : ratio of sizes: DestinationFileSize/SourceFileSize;
    ErrorStr                      : error text or empty if no error (reserve >1024 bytes);
  RETURN:
    General Returncodes
*/
PRE_DECL long ALL_DECL _CDDLG_LZSSCompressUncompressLocalFile(
						char	*SourceFullFilename,
						char	*DestinationFullFilename,
						long	IsCompress,
						double	*DestinationToSourceSizeRatio,
						char	*ErrorStr);

/******************************************************************************/

/*#########################################################################*/
/* End header                                                              */
/*#########################################################################*/

#ifdef __cplusplus
}
#endif