// SparcSimDoc.cpp : implementation of the CSparcSimDoc class
//

#include "stdafx.h"
#include "SparcSim.h"

#include "SparcSimDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSparcSimDoc

IMPLEMENT_DYNCREATE(CSparcSimDoc, CDocument)

BEGIN_MESSAGE_MAP(CSparcSimDoc, CDocument)
	//{{AFX_MSG_MAP(CSparcSimDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSparcSimDoc construction/destruction

CSparcSimDoc::CSparcSimDoc()
{
	// TODO: add one-time construction code here

}

CSparcSimDoc::~CSparcSimDoc()
{
}

BOOL CSparcSimDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSparcSimDoc serialization

void CSparcSimDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSparcSimDoc diagnostics

#ifdef _DEBUG
void CSparcSimDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSparcSimDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSparcSimDoc commands
