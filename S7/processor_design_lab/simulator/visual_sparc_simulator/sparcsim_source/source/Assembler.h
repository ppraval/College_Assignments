// Assembler.h: interface for the CAssembler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSEMBLER_H__DA62B501_894F_11D3_B783_925659E2841D__INCLUDED_)
#define AFX_ASSEMBLER_H__DA62B501_894F_11D3_B783_925659E2841D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

struct ParserResult
{
	int parserinfo[2];
	//________parserinfo[1] ---  1 based ( no of arguments )_____
	CString parser[20];
};



class CAssembler  
{
public:
	UINT instructions[16000];
	int ins_format[16000];
	char data[32000];

	//_____Symbol Table_______
	CString labels[200];
	UINT labels_addr[200];
	char labels_type[200]; //___Determine data or instruction label________
	UINT no_of_labels;


	//____ ____General Info
	UINT no_of_instructions;
	UINT data_size;



private:
	int GetLabelAddr(CString label);
	void SetBits(UINT& dest,UINT src,int start,int end);
	//__________-1 Error ____0 _______Okay__________
	int Ins_to_machine(CString instruction,int ins_addr,UINT& dest,int& format);
	int GetRegNo(CString reg);
	CString ParseString(CString line,ParserResult &r);
	
public:	
	void Init();
	CAssembler();
	virtual ~CAssembler();

	int Assemble(CEdit* source);
	



};

#endif // !defined(AFX_ASSEMBLER_H__DA62B501_894F_11D3_B783_925659E2841D__INCLUDED_)
