// RegfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "RegfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegfileDlg dialog


CRegfileDlg::CRegfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegfileDlg)
	//}}AFX_DATA_INIT
}


void CRegfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegfileDlg)
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegfileDlg, CDialog)
	//{{AFX_MSG_MAP(CRegfileDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegfileDlg message handlers
