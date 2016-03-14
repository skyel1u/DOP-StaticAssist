#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include "pin.H"

bool disabled = false;

fstream fout;

KNOB<string> KnobOutfile(KNOB_MODE_WRITEONCE, "pintool",
			 "o", "funcs.out",
			 "Output file");

static void FunctionHook(ADDRINT addr)
{
	std::string s = RTN_FindNameByAddress(addr);

	// assert(!(disabled && fout.is_open()));
	// assert(disabled || fout.is_open());
	if (s != ".plt")
		fout << s << std::endl;
}

static void InstrumentRtn(RTN rtn, VOID *)
{
	ADDRINT a = RTN_Address(rtn);
	IMG img = IMG_FindByAddress(a);

	if (IMG_Valid(img) &&
	    IMG_IsMainExecutable(img)) {
		RTN_Open(rtn);
		RTN_InsertCall(rtn, IPOINT_BEFORE, AFUNPTR(FunctionHook), IARG_ADDRINT, a, IARG_END);
		RTN_Close(rtn);
	}
}

void BeforeFork(THREADID threadid, const CONTEXT* ctxt, VOID * arg)
{
	fout.flush();
}

void FollowParent(THREADID threadid, const CONTEXT* ctxt, VOID * arg)
{
}

void FollowChild(THREADID threadid, const CONTEXT* ctxt, VOID * arg)
{
	char *new_name;
	int x;

	std::cerr << "New child " << PIN_GetPid() << std::endl;

	x = asprintf(&new_name, "%s_%d", KnobOutfile.Value().c_str(), PIN_GetPid());

	fout.close();
	fout.open(new_name, std::ios_base::out);

	free(new_name);

	(void)x;
	// fout.open(KnobOutfile.Value().c_str(), std::ios_base::out|std::ios_base::app);
	// assert(fout.is_open());
}


VOID Fini(INT32 code, VOID *v)
{
    fout.close();
}

int main(int argc, char * argv[])
{
	PIN_Init(argc, argv);
	PIN_InitSymbols();

	fout.open(KnobOutfile.Value().c_str(), std::ios_base::out);

	PIN_AddForkFunction(FPOINT_AFTER_IN_CHILD, FollowChild, 0);
	PIN_AddForkFunction(FPOINT_AFTER_IN_PARENT, FollowParent, 0);
	PIN_AddForkFunction(FPOINT_BEFORE, BeforeFork, 0);

	PIN_AddFiniFunction(Fini, 0);

	RTN_AddInstrumentFunction(InstrumentRtn, 0);
	PIN_StartProgram();

	return 0;
}
