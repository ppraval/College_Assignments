#if !defined(AFX_DMEMDLG_H__392120F0_636A_11D0_A9CE_8911D76FD706__INCLUDED_)
#define AFX_DMEMDLG_H__392120F0_636A_11D0_A9CE_8911D76FD706__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DmemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDmemDlg dialog

class CDmemDlg : public CDialog
{
// Construction
public:
	CDmemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDmemDlg)
	enum { IDD = IDD_DMEM };
	CEdit	m_edit1;
	CListBox	m_list1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDmemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDmemDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMEMDLG_H__392120F0_636A_11D0_A9CE_8911D76FD706__INCLUDED_)
