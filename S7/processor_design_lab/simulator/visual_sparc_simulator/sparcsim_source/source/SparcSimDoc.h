// SparcSimDoc.h : interface of the CSparcSimDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPARCSIMDOC_H__2B410BD6_88CE_11D3_B783_E3C9103A5A10__INCLUDED_)
#define AFX_SPARCSIMDOC_H__2B410BD6_88CE_11D3_B783_E3C9103A5A10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSparcSimDoc : public CDocument
{
protected: // create from serialization only
	CSparcSimDoc();
	DECLARE_DYNCREATE(CSparcSimDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSparcSimDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSparcSimDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSparcSimDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPARCSIMDOC_H__2B410BD6_88CE_11D3_B783_E3C9103A5A10__INCLUDED_)
