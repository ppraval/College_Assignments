// Processor.h: interface for the Processor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSOR_H__25F74142_637C_11D0_A9B8_A8F91AC75D72__INCLUDED_)
#define AFX_PROCESSOR_H__25F74142_637C_11D0_A9B8_A8F91AC75D72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/*
enum compnos{S1_PC,S1_IM,S1_ALU1,S1_MUX1,PR_IFID,S2_ALU1,
	S2_MUX1,S2_SPAD,S2_MUX2,S2_MUX3,PR_IDEX,S2_HU,S2_BU,S2_AND,
	CU,S3_MUX1,S3_MUX2,S3_MUX3,S3_ALU,S3_MUX4,S3_FU,PR_EXMEM,
	S4_MUX1,S4_DM,S4_MUX2,PR_MEMWB,S5_MUX1};*/


#include "Assembler.h"

extern CString itos(int i,int base);
class EventManager
{

public :
	int eventlist[100][3];

	EventManager()
	{
		Init();
	}

	void Init()
	{
		for(int i=0;i<100;i++)
			eventlist[i][2]=0;


	}

	void AddEvent(int time,int ID)
	{
	
		//_______Check if event is already there________
		for(int i=0;i<100;i++)
		{
			if(eventlist[i][2]==1 && eventlist[i][0]==time &&
				eventlist[i][1]==ID)
			{
				return;
			}
		}

		//____Add event___
		for( i=0;i<100;i++)
		{
			if(eventlist[i][2]==0)
			{
				eventlist[i][0]=time;
				eventlist[i][1]=ID;
				eventlist[i][2]=1;
				return;
			}
		}
	}

	void DeleteEvent(int time,int ID)
	{
		for(int i=0;i<100;i++)
		{
			if(eventlist[i][2]==1 && eventlist[i][0]==time &&
				eventlist[i][1]==ID)
			{
				eventlist[i][2]=0; //___turn off valid bit___
				return;
			}
		}
	}

	int GetTime(int ID)
	{
		
	//_________Return minimum Time related with the ID____
		int min_time=-1;
		for(int i=0;i<100;i++)
		{
			if(eventlist[i][2]==1 && eventlist[i][1]==ID)
			{
				if(min_time==-1) 
					min_time=eventlist[i][0]; 
				else
					if(eventlist[i][0]<min_time)
						min_time=eventlist[i][0];
			}
		}

		return min_time;
	
	}
};


class Component
{

public:
	int *enabled;

	int* minputs[15];
	EventManager* eventmanager;
	UINT* time;
	
	int sinputs[15];
	int outputs[15];
	int toutputs[15];
	int delay;
	int stateelement;
	int ID;
	int n_inputs;
	CString name;

public:

	Component()
	{

		for(int i=0;i<15;i++)
		{
			sinputs[i]=outputs[i]=toutputs[i]=0;
		}
	}

	virtual int Compute(int mode)
	{

		int flag=0;
		//_____STEP 4___________
		//_________Clock has fallen__________
			if(mode==1)
			{
				if(stateelement && *enabled)
				{
					for(int i=0;i<15;i++)
						outputs[i]=toutputs[i];

				}
				return 0;
			}

		//_________STEP 1__________
		
//		AfxMessageBox("going to do it");


		if(delay==0 || eventmanager->GetTime(ID) ==(*time))
		{
			DoIt();
			if (delay>0) 
			{
				eventmanager->DeleteEvent(*time,ID);
				//flag=1;
			}

		}

//		AfxMessageBox("Done it");

//______________Check if input has changed and set the flag and transfer the inputs
	
		for(int i=0;i<n_inputs;i++)
		{
/*			if (name.CompareNoCase ("ScratchPad") == 0)
			{
				AfxMessageBox ("ScratchPad : "+itos(n_inputs,10)+" : " + " data : "+itos(*minputs[0],10)
					+" : " +itos(*minputs[3],10)+" : " +itos(*minputs[4],10));

			}*/

			if(*minputs[i]!=sinputs[i])
			{
				flag=1;
		
//			if (name.CompareNoCase ("ALU") == 0)
//					AfxMessageBox ("ALUChanging");

			
/*		
			char buffer[80];
				itoa(*minputs[i],buffer,10);
				CString data=(CString)"Masters : " + buffer;

				itoa(sinputs[i],buffer,10);
				data=data+(CString)" Slaves : " + buffer;

				AfxMessageBox(name+"  " +data);
		*/
				sinputs[i]=*minputs[i];
			}
		}
		
		//__________STEP 2____________

//		AfxMessageBox("dalay start");
		//_______For delayed comp add to event list_______
		if(delay>0)
			if(flag) eventmanager->AddEvent(*time+delay,ID);
	
//		AfxMessageBox("dalay end");
		

		
		return flag;
					
				
	}

	virtual void DoIt() { };
	virtual void Init() {};
	
	int GetBits(UINT src,int start,int end)
	{
		UINT t=src<<(32-(end+1));
		t=t>>(32-(end-start)-1);

		return t;
	}


	void SetBits(UINT& dest,UINT src,int start,int end)
	{
	
		int l=(end-start)+1;
		src=(src << (32-l)) >> (32-l); //______Clear other bits of src
		src =src << start;
		dest=dest | src;
	}
};

class Processor  
{
public:
	int ExecuteCycles(int n_cycles,int ns);
	
	Component* comps[50];
	int n_components;

	EventManager em;
	UINT time;
	int time_per_cycle;

	//___________Store imp IDs___________
	int id_imemory,id_dmemory,id_pr[4],id_registerfile,id_pc,id_hu;

//____________Constants_________
	int *one,*two,*four,*fifteen,*zero;
	



//________________Functions___________

	Processor(); //__________Wire up_____________
	virtual ~Processor();

	void LoadProgram(CAssembler& assembler);



};




class _PR : public Component
{
public:
	
	_PR(int d, int id, int n,CString string="noname")
	{
		stateelement=1;
		delay=d;
		ID=id;
		n_inputs=n;
		name=string;


	}

	virtual void DoIt()
	{
		for(int i=0;i<n_inputs;i++)
		{
			toutputs[i]=*minputs[i];
		}
	}
	
	
};




class _PC: public Component
{
public:
	
	_PC(int d, int id,CString string="noname")
	{
		stateelement=1;
		delay=d;
		ID=id;
		n_inputs=1;
		name=string;

	}

	virtual void DoIt()
	{
		for(int i=0;i<n_inputs;i++)
		{
			toutputs[i]=*minputs[i];
		}
	}	
};



class _Memory : public Component
{
public :
	unsigned char data[10000];


	
	
	_Memory (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 3;
		name=string;
	}

	virtual void Init()
	{
		for(int i=0;i<10000;i++)
			data[i]=0;
	}
	
	UINT ReadWord (int address)
	{
		UINT temp;
		UINT word=0;

		temp=(UINT)data[address++];
		word|=temp;
		word=word<< 8;

/*		char buffer[50];
				UINT ins;

				ins=(UINT)word;
				itoa(ins,buffer,2);
				AfxMessageBox("Reading word");
				AfxMessageBox((CString)buffer);
	*/

		temp=(UINT)data[address++];
		word|=temp;
		word=word<< 8;
		
		
		temp=(UINT)data[address++];
		word|=temp;
		word=word<< 8;

		
		temp=(UINT)data[address++];
		word|=temp;
	
		
		return word;
	}

	void WriteWord (int address,UINT word)
	{
				data[address++]=(unsigned char)(word >> 24);

/*				char buffer[80];
				itoa(word,buffer,10);
				CString msg = "Writing " + (CString)buffer;
				itoa (address, buffer, 10);
				msg += " at " + (CString)buffer;
				AfxMessageBox(msg);
*/
				data[address++]=(unsigned char)((word <<8) >> 24);

				
				
				data[address++]=(unsigned char)((word <<16) >> 24);
				
				data[address++]=(unsigned char)((word <<24) >> 24);


	}

	virtual void DoIt ()
	{
		if (*minputs[0] == 1)
			outputs[0] = ReadWord(*minputs[1]);

		if (*minputs[0] == 2)
			WriteWord(*minputs[1],UINT( *minputs[2]));
	}
};



class _MUX : public Component
{
public :

	_MUX (int d, int id, int n,CString string="noname")
	{
		ID = id;
		stateelement = 0;
		delay = 0;
		n_inputs = n;
		name=string;
	}

	virtual void DoIt ()
	{
		outputs[0] = *minputs[*minputs[0]+1];
//		if(name.CompareNoCase("s3_amux")==0)
//			AfxMessageBox("Inputs : " + itos(*minputs[0],10) + " , " + itos(*minputs[1],10) + " , "+ itos(*minputs[2],10) + " , "+ itos(*minputs[3],10) + " ,outputs : "+ itos(outputs[0],10) );
	}
};



class _Shifter : public Component
{
public :
	int shamt;

	_Shifter (int d, int id, int sh,CString string="noname")
	{
		ID = id;
		stateelement = 0;
		delay = 0;
		n_inputs = 1;
		shamt = sh;
		name=string;
	}

	virtual void DoIt ()
	{
		if (shamt > 0)
			outputs[0] = *minputs[0] << shamt; //____Arithmetic Shifters !!!___
		else
			outputs[0] = *minputs[0] >> (-shamt);
	}
};

class _SignExt : public Component
{
public :
	int ubit;
	//______________ubit is the upper most bit to sign extend_______

	_SignExt (int d, int id, int u,CString string="noname")
	{
		ID = id;
		stateelement = 0;
		delay = 0;
		n_inputs = 1;
		ubit = u;
		name=string;
	}

	virtual void DoIt ()
	{
		int in=*minputs[0];
		
		//________sign extend_________
		in=in << 31-ubit;
		in=in >> 31-ubit;

		outputs[0]=in;

		//AfxMessageBox("Shifting : "+itos(in,2));

	}
};


class _BusSplitter : public Component
{
public :
	int start, end;

	_BusSplitter (int d, int id, int s, int e,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 1;
		start = s;
		end = e;
		name=string;
	}

	virtual void DoIt ()
	{
		outputs[0] = GetBits (*minputs[0], start, end);
	}
};


class _ALU : public Component
{
public :

	_ALU (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 3;
		name=string;
	}
	

	virtual void DoIt ()
	{
		int temp = *minputs[0];
		int func = GetBits (temp, 0, 3);
		int cc = GetBits (temp, 4, 4);
		int op1 = *minputs[1];
		int op2 = *minputs[2];
		int out;
		int nzvc;

		switch (func)
		{
		case 0 :
			out = op1 & op2;
			break;
		case 1 :
			out = op1 | op2;
			break;
		case 2 :
			out = op1 + op2;
			break;
		case 3 :
			out = op1 - op2;
			break;
		case 4 :
			out = op1 * op2;
			break;
		case 5 :
			out = op1 / op2;
			break;
		case 6 :
			out = op1 << op2;
			break;
		case 7 :
			out = op1 >> op2;
			break;
		case 8 :
			out = ~(op1 & op2);
			break;
		case 9 :
			out = ~(op1 | op2);
			break;
		}

		if (cc)
		{
			nzvc = 0;
			if (out < 0)
				nzvc = 8;
			if (out == 0)
				nzvc = 4;
			outputs[1] = nzvc;
			AfxMessageBox ("setting nzvc : " + itos (nzvc, 2));
		}

		outputs[0] = out;
	}
};


class _ScratchPad : public Component
{
public :
	int registers[512];
	int CWP;

	
	_ScratchPad (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 6;
		CWP = 0;
		name=string;
	}

	virtual void Init()
	{

		for(int i=0;i<512;i++)
			registers[i]=0;
	}

	virtual void DoIt ()
	{
		if (*minputs[0])
			registers[*minputs[3]] = *minputs[4];

		outputs[0] = registers[*minputs[1]];
		outputs[1] = registers[*minputs[2]];
	}
};

class _BranchUnit : public Component
{
public :
	_BranchUnit (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 1;
		name=string;
	}

	virtual void DoIt ()
	{
		UINT cond = GetBits(*minputs[0], 25, 28);
		int branch = 0;
		int nzvc = *minputs[1];
		
		switch (cond)
		{
		case 8 : // ba
			branch = 1;
			break;
		case 1 : // be
			AfxMessageBox ("nzvc : " + itos (nzvc, 2));
			if (nzvc & 4)
				branch = 1;
			break;
		case 9 : // bne
			if (!(nzvc & 4))
				branch = 1;
			break;
		case 6 : // bneg
			if (nzvc & 8)
				branch = 1;
			break;
		}

		outputs[0] = branch;
	}
};


class _HazardUnit : public Component
{
public :

	_HazardUnit (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 4;
		name=string;
		Init();
	}

	virtual void Init()
	{
		outputs[0]=1;
		outputs[1]=1;
		outputs[2]=0;
	}

	virtual void DoIt ()
	{

		//_____________In no hazard , put on enable , disable flush
		if (!(*minputs[0])) //_____________id/ex memread
		{
			outputs[0] = 1;
			outputs[1] = 1;
			outputs[2] = 0;
			return;
		}

		if ((*minputs[1] == *minputs[2]) || (*minputs[1] == *minputs[3]))
		{
			outputs[0] = 0;
			outputs[1] = 0;
			outputs[2] = 1;
		}
		else
		{
			outputs[0] = 1;
			outputs[1] = 1;
			outputs[2] = 0;
		}
	}
};


class _ForwardingUnit : public Component
{
public :

	_ForwardingUnit (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 11;
		name=string;
	}

	virtual void DoIt ()
	{
		int idexrs1 = *minputs[0];
		int idexrs2 = *minputs[1];
		int ifidrs1 = *minputs[2];
		int memwbregwrite = GetBits (*minputs[3], 0, 0);
		int memwbregsrc = GetBits (*minputs[3], 1, 1);
		int exmemrd = *minputs[4];
		int exmemwrite = GetBits (*minputs[5], 1, 1);
		int exmemread = GetBits (*minputs[5], 0, 0);
		int memwbrd = *minputs[6];
		int exmemregwrite = GetBits (*minputs[7], 0, 0);
		int idexregwrite = GetBits (*minputs[8], 0, 0);
		int idexrd = *minputs[9];
		int idexmemwrite = GetBits (*minputs[10], 1, 1);

//_______________jmpl_________________

//__________Set all outputs to 0_______
	for(int i=0;i<15;i++)
		outputs[i]=0;



		if (idexregwrite && (idexrd == ifidrs1))
			outputs[0] = 1;

		else if (exmemregwrite && (exmemrd == ifidrs1))
		{
			if (exmemread)
				outputs[0] = 3;
			else
				outputs[0] = 2;
		}
		else 
			outputs[0]=0;
		

//______________ForwardA & B_______________

	
	//__________There is priority for MEM_________
	//_______________WB stage_________________

		if (memwbregwrite && memwbrd && (memwbrd == idexrs1))
		{
			outputs[1] = 2;
//			AfxMessageBox("Forwarding from WB to rs1");
		}
		
		if (memwbregwrite && memwbrd && (memwbrd == idexrs2))
		{
//			AfxMessageBox("Forwarding from WB to rs2");
			outputs[2] = 2;
		}
		
		//_______________st Hazard_____________
		if (idexmemwrite && memwbrd && (memwbrd == idexrd))
		{
			outputs[2] = 2;
		}
		

	//_______________MEM stage_________________

		if (exmemregwrite && exmemrd && (exmemrd == idexrs1))
		{
		//	AfxMessageBox("Forwarding from MEM to rs1");
			outputs[1] = 1;
		}
		

		
		if (exmemregwrite && exmemrd && (exmemrd == idexrs2))
		{
		//AfxMessageBox("Forwarding from MEM to rs2");
			outputs[2] = 1;
		}

		//_______________st Hazard_____________
		if (idexmemwrite && exmemrd && (exmemrd == idexrd))
		{
			outputs[2] = 1;
		}


//______________Data In___________________

		if (memwbregwrite &&
			memwbregsrc &&
			exmemwrite &&
			(memwbrd == exmemrd))
		{
			outputs[3] = 1;
		}

	}
};


class _ControlUnit : public Component
{
public :

	_ControlUnit (int d, int id,CString string="noname")
	{
		stateelement = 0;
		delay = d;
		ID = id;
		n_inputs = 2;
		name=string;
	}

	virtual void DoIt ()
	{
		UINT branch = *minputs[1];
		UINT instruction = *minputs[0];

		for (int i = 0; i < 15; i++)
			outputs[i] = 0;

		UINT controls = 0;
		UINT ex = 0;
		UINT m = 0;
		UINT wb = 0;

		UINT op = GetBits (instruction, 30, 31);

		UINT rd = GetBits (instruction, 25, 29);

		if (op == 1)            //________Call________
		{
			ex |= 32;
			wb |= 1;
			outputs[1] = 2;
			outputs[2] = 1;
		}

		if (op == 0)			//_____sethi or branch_____
		{
			UINT op2 = GetBits (instruction, 22, 24);
			
			if (op2 == 2 && branch)		//_______branch______
			{
				outputs[1] = 1;
		//		AfxMessageBox("Branching...");
			}

			if (op2 == 4)				//_______sethi_______
			{
				m |= 4;
				if (rd != 0)
					wb |= 1;
			}
		}

		if (op == 2)					//_______arith_______
		{
			UINT op3 = GetBits (instruction, 19, 24);
			UINT low = GetBits (op3, 0, 3);
			UINT high = GetBits (op3, 4, 5);

			if (high != 3)
			{
				switch (low)
				{
				case 1 :	// and
					ex |= 0;
					break;
				case 2 :	// or
					ex |= 1;
					break;
				case 0 :	// add
					ex |= 2;
					break;
				case 4 :	// sub
					ex |= 3;
					break;
				case 5 :	// sll
					if (high == 2)
						ex |= 6;
					break;
				case 6 :	// srl
					if (high == 2)
						ex |= 7;
					else
						ex |= 9; // orn
					break;
				case 13 :	// andn
					ex |= 8;
					break;
				case 11 :	// mul
					ex |= 4;
					break;
				case 15 :	// div
					ex |= 5;
					break;
				}

				
				if (high & 1)
					ex |= 16;		//_______set cc_______

				//_______Immediate addressing______________
				UINT imm=GetBits(instruction,13,13);
				if(imm)
					ex |= 64;
			

				
				if (rd != 0) //______Disable rd if 0____
					wb |= 1;
			}
			else
			{
				if (low == 8)		//______jmpl________
				{
					outputs[1] = 2;
					ex |= 32;
					if (rd != 0)
						wb |= 1;
				}
			}
		}  // endif (op == 2)

		//________________load/store______________

		if (op == 3)
		{
			UINT op3 = GetBits (instruction, 19, 24);

			//_________load__________
			if (op3 == 8)
			{
				ex |= 2;
				ex |= 64;
				m |= 1 ;
				if (rd != 0)
					wb |= 1;
				wb |= 2;
			}

			//__________store__________
			if (op3 == 4)
			{
				outputs[3] = 1;
				ex |= 2;
				ex |= 64;
				m |= 2;
			}
		}

		controls |= wb;
		controls = controls << 11;
		controls |= m;
		controls = controls << 11;
		controls |= ex;

		outputs[0] = controls;
	}
};



/*class s1_im : public Component
{
public:
	UINT output;
	UINT t_output;
	int rval;

	virtual int Compute(int mode,Component* comps)
	{
		
		extern CAssembler assembler;

		t_output=assembler.instructions[comps[S1_PC].output/4];
		if(mode==1) output=t_output;
		return 0;
	}

};

		

class s1_alu1 : public Component
{
public:
	UINT output;


	virtual int Compute(int mode,Component* comps)
	{
		
		rval=(int)(output==comps[S1_PC]);
		output=comps[S1_PC].output+4;
		return rval;
	}
};
*/



#endif // !defined(AFX_PROCESSOR_H__25F74142_637C_11D0_A9B8_A8F91AC75D72__INCLUDED_)
