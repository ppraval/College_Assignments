// StatsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "StatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog


CStatsDlg::CStatsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsDlg)
	m_cyclesexec = 0;
	//}}AFX_DATA_INIT
}


void CStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDlg)
	DDX_Text(pDX, IDC_EDIT1, m_cyclesexec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg message handlers
