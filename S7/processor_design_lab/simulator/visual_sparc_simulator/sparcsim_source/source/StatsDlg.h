#if !defined(AFX_STATSDLG_H__E661CB09_6370_11D0_A9CF_A9A485529D0E__INCLUDED_)
#define AFX_STATSDLG_H__E661CB09_6370_11D0_A9CF_A9A485529D0E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StatsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog

class CStatsDlg : public CDialog
{
// Construction
public:
	CStatsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatsDlg)
	enum { IDD = IDD_STATS };
	UINT	m_cyclesexec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATSDLG_H__E661CB09_6370_11D0_A9CF_A9A485529D0E__INCLUDED_)
