// ParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SparcSim.h"
#include "ParamsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamsDlg dialog


CParamsDlg::CParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParamsDlg)
	m_cycleff = 0;
	m_nscycles = 0;
	//}}AFX_DATA_INIT
}


void CParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamsDlg)
	DDX_Text(pDX, IDC_EDIT1, m_cycleff);
	DDV_MinMaxUInt(pDX, m_cycleff, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_nscycles);
	DDV_MinMaxUInt(pDX, m_nscycles, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamsDlg, CDialog)
	//{{AFX_MSG_MAP(CParamsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamsDlg message handlers

void CParamsDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
