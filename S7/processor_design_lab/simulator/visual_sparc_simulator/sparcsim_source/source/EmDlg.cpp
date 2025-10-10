// EmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "EmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmDlg dialog


CEmDlg::CEmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmDlg)
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmDlg, CDialog)
	//{{AFX_MSG_MAP(CEmDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmDlg message handlers
