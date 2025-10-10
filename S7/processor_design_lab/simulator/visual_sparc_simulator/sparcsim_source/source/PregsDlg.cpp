// PregsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "PregsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPregsDlg dialog


CPregsDlg::CPregsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPregsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPregsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPregsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPregsDlg)
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPregsDlg, CDialog)
	//{{AFX_MSG_MAP(CPregsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPregsDlg message handlers
