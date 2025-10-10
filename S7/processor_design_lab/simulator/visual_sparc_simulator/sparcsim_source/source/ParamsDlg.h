#if !defined(AFX_PARAMSDLG_H__E661CB07_6370_11D0_A9CF_A9A485529D0E__INCLUDED_)
#define AFX_PARAMSDLG_H__E661CB07_6370_11D0_A9CF_A9A485529D0E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ParamsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamsDlg dialog

class CParamsDlg : public CDialog
{
// Construction
public:
	CParamsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamsDlg)
	enum { IDD = IDD_PARAMS };
	UINT	m_cycleff;
	UINT	m_nscycles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamsDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSDLG_H__E661CB07_6370_11D0_A9CF_A9A485529D0E__INCLUDED_)
