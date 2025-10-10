// SparcSim.h : main header file for the SPARCSIM application
//

#if !defined(AFX_SPARCSIM_H__2B410BCE_88CE_11D3_B783_E3C9103A5A10__INCLUDED_)
#define AFX_SPARCSIM_H__2B410BCE_88CE_11D3_B783_E3C9103A5A10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSparcSimApp:
// See SparcSim.cpp for the implementation of this class
//

class CSparcSimApp : public CWinApp
{
public:
	CSparcSimApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSparcSimApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSparcSimApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPARCSIM_H__2B410BCE_88CE_11D3_B783_E3C9103A5A10__INCLUDED_)
