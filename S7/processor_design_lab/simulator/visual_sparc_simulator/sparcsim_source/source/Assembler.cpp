// Assembler.cpp: implementation of the CAssembler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SparcSim.h"
#include "Assembler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAssembler::CAssembler()
{

}

CAssembler::~CAssembler()
{

}



CString CAssembler::ParseString(CString line,ParserResult& r)
{
	line.TrimLeft();
	
	//_________Assume blank line_____________
	r.parserinfo[0]=3;
	r.parserinfo[1]=0;

	if(line.GetLength()==0) return (CString)"";

	CString buffer="";

	int i=0;
	
	int length=line.GetLength();
	int parserpos=0;

	//________Some flags needed___
	int delimflag=0; //_______for the combination comma,space_________
	int spaceflag=0;
	int quotationflag=0;
	
	
	CString letter;

	//________if Directive____
	if( line[0]=='.')
		r.parserinfo[0]=0;
	else if(line[0]!='#')
		r.parserinfo[0]=2;

	r.parserinfo[1]=0;

	
	for(i=0;i<length;i++)
	{
		letter=line.Mid(i,1);
	
		if ((letter.SpanIncluding(" ,:#")).GetLength()==1 && quotationflag==0)
		{
			
			//____________label___________
			if(letter==":")
			{
				r.parserinfo[0]=1;
				r.parserinfo[1]=1;
				
				r.parser[0]=line.Left(i);
				r.parser[0].TrimRight();
				r.parser[0].TrimLeft();

				CString after_ins=line.Right(length-(i+1));
				after_ins.TrimRight();
				after_ins.TrimLeft();
	//			AfxMessageBox((CString)"Returning"+after_ins+"X");
				return after_ins;

				

			}
			else if(letter=="#")
			{
				if(buffer.GetLength()>0)
				{
					//AfxMessageBox("token : "+buffer);
					r.parser[parserpos++]=buffer;
					
					r.parserinfo[1]++;
				}
				break;
			}
			else if((letter==" " && spaceflag==0) || letter==",")
			{
			//	AfxMessageBox("token 1: "+buffer+"X"+letter+"X");
				r.parser[parserpos++]=buffer;
				
				r.parserinfo[1]++;
				buffer="";
				
				//________ignore further spaces____
				if(letter==" ")
				{
					while((i<length-1) && line[i]==' ') i++;
					i--;
					spaceflag=1;
				}

				
			}
		}
		else
		{
			buffer+=letter;
			if (letter=="\"") 
				quotationflag=1-quotationflag;
			
		//	delimflag=0;

			
		}

	}
	
	//____If string suddenly ends______	
	if(buffer.GetLength()>0)
	{
		//AfxMessageBox("token 2: "+buffer);
		r.parser[parserpos]=buffer;
		r.parserinfo[1]++;
	}	

	return (CString)"";
	//AfxMessageBox("Getting out of parse");
}


//_________Checks for all four parts
int CAssembler::Assemble(CEdit* source)
{
	int i=0;
	int instruction_count=0;
	int data_count=0;
	char buffer[255];
	CString line;


	//______Array Counters__
	int label_counter=0;
	int branch_counter=0;

	//___Flags___
	int text_section_flag=0;
	int label_ins_flag=0;

	ParserResult r;
	

	//AfxMessageBox("Starting Pass 1");
	//____________PASS 1______________
	//________Process Each Line ( labels and directives )______
	for(i=0;i<source->GetLineCount();i++)
	{
	
		
		if(!label_ins_flag)
		{
			source->GetLine(i,buffer,255);
			buffer[source->LineLength(source->LineIndex(i))]=0;
			line=buffer;
		}
		else
			label_ins_flag=0;

		
		//________Check for instructions after a label________
		CString after_label=ParseString(line,r);
		
		
		//______Instruction after label_____
		if(after_label.GetLength()>0) 
		{
			label_ins_flag=1;
			line=after_label; //_______Line has been parsed
			i--;
		}

		//AfxMessageBox("Parsed" + (CString)buffer);
		//_______It is a command____
		
		int cn;
		
		//_____if command and in text_Section_____
		if(r.parserinfo[0]==2 && text_section_flag)
		{
				instruction_count++;

		}
		//_________It is a directive_____
		//________Pay special attention to data counter
		//______And fill up the data memory_____
		if(r.parserinfo[0]==0)
		{

			if(r.parser[0].CompareNoCase(".data")==0) text_section_flag=0;
			if(r.parser[0].CompareNoCase(".org")==0) text_section_flag=1;
			if(r.parser[0].CompareNoCase(".end")==0) break;
			if(r.parser[0].CompareNoCase(".space")==0 && !text_section_flag) 
			{
				int space=atoi(LPCTSTR(r.parser[1]));
				data_count+=space;
			}
			
			if(( r.parser[0].CompareNoCase(".asciiz")==0 || r.parser[0].CompareNoCase(".ascii")==0) && !text_section_flag) 
			{
				CString string=r.parser[1];
				string.TrimRight();
				string.TrimLeft();
				int l=string.GetLength();
				for(i=1;i<(l-1);i++) //_____Neglect quotation marks_____
					data[data_count++]=string[i];
			
				if(r.parser[0].CompareNoCase(".asciiz")==0) 
					data[data_count++]=0; //______Null terminate
			}


			if(r.parser[0].CompareNoCase(".word")==0 && !text_section_flag) 
			{
				UINT word=atoi(LPCTSTR(r.parser[1]));

				//______Store the int in smallEndian Format_____
				data[data_count++]=(unsigned char)word >> 24;
				data[data_count++]=(unsigned char)(word <<8) >> 24;
				data[data_count++]=(unsigned char)(word <<16) >> 24;
				data[data_count++]=(unsigned char)(word <<24) >> 24;
			}

		}
		//__________it is a label_____
		if(r.parserinfo[0]==1)
		{
			//______Do special Processing____
			
			if(text_section_flag)
			{

				labels[label_counter]=r.parser[0];
				labels_addr[label_counter]=instruction_count*4;
				labels_type[label_counter]=1;
				label_counter++;
			}
			
			else 
			{

				labels[label_counter]=r.parser[0];
				labels_addr[label_counter]=data_count;
				labels_type[label_counter]=2;
				label_counter++;
			}
		}
	} //_______End of for loop


	no_of_labels=label_counter;
	no_of_instructions=instruction_count;
	data_size=data_count;

	//___Flags___
	text_section_flag=0;
	label_ins_flag=0;
	instruction_count=0;
	data_count=0;

	//AfxMessageBox("Starting Pass 2");		
	//________This converts to machine_____________
	//____________PASS 2______________
	//________Process Each Line______
	for(i=0;i<source->GetLineCount();i++)
	{
		if(!label_ins_flag)
		{
			source->GetLine(i,buffer,255);
			buffer[source->LineLength(source->LineIndex(i))]=0;
			line=buffer;
		}
		else
			label_ins_flag=0;

		
		//________Check for instructions after a label________
		CString after_label=ParseString(line,r);
		
		
		//______Instruction after label_____
		if(after_label.GetLength()>0) 
		{
			label_ins_flag=1;
			line=after_label; //_______Line has been parsed
			i--;
		}

		
		//_____if command and in text_Section_____
		if(r.parserinfo[0]==2 && text_section_flag)
		{
			 //__Check if valid instruction_____
			
				if(Ins_to_machine(line,instruction_count*4,instructions[instruction_count],ins_format[instruction_count])==-1)
				{
					AfxMessageBox("Error !!");
					return i+1; //_______Return error

				}
				instruction_count++;
			
			
		}
		//_________It is a directive_____
		if(r.parserinfo[0]==0)
		{

			if(r.parser[0].CompareNoCase(".data")==0) text_section_flag=0;
			if(r.parser[0].CompareNoCase(".org")==0) text_section_flag=1;
			if(r.parser[0].CompareNoCase(".end")==0) break;
			

		}
	} //_______End of for loop

	return 0;
}


int CAssembler::Ins_to_machine(CString instruction,int ins_addr,UINT& dest,int& f)
{

		int format;
		int op,op2,op3,cond;
		int noofregs=3;

		ParserResult r;
		ParseString(instruction,r);


		//______Now convert to assembly
		if(r.parser[0].CompareNoCase("ldsb")==0)
		{
			format=5;
			op3=9;
			op=3;

		}
		else if(r.parser[0].CompareNoCase("ldsh")==0)
		{
			format=5;
			op3=10;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("ld")==0)
		{
			format=5;
			op3=8;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("ldub")==0)
		{
			format=5;
			op3=1;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("lduh")==0)
		{
			format=5;
			op3=2;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("ldd")==0)
		{
			format=5;
			op3=3;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("stb")==0)
		{
			format=5;
			op3=5;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("sth")==0)
		{
			format=5;
			op3=6;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("st")==0)
		{
			format=5;
			op3=4;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("std")==0)
		{
			format=5;
			op3=7;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("swap")==0)
		{
			format=5;
			op3=15;
			op=3;
		}
		else if(r.parser[0].CompareNoCase("sethi")==0)
		{
			format=3;
			op2=4;
			op=0;
		}
		else if(r.parser[0].CompareNoCase("add")==0)
		{
			format=5;
			op3=0;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("and")==0)
		{
			format=5;
			op3=1;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("andn")==0)
		{
			format=5;
			op3=13;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("or")==0)
		{
			format=5;
			op3=2;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("orn")==0)
		{
			format=5;
			op3=6;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("udiv")==0)
		{
			format=5;
			op3=14;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("umul")==0)
		{
			format=5;
			op3=10;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("smul")==0)
		{
			format=5;
			op3=11;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("sdiv")==0)
		{
			format=5;
			op3=15;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("sub")==0)
		{
			format=5;
			op3=4;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("xor")==0)
		{
			format=5;
			op3=3;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("xnor")==0)
		{
			format=5;
			op3=7;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("sll")==0)
		{
			format=5;
			op3=5+32;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("srl")==0)
		{
			format=5;
			op3=6+32;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("sra")==0)
		{
			format=5;
			op3=7+32;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("addcc")==0)
		{
			format=5;
			op3=0+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("andcc")==0)
		{
			format=5;
			op3=1+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("andncc")==0)
		{
			format=5;
			op3=13+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("orcc")==0)
		{
			format=5;
			op3=2+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("orncc")==0)
		{
			format=5;
			op3=6+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("udivcc")==0)
		{
			format=5;
			op3=14+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("umulcc")==0)
		{
			format=5;
			op3=10+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("smulcc")==0)
		{
			format=5;
			op3=11+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("sdivcc")==0)
		{
			format=5;
			op3=15+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("subcc")==0)
		{
			format=5;
			op3=4+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("xorcc")==0)
		{
			format=5;
			op3=3+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("xnorcc")==0)
		{
			format=5;
			op3=7+16;
			op=2;
		}
		else if(r.parser[0].CompareNoCase("ba")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=8;
		}
		else if(r.parser[0].CompareNoCase("bne")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=9;
		}
		else if(r.parser[0].CompareNoCase("be")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=1;
		}
		else if(r.parser[0].CompareNoCase("ble")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=2;
		}
		else if(r.parser[0].CompareNoCase("bcc")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=13;
		}
		else if(r.parser[0].CompareNoCase("bcs")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=5;
		}
		else if(r.parser[0].CompareNoCase("bneg")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=6;
		}
		else if(r.parser[0].CompareNoCase("bvc")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=15;
		}
		else if(r.parser[0].CompareNoCase("bvs")==0)
		{
			format=2;
			op2=2;
			op=0;
			cond=7;
		}
		else if(r.parser[0].CompareNoCase("call")==0)
		{
			format=1;
			op=1;
		}
		else if(r.parser[0].CompareNoCase("jmpl")==0)
		{
			format=5;
			op3=0x38;
			op=2;
			noofregs=2;
			
		}
		else if(r.parser[0].CompareNoCase("save")==0)
		{
			format=5;
			op3=0x3c;
			op=2;
			
		}
		else if(r.parser[0].CompareNoCase("restore")==0)
		{
			format=5;
			op3=0x3d;
			op=2;
			
		}
		//_______Error , No instruction matches__________
		else 
			return -1; 
		





		//__________Now encode the instruction____

		f=format;

		//________dest = the final instruction
		dest=0;

		if(format==1) //____________Call___________
		{
			int addr=GetLabelAddr(r.parser[1]);
			SetBits(dest,op,30,31);
			SetBits(dest,addr/4,0,29);
		}
		else if(format==2) //______Branch_____
		{
			int addr=GetLabelAddr(r.parser[1]);
			
			SetBits(dest,op,30,31);
			SetBits(dest,(addr-(ins_addr+4))/4,0,21);

			SetBits(dest,cond,25,28);
			SetBits(dest,op2,22,24);
		}
		else if(format==3) //_________SetHi__________
		{
			int cons=atoi(LPCTSTR(r.parser[1]));
			int rd=GetRegNo(LPCTSTR(r.parser[2]));
			if(rd==-1) return -1;

			SetBits(dest,op,30,31);
			SetBits(dest,rd,25,29);
			SetBits(dest,op2,22,24);
			SetBits(dest,cons,0,21);
		}
		else if(format==5) //______load stores , arithmetic________
		{
			
			int cons;

			//_________Load and store______
			//________Only two arguments________
			
			int rs1=GetRegNo(LPCTSTR(r.parser[1]));
			int rs2,rd;
			
			if(noofregs==3)
			{
				rs2=GetRegNo(LPCTSTR(r.parser[2]));
				rd=GetRegNo(LPCTSTR(r.parser[3]));
			}
			else
			{
				rd=GetRegNo(LPCTSTR(r.parser[2]));

			}

			//___________If 2nd arg is register______
			if(rs2==-1 && noofregs==3)
			{
				cons=atoi(LPCTSTR(r.parser[2]));
			}

			if(rs1==-1 || rd==-1) return -1;


			SetBits(dest,op,30,31);
			SetBits(dest,rd,25,29);
			SetBits(dest,op3,19,24);
			SetBits(dest,rs1,14,18);
			
			if(noofregs==3)
			
			{
			if(rs2>=0)
			{
				SetBits(dest,0,13,13);
				SetBits(dest,rs2,0,4);

			}
			else
			{
				SetBits(dest,1,13,13);
				SetBits(dest,cons,0,12);
			}
			}
		}


		//______done______________
		return 0;
}


//___________Return register no. on seeing the register string
int CAssembler::GetRegNo(CString reg)
{

	reg.TrimLeft();
	reg.TrimRight();

	int l=reg.GetLength();

	//________Error ____
	if(reg.Left(2).CompareNoCase("%r")) return -1;

	CString no_s=reg.Right(l-2);
	int no=atoi(LPCTSTR(no_s));

	return no;
}



void CAssembler::SetBits(UINT& dest,UINT src,int start,int end)
{
	
	int l=(end-start)+1;
	src=(src << (32-l)) >> (32-l); //______Clear other bits of src
	src =src << start;
	dest=dest | src;
}

int CAssembler::GetLabelAddr(CString label)
{
	int i=0;
	for(i=0;i<(int)no_of_labels;i++)
		if(labels[i].CompareNoCase(label)==0)
			return labels_addr[i];

	return -1;

}

void CAssembler::Init()
{
	no_of_labels=0;
	no_of_instructions=0;
	data_size=0;

}
