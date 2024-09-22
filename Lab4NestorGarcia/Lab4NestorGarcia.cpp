//// interpreter for simplified infix expression with {+, -, *, / } operations;
//// keyboard input, single digit numbers only and no spaces are allowed;
//// compile: $> g++ prog1.cpp
//// run with: 2+3*4/2+3+4*2
#include <cstdlib> //for atoi()
#include <iostream>

#include <list>

#include <string>
#include <vector>
#include <fstream> // file reading
#include <unordered_map>
#include <sstream>


using namespace std;

// using file input

int exp(), term(), fact(), exp2(int), term2(int), fact2(int), Num();

//int word(string);

void wordFound(string);

void Declarations();
void Declaration(string);

void myWord();
int myChar();


string prog;
string curWord; //string for reading 1-line input expression (program)


int indexx = 0; //global index for program string
int tableIn = 0;


// SYMBOL TABLE	//
const int mySize = 100;
struct symbolTable
{
	char id;
	string type;
	int val;
	
	symbolTable();

};
unordered_map<int, symbolTable> myTable;


/***************************/

void fileReading()
{
	string line;
	ifstream myfile;
	myfile.open("myProg.txt");

	while (getline(myfile, line)) {
		prog += line + " ";	// This allows us to seperate program instructions by spaces to make it readable with code

	}

	myfile.close();
}

//void nextWord(istringstream iss(string));






int main(int argc, const char** argv)
{
	

	fileReading();

	myWord();
	if (curWord == "program")
	{
		Declarations();
	}
	else
	{
		cout << "Error" << endl; // Make proper error
		exit(1);
	}

 }









// exp() returns value from exp2(term())
int exp()		// exp: tail-end recursion to call our non-terminals
{

	return exp2(term());	// if exp is called, we goto exp2 with our term
}

// term() returns value from term2(fact())
int term() 	// term: using tail-end recursion to call non-terminals
{

	return term2(fact()); // term calls fact() -> fact2() -> term2()
}

int fact()
{

	return fact2(Num());
}

// using input 'inp' we make our local variable 'result' be 'inp'
// if the current indexx is less than our prog.length then it is not at the end of string
// we get a single char from program string and make it local variable char 'a'
// if 'a' is '+' we have 'result' value change to exp2(result + term())
// if 'a' is '-' we have 'result' value change to exp2(result - term())
// after all the calls, the 'result' is returned

int exp2(int inp)		// implements right-recursive form to get our 'inp' by
{						// calling exp2 either adding or subtracting our result and our 'term'

	int result = inp;
	if (indexx < prog.length()) //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string

		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '+')
			result = exp2(result + term()); //handles t+t
		else if (a == '-')
			result = exp2(result - term()); //handles t-t
		else if (a == ')')
			indexx--;		// if none of these, we go back one step


	}
	return result;
}


// using input 'inp' we make it a local variable 'result' be 'inp'
// if our 'indexx' is less than the length of 'prog' then it is not the end of string, so we continue
// we then get char 'a' to be a single char from the string
// if 'a' is '*' we have 'result' value change to term2(result * term())
// if 'a' is '/' we have 'result' value change to term2(result / term())
// if 'a' is either '+' or '-' then we go back one position
// after all these we return our result

int term2(int inp)
{

	int result = inp;
	if (indexx < prog.length()) //if not the end of program string
	{
		char a = prog.at(indexx++); //get one chr from program string

		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '*')
			result = term2(result * fact()); //handles consecutive * operators
		else if (a == '/')
			result = term2(result / fact()); //handles consecutive / operators
		else if (a == ')' || a == '+' || a == '-') //if + or -, get back one position
			indexx--;

	}
	return result;
}

// fact2() does our power, if our current character is a '^', we make our result
// be the base to our next character, and so forth, nested so it can recursively check
// for the upcoming characters
int fact2(int inp)
{
	int result = inp;
	if (indexx < prog.length())
	{
		char a = prog.at(indexx++);

		while (a == ' ' && (indexx < prog.length()))
		{
			a = prog.at(indexx++);
		}

		if (a == '^')
		{
			result = fact2(pow(result, fact()));  // Correctly handle right-associative
		}
		else if (a == ')' || a == '*' || a == '/' || a == '+' || a == '-')
			indexx--;  // Step back if not '^'

	}
	return result;
}


int Num()
{

	char a = prog.at(indexx++);
	while (a == ' ' && (indexx < prog.length()))
	{
		a = prog.at(indexx++);
	}

	if (a == '(')			// if left parantheses at indexx
	{
		//indexx++;					// we incremenet to get next char
		int result = exp();			// keep result of exp() in result
		if (prog.at(indexx) == ')')	// checks to see if the indexx is sitting at right parantheses
		{
			indexx++;				// if so, we increment to next char
		}
		return result;				// we return result to fact2() which is nested in term2()

	}

	if (isdigit(a))
	{
	

		int num = a - '0';  // using ascii values to get num
		
		while (indexx < prog.length() && isdigit(prog.at(indexx)))
		{

			num = num * 10 + (prog.at(indexx++) - '0');  // Multiplying our initial num by 10 to move numbers place
			//cout << num << endl;

		}

		return num;  // Return the multi-digit number
	}




	return atoi(&a);


}


void Declarations()
{
	myWord();
	if (curWord == "begin")
	{
		exit(1);
	}
	else if (curWord == "int" || curWord == "double")
	{
		Declaration(curWord);
	}

	Declarations();
}

void Declaration(string inpWord) 
{
	string varType = inpWord;


	char curChar = prog.at(indexx++);
	while (curChar == ' ' && (indexx < prog.length()))
	{
		curChar = prog.at(indexx++);
	}

	myTable[tableIn].id = curChar;
	myTable[tableIn].type = varType;
	tableIn++;

	if (prog.at(indexx++) == ',')
	{
		Declaration(varType);

	}
	else if (prog.at(indexx++) == ';')
	{
		exit(1);
	}

}


void myWord()
{

	//char a = prog.at(indexx++);
	
	string tempString;

	while (prog.at(indexx) == ' ' && (indexx < prog.length()))
	{
		indexx++;
	}

	if (isalpha(prog.at(indexx)))
	{
		while (indexx < prog.length() && isalpha(prog.at(indexx)))
		{

			tempString += prog.at(indexx++);  // Multiplying our initial num by 10 to move numbers place
			//cout << num << endl;

		}
		curWord = tempString;
	}


}



int myChar()
{

	char a = prog.at(indexx++); //get one chr from program string
	while (a == ' ' && (indexx < prog.length()))
	{
		a = prog.at(indexx++);
	}
	return atoi(&a); //converts a char to a numeric number and return


}

/****************************************************************/
symbolTable::symbolTable()	// Constructor for our symbol table
{
	// Defaults
	id = '0';
	type = "EMPTY";
	val = -1;
}