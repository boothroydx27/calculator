/////////////////////////////////////////////////////                         ////////////////////////////////////////////////////
///////////////////////////////////////////////////// D E C L A R A T I O N S ////////////////////////////////////////////////////
/////////////////////////////////////////////////////                         ////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// CLASSES & FUNCTION MEMBERS
class Token {
public:
	char kind;
	double value;
	string name;
	Token(char ch);
	Token(char ch, double val);
	Token(char ch, string n);
};
class Token_stream {
public:
	Token_stream();
	Token GetToken();
	Token GetOtherToken(char);
	void PutbackIntoBuffer(Token t);
	
private:
	bool full{ false};
	Token buffer;
};
class Variable {
public:
	Variable(string n, double v);
	string name;
	double value;
};
class Error_Local {};            // a class used for error handling

// GLOBAL OBJECTS
static Token_stream ts;          // only one ts is instantiated  
static vector<Variable>var_table;// only one var_table is intsantiated 
int StatementType = 0;           // The variable that is used to control the input loop for letter characters. Object is modified by ::AutoSetStatementType() and ::ManualSetStatementType()
int ErrorCase = 0;               // The variable that is used to print the correct error message. Object will only ever be modified when inside statement(), CaseLet() or CaseSet()

// ERROR CODES
const string E1 =  "ERROR: BAD TOKEN!";
const string E2 =  "ERROR: INVALID FIRST TOKEN";
const string E3 =  "ERROR: EXPECTED 'Q', 'VAR;', 'LET' OR 'SET' IF STARTING WITH A LETTER CHARACTER";
const string E4 =  "ERROR: EXPECTED 'Q', 'VAR;', 'LET' OR 'SET' IF STARTING WITH A LETTER CHARACTER. (ALSO THIS NAME WOULD BE ILLEGAL)";
const string E5 =  "ERROR: INVALID EXPRESSION";
const string E6 =  "ERROR: EXPECTED ')'";
const string E7 =  "ERROR: EXPECTED PRIMARY OR A VARIABLE NAME";
const string E8 =  "ERROR: DECLARED NAMES MUST START WITH A LOWERCASE LETTER AND CONTAIN ONLY LOWERCASE LETTERS AND DIGITS";
const string E9 =  "ERROR: EXPECTED '=' AFTER VARIABLE NAME";
const string E10 = "ERROR: DIVIDE BY ZERO";
const string E11 = "ERROR: THE VARIABLE NAME YOU ARE TRYING TO USE IS UNDECLARED";
const string E12 = "ERROR: VARIABLE VALUE EXPECTED ')'";
const string E13 = "ERROR: VARIABLE VALUE BAD TOKEN!";
const string E14 = "ERROR: VARIABLE VALUE INVALID FIRST TOKEN!";
const string E15 = "ERROR: VARIABLE VALUE IS AN INVALID EXPRESSION";
const string E16 = "ERROR: VARIABLE VALUE EXPECTED PRIMARY OR A VARIABLE NAME";
const string E17 = "ERROR: VARIABLE VALUE CANNOT CONTAIN ILLEGAL NAMES";
const string E18 = "ERROR: THE NAME YOU ARE TRYING TO USE IN YOUR VARIABLE VALUE IS UNDECLARED";
const string E19 = "ERROR: DECLARED TWICE";
const string E20 = "ERROR: YOU CANNOT SET A NEW VALUE FOR AN UNDECLARED VARIABLE";

/* all the obove error codes are found in the following functions
			statement()              contains 4 unique codes
			calculation()            contains 1 unique code
			GetDeclarationName()     contains 1 unique code
			GetEqualsToken()         contains 1 unique code
			GetNameValue()           contains 3 unique codes
			GetPrintToken            contains 1 unique code
			primary()                contains 4 unique codes
			term()                   contains 1 unique code
			DefineNameAndValue()     contains 1 unique code
			ReDefineValue()          contains 1 unique code
			ReturnVariable           contains 2 unique codes
*/
// MAIN PROGRAM FUNCTIONS
void   statement();
void   calculation();
double primary();
double term();
double expression();
void   CaseLet();
void   CaseSet();
void   CaseVar();
string GetDeclarationName();
void   GetEqualsToken();
double GetNameValue();
void   GetPrintToken();
void   SetValue(string, double);
void   AlterValue(string, double);
double DefineNameAndValue(string, double);
void   ReDefineValue(string, double);
bool   IsDeclared(string);
double ReturnVariable(Token, int);
void   ManualSetStatementType(int val);
void   AutoSetStatementType(char);

// ERROR HANDLING FUNCION
void ThrowToLocal();
void Ignore();