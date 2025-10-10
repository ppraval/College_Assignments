#if !defined(AFX_PREGSDLG_H__392120F2_636A_11D0_A9CE_8911D76FD706__INCLUDED_)
#define AFX_PREGSDLG_H__392120F2_636A_11D0_A9CE_8911D76FD706__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PregsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPregsDlg dialog

class CPregsDlg : public CDialog
{
// Construction
public:
	CPregsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPregsDlg)
	enum { IDD = IDD_PREGS };
	CListBox	m_list1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPregsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPregsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREGSDLG_H__392120F2_636A_11D0_A9CE_8911D76FD706__INCLUDED_)
