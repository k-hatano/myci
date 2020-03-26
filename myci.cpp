#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

/* tools */

int debugPrint = 0;

enum Kind
{
	NOT_KIND,
	NUMERIC,
	VARIABLE,
	OPERATE,
	WHITESPACE,
	LINE_END,
	NULLVALUE,
	COMMAMENT
};

enum Type
{
	NOT_TYPE,
	VOID_TYPE,
	INT_TYPE
};

enum Operate
{
	NOT_OPERATE,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	MODULO,
	EQUAL,
	NOT_EQUAL,
	GREATER_THAN,
	LESS_THAN,
	EQUAL_OR_GREATER_THAN,
	EQUAL_OR_LESS_THAN,
	AND,
	OR,
	VARIABLE_LET,
	PARENTHESIS_START,
	PARENTHESIS_END,
	BRACE_START,
	BRACE_END,
	BRACKET_START,
	BRACKET_END,
	COMMA,
	RETURN,
	PRINT,
	SEMICOLON,
	INT,
	VOID,
	FUNCTION_LET,
	FUNCTION_START,
	FUNCTION_END,
	IF,
	WHILE,
	COMMENT_HASH
};

string getCharOfOperate(int op)
{
	switch (op)
	{
	case COMMENT_HASH:
		return "#";
	case ADD:
		return "+";
	case SUBTRACT:
		return "-";
	case MULTIPLY:
		return "*";
	case DIVIDE:
		return "/";
	case MODULO:
		return "%";
	case EQUAL:
		return "==";
	case NOT_EQUAL:
		return "!=";
	case GREATER_THAN:
		return ">";
	case LESS_THAN:
		return "<";
	case EQUAL_OR_GREATER_THAN:
		return ">=";
	case EQUAL_OR_LESS_THAN:
		return "<=";
	case AND:
		return "&&";
	case OR:
		return "||";
	case VARIABLE_LET:
		return "=";
	case INT:
		return "int";
	case VOID:
		return "void";
	case RETURN:
		return "return";
	case IF:
		return "if";
	case WHILE:
		return "while";
	case COMMA:
		return ",";
	case PARENTHESIS_START:
		return "(";
	case PARENTHESIS_END:
		return ")";
	case BRACE_START:
		return "{";
	case BRACE_END:
		return "}";
	case BRACKET_START:
		return "[";
	case BRACKET_END:
		return "]";
	case SEMICOLON:
		return ";";
	case PRINT:
		return "print";
	case FUNCTION_LET:
		return "FUNCTION_LET";
	case FUNCTION_START:
		return "FUNCTION_START";
	case FUNCTION_END:
		return "FUNCTION_END";

	default:
		return "??";
	}
}

typedef class Opcode
{
public:
	Kind kind;
	int value;
	Opcode()
	{
		kind = NOT_KIND;
		value = 0;
	}
	Opcode(Kind k, int v)
	{
		kind = k;
		value = v;
	}
	Kind getKind()
	{
		return kind;
	}
	void setKind(Kind k)
	{
		kind = k;
	}
	int getValue()
	{
		return value;
	}
	void setValue(int v)
	{
		value = v;
	}
	void set(Kind k, int v)
	{
		kind = k;
		value = v;
	}
} Opcode;

typedef class Program
{
public:
	vector<string> names;
	vector<string> tokens;
	vector<Opcode> program;
	vector<Program> functions;
	vector<int> variables;
	vector<int> params;
	Program *parent;
	int head;

	void init()
	{
		program.clear();
		tokens.clear();

		head = 0;
	}

	vector<string> getNames()
	{
		return names;
	}
	vector<string> *getNamesPtr()
	{
		return &names;
	}
	vector<string> appendName(string name)
	{
		names.push_back(name);
		return names;
	}
	vector<string> getTokens()
	{
		return tokens;
	}
	vector<string> *getTokensPtr()
	{
		return &tokens;
	}
	vector<string> appendToken(string token)
	{
		tokens.push_back(token);
		return tokens;
	}
	void setTokens(vector<string> _tokens)
	{
		tokens = _tokens;
	}
	int findToken(string token)
	{
		int i = 0;
		vector<string>::iterator it = tokens.begin();
		while (it != tokens.end())
		{
			if ((*it).compare(token) == 0) {
				return i;
			}
			it++;
			i++;
		}
		return 0;
	}
	vector<Opcode> getProgram()
	{
		return program;
	}
	vector<Opcode> *getProgramPtr()
	{
		return &program;
	}
	vector<Opcode> appendOpcode(Opcode opcode)
	{
		program.push_back(opcode);
		return program;
	}
	void setProgram(vector<Opcode> p)
	{
		program = p;
	}
	vector<int> getVariables()
	{
		return variables;
	}
	void setValueOfVariable(int var, int value)
	{
		while (variables.size() <= var) {
			variables.push_back(0);
		}
		variables[var] = value;
	}

	int getIndexOfName(string name, int h)
	{
		vector<string>::iterator it = names.begin();
		int i;
		for (i = names.size() - 1; i >= 0; i--)
		{
			if (name == names.at(i)) {
				return i;
			}
		}
		throw(char *) "Error: Undefined Variable found";
	}

	void setNames(vector<string> n)
	{
		names = n;
	}

	void removeNamesAfter(int hd)
	{
		while (names.size() > hd)
		{
			names.pop_back();
			variables.pop_back();
		}
	}

	vector<class Program> getFuncs()
	{
		return functions;
	}

	int appendFunc(Program newfunc)
	{
		functions.push_back(newfunc);
		return functions.size();
	}

	int appendVariableName(string name)
	{
		names.push_back(name);
		variables.push_back(0);
		return names.size() - 1;
	}

	void setHead(int h)
	{
		head = h;
	}

	int getHead()
	{
		return head;
	}

} Program;

Kind getKindOfChar(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
		c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || 
		c == '{' || c == '}' || c == '[' || c == ']' || c == '!' || 
		c == '&' || c == '|' || c == ',')
	{
		return OPERATE;
	}
	else if (c >= '0' && c <= '9')
	{
		return NUMERIC;
	}
	else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return VARIABLE;
	}
	else if (c <= 0)
	{
		return NULLVALUE;
	}
	else if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
	{
		return WHITESPACE;
	}
	else if (c == ';')
	{
		return LINE_END;
	}
	else if (c == '#')
	{
		return COMMAMENT;
	}
	return NOT_KIND;
}

Operate getCodeOfOperate(string ope)
{
	if (ope == "+") {
		return ADD;
	}
	else if (ope == "-") {
		return SUBTRACT;
	}
	else if (ope == "*") {
		return MULTIPLY;
	}
	else if (ope == "/") {
		return DIVIDE;
	}
	else if (ope == "%") {
		return MODULO;
	}
	else if (ope == "==") {
		return EQUAL;
	}
	else if (ope == "!=") {
		return NOT_EQUAL;
	}
	else if (ope == ">") {
		return GREATER_THAN;
	}
	else if (ope == "<") {
		return LESS_THAN;
	}
	else if (ope == ">=") {
		return EQUAL_OR_GREATER_THAN;
	}
	else if (ope == "<=") {
		return EQUAL_OR_LESS_THAN;
	}
	else if (ope == "&&") {
		return AND;
	}
	else if (ope == "||") {
		return OR;
	}
	else if (ope == "=") {
		return VARIABLE_LET;
	}
	else if (ope == "print") {
		return PRINT;
	}
	else if (ope == "return") {
		return RETURN;
	}
	else if (ope == "int") {
		return INT;
	}
	else if (ope == "void") {
		return VOID;
	}
	else if (ope == "if") {
		return IF;
	}
	else if (ope == "while") {
		return WHILE;
	}
	else if (ope == ";") {
		return SEMICOLON;
	}
	else if (ope == ",") {
		return COMMA;
	}
	else if (ope == "(") {
		return PARENTHESIS_START;
	}
	else if (ope == ")") {
		return PARENTHESIS_END;
	}
	else if (ope == "{") {
		return BRACE_START;
	}
	else if (ope == "}") {
		return BRACE_END;
	}
	else if (ope == "[") {
		return BRACKET_START;
	}
	else if (ope == "]") {
		return BRACKET_END;
	}
	else if (ope == "#") {
		return COMMENT_HASH;
	}
	return NOT_OPERATE;
}

Type getTypeOfOperate(Operate ope)
{
	if (ope == INT) {
		return INT_TYPE;
	}
	else if (ope == VOID) {
		return VOID_TYPE;
	}
	return NOT_TYPE;
}

int onlyWhitespace(string str)
{
	int i;
	for (i = 0; str[i] > 0; i++)
	{
		if (getKindOfChar(str[i]) != WHITESPACE)
			return 0;
	}
	return 1;
}

int validTwoByteOperation(char a, char b)
{
	if (a == '=' && b == '=') {
		return 1;
	}
	if (a == '!' && b == '=') {
		return 1;
	}
	if (a == '&' && b == '&') {
		return 1;
	}
	if (a == '|' && b == '|') {
		return 1;
	}
	if (a == '<' && b == '=') {
		return 1;
	}
	if (a == '>' && b == '=') {
		return 1;
	}
	return 0;
}

int getOperatePriority(Operate op)
{
	switch (op)
	{
	case MULTIPLY:
	case DIVIDE:
	case MODULO:
		return 2;
	case ADD:
	case SUBTRACT:
		return 3;
	case GREATER_THAN:
	case LESS_THAN:
		return 4;
	case EQUAL:
	case NOT_EQUAL:
		return 5;
	case AND:
		return 6;
	case OR:
		return 7;
	case VARIABLE_LET:
		return 8;

	case COMMA:
		return 12;
	case IF:
	case WHILE:
		return 13;
	case INT:
	case VOID:
		return 14;
	case SEMICOLON:
		return 15;
	case FUNCTION_START:
	case FUNCTION_END:
	case FUNCTION_LET:
		return 16;
	case PRINT:
		return 32;
	case RETURN:
		return 33;
	case PARENTHESIS_START:
	case PARENTHESIS_END:
	case BRACE_START:
	case BRACE_END:
	case BRACKET_START:
	case BRACKET_END:
		return 256;
	default:
		return -1;
	}
}

int executeOpcode(Program &prog, Opcode opcode)
{
	Kind kind = opcode.getKind();
	if (kind == NUMERIC) {
		return opcode.getValue();
	}
	else if (kind == VARIABLE)
	{
		return (prog.getVariables())[opcode.getValue()];
	}
	throw(char *) "Error: Execution failed";
}

/* program */

void initProgram(Program &prog)
{
	prog.init();
}

void prepareMainFunc(Program &prog)
{
	Program blank;

	prog.appendVariableName("main");
	prog.appendFunc(blank);
	prog.setValueOfVariable(prog.getIndexOfName("main", 0), prog.functions.size() - 1);
}

int readTokens(Program &prog, FILE *fp)
{
	if (debugPrint) {
		cout << "READ TOKENS" << endl;
	}
	int c = 0, cl = 0, cache = 0, depth = 0;
	Kind last = NOT_KIND, current = NOT_KIND;
	string s = "";

	if (fp == NULL) {
		printf(">");
	}
	while (1)
	{
		if (cache == 0)
		{
			cl = c;
			c = fgetc(fp == NULL ? stdin : fp);
		}
		else
		{
			c = cache;
			cache = 0;
		}
		current = getKindOfChar(c);
		if (c <= 0)
		{
			throw 0;
		}
		else if (current == COMMAMENT)
		{
			while (1)
			{
				c = fgetc(fp == NULL ? stdin : fp);
				if (c == '\n' || c == '\r' || c == 0) {
					break;
				}
			}
			break;
		}
		else if (current == LINE_END)
		{
			if (depth <= 0) {
				break;
			}
			if (s.length() > 0)
			{
				prog.getTokensPtr()->push_back(s);
				s = "";
			}
			s.append(1, c);
			last = current;
		}
		else if (current == NOT_KIND)
		{
			throw(char *) "Error: Unknown character found";
		}
		else if (current == last || last == NOT_KIND)
		{
			if (c == '{') {
				depth++;
			}
			if (c == '}') {
				depth--;
			}
			if (current == OPERATE && s.length() > 0 && !validTwoByteOperation(cl, c))
			{
				prog.getTokensPtr()->push_back(s);
				s = "";
			}
			s.append(1, c);
			last = current;
			if (c == '}' && depth <= 0) {
				break;
			}
		}
		else
		{
			if (!onlyWhitespace(s)) {
				prog.getTokensPtr()->push_back(s);
			}
			s = "";
			cache = c;
			last = NOT_KIND;
		}
	}
	if (s.length() > 0 && !onlyWhitespace(s)) {
		prog.getTokensPtr()->push_back(s);
	}
	return 0;
}

void printTokens(Program &prog)
{
	vector<string> tokens = prog.getTokens();
	vector<string>::iterator it = tokens.begin();
	while (it != tokens.end())
	{
		cout << (*it) << endl;
		it++;
	}
}

vector<Opcode> parseFormula(vector<string> tokens, Program *prog)
{
	if (debugPrint) {
		cout << "PARSE FORMULA" << endl;
	}

	vector<int> heads;
	vector<Opcode> result;
	vector<Operate> opstk;
	Operate operate;
	Opcode op;

	Type type = NOT_TYPE;

	heads.push_back(0);

	vector<string>::iterator it = tokens.begin();
	while (it != tokens.end())
	{
		char c = (*it)[0];
		int k = getKindOfChar(c);
		switch (k)
		{
		case NUMERIC:
			op.set(NUMERIC, atoi((*it).c_str()));
			result.push_back(op);
			break;
		case OPERATE:
		case LINE_END:
			operate = getCodeOfOperate(*it);
			if (operate == PARENTHESIS_START)
			{
				if (result.size() > 0 && result.back().getKind() == VARIABLE)
				{
					op.set(OPERATE, FUNCTION_START);
					result.push_back(op);
					opstk.push_back(FUNCTION_END);
				}
				opstk.push_back(operate);
				op.set(OPERATE, PARENTHESIS_START);
				result.push_back(op);
			}
			else if (operate == PARENTHESIS_END)
			{
				while (1)
				{
					if (opstk.size() <= 0) {
						throw(char *) "Error: Parentheses Mismatch";
					}
					if (opstk.back() == BRACKET_START) {
						throw(char *) "Error: Parentheses Mismatch";
					}
					if (opstk.back() == PARENTHESIS_START) {
						break;
					}
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
				op.set(OPERATE, PARENTHESIS_END);
				result.push_back(op);
				opstk.pop_back();
				if (opstk.back() == IF || opstk.back() == WHILE)
				{
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
			}
			else if (operate == BRACKET_START)
			{
				opstk.push_back(operate);
				op.set(OPERATE, BRACKET_START);
				result.push_back(op);
			}
			else if (operate == BRACKET_END)
			{
				while (1)
				{
					if (opstk.size() <= 0) {
						throw(char *) "Error: Brackets Mismatch";
					}
					if (opstk.back() == PARENTHESIS_START) {
						throw(char *) "Error: Brackets Mismatch";
					}
					if (opstk.back() == BRACKET_START) {
						break;
					}
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
				op.set(OPERATE, BRACKET_END);
				result.push_back(op);
				opstk.pop_back();
			}
			else if (operate == BRACE_START)
			{
				if (result.size() > 0
					 && result.back().getKind() == OPERATE
					 && result.back().getValue() == PARENTHESIS_END)
				{
					op.set(OPERATE, FUNCTION_LET);
					result.push_back(op);
				}
				heads.push_back((*prog).getNames().size());
				opstk.push_back(operate);
				op.set(OPERATE, BRACE_START);
				result.push_back(op);
			}
			else if (operate == BRACE_END)
			{
				while (1)
				{
					if (opstk.size() <= 0) {
						throw(char *) "Error: Braces Mismatch";
					}
					if (opstk.back() == BRACE_START) {
						break;
					}
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
				op.set(OPERATE, BRACE_END);
				result.push_back(op);
				opstk.pop_back();

				(*prog).removeNamesAfter(heads.back());
				heads.pop_back();
			}
			else if (getOperatePriority(operate) > 10)
			{
				while (1)
				{
					if (opstk.size() <= 0) {
						break;
					}
					if (getOperatePriority(opstk.back()) > getOperatePriority(operate)) {
						break;
					}
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
				opstk.push_back(operate);
				op.set(OPERATE, operate);
				result.push_back(op);
			}
			else
			{
				while (opstk.size() > 0 &&
					   getOperatePriority(opstk.back()) < getOperatePriority(operate))
				{
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
				opstk.push_back(operate);
			}
			break;
		case VARIABLE:
			operate = getCodeOfOperate(*it);
			if (getTypeOfOperate(operate) != NOT_TYPE)
			{
				type = getTypeOfOperate(operate);
				op.set(OPERATE, operate);
				result.push_back(op);
			}
			else if (operate == NOT_OPERATE)
			{
				if (type != NOT_TYPE)
				{
					prog->appendVariableName(*it);
					type = NOT_TYPE;
				}
				op.set(VARIABLE, prog->getIndexOfName((*it), heads.back()));
				result.push_back(op);
			}
			else
			{
				while (opstk.size() > 0 &&
					   getOperatePriority(opstk.back()) < getOperatePriority(operate))
				{
					op.set(OPERATE, opstk.back());
					result.push_back(op);
					opstk.pop_back();
				}
				opstk.push_back(operate);
			}
			break;
		}
		it++;
	}
	while (opstk.size() > 0)
	{
		op.set(OPERATE, opstk.back());
		result.push_back(op);
		opstk.pop_back();
	}

	return result;
}

void printOpcode(Opcode opcode, vector<string> names)
{
	switch (opcode.getKind())
	{
	case NUMERIC:
		cout << "NUM";
		break;
	case VARIABLE:
		cout << "VAR";
		break;
	case OPERATE:
		cout << "OPE";
		break;
	case WHITESPACE:
		cout << "SPA";
		break;
	case LINE_END:
		cout << "LED";
		break;
	case NULLVALUE:
		cout << "NUL";
		break;
	default:
		cout << "???";
	}
	cout << " " << opcode.getValue();

	if (opcode.getKind() == OPERATE) {
		cout << " (" << getCharOfOperate(opcode.getValue()) << ")";
	}
	else if (opcode.getKind() == VARIABLE)
	{
		if (opcode.getValue() >= names.size()) {
			cout << " ( ?? )";
		}
		else {
			cout << " ( " << names[opcode.getValue()] << " )";
		}
	}

	cout << endl;
}

void printProgram(vector<Opcode> program, vector<string> names)
{
	vector<Opcode>::iterator it = program.begin();
	while (it != program.end())
	{
		printOpcode(*it, names);

		it++;
	}
}

void printStack(vector<Opcode> stk)
{
	vector<Opcode>::iterator it = stk.begin();
	cout << "[ stk: ";
	while (it != stk.end())
	{
		Kind kind = (*it).getKind();
		int val = (*it).getValue();
		switch (kind)
		{
		case NUMERIC:
			cout << val << " ";
			break;
		case VARIABLE:
			cout << "$" << val << " ";
			break;
		case OPERATE:
			cout << getCharOfOperate(val) << " ";
			break;
		default:
			cout << "?? ";
		}
		it++;
	}
	cout << "]" << endl;
}

void printVariables(Program &prog)
{
	int i = 0;
	vector<string> names = prog.getNames();
	vector<int> variables = prog.getVariables();
	vector<int>::iterator it = variables.begin();
	cout << "( var: ";
	while (it != variables.end())
	{
		if (i >= names.size()) {
			cout << "$" << i << "=";
		}
		else {
			cout << names[i] << "=";
		}
		cout << (*it) << " ";
		i++;
		it++;
	}
	cout << ")" << endl;
}

int runProgram(Program &param)
{
	if (debugPrint) {
		cout << "RUN PROGRAM" << endl;
	}

	vector<Opcode> stk;
	vector<Opcode> program = param.getProgram();
	int depth = 0, i, let = 0, flet = 0, loop = 0;

	int inFunc = 0;
	vector<int> pns;
	vector<Opcode> pvs;

	pns.clear();
	pvs.clear();

	Opcode a, b, c, op;

	vector<Opcode> newFunc;

	Program newProgram;

	vector<Opcode>::iterator it = program.begin();
	while (it != program.end())
	{
		if (debugPrint) {
			printStack(stk);
			printVariables(param);
			printOpcode(*it, param.getNames());
		}
		Kind kind = (*it).getKind();
		int val = (*it).getValue();
		switch (kind)
		{
		case NUMERIC:
		case VARIABLE:
			stk.push_back(*it);
			if (kind == VARIABLE && inFunc)
			{
				pns.push_back((*it).getValue());
			}
			let = 0;
			break;
		case OPERATE:
			switch (val)
			{
			case INT:
			case VOID:
				let = 1;
				break;
			case FUNCTION_LET:
				flet = 1;
				break;
			case PRINT:
				if (stk.size() < 1) {
					throw(char *) "Error: Stack Overflow";
				}
				a = stk.back();
				stk.pop_back();
				printf("%d\n", executeOpcode(param, a));
				break;
			case ADD:
			case SUBTRACT:
			case MULTIPLY:
			case DIVIDE:
			case MODULO:
			case EQUAL:
			case NOT_EQUAL:
			case GREATER_THAN:
			case LESS_THAN:
			case EQUAL_OR_GREATER_THAN:
			case EQUAL_OR_LESS_THAN:
			case AND:
			case OR:
				if (stk.size() < 2) {
					throw(char *) "Error: Stack Overflow";
				}
				b = stk.back();
				stk.pop_back();
				a = stk.back();
				stk.pop_back();
				if (val == DIVIDE && executeOpcode(param, b) == 0) {
					throw(char *) "Error: Division by 0";
				}
				else if (val == ADD) {
					c.setValue(executeOpcode(param, a) + executeOpcode(param, b));
				}
				else if (val == SUBTRACT) {
					c.setValue(executeOpcode(param, a) - executeOpcode(param, b));
				}
				else if (val == MULTIPLY) {
					c.setValue(executeOpcode(param, a) * executeOpcode(param, b));
				}
				else if (val == DIVIDE) {
					c.setValue(executeOpcode(param, a) / executeOpcode(param, b));
				}
				else if (val == MODULO) {
					c.setValue(executeOpcode(param, a) % executeOpcode(param, b));
				}
				else if (val == EQUAL) {
					c.setValue(executeOpcode(param, a) == executeOpcode(param, b));
				}
				else if (val == NOT_EQUAL) {
					c.setValue(executeOpcode(param, a) != executeOpcode(param, b));
				}
				else if (val == GREATER_THAN) {
					c.setValue(executeOpcode(param, a) > executeOpcode(param, b));
				}
				else if (val == LESS_THAN) {
					c.setValue(executeOpcode(param, a) < executeOpcode(param, b));
				}
				else if (val == EQUAL_OR_GREATER_THAN) {
					c.setValue(executeOpcode(param, a) >= executeOpcode(param, b));
				}
				else if (val == EQUAL_OR_LESS_THAN) {
					c.setValue(executeOpcode(param, a) <= executeOpcode(param, b));
				}
				else if (val == AND) {
					c.setValue(executeOpcode(param, a) && executeOpcode(param, b));
				}
				else if (val == OR) {
					c.setValue(executeOpcode(param, a) || executeOpcode(param, b));
				}
				c.setKind(NUMERIC);
				stk.push_back(c);
				break;
			case RETURN:
				if (stk.size() < 1) {
					throw(char *) "Error: Stack Overflow";
				}
				a = stk.back();
				stk.pop_back();
				if (debugPrint) {
					cout << "FINISHED WITH RETURNURN VALUE " << executeOpcode(param, a) << endl;
				}
				return executeOpcode(param, a);
				break;
			case VARIABLE_LET:
				if (stk.size() < 2) {
					throw(char *) "Error: Stack Overflow";
				}
				b = stk.back();
				stk.pop_back();
				a = stk.back();
				stk.pop_back();
				if (a.getKind() != VARIABLE) {
					throw(char *) "Error: Assignment to const value";
				}
				param.setValueOfVariable(a.getValue(), executeOpcode(param, b));
				c.setValue(executeOpcode(param, b));
				c.setKind(NUMERIC);
				stk.push_back(c);
				break;
			case FUNCTION_START:
				inFunc = 1;
				pns.clear();
				op.set(OPERATE, FUNCTION_START);
				stk.push_back(op);
				break;
			case FUNCTION_END:
				while ((stk.back().getKind() != OPERATE) || (stk.back().getValue() != FUNCTION_START))
				{
					pvs.push_back(stk.back());
					stk.pop_back();
					if (stk.size() <= 0) {
						throw(char *) "Error: Braces Mismatch";
					}
				}
				reverse(pvs.begin(), pvs.end());
				stk.pop_back();
				i = executeOpcode(param, stk.back());
				stk.pop_back();
				if (debugPrint) {
					cout << "Preparing to call function " << i << endl;
					cout << "# of functions :" << param.getFuncs().size() << endl;
				}
				newProgram = param.getFuncs()[i];
				newProgram.names = param.names;
				newProgram.variables = param.variables;
				newProgram.functions = param.functions;
				if (debugPrint) {
					cout << "Called function " << i << endl;
				}
				for (int j = 0; j < newProgram.params.size(); j++)
				{
					newProgram.variables[newProgram.params[j]] = executeOpcode(param, pvs.at(j));
					if (debugPrint) {
						cout << "  "
							 << "param " << j << " : " << executeOpcode(param, pvs.at(j)) << endl;
					}
				}
				i = runProgram(newProgram);
				c.setKind(NUMERIC);
				c.setValue(i);
				stk.push_back(c);
				break;
			case BRACE_START:
				if (flet)
				{
					inFunc = 0;
					newFunc.clear();
					depth = 1;
					it++;
					while (it != program.end())
					{
						if ((*it).getKind() == OPERATE && (*it).getValue() == BRACE_START)
							depth++;
						if ((*it).getKind() == OPERATE && (*it).getValue() == BRACE_END)
						{
							depth--;
							if (depth <= 0)
								break;
						}
						newFunc.push_back(*it);
						it++;
					}
					it++;
					if (it == program.end()) {
						throw(char *) "Error: Braces Mismatch";
					}

					newProgram.setProgram(newFunc);
					newProgram.params = pns;

					newProgram.names = param.names;
					newProgram.variables = param.variables;
					newProgram.functions = param.functions;

					param.appendFunc(newProgram);
					i = param.functions.size() - 1;
					while (stk.back().getKind() != VARIABLE)
					{
						stk.pop_back();
						if (stk.size() <= 0) {
							throw(char *) "Error: Parentheses Mismatch";
						}
					}
					a = stk.back();
					stk.pop_back();
					param.setValueOfVariable(a.getValue(), i);
					flet = 0;
				}
				break;
			case COMMA:
			case PARENTHESIS_END:
				if (inFunc && let == 0)
				{
					if (stk.size() <= 0 || stk.back().getKind() == OPERATE)
					{
						c.setKind(NUMERIC);
						c.setValue(0);
					}
					else
					{
						a = stk.back();
						c.setKind(NUMERIC);
						c.setValue(executeOpcode(param, a));
					}
				}
				break;
			case BRACKET_END:
				if (let)
				{
					a = stk.back();
					stk.pop_back();
					int n = executeOpcode(param, a);
					b = stk.back();
					stk.pop_back();
					if (b.getKind() != VARIABLE) {
						throw(char *) "Error: Illegal Brackets Usage";
					}
					param.setValueOfVariable(b.getValue(), param.getVariables().size());
					for (i = 0; i < n; i++)
					{
						param.variables.push_back(0);
					}
					let = 0;
				}
				else
				{
					a = stk.back();
					stk.pop_back();
					int n = executeOpcode(param, a);
					b = stk.back();
					stk.pop_back();
					if (b.getKind() != VARIABLE) {
						throw(char *) "Error: Illegal Brackets Usage";
					}
					b.setKind(VARIABLE);
					b.setValue(b.getValue() + n);
					stk.push_back(b);
				}
				break;
			case WHILE:
				loop = 1;
				let = 0;
			case IF:
				if (stk.size() < 1) {
					throw(char *) "Error: Stack Overflow";
				}
				a = stk.back();
				stk.pop_back();
				if (executeOpcode(param, a) == 0)
				{
					loop = 0;
					int d = depth;
					while (it != program.end())
					{
						if (debugPrint)
						{
							cout << "  SKIPPING ";
							printOpcode(*it, param.getNames());
						}
						Kind k = (*it).getKind();
						int v = (*it).getValue();
						if (k == OPERATE)
						{
							if (v == BRACE_START)
								d++;
							if (v == BRACE_END)
							{
								d--;
								if (d == depth) {
									break;
								}
							}
							if (v == LINE_END)
							{
								if (d == depth) {
									break;
								}
							}
						}
						it++;
					}
					it--;
				}
				break;
			case BRACE_END:
				if (loop)
				{
					int depth = 1;
					while (it != program.begin())
					{
						it--;
						if ((*it).getKind() == OPERATE && (*it).getValue() == BRACE_END) {
							depth++;
						}
						if ((*it).getKind() == OPERATE && (*it).getValue() == BRACE_START)
						{
							depth--;
							if (depth == 0) {
								break;
							}
						}
					}
					if (it == program.begin()) {
						throw(char *) "Error: Braces Mismatch";
					}
					it--;
					depth = 0;
					while (it != program.begin())
					{
						it--;
						if ((*it).getKind() == OPERATE && (*it).getValue() == PARENTHESIS_END) {
							depth++;
						}
						if ((*it).getKind() == OPERATE && (*it).getValue() == PARENTHESIS_START)
						{
							depth--;
							if (depth == 0){
								break;
							}
						}
					}
					break;
					loop = 0;
				}
			default:
				//throw (char*)"Error: Unknown Operation";
				break;
			}
			break;
		default:
			//throw (char*)"Error: Unknown Opcode";
			break;
		}
		it++;
	}

	if (debugPrint) {
		printStack(stk);
		printVariables(param);
		cout << "FINISHED" << endl;
	}
	return 0;
}

int main(int argc, char **argv)
{
	Program prog;
	int res;
	FILE *fp = NULL;
	try
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-l") == 0) {
				debugPrint = 1;
			}
			else if (strcmp(argv[i], "") != 0)
			{
				if (fp == NULL)
				{
					fp = fopen(argv[i], "r");
					if (fp == NULL) {
						throw(char *) "Error: Couldn't open file";
					}
				}
			}
		}
		initProgram(prog);
		prepareMainFunc(prog);
		while (1)
		{
			initProgram(prog);
			readTokens(prog, fp);
			if (debugPrint) {
				printTokens(prog);
			}
			prog.setProgram(parseFormula(prog.getTokens(), &prog));
			if (debugPrint) {
				printProgram(prog.getProgram(), prog.getNames());
			}
			res = runProgram(prog);
		}
	}
	catch (char *msg)
	{
		cout << msg << endl;
	}
	catch (int r)
	{
	}
	Opcode op;
	op.setKind(VARIABLE);
	op.setValue(prog.getIndexOfName("main", 0));
	res = runProgram(prog.functions[executeOpcode(prog, op)]);
	if (fp != NULL)
	{
		fclose(fp);
	}
	return 0;
}
