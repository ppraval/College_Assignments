// DmemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "DmemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDmemDlg dialog


CDmemDlg::CDmemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDmemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDmemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDmemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDmemDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDmemDlg, CDialog)
	//{{AFX_MSG_MAP(CDmemDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDmemDlg message handlers
