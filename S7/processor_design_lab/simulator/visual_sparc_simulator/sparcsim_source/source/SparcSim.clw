; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CParamsDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SparcSim.h"
LastPage=0

ClassCount=13
Class1=CSparcSimApp
Class2=CSparcSimDoc
Class3=CSparcSimView
Class4=CMainFrame

ResourceCount=11
Resource1=IDD_REGFILE
Resource2=IDR_SSDOCTYPE
Class5=CAboutDlg
Class6=CChildFrame
Resource3=IDD_PREGS
Resource4=IDD_DIALOG1
Class7=CRegfileDlg
Resource5=IDR_MAINFRAME
Resource6=IDD_ABOUTBOX
Class8=CDmemDlg
Class9=CImemDlg
Resource7=IDD_IMEM
Class10=CPregsDlg
Class11=CEmDlg
Resource8=IDD_PARAMS
Resource9=IDD_EM
Class12=CParamsDlg
Resource10=IDD_STATS
Class13=CStatsDlg
Resource11=IDD_DMEM

[CLS:CSparcSimApp]
Type=0
HeaderFile=SparcSim.h
ImplementationFile=SparcSim.cpp
Filter=N
LastObject=CSparcSimApp

[CLS:CSparcSimDoc]
Type=0
HeaderFile=SparcSimDoc.h
ImplementationFile=SparcSimDoc.cpp
Filter=N

[CLS:CSparcSimView]
Type=0
HeaderFile=SparcSimView.h
ImplementationFile=SparcSimView.cpp
Filter=C
LastObject=CSparcSimView
BaseClass=CEditView
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_PARAMS
BaseClass=CMDIFrameWnd
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M

[CLS:CAboutDlg]
Type=0
HeaderFile=SparcSim.cpp
ImplementationFile=SparcSim.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_ASSEMBLE
Command10=ID_EXECUTE
Command11=ID_EXCYCLE
Command12=ID_EXMCYCLES
Command13=ID_EXNS
Command14=ID_SHOWTOOLS
Command15=ID_PARAMS
CommandCount=15

[MNU:IDR_SSDOCTYPE]
Type=1
Class=CSparcSimView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_SIM_ASSEMBLE
Command18=ID_SIM_LOAD
Command19=ID_SIM_EXECUTE
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_APP_ABOUT
CommandCount=24

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_REGFILE]
Type=1
Class=CRegfileDlg
ControlCount=1
Control1=IDC_LIST1,listbox,1352732929

[CLS:CRegfileDlg]
Type=0
HeaderFile=RegfileDlg.h
ImplementationFile=RegfileDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRegfileDlg
VirtualFilter=dWC

[DLG:IDD_PREGS]
Type=1
Class=CPregsDlg
ControlCount=1
Control1=IDC_LIST1,listbox,1352733057

[DLG:IDD_DMEM]
Type=1
Class=CDmemDlg
ControlCount=4
Control1=IDC_LIST1,listbox,1352732929
Control2=IDC_EDIT1,edit,1352728708
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_IMEM]
Type=1
Class=CImemDlg
ControlCount=1
Control1=IDC_LIST1,listbox,1352732929

[CLS:CDmemDlg]
Type=0
HeaderFile=DmemDlg.h
ImplementationFile=DmemDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT1
VirtualFilter=dWC

[CLS:CImemDlg]
Type=0
HeaderFile=ImemDlg.h
ImplementationFile=ImemDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CImemDlg
VirtualFilter=dWC

[CLS:CPregsDlg]
Type=0
HeaderFile=PregsDlg.h
ImplementationFile=PregsDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPregsDlg
VirtualFilter=dWC

[DLG:IDD_EM]
Type=1
Class=CEmDlg
ControlCount=1
Control1=IDC_LIST1,listbox,1352732929

[CLS:CEmDlg]
Type=0
HeaderFile=EmDlg.h
ImplementationFile=EmDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CEmDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[DLG:IDD_PARAMS]
Type=1
Class=CParamsDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_EDIT2,edit,1350631552

[CLS:CParamsDlg]
Type=0
HeaderFile=ParamsDlg.h
ImplementationFile=ParamsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_STATS]
Type=1
Class=CStatsDlg
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350641792

[CLS:CStatsDlg]
Type=0
HeaderFile=StatsDlg.h
ImplementationFile=StatsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

