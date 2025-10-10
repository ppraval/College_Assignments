#if !defined(AFX_IMEMDLG_H__392120F1_636A_11D0_A9CE_8911D76FD706__INCLUDED_)
#define AFX_IMEMDLG_H__392120F1_636A_11D0_A9CE_8911D76FD706__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ImemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImemDlg dialog

class CImemDlg : public CDialog
{
// Construction
public:
	CImemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImemDlg)
	enum { IDD = IDD_IMEM };
	CListBox	m_list1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImemDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMEMDLG_H__392120F1_636A_11D0_A9CE_8911D76FD706__INCLUDED_)
