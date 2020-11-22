#include "C:\dev\c++\Calculator\src\DEC.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////                             /////////////////////////////////////
///////////////////////////////////////////////////////////////   D E F I N I T I O N S     /////////////////////////////////////

// CONSTANTS 
const string prompt = "NEW STATEMENT> ";
const string result = "= ";  // used to indicate that what follows is a result
const string ProgramTerminationMessage = "TERMINATING PROGRAM...";

	// Tokens
const char BadToken = '!';    // t.kind == an illegal token.   avoiding a magic constant, actual value is irrelevant. any Token that is not legal will be constructed as a "BadToken" Token. 
const char BadName = '#';     // t.kind == an illegal name.    avoiding a magic constant, actual value is irrelevant. any Name that is not legal will be constructed as a "BadName" Token.
const char number = '8';      // t.kind == number token.       avoiding a magic constant, actual value is irrelevant. number literals will be constructed as a "number" Token
const char name = 'a';        // t.kind == name token.         avoiding a magic constant, actual value is irrelevant. any legal name will be constructed as a "name" Token
const char let = 'L';         // t.kind == declaration token.  
const char var = 'V';         // t.kind == var_table token.    
const char set = 'S';         // t.kind == set value token.    
const char quit = 'Q';        // t.kind == quit token.
const char print = ';';       // t.kind == print token.
const string declkey = "LET"; // t.kind == declaration keyword.
const string varkey = "VAR;"; // t.kind == see var table keyword.
const string setkey = "SET";  // t.kind == set variable a new value

// CLASS CONSTRUCTORS
Token::Token(char ch)
	:kind(ch), value(0) {}
Token::Token(char ch, double val)
	: kind(ch), value(val) {}
Token::Token(char ch, string n)
	: kind(ch), value(0), name(n) {}
Token_stream::Token_stream()
	: full(false), buffer(0) {}
Variable::Variable(string n, double v)
	: name(n), value(v) {}

// CLASS FUNCTION MEMBERS
	// class Token_stream
void Token_stream::PutbackIntoBuffer(Token t) {
	if (Token_stream::full) {
		std::cout << "ERROR: PUTBACK INTO A FULL BUFFER" << endl; // program has been checked over and this error message should never occur
		ThrowToLocal();
	}
	Token_stream::buffer = t;
	Token_stream::full = true;
}
Token Token_stream::GetToken() {
	
	if (Token_stream::full) {          // do we already have a Token ready?
		Token_stream::full = false;    // remove Token from buffer, then return it
		return Token_stream::buffer;
	}

	// read the first character from cin.
	char ch;
	std::cin >> ch; // note that >> skips whitespace (space, newline, tab, etc.)
	switch (ch) {
	case quit:
	case print:

		// let each character represent itself
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '=':
		return Token{ ch };

		// numeric literals. Floating-point literals can start with a dot.
	case '.':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{ std::cin.putback(ch); // put digit back into the input stream so all the digits can be read at once.
	double val;
	std::cin >> val;
	return Token{ number, val };
	}
	// if not an expression character or a number literal above then check for letters or bad input with Token_stream::GetOtherToken().
	default:
		Token t = GetOtherToken(ch);
		return t;
	}
}
Token Token_stream::GetOtherToken(char ch) {
	if (isalpha(ch) == false) return Token{ BadToken };      // names can only start with a letter character
	::AutoSetStatementType(ch);                              // check or amend ::StatementType variable. This variable controls what characters are accepted in the cin loop.
	string s = "";                                           // define a string to read into.
	                                                         // remember that cin.get() does not skip whitespace!
	switch (::StatementType) {   
	case 1: {  // search for a variable name to be used in the calculation. If conditions are not met return BadName Token
		cin.putback(ch);
		while (std::cin.get(ch)) {
			if (isupper(ch)) return Token{ BadName, s };     // upper case letters in names are illegal
			if ((isalpha(ch) || isdigit(ch))) s += ch;
			else if (ch == '\n')  { cin.putback(ch); break; }
			else if (ch == print) { cin.putback(ch); break; }
			else if (ch == ')')   { cin.putback(ch); break; }
			else if (ch == '+')   { cin.putback(ch); break; }
			else if (ch == '-')   { cin.putback(ch); break; }
			else if (ch == '*')   { cin.putback(ch); break; }
			else if (ch == '/')   { cin.putback(ch); break; }
			else if (ch == '%')   { cin.putback(ch); break; }
			else return Token{ BadName, s };
		}
		break;
	}
	case 2: { // declare a new variable name. Names can only contain lowercase letters and digits. If whitespace is found, that is the end of the variable name.
		cin.putback(ch);
		while (std::cin.get(ch)) {
			if (isupper(ch)) return Token{ BadName, s };
			if ((isalpha(ch) || isdigit(ch))) s += ch;
			else if (isspace(ch)) { std::cin.putback(ch); break; }
			else return Token{ BadName, s };
		}
		break;
	}
	case 3: case 4: { // search for 'LET' OR 'SET'. If conditions are not met, return a BadName Token
		cin.putback(ch);
		while (std::cin.get(ch)) {
			if ((isalpha(ch))) s += ch;
			else if (isspace(ch)) { std::cin.putback(ch); break; }
			else return Token{ BadName, s };
		}
		break;
	}
	case 5: { // search for 'VAR;'. If conditions are not met, return a BadName Token
		cin.putback(ch);
		while (std::cin.get(ch)) {
			if ((isalpha(ch))) s += ch;
			else if (ch == print) { s += ch; break; }
			else if (ch == '\n') { std::cin.putback(ch); break; }
			else return Token{ BadName, s };
		}
		break;
	}
	} 
	// compose an associated Token once finished reading valid input.
	if (s == varkey)  return Token{ var };    // see the var table keyword
	if (s == declkey) return Token{ let };    // declaration keyword
	if (s == setkey)  return Token{ set };    // set new variable value keyword
	return Token{ name, s };
}

// MAIN PROGRAM FUNCTIONS
void   statement() {
	while (true) {
		try {
			::ManualSetStatementType(1);        // Reset StatementType. Assume we are starting with a calculation cin loop
			::ErrorCase = 1;                    // Reset ErrorCase.     Assume we are starting with a calculation cin loop
			std::cout << prompt;
			Token t = ts.GetToken();            // compose the first Token from cin.
			switch (t.kind) {
			case quit: return;                  // allows terminating the program with 'Q'.
			case '(': case '-': case number: {  // primary Tokens - calls ::calculation()
				ts.PutbackIntoBuffer(t);        // put primary Token into Token_stream::buffer member to be used by ::calculation()
				::calculation();
				break;
			}
			case var: { ::CaseVar(); break; }   // allows printing the table of stored variables
			case let: { ::CaseLet(); break; }   // allows declaring a variable
			case set: { ::CaseSet(); break; }   // allows altering a variable name's value if already declared                 

			// ERROR CASES - all other Tokens returned at this stage will result in an error.

			case BadToken: {   // illegal Tokens
				std::cout << E1 << endl;
				ThrowToLocal();
			}
			case '=': case')': case '+': case '*': case '/': case '%': case print: {  // expression, equals and print Token(s)
				std::cout << E2 << endl;
				ThrowToLocal();
			}
			case name: {   // valid names. valid names can only be used once inside ::calculation()
				std::cout << E3 << endl;
				ThrowToLocal();
			}
			case BadName: {   // invalid names
				std::cout << E4 << endl;
				ThrowToLocal();
			}
			}
		}
		catch (Error_Local&) {
			::Ignore();       // discard all input after we hit a Token than cannot be used
		}
	}
}
void   calculation() {
	
	double calculation = expression();                  // call expression(). final result is returned into a variable if the next read Token stops the expression() loop.
	Token t = ts.GetToken();                            // check if that next Token is Print, otherwise report an error.
	if (t.kind == print) {
		std::cout << result << calculation << endl;     // print the result to the terminal.
		::Ignore();                                     // discard any Tokens after print. If no character Tokens are present, discard newline/whitespace.
		return;
	}
	else std::cout << E5 << endl;
	ThrowToLocal();
}
void   CaseLet() {
	
	::ManualSetStatementType(2);               // make sure we use case 2: cin loop to find a variable name
	ErrorCase = 3;                             // ErrorCase matches CaseLet() input loop number
	string name =  ::GetDeclarationName();
	::GetEqualsToken();
	::ManualSetStatementType(1);               // make sure we use case 1: cin loop to find an expression
	double value = GetNameValue(); 
	::GetPrintToken();
	::SetValue(name, value);
	std::cout << "NEW DECLARED VARIABLE ADDED TO VARIABLE TABLE" << endl;
	::Ignore();                               // discard any Tokens after Print. If no character Tokens are present, discard newline/whitespace.
	return;
}
void   CaseSet() {
	
	::ManualSetStatementType(2);                 // make sure we use case 2: cin loop to find a variable name
	ErrorCase = 4;                               // ErrorCase matches CaseSet() input loop number
	string name = ::GetDeclarationName();
	::GetEqualsToken();
	::ManualSetStatementType(1);                 // make sure we use case 1: cin loop to find an expression
	double value = GetNameValue();
	::GetPrintToken();
	::AlterValue(name, value);
	std::cout << "VALUE OF VARIABLE NAME HAS BEEN ALTERED" << endl;
	::Ignore();                                  // discard any Tokens after Print. If no character Tokens are present, discard newline/whitespace.
	return;
}
void   CaseVar() {
	
	std::cout << endl;
	for (int i = 0; i < ::var_table.size(); ++i)
		std::cout << var_table[i].name << ", " << var_table[i].value << endl;
	std::cout << endl;
	return;
}
string GetDeclarationName() {

	Token t = ts.GetToken();      // get declaration variable name Token
	if (t.kind != name) {
		std::cout << E8 << endl;
		ThrowToLocal();
	}
	string var_name = t.name;
	return t.name;
}
void   GetEqualsToken() {

	Token t = ts.GetToken();
	if (t.kind != '=') {
		std::cout << E9 << endl;
		ThrowToLocal();
	}
}
double GetNameValue() {
	// return an expression to read into the variable name
	Token t = ts.GetToken();
	switch (t.kind) {                      // search for a primary Token. Report an error if a primary is not returned.

	// primary Tokens - starts the expression() loop.
	case'(': case'-': case number: case name: {
		ts.PutbackIntoBuffer(t);           // A primary was found, put Token back and continue finding the rest of the expression.
		return expression();
	}
	
	// ERROR CASES - any Token returned that is not a primary will result in an error.
	case var: case let: case set: case BadName: {  // any case which contains an invalid variable name
		std::cout << E17 << endl;
		ThrowToLocal();
	}
	case')': case '+': case '*': case '/': case '%': case print: { // any case which contains invalid expressions, equals and print Token(s)
		std::cout << E14 << endl;
		ThrowToLocal();
	}
	case BadToken: case '=': {   // illegal Tokens
		std::cout << E13 << endl;
		ThrowToLocal();
	}
	}
}
void   GetPrintToken() {
	// check if that next Token is print, otherwise report an error.
	Token t = ts.GetToken();
	if (t.kind == print) {}
	else { std::cout << E15 << endl; ThrowToLocal(); }
	return;
}
void   SetValue(string name, double value) {
	       
	::DefineNameAndValue(name, value);      // define the new variable and store its value in ::var_table
	return;
}
void   AlterValue(string name, double value) {
	
	::ReDefineValue(name, value);      // change the existing variable value and store in ::var_table
	return;
	
}
double primary() { // deal with numbers, parentheses and valid variable names

	Token t = ts.GetToken();
	switch (t.kind) {
	case number: return t.value;     // return the number’s value
	case '-':    return -primary();
	case '(': {                      // handle '(' expression ')'
		double d = expression();
		t = ts.GetToken();
		if (t.kind != ')') {
			switch (::ErrorCase) {
			case 1: {std::cout << E6 << endl;  ThrowToLocal(); }
			case 3: case 4:
				std::cout << E12 << endl;
				ThrowToLocal();
			}
		}
		return d;
	}
	case name: {  // error checks are placed inside ReturnVariable()
		double x = ReturnVariable(t, ::ErrorCase);
		return x;
	} 
	case BadName: {
		std::cout << E17 << endl;
		ThrowToLocal();
	}
	default:
		switch (::ErrorCase) {
		case 1: {std::cout << E7 << endl;  ThrowToLocal(); }
		case 3: case 4: {
			std::cout << E16 << endl;
			ThrowToLocal();
		}
		}
	}
}
double term() {
	double left = primary();
	Token t = ts.GetToken();
	while (true) {
		switch (t.kind) {
		case '*': {
			left *= primary();
			t = ts.GetToken();
			break;
		}
		case '/': {
			double d = primary();
			if (d == 0) {
				std::cout << E10 << endl;
				ThrowToLocal();
			}
			left /= d;
			t = ts.GetToken();
			break;
		}
		case '%': {
			double d = primary();
			if (d == 0) {
				std::cout << E10 << endl;
				ThrowToLocal();
			}
			left = fmod(left, d);
			t = ts.GetToken();
			break;
		}
		default:
			ts.PutbackIntoBuffer(t);
			return left;
		}
	}
}
double expression() {
	double left = term();
	Token t = ts.GetToken();
	while (true) {
		switch (t.kind) {
		case '+':
			left += term();
			t = ts.GetToken();
			break;
		case '-':
			left -= term();
			t = ts.GetToken();
			break;
		default:
			ts.PutbackIntoBuffer(t);
			return left;

		}
	}
}
double ReturnVariable(Token t, int ErrorCase) {
	int i = 0;
	while (i < var_table.size()) {
		if (t.name == var_table[i].name)
			return var_table[i].value;

		else
			++i;
	}
	switch (ErrorCase) {
	case 1: {
		std::cout << E11 << endl;
		ThrowToLocal();
	}
	case 3: case 4: {
		std::cout << E18 << endl;
		ThrowToLocal();
	}
	}
}
void   ManualSetStatementType(int val) {
	::StatementType = val;
	return;
}
void   AutoSetStatementType(char ch) {
	// check StatementType's status and change if ch is == 'L', 'V' OR 'S'
	if (::StatementType == 1) {};            // if true, use case 1: input loop. StatemenType 1 is for a calculation() using variable names already declared
	if (::StatementType == 2) {};            // if true, use case 2: input loop. StatemenType 2 is to search for a variable name to be declared
	if (ch == 'L') ::StatementType = 3;      // if true, use case 3: input loop. StatemenType 3 is to search for the phrase 'LET'
	if (ch == 'S') ::StatementType = 4;      // if true, use case 4: input loop. StatemenType 4 is to search for the phrase 'SET'
	if (ch == 'V') ::StatementType = 5;      // if true, use case 5: input loop. StatemenType 5 is to search for the phrase 'VAR;'
	return;
}
bool   IsDeclared(string var)
// is var already in var_table?
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == var) return true;
	return false;
}
double DefineNameAndValue(string var, double val)
// add (var,val) to var_table
{
	if (IsDeclared(var)) {
		std::cout << E19 << " (" << var << ')' << endl;
		ThrowToLocal();
	}
	var_table.push_back(Variable(var, val));
	return val;
}
void   ReDefineValue(string s, double d)
// set the Variable named s to d
{
	for (int i = 0; i < var_table.size(); ++i)
		if (var_table[i].name == s) {
			var_table[i].value = d;
			return;
		}
	std::cout << E20 << endl;
	ThrowToLocal();
}

// ERROR HANDLING FUNCTIONS
void ThrowToLocal() {
	Error_Local x;
	throw x;
}
void Ignore() {
	string extra;
	std::getline(std::cin, extra);
	return;
}