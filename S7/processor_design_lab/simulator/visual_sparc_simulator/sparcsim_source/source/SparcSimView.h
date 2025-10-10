// SparcSimView.h : interface of the CSparcSimView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPARCSIMVIEW_H__2B410BD8_88CE_11D3_B783_E3C9103A5A10__INCLUDED_)
#define AFX_SPARCSIMVIEW_H__2B410BD8_88CE_11D3_B783_E3C9103A5A10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Assembler.h"
#include "Processor.h"
#include "RegfileDlg.h"
#include "pregsdlg.h"
#include "imemdlg.h"
#include "dmemDlg.h"
#include "emDlg.h"


class CSparcSimView : public CEditView
{
protected: // create from serialization only
	CSparcSimView();
	DECLARE_DYNCREATE(CSparcSimView)

// Attributes
public:
	CSparcSimDoc* GetDocument();

// Operations
public:
	//_____________Assembler object________
	CAssembler ass;
	Processor processor;

	int ns;
	int nc;

	//________Globals________
	int n_cycles;

	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSparcSimView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateTools();
	CString PadZeros(CString arg);
	virtual ~CSparcSimView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSparcSimView)
	afx_msg void OnAssemble();
	afx_msg void OnExecute();
	afx_msg void OnExcycle();
	afx_msg void OnShowtools();
	afx_msg void OnExns();
	afx_msg void OnExmcycles();
	afx_msg void OnParams();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SparcSimView.cpp
inline CSparcSimDoc* CSparcSimView::GetDocument()
   { return (CSparcSimDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPARCSIMVIEW_H__2B410BD8_88CE_11D3_B783_E3C9103A5A10__INCLUDED_)
