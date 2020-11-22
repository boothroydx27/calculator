#include "C:\dev\c++\Calculator\src\DEF.h"
/*
Simple calculator

Revision history:
	Edited by boothroydx27 Nov 2020 to include more sophisticad error message checking
	Revised by Bjarne Stroustrup November 2013
	Revised by Bjarne Stroustrup May 2007
	Revised by Bjarne Stroustrup August 2006
	Revised by Bjarne Stroustrup August 2004
	Originally written by Bjarne Stroustrup (bs@cs.tamu.edu) Spring 2004.

	This program implements a basic expression calculator.
	Input from cin; output to cout.

The grammar for input is:

Statement:
	Calculate
	Define a variable
	Alter a variable
	Print the variable table
	Print result
	Quit

Print:
;
Quit:
Q

Expression:
	Term
	Expression + Term
	Expression – Term

Term:
	Primary
	Term * Primary
	Term / Primary
	Term % Primary

Primary:
	Number
	( Expression )
	– Primary
	+ Primary

Number:
	floating-point-literal

	Input comes from cin through the Token_stream called ts.
*/ // 

int main()
{
	try 
	{
		// predefine names
		DefineNameAndValue("pi", 3.1415926535);
		DefineNameAndValue("e", 2.7182818284);
		DefineNameAndValue("k", 1000);

	    ::statement();
		std::cout << ::ProgramTerminationMessage;
		return 0;
	}
	catch (runtime_error& e) {
		cerr << e.what() << '\n';
		std::cout << ::ProgramTerminationMessage;
		return 1;
	}
	catch (...) {
		cerr << "EXCEPTION \n";
		std::cout << ::ProgramTerminationMessage;
		return 2;
	}
}