// Processor.cpp: implementation of the Processor class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SparcSim.h"
#include "Processor.h"
#include "Assembler.h"

#include "RegfileDlg.h"
#include "pregsdlg.h"
#include "imemdlg.h"
#include "dmemDlg.h"
#include "emDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString itos(int i,int base);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Processor::Processor()
{

//____________Install the Components_____________

	one = new int;
	*one = 1;

	two = new int;
	*two = 2;

	four = new int;
	*four = 4;

	fifteen = new int;
	*fifteen = 15;

	zero = new int;
	*zero=0;

	int c = 0;

	_PC *PC;
	comps[c++] = PC = new _PC (0, c,"PC");
	id_pc=c-1;

	_Memory *Imemory;
	comps[c++] = Imemory = new _Memory (2, c,"Instruction Memory");
	id_imemory=c-1;


	_ALU *s1_ALU;
	comps[c++] = s1_ALU = new _ALU (0, c,"S1 ALU");

	_Shifter* s1_sh3;
	comps[c++] = s1_sh3 = new _Shifter (0, c, 2,"s1_sh3");

	_MUX *s1_callMUX;
	comps[c++] = s1_callMUX = new _MUX (0, c, 3,"s1_callMux");

	_MUX *s1_jmplMUX;
	comps[c++] = s1_jmplMUX = new _MUX (0, c, 5,"s1_jmplMUX");

	_MUX *s1_branchMUX;
	comps[c++] = s1_branchMUX = new _MUX (0, c, 4,"s1_branchMux");

	_PR *IFID;
	comps[c++] = IFID = new _PR (1, c, 2,"IFID");
	id_pr[0]=c-1;

	_BusSplitter *s2_bs1;
	comps[c++] = s2_bs1 = new _BusSplitter (0, c, 14, 18,"S2_BS1");

	_BusSplitter *s2_bs2;
	comps[c++] = s2_bs2 = new _BusSplitter (0, c, 0, 4,"S2_BS2");

	_BusSplitter *s2_bs3;
	comps[c++] = s2_bs3 = new _BusSplitter (0, c, 0, 21,"S2_BS3");

	_BusSplitter *s2_bs4;
	comps[c++] = s2_bs4 = new _BusSplitter (0, c, 25, 29,"S2_BS4");

	_BusSplitter *s2_bs5;
	comps[c++] = s2_bs5 = new _BusSplitter (0, c, 0, 12,"S2_BS5");

	_Shifter *s2_sh1;
	comps[c++] = s2_sh1 = new _Shifter (0, c, 2,"S2_SH1");

	_Shifter *s2_sh2;
	comps[c++] = s2_sh2 = new _Shifter (0, c, 10,"S2_SH2");

	_SignExt *s2_se1;
	comps[c++] = s2_se1 = new _SignExt (0, c, 21,"S2_SE1");

	_SignExt *s2_se2;
	comps[c++] = s2_se2 = new _SignExt (0, c, 12,"S2_SE2");

	
	_ALU *s2_ALU;
	comps[c++] = s2_ALU = new _ALU (0, c,"S2_ALU");

	_MUX *s2_stMUX;
	comps[c++] = s2_stMUX = new _MUX (0, c, 3,"S2_stMux");

	_MUX *s2_callMUX;
	comps[c++] = s2_callMUX = new _MUX (0, c, 3,"s2_callMux");

	_ScratchPad *ScratchPad;
	comps[c++] = ScratchPad = new _ScratchPad (2, c,"ScratchPad");
	id_registerfile=c-1;


	_HazardUnit *HazardUnit;
	comps[c++] = HazardUnit = new _HazardUnit (1, c,"Hazard Unit");
	id_hu=c-1;

	_BranchUnit *BranchUnit;
	comps[c++] = BranchUnit = new _BranchUnit (1, c,"Branch Unit");

	_ControlUnit *ControlUnit;
	comps[c++] = ControlUnit = new _ControlUnit (1, c,"ControlUnit");


	_MUX *s2_stallMUX;
	comps[c++] = s2_stallMUX = new _MUX (0, c, 3,"s2_stallMux");
	
	_BusSplitter *s2_bswb;
	comps[c++] = s2_bswb = new _BusSplitter (0, c, 22, 31,"s2_BSwb");

	_BusSplitter *s2_bsm;
	comps[c++] = s2_bsm = new _BusSplitter (0, c, 11, 21,"s2_BSm");

	_BusSplitter *s2_bsex;
	comps[c++] = s2_bsex = new _BusSplitter (0, c, 0, 10,"s2_BSex");



	_PR *IDEX;
	comps[c++] = IDEX = new _PR (1, c, 12,"IDEX");
	id_pr[1]=c-1;

	_BusSplitter *s3_bsmemread;
	comps[c++] = s3_bsmemread = new _BusSplitter (0, c, 0, 0,"s3_BSMemread");


	_BusSplitter *s3_bsaluop;
	comps[c++] = s3_bsaluop = new _BusSplitter (0, c, 0, 4,"s3_BSAluop");

	_BusSplitter *s3_bsalusrc;
	comps[c++] = s3_bsalusrc = new _BusSplitter (0, c, 6, 6,"s3_BSAlusrc");

	_BusSplitter *s3_bsjmpl;
	comps[c++] = s3_bsjmpl = new _BusSplitter (0, c, 5, 5,"s3_BSjmpl");

	_ForwardingUnit *ForwardingUnit;
	comps[c++] = ForwardingUnit = new _ForwardingUnit (0, c,"ForwardingUnit");

	_MUX *s3_AMUX;
	comps[c++] = s3_AMUX = new _MUX (0, c, 4,"s3_AMUX");

	_MUX *s3_BMUX;
	comps[c++] = s3_BMUX = new _MUX (0, c, 4,"s3_BMUX");

	_MUX *s3_aluMUX;
	comps[c++] = s3_aluMUX = new _MUX (0, c, 3,"s3_ALUMUX");

	_ALU *ALU;
	comps[c++] = ALU = new _ALU (2, c,"ALU");

	_MUX *s3_jmplMUX;
	comps[c++] = s3_jmplMUX = new _MUX (0, c, 3,"s3_jmplMUX");


	_PR *EXMEM;
	comps[c++] = EXMEM = new _PR (1, c, 7,"EXMEM");
	id_pr[2]=c-1;

	_BusSplitter *s4_bsmemop;
	comps[c++] = s4_bsmemop= new _BusSplitter (0, c, 0, 1,"s4_BSMemop");


	_BusSplitter *s4_bssethi;
	comps[c++] = s4_bssethi = new _BusSplitter (0, c, 2, 2,"s4_BSSethi");

	
	_MUX *s4_datainMUX;
	comps[c++] = s4_datainMUX = new _MUX (0, c, 3,"s4_datainMUX");

	_MUX *s4_sethiMUX;
	comps[c++] = s4_sethiMUX = new _MUX (0, c, 3,"s4_sethiMUX");

	_Memory *Dmemory;
	comps[c++] = Dmemory = new _Memory (2, c,"Dmemory");
	id_dmemory=c-1;

	
	_PR *MEMWB;
	comps[c++] = MEMWB = new _PR (1, c, 5,"MEMWB");
	id_pr[3]=c-1;

	
	_BusSplitter *s5_bsregwrite;
	comps[c++] = s5_bsregwrite = new _BusSplitter (0, c, 0, 0,"s5_BSRegwrite");

	_BusSplitter *s5_bsregsrc;
	comps[c++] = s5_bsregsrc = new _BusSplitter (0, c, 1, 1,"s5_BSRegsrc");

	_MUX *s5_srcMUX;
	comps[c++] = s5_srcMUX = new _MUX (0, c, 3,"s5_srcMUX");


//____________Wire 'er up !!!______________

	//_____________PC___________

	PC->minputs[0] = &(s1_branchMUX->outputs[0]);
	//PC->minputs[0] = &(s1_ALU->outputs[0]);


	//__________Instruction Memory_________

	Imemory->minputs[0] = one;
	Imemory->minputs[1] = &(PC->outputs[0]);
	Imemory->minputs[2] = zero;
	

	//___________s1_ALU__________

	s1_ALU->minputs[0] = two;
	s1_ALU->minputs[1] = four;
	s1_ALU->minputs[2] = &(PC->outputs[0]);

	//___________Instruction Register__________

	IFID->minputs[1] = &(Imemory->outputs[0]);
	IFID->minputs[0] = &(s1_ALU->outputs[0]);

	//___________Bus Splitters_____________

	s2_bs1->minputs[0] = &(IFID->outputs[1]);
	s2_bs2->minputs[0] = &(IFID->outputs[1]);
	s2_bs3->minputs[0] = &(IFID->outputs[1]);
	s2_bs4->minputs[0] = &(IFID->outputs[1]);
	s2_bs5->minputs[0] = &(IFID->outputs[1]);

	s2_bswb->minputs[0] = &(s2_stallMUX->outputs[0]);
	s2_bsm->minputs[0] = &(s2_stallMUX->outputs[0]);
	s2_bsex->minputs[0] = &(s2_stallMUX->outputs[0]);

	s4_bsmemop->minputs[0] = &(EXMEM->outputs[1]);
	
	s4_bssethi->minputs[0] = &(EXMEM->outputs[1]);

	s5_bsregwrite->minputs[0] = &(MEMWB->outputs[0]);
	s5_bsregsrc->minputs[0] = &(MEMWB->outputs[0]);



	//___________s2_signextenders_____________

	s2_se1->minputs[0] = &(s2_bs3->outputs[0]);
	s2_se2->minputs[0] = &(IFID->outputs[1]);

	//___________s2_shifters_____________
	
	s2_sh1->minputs[0] = &(s2_se1->outputs[0]);
	s2_sh2->minputs[0] = &(s2_se2->outputs[0]);

	//_____________s2_ALU______________

	s2_ALU->minputs[0] = two;
	s2_ALU->minputs[1] = &(IFID->outputs[0]);
	s2_ALU->minputs[2] = &(s2_sh1->outputs[0]);

	//____________st MUX_____________

	s2_stMUX->minputs[0] = &(ControlUnit->outputs[3]);
	s2_stMUX->minputs[1] = &(s2_bs2->outputs[0]);
	s2_stMUX->minputs[2] = &(s2_bs4->outputs[0]);

	//___________call MUX____________

	s2_callMUX->minputs[0] = &(ControlUnit->outputs[2]);
	s2_callMUX->minputs[2] = fifteen;
	s2_callMUX->minputs[1] = &(s2_bs4->outputs[0]);

	//______________REGISTER FILE___________

	ScratchPad->minputs[0] = &(s5_bsregwrite->outputs[0]);
	ScratchPad->minputs[1] = &(s2_bs1->outputs[0]);
	ScratchPad->minputs[2] = &(s2_stMUX->outputs[0]);
	ScratchPad->minputs[3] = &(MEMWB->outputs[4]);
	ScratchPad->minputs[4] = &(s5_srcMUX->outputs[0]);
	ScratchPad->minputs[5] = &(MEMWB->outputs[3]);

	//_____________Branch MUX_______________

	s1_branchMUX->minputs[0] = &(ControlUnit->outputs[1]);
	s1_branchMUX->minputs[1] = &(s1_ALU->outputs[0]);
	s1_branchMUX->minputs[2] = &(s2_ALU->outputs[0]);
	s1_branchMUX->minputs[3] = &(s1_jmplMUX->outputs[0]);

	//____________HAZARD UNIT_______________



	s3_bsmemread->minputs[0]=&(IDEX->outputs[1]);

	HazardUnit->minputs[0] = &(s3_bsmemread->outputs[0]);
	HazardUnit->minputs[1] = &(IDEX->outputs[9]);
	HazardUnit->minputs[2] = &(s2_bs1->outputs[0]);
	HazardUnit->minputs[3] = &(s2_bs2->outputs[0]);

	//____________Branch Units____________

	BranchUnit->minputs[0]=&(IFID->outputs[1]);
	BranchUnit->minputs[1]=&(ALU->outputs[1]);

	
	s2_stallMUX->minputs[0]=&(HazardUnit->outputs[2]);
	//s2_stallMUX->minputs[0]=zero;
	s2_stallMUX->minputs[1]=&(ControlUnit->outputs[0]);
	s2_stallMUX->minputs[2]=zero;

	IDEX->minputs[0]=&(s2_bswb->outputs[0]);
	IDEX->minputs[1]=&(s2_bsm->outputs[0]);
	IDEX->minputs[2]=&(s2_bsex->outputs[0]);
	IDEX->minputs[3]=&(IFID->outputs[0]);
	IDEX->minputs[4]=&(ScratchPad->outputs[0]);
	IDEX->minputs[5]=&(ScratchPad->outputs[1]);
	IDEX->minputs[6]=&(ScratchPad->outputs[2]);
	IDEX->minputs[7]=&(s2_bs1->outputs[0]);
	IDEX->minputs[8]=&(s2_bs2->outputs[0]);
	IDEX->minputs[9]=&(s2_callMUX->outputs[0]);
	IDEX->minputs[10]=&(s2_bs5->outputs[0]); //_________Sign Ex
	IDEX->minputs[11]=&(s2_sh2->outputs[0]);


	//___________Call Mux S1____________
	
	s1_sh3->minputs[0]=&(IFID->outputs[1]);


	s1_callMUX->minputs[0]=&(ControlUnit->outputs[2]);
	s1_callMUX->minputs[1]=&(s1_jmplMUX->outputs[0]);
	s1_callMUX->minputs[2]=&(s1_sh3->outputs[0]);


	//_____________s1__________jmpl Mux_________


	s1_jmplMUX->minputs[0]=&(ForwardingUnit->outputs[0]);
	s1_jmplMUX->minputs[1]=&(ScratchPad->outputs[0]);
	s1_jmplMUX->minputs[2]=&(s3_jmplMUX->outputs[0]);
	s1_jmplMUX->minputs[3]=&(EXMEM->outputs[2]);
	s1_jmplMUX->minputs[4]=&(Dmemory->outputs[0]);


	//_______________Forwarding Unit_____________

	ForwardingUnit->minputs[0]=&(IDEX->outputs[7]);
	ForwardingUnit->minputs[1]=&(IDEX->outputs[8]);
	ForwardingUnit->minputs[2]=&(s2_bs1->outputs[0]);
	ForwardingUnit->minputs[3]=&(MEMWB->outputs[0]);
	ForwardingUnit->minputs[4]=&(EXMEM->outputs[5]);
	ForwardingUnit->minputs[5]=&(EXMEM->outputs[1]);
	ForwardingUnit->minputs[6]=&(MEMWB->outputs[4]);
	ForwardingUnit->minputs[7]=&(EXMEM->outputs[0]);
	ForwardingUnit->minputs[8]=&(IDEX->outputs[0]);
	ForwardingUnit->minputs[9]=&(IDEX->outputs[9]);
	ForwardingUnit->minputs[10]=&(IDEX->outputs[1]);


	//______________s3______aMux____

	s3_AMUX->minputs[0]=&(ForwardingUnit->outputs[1]);
	s3_AMUX->minputs[1]=&(IDEX->outputs[4]);
	s3_AMUX->minputs[2]=&(EXMEM->outputs[2]);
	s3_AMUX->minputs[3]=&(s5_srcMUX->outputs[0]);

	//______________s3______bMux____

	s3_BMUX->minputs[0]=&(ForwardingUnit->outputs[2]);
	s3_BMUX->minputs[1]=&(IDEX->outputs[5]);
	s3_BMUX->minputs[2]=&(EXMEM->outputs[2]);
	s3_BMUX->minputs[3]=&(s5_srcMUX->outputs[0]);

	//__________s3 _______ALU MUX_____


	s3_bsaluop->minputs[0]=&(IDEX->outputs[2]);

	s3_bsjmpl->minputs[0]=&(IDEX->outputs[2]);

	
	s3_bsalusrc->minputs[0]=&(IDEX->outputs[2]);


	s3_aluMUX->minputs[0]=&(s3_bsalusrc->outputs[0]);
	s3_aluMUX->minputs[1]=&(s3_BMUX->outputs[0]);
	s3_aluMUX->minputs[2]=&(IDEX->outputs[10]);


	//_____________ALU_______

	ALU->minputs[0]=&(s3_bsaluop->outputs[0]);
	ALU->minputs[1]=&(s3_AMUX->outputs[0]);
	ALU->minputs[2]=&(s3_aluMUX->outputs[0]);


	//_______jmpl MUX________

	s3_jmplMUX->minputs[0]=&(s3_bsjmpl->outputs[0]);
	s3_jmplMUX->minputs[1]=&(ALU->outputs[0]);
	s3_jmplMUX->minputs[2]=&(IDEX->outputs[3]);
	

	//__________Control Unit__________

	ControlUnit->minputs[0]=&(IFID->outputs[1]);
	ControlUnit->minputs[1]=&(BranchUnit->outputs[0]);

	//________ex__mem_________

	EXMEM->minputs[0]=&(IDEX->outputs[0]);
	EXMEM->minputs[1]=&(IDEX->outputs[1]);
	EXMEM->minputs[2]=&(s3_jmplMUX->outputs[0]);
	EXMEM->minputs[3]=&(s3_BMUX->outputs[0]);
	EXMEM->minputs[4]=&(IDEX->outputs[6]);
	EXMEM->minputs[5]=&(IDEX->outputs[9]);
	EXMEM->minputs[6]=&(IDEX->outputs[11]);

	//____________________d4 datain mux______________

	s4_datainMUX->minputs[0]=&(ForwardingUnit->outputs[3]);
	s4_datainMUX->minputs[1]=&(EXMEM->outputs[3]);
	s4_datainMUX->minputs[2]=&(s5_srcMUX->outputs[0]);

	//_______DataMemory_________________


	Dmemory->minputs[0]=&(s4_bsmemop->outputs[0]);
	Dmemory->minputs[1]=&(EXMEM->outputs[2]);
	Dmemory->minputs[2]=&(s4_datainMUX->outputs[0]);
	

	s4_sethiMUX->minputs[0]=&(s4_bssethi->outputs[0]);
	s4_sethiMUX->minputs[1]=&(EXMEM->outputs[2]);
	s4_sethiMUX->minputs[2]=&(EXMEM->outputs[6]);


	//________MEMwb___________

	MEMWB->minputs[0]=&(EXMEM->outputs[0]);
	MEMWB->minputs[1]=&(Dmemory->outputs[0]);
	MEMWB->minputs[2]=&(s4_sethiMUX->outputs[0]);
	MEMWB->minputs[3]=&(EXMEM->outputs[4]);
	MEMWB->minputs[4]=&(EXMEM->outputs[5]);

	//_________s5_______SrcMUX_____________

	s5_srcMUX->minputs[0]=&(s5_bsregsrc->outputs[0]);
	s5_srcMUX->minputs[1]=&(MEMWB->outputs[2]);
	s5_srcMUX->minputs[2]=&(MEMWB->outputs[1]);


	
	
	//_________________Set enable signals___________
	for(int i=0;i<c;i++)
	{
			comps[i]->enabled=one;
	}

	PC->enabled=&(HazardUnit->outputs[0]);
	IFID->enabled=&(HazardUnit->outputs[1]);
	

	n_components = c;


	//____________init IDs of main comps_________


	//_________Setting Eventmanager and time pointers________

	for( i=0;i<c;i++)
	{
		comps[i]->eventmanager=&em;
		comps[i]->time=&time;
	}

	time=0; //________In nanoseconds_________
	time_per_cycle=5;
}


Processor::~Processor()
{
	for (int i = 0; i < n_components; i++)
		delete comps[i];
}


void Processor::LoadProgram(CAssembler& assembler)
{
	int i;

	//__________Initialize components_________
	for( i=0;i<n_components;i++)
	{
		for(int j=0;j<15;j++)
		{
			comps[i]->sinputs[j]=comps[i]->outputs[j]=comps[i]->toutputs[j]=0;
			comps[i]->Init();
		}

	}




		time=0;

	
	//_____________Filling Data Memory___________
	Component* dm=comps[id_dmemory];
	for( i=0;i<assembler.data_size;i++)
	{
		((_Memory *)dm)->data[i]=assembler.data[i];
	}

	
	//_________Write instructions_______
	Component* im=comps[id_imemory];
	char buffer[80];

	for(i=0;i<assembler.no_of_instructions;i++)
	{
		((_Memory *)im)->WriteWord(i*4,assembler.instructions[i]);
		
		/*UINT ins=(UINT)((_Memory*)im)->ReadWord(i*4);
		itoa(ins,buffer,2);
		AfxMessageBox(CString(buffer));*/
	
	}

	time=0;
	em.Init();


}

int Processor::ExecuteCycles(int n_cycles,int ns)
{
	extern CEmDlg edlg;
	for(int i=0;i<n_cycles;i++)
	{
		for(int j=0;j<time_per_cycle;j++)
		{
			if(ns) AfxMessageBox("Time starting ( in nanoseconds ) :" + itos(j+1,10));
			int flag;
			do //_______Keep on calling comp List______
			{
				flag=0;				
				//if(ns) AfxMessageBox("Stabilising loop");
				for(int k=0;k<n_components;k++)
				{
				//	AfxMessageBox(comps[k]->name);
					flag |= comps[k]->Compute(0);
				}
		
			}
			while(flag);

			time++;

			//_________________Add to eventlist___________
			edlg.m_list1.ResetContent();
			char buffer[80];
			for(int k=0;k<100;k++)
			{
			//	AfxMessageBox("adding item");
				if(em.eventlist[k][2])
				{
					UINT t=em.eventlist[k][0];
					
					itoa(t,buffer,10);

					CString data=comps[em.eventlist[k][1]]->name+" ," +CString(buffer);
					//AfxMessageBox(data);
					edlg.m_list1.AddString(data);
					

				}
			}


		}

		//______Call in propagate mode__________		

		for(int k=0;k<n_components;k++)
		{
			comps[k]->Compute(1);
		}

		//_________________Clock Has ended_____________
		//__________Check if event list has a valid event
		for( k=0;k<100;k++)
			{
				if(em.eventlist[k][2])
				{
					AfxMessageBox("WARNING! Clock period too low.\nEvents remaining in Event Manager!!!");		
					break;

				}
			}

	
	}


	return 0;

}

