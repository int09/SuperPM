; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProcessManager
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "superprocessmanager.h"
LastPage=0

ClassCount=14
Class1=CMainFrame
Class2=CProcessFileIODlg
Class3=CProcessManager
Class4=CProcessNetworkIODlg
Class5=CProcessRegIODlg
Class6=CPublicTextBarDlg
Class7=CRunlogDlg
Class8=CSuperProcessManagerApp
Class9=CAboutDlg
Class10=CSuperProcessManagerDoc
Class11=CSuperProcessManagerView

ResourceCount=14
Resource1=IDR_NETWORK_IO_DATA_LIST_MENU
Resource2=IDR_NOTIFYICON_MENU
Resource3=IDD_MONITOR_NETWORKIO_SETTING_DLG
Resource4=IDD_PROCESS_FILEIO_DLG
Resource5=IDD_PROCESS_MANAGER_DLG
Resource6=IDD_MONITOR_FILE
Resource7=IDD_RUNLOG_DLG
Resource8=IDD_PROCESS_NETWORKIO_DLG
Resource9=IDD_PROCESS_REGIO_DLG
Resource10=IDD_ABOUTBOX
Resource11=IDR_PROCESS_LIST_MENU
Class12=CMonitorNetworkIoSettingDlg
Resource12=IDD_TEXTBAR_DLG
Class13=CMonitorFileDlg
Resource13=IDD_LISTCTRL_FILTER_DLG
Class14=CCMListCtrlFilterDlg
Resource14=IDR_MAINFRAME

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=IDM_MONITOR_FILE

[CLS:CProcessFileIODlg]
Type=0
BaseClass=CDialog
HeaderFile=ProcessFileIODlg.h
ImplementationFile=ProcessFileIODlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CProcessFileIODlg

[CLS:CProcessManager]
Type=0
BaseClass=CDialog
HeaderFile=ProcessManager.h
ImplementationFile=ProcessManager.cpp
Filter=D
VirtualFilter=dWC
LastObject=CProcessManager

[CLS:CProcessNetworkIODlg]
Type=0
BaseClass=CDialog
HeaderFile=ProcessNetworkIODlg.h
ImplementationFile=ProcessNetworkIODlg.cpp
Filter=W
VirtualFilter=dWC
LastObject=IDM_FILTER

[CLS:CProcessRegIODlg]
Type=0
BaseClass=CDialog
HeaderFile=ProcessRegIODlg.h
ImplementationFile=ProcessRegIODlg.cpp
LastObject=CProcessRegIODlg

[CLS:CPublicTextBarDlg]
Type=0
BaseClass=CDialogBar
HeaderFile=PublicTextBarDlg.h
ImplementationFile=PublicTextBarDlg.cpp

[CLS:CRunlogDlg]
Type=0
BaseClass=CDialog
HeaderFile=RunlogDlg.h
ImplementationFile=RunlogDlg.cpp

[CLS:CSuperProcessManagerApp]
Type=0
BaseClass=CWinApp
HeaderFile=SuperProcessManager.h
ImplementationFile=SuperProcessManager.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=SuperProcessManager.cpp
ImplementationFile=SuperProcessManager.cpp
LastObject=CAboutDlg

[CLS:CSuperProcessManagerDoc]
Type=0
BaseClass=CDocument
HeaderFile=SuperProcessManagerDoc.h
ImplementationFile=SuperProcessManagerDoc.cpp

[CLS:CSuperProcessManagerView]
Type=0
BaseClass=CView
HeaderFile=SuperProcessManagerView.h
ImplementationFile=SuperProcessManagerView.cpp

[DLG:IDD_PROCESS_FILEIO_DLG]
Type=1
Class=CProcessFileIODlg
ControlCount=1
Control1=IDC_LIST,SysListView32,1350631429

[DLG:IDD_PROCESS_MANAGER_DLG]
Type=1
Class=CProcessManager
ControlCount=1
Control1=IDC_PROCESS_LIST,SysListView32,1350631437

[DLG:IDD_PROCESS_NETWORKIO_DLG]
Type=1
Class=CProcessNetworkIODlg
ControlCount=3
Control1=IDC_DATA_LIST,SysListView32,1350631437
Control2=IDC_HEX_TEXT,edit,1352732676
Control3=IDC_ASCII_TEXT,edit,1352732676

[DLG:IDD_PROCESS_REGIO_DLG]
Type=1
Class=CProcessRegIODlg
ControlCount=0

[DLG:IDD_TEXTBAR_DLG]
Type=1
Class=CPublicTextBarDlg
ControlCount=1
Control1=IDC_COMBO_ADDRESS,combobox,1344340034

[DLG:IDD_RUNLOG_DLG]
Type=1
Class=CRunlogDlg
ControlCount=1
Control1=IDC_LOG_LIST,listbox,1352728835

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_MYSAY,static,1342308608
Control6=IDC_STATIC,static,1342181383

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_COPY
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=IDM_PUBLIC_TEXT_BAR
Command11=IDM_MONITOR_FILE
Command12=IDM_MONITOR_NETWORKIO_SETTING
Command13=ID_APP_ABOUT
CommandCount=13

[MNU:IDR_PROCESS_LIST_MENU]
Type=1
Class=CProcessManager
Command1=IDM_TERMINATOR_PROCESS_CUREENT
Command2=IDM_TERMINATOR_PROCESS_CHECKBOX
Command3=IDM_TERMINATOR_PROCESS_TREE
Command4=IDM_SUSPPED_PROCESS
Command5=IDM_INJECT_PROCESS
Command6=IDM_DEBUG_PROCESS
Command7=IDM_SET_PRIORITY_REALTIME
Command8=IDM_SET_PRIORITY_HEIGHT
Command9=IDM_SET_PRIORITY_ABOVE_HEIGHT
Command10=IDM_SET_PRIORITY_NORMAL
Command11=IDM_SET_PRIORITY_BELOW_NORMAL
Command12=IDM_SET_PRIORITY_IDEL
Command13=IDM_PROCESS_MODULE
Command14=IDM_PROCESS_THREAD
Command15=IDM_PROCESS_HANDLE
Command16=IDM_MONITOR_NETWOKIO
Command17=IDM_MONITOR_FILEIO
Command18=IDM_MONITOR_REGIO
Command19=IDM_MONITOR_ALL
Command20=IDM_POSITIONING_FILE
Command21=IDM_ATTRIBUTE_FILE
Command22=IDM_GOOGLE_SEARCH
Command23=IDM_BAIDU_SEARCH
Command24=IDM_ONLINE_VIRUS_SCAN
CommandCount=24

[MNU:IDR_NOTIFYICON_MENU]
Type=1
Class=?
Command1=IDM_SHOW_WINDOW
Command2=IDM_TOPMOST
Command3=IDM_SETTING
Command4=IDM_UPDATE
Command5=IDM_ABOUT
Command6=IDM_EXIT
CommandCount=6

[MNU:IDR_NETWORK_IO_DATA_LIST_MENU]
Type=1
Class=CProcessNetworkIODlg
Command1=IDM_SETTING
Command2=IDM_MONITOR_FILE
Command3=IDM_FILTER
Command4=IDM_TERMENATER_PROCESS
Command5=IDM_SUSPEND_PROCESS
Command6=IDM_RESUME_PROCESS
Command7=IDM_DELETE_INVALID_DATA
Command8=IDM_DELETE_SELECED
Command9=IDM_DELETE_CURRENT
Command10=IDM_SELECT_ALL
Command11=IDM_SELECT_REVERSE_ALL
Command12=IDM_CANCEL_ALL
Command13=IDM_OPEN_IP
Command14=IDM_OPEN_DNS
Command15=IDM_BAIDU_SEARCH_IP
Command16=IDM_GOOGLE_SEARCH_IP
Command17=IDM_BAIDU_SEARCH_DNS
Command18=IDM_GOOGLE_SEARCH_DNS
CommandCount=18

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_MONITOR_NETWORKIO_SETTING_DLG]
Type=1
Class=CMonitorNetworkIoSettingDlg
ControlCount=12
Control1=IDC_STATIC,button,1342177287
Control2=IDC_HOOK_SEND,button,1342242819
Control3=IDC_HOOK_WSASEND,button,1342242819
Control4=IDC_HOOK_RECV,button,1342242819
Control5=IDC_HOOK_WSARECV,button,1342242819
Control6=IDC_STATIC,button,1342177287
Control7=IDC_HOOK_SENDTO,button,1342242819
Control8=IDC_HOOK_WSASENDTO,button,1342242819
Control9=IDC_HOOK_RECVFROM,button,1342242819
Control10=IDC_HOOK_WSARECVFROM,button,1342242819
Control11=IDC_STATIC,static,1342308352
Control12=IDC_DONE,button,1342242817

[CLS:CMonitorNetworkIoSettingDlg]
Type=0
HeaderFile=MonitorNetworkIoSettingDlg.h
ImplementationFile=MonitorNetworkIoSettingDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMonitorNetworkIoSettingDlg
VirtualFilter=dWC

[DLG:IDD_MONITOR_FILE]
Type=1
Class=CMonitorFileDlg
ControlCount=21
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FILE_PATH,edit,1350631552
Control3=IDC_BROWSER_FILE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMMAN_LINE,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_WORK_DIRECTORY,edit,1350631552
Control8=IDC_BROWSER_DIR,button,1342242816
Control9=IDC_HIDE_RUN,button,1342177289
Control10=IDC_SHOW_RUN,button,1342177289
Control11=IDC_STATIC,static,1342308352
Control12=IDC_DESKTOP,edit,1350631552
Control13=IDC_STATIC,button,1342177287
Control14=IDC_MONITOR_TAGERT,button,1342242816
Control15=IDC_STATIC,button,1342177287
Control16=IDC_MONITOR_NETWORKIO,button,1342242819
Control17=IDC_MONITOR_FILEIO,button,1342242819
Control18=IDC_MONITOR_REGIO,button,1342242819
Control19=IDC_SWITCH_NETWORKMONITRO,button,1342177289
Control20=IDC_SWITCH_FILEMONITRO,button,1342177289
Control21=IDC_SWITCH_REGMONITRO,button,1342177289

[CLS:CMonitorFileDlg]
Type=0
HeaderFile=MonitorFileDlg.h
ImplementationFile=MonitorFileDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=IDC_FILE_PATH
VirtualFilter=dWC

[DLG:IDD_LISTCTRL_FILTER_DLG]
Type=1
Class=CCMListCtrlFilterDlg
ControlCount=12
Control1=IDC_COMBO_COLUMN,combobox,1344339971
Control2=IDC_COMBO_RULE,combobox,1344339971
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PARAMETER_TEXT,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_OPERATE_SELECT,button,1342242816
Control8=IDC_OPERATE_DELETE,button,1342242816
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_RESULT_TEXT,static,1342308608
Control12=IDC_STATIC,button,1342177287

[CLS:CCMListCtrlFilterDlg]
Type=0
HeaderFile=CMListCtrlFilterDlg.h
ImplementationFile=CMListCtrlFilterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO1
VirtualFilter=dWC

