// SparcSimView.cpp : implementation of the CSparcSimView class
//

#include "stdafx.h"
#include "SparcSim.h"

#include "SparcSimDoc.h"
#include "SparcSimView.h"
#include "Assembler.h"
#include "Processor.h"

#include "RegfileDlg.h"
#include "pregsdlg.h"
#include "imemdlg.h"
#include "dmemDlg.h"
#include "emDlg.h"
#include "statsdlg.h"
#include "paramsdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSparcSimView

IMPLEMENT_DYNCREATE(CSparcSimView, CEditView)

BEGIN_MESSAGE_MAP(CSparcSimView, CEditView)
	//{{AFX_MSG_MAP(CSparcSimView)
	ON_COMMAND(ID_ASSEMBLE, OnAssemble)
	ON_COMMAND(ID_EXECUTE, OnExecute)
	ON_COMMAND(ID_EXCYCLE, OnExcycle)
	ON_COMMAND(ID_SHOWTOOLS, OnShowtools)
	ON_COMMAND(ID_EXNS, OnExns)
	ON_COMMAND(ID_EXMCYCLES, OnExmcycles)
	ON_COMMAND(ID_PARAMS, OnParams)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSparcSimView construction/destruction

CString itos(int i,int base);

CSparcSimView::CSparcSimView()
{
	// TODO: add construction code here
	ns=0;
	nc=5;


}

CSparcSimView::~CSparcSimView()
{
}

BOOL CSparcSimView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CSparcSimView drawing

void CSparcSimView::OnDraw(CDC* pDC)
{
	CSparcSimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSparcSimView printing

BOOL CSparcSimView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CSparcSimView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CSparcSimView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CSparcSimView diagnostics

#ifdef _DEBUG
void CSparcSimView::AssertValid() const
{
	CEditView::AssertValid();
}

void CSparcSimView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CSparcSimDoc* CSparcSimView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSparcSimDoc)));
	return (CSparcSimDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSparcSimView message handlers

void CSparcSimView::OnAssemble() 
{
	// TODO: Add your command handler code here



	ass.Init();
	ass.Assemble((CEdit*)this);

	CString data;

	GetWindowText(data);
	data+="\r\n\r\n";

	int i;
	char buffer[80];
	for(i=0;i<ass.no_of_instructions;i++)
	{
		itoa(ass.instructions[i],buffer,2);
		data+="\r\n"+PadZeros(CString(buffer));
	}


	SetWindowText(data);	

	//_________Assembled___________
}





//__________appends zeros to a string ( 32-l )______
CString CSparcSimView::PadZeros(CString arg)
{

	int l=arg.GetLength();
	CString letter="0";
	for(int i=0;i<(32-l);i++)
		arg=letter+arg;

	return arg;

}

void CSparcSimView::OnExecute()  //________Loads the program
{
	// TODO: Add your command handler code here
	

	//________Load the program
	processor.LoadProgram(ass);


	extern CRegfileDlg rdlg;
	extern CPregsDlg pdlg;
	extern CDmemDlg ddlg;
	extern CImemDlg idlg;
	extern CEmDlg edlg;

	rdlg.m_list1.ResetContent();
	pdlg.m_list1.ResetContent();
	ddlg.m_list1.ResetContent();
	edlg.m_list1.ResetContent();
	idlg.m_list1.ResetContent();

	//________Add instructions______
	Component* imem=processor.comps[processor.id_imemory];
	char buffer[80];

	// AfxMessageBox(imem->name);
	for(int i=0;i<ass.no_of_instructions;i++)
	{
		UINT ins=(UINT)((_Memory*)imem)->ReadWord(i*4);
		itoa(ins,buffer,2);
		idlg.m_list1.AddString(PadZeros(CString(buffer)));
	}

	UpdateTools();
	
}

void CSparcSimView::OnExcycle() 
{
	// TODO: Add your command handler code here
	processor.ExecuteCycles(1,0);
	UpdateTools();



	//___________Update all Windows________



}

CString itos(int i,int base)
{
	char buffer[35];
	itoa(i,buffer,base);

	CString s=buffer;
	return s;

}

void CSparcSimView::OnShowtools() 
{
	// TODO: Add your command handler code here
	extern CRegfileDlg rdlg;
	extern CPregsDlg pdlg;
	extern CDmemDlg ddlg;
	extern CImemDlg idlg;
	extern CEmDlg edlg;
	extern CStatsDlg sdlg;

	rdlg.ShowWindow(SW_SHOWNORMAL);
	pdlg.ShowWindow(SW_SHOWNORMAL);
	ddlg.ShowWindow(SW_SHOWNORMAL);
	idlg.ShowWindow(SW_SHOWNORMAL);
	edlg.ShowWindow(SW_SHOWNORMAL);
	sdlg.ShowWindow(SW_SHOWNORMAL);
	
}

void CSparcSimView::OnExns() 
{
	// TODO: Add your command handler code here
	processor.ExecuteCycles(1,1);
	UpdateTools();

	
}

void CSparcSimView::OnExmcycles() 
{
	// TODO: Add your command handler code here
	
	processor.ExecuteCycles(nc,0);
	UpdateTools();

}

void CSparcSimView::OnParams() 
{
	// TODO: Add your command handler code here
	CParamsDlg p;

	p.m_cycleff=nc;
	p.m_nscycles=processor.time_per_cycle;
	//UpdateData(FALSE);


	if(p.DoModal()==IDOK)
	{
		nc=p.m_cycleff;
		processor.time_per_cycle=p.m_nscycles;
	}

}

void CSparcSimView::UpdateTools()
{
	extern CRegfileDlg rdlg;
	extern CPregsDlg pdlg;
	extern CDmemDlg ddlg;
	extern CImemDlg idlg;
	extern CEmDlg edlg;
	extern CStatsDlg sdlg;
	
	char buffer[80];


	rdlg.m_list1.ResetContent();
	pdlg.m_list1.ResetContent();
	ddlg.m_list1.ResetContent();
	ddlg.m_edit1.SetSel (0, -1);
	ddlg.m_edit1.Clear ();
	edlg.m_list1.ResetContent();


	//___________Stats_________
	sdlg.m_cyclesexec=processor.time/processor.time_per_cycle;
	sdlg.UpdateData(FALSE);

	
	
	
	//___________Register File output______________
	Component* reg=processor.comps[processor.id_registerfile];

	// AfxMessageBox(imem->name);

	rdlg.m_list1.AddString("Registers :");
	rdlg.m_list1.AddString("  ");

	
	for(int i=0;i<32;i++)
	{
		UINT ins=(UINT)((_ScratchPad*)reg)->registers[i];
		itoa(ins,buffer,2);
		rdlg.m_list1.AddString(PadZeros(CString(buffer)));
	}


//____________PipeLine Registers__________

	
	//___________Display PC_________
	Component* PC=processor.comps[processor.id_pc];

	pdlg.m_list1.AddString("PC : "+PadZeros(itos(PC->outputs[0],2)));
	pdlg.m_list1.AddString("  ");

	
	//_____________Ist_________________-
	Component* preg=processor.comps[processor.id_pr[0]];

	// AfxMessageBox(imem->name);

	pdlg.m_list1.AddString("  ");
	pdlg.m_list1.AddString("PipeLine Register 1 :");
	pdlg.m_list1.AddString("  ");

	
	UINT ins=(UINT)((_PR*)preg)->outputs[0];
	pdlg.m_list1.AddString("PC + 4 : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[1];
	pdlg.m_list1.AddString("Instruction : "+PadZeros(itos(ins,2)));


	//___________________2nd______________
	preg=processor.comps[processor.id_pr[1]];

	// AfxMessageBox(imem->name);

	pdlg.m_list1.AddString("  ");
	pdlg.m_list1.AddString("PipeLine Register 2 :");
	pdlg.m_list1.AddString("  ");

	
	ins=(UINT)((_PR*)preg)->outputs[0];
	pdlg.m_list1.AddString("WBc : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[1];
	pdlg.m_list1.AddString("MEMc : "+PadZeros(itos(ins,2)));

	ins=(UINT)((_PR*)preg)->outputs[2];
	pdlg.m_list1.AddString("EXc : "+PadZeros(itos(ins,2)));

	
	ins=(UINT)((_PR*)preg)->outputs[3];
	pdlg.m_list1.AddString("PC + 4 : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[4];
	pdlg.m_list1.AddString("rs1,d : "+PadZeros(itos(ins,2)));

	ins=(UINT)((_PR*)preg)->outputs[5];
	pdlg.m_list1.AddString("rs2,d : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[6];
	pdlg.m_list1.AddString("CWP : "+PadZeros(itos(ins,2)));
	
	
	ins=(UINT)((_PR*)preg)->outputs[7];
	pdlg.m_list1.AddString("rs1,n : "+PadZeros(itos(ins,2)));

	ins=(UINT)((_PR*)preg)->outputs[8];
	pdlg.m_list1.AddString("rs2,n : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[9];
	pdlg.m_list1.AddString("rd,n : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[10];
	pdlg.m_list1.AddString("imm field : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[11];
	pdlg.m_list1.AddString("sethi imm : "+PadZeros(itos(ins,2)));

	//___________________3nd______________
	preg=processor.comps[processor.id_pr[2]];

	// AfxMessageBox(imem->name);

	pdlg.m_list1.AddString("  ");
	pdlg.m_list1.AddString("PipeLine Register 3 :");
	pdlg.m_list1.AddString("  ");

	
	ins=(UINT)((_PR*)preg)->outputs[0];
	pdlg.m_list1.AddString("WBc : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[1];
	pdlg.m_list1.AddString("MEMc : "+PadZeros(itos(ins,2)));

	ins=(UINT)((_PR*)preg)->outputs[2];
	pdlg.m_list1.AddString("ALU out : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[3];
	pdlg.m_list1.AddString("MemDataIn : "+PadZeros(itos(ins,2)));
	
	
	ins=(UINT)((_PR*)preg)->outputs[4];
	pdlg.m_list1.AddString("CWP : "+PadZeros(itos(ins,2)));

	ins=(UINT)((_PR*)preg)->outputs[5];
	pdlg.m_list1.AddString("rd,n : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[6];
	pdlg.m_list1.AddString("Sethi imm : "+PadZeros(itos(ins,2)));
	
	//___________________4th______________
	preg=processor.comps[processor.id_pr[3]];

	// AfxMessageBox(imem->name);

	pdlg.m_list1.AddString("  ");
	pdlg.m_list1.AddString("PipeLine Register 4 :");
	pdlg.m_list1.AddString("  ");

	
	ins=(UINT)((_PR*)preg)->outputs[0];
	pdlg.m_list1.AddString("WBc : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[1];
	pdlg.m_list1.AddString("Memdataout: "+PadZeros(itos(ins,2)));

	ins=(UINT)((_PR*)preg)->outputs[2];
	pdlg.m_list1.AddString("ALUsrc2 : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[3];
	pdlg.m_list1.AddString("CWP : "+PadZeros(itos(ins,2)));
	
	ins=(UINT)((_PR*)preg)->outputs[4];
	pdlg.m_list1.AddString("rd,n : "+PadZeros(itos(ins,2)));

	
	//___________Data Memory______________

	Component *dmem = processor.comps[processor.id_dmemory];
	
	//____________Word View_____________
	for(i=0;i<100;i++)
	{
		UINT ins=(UINT)((_Memory*)dmem)->ReadWord(i*4);
		ddlg.m_list1.AddString(PadZeros(itos(ins,2)));
	}

	//____________Char View_____________
	for(i=0;i<400;i++)
	{
		char c=((_Memory*)dmem)->data[i];
		CString ins = c;
		ddlg.m_edit1.ReplaceSel ((LPCTSTR)ins);
	}

}
