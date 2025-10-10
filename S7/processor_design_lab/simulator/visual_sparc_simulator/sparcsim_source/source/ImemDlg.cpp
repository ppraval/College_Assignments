// ImemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "ImemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImemDlg dialog


CImemDlg::CImemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImemDlg)
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImemDlg, CDialog)
	//{{AFX_MSG_MAP(CImemDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImemDlg message handlers
