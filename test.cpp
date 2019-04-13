#include "decode.h"

using namespace std;

fetch_registers regs;
decode_registers alu_regs;
global_registers global_regs;

vector<event*> getEventList(Decode* dec, string& source, string& dest, void* magic) {
	
	message * mex = new message;
	strcpy(mex->source,(const char*)source.c_str());
	strcpy(mex->dest,(const char*)dest.c_str());
	mex->magic_struct = magic;
	mex->next = NULL;
	event  ev;
	ev.time = 0;
	ev.m = mex;
	return dec->notify(&ev);
}

int main(){
	string fetch_s(FETCH);
	string decode_s(DECODE);
	string ALU_s(ALU);
	vector<event*> event_list;	
	Decode* dec = new Decode("Decode",0);

	string mexDecode = "DECODE send message to ";
	string mexFD = "\t  FETCH send messages to DECODE -> ";
	string mexAD = "\t  ALU send messages to DECODE -> ";
	string mexMD = "\t  MEMORY send messages to DECODE -> ";

	int nameJump_length = 18;
	string nameJump[nameJump_length] = {	
									"JE","JNE","JA","JAE","JB","JBE","JG","JGE","JL",
									"JLE","JZ","JNZ","JC","JNC","JO","JNO","JS","JNS"
									};
	int nameF2_length = 4;
	string nameF2[nameF2_length] = {"INC", "DEC", "NEG", "NOT"};

	cout << "Legenda: INPUT -> OUTPUT" <<endl; 
	cout << "***** START *****" << endl;

	//F0
	cout << "Format 0" << endl;
	cout << "\tNOP" << endl;
	regs.opcode = 0x01;
	event_list = getEventList(dec,fetch_s, decode_s, NULL);
	cout << mexFD << mexDecode << event_list[0]->m->dest << endl;

	//F1
	cout << "Format 1" << endl;
	for(int i = 0; i<nameJump_length; i++){
		cout << "\t" << nameJump[i] << endl;
		regs.opcode = 0x20 + i;
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		cout << mexFD << mexDecode << event_list[0]->m->dest << endl;		
	}

	//F2
	cout << "Format 2" << endl;
	for(int i = 0; i<nameJump_length; i++){
		cout << "\t" << nameJump[i] << endl;
		regs.opcode = 0x40 + i;
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		cout << mexFD << mexDecode << event_list[0]->m->dest << endl;		
	}

	for(int i = 0; i<nameF2_length; i++){
		cout << "\t" << nameF2[i] << endl;
		regs.opcode = 0x52 + i;		
		// Send frome FETCH TO DECODE
		event_list = getEventList(dec,fetch_s, decode_s, NULL);
		cout << mexFD << mexDecode << event_list[0]->m->dest << endl;

		// DECODE must send message to ALU, then ALU send message to DECODE
		event_list = getEventList(dec,ALU_s, decode_s, NULL);
		cout << mexAD << mexDecode << event_list[0]->m->dest << endl;
	}

	return 0;
}