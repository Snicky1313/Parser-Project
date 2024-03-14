/*  Mini-Compiler Parser Project  
* Lexical and Syntax Analyzers
* 
* Nancy Child
* nchild2023@fau.edu
* Project 1 for COP 4020: Programming Languages
* Spring 2024

    prog        ->  begin stmt_list end

    stmt_list   ->  stmt ; stmt_list
                 |  stmt
				 |	stmt;

    stmt        ->  var = expr

    var         ->  A | B | C

    expr        ->  var + expr
                 |  var - expr
                 |  var0202

*/

#include    <iostream>
#include    <fstream>
#include    <cctype>
#include    "token.h"
#include    "functions.h"
#include <string> // Include the string library

using namespace std;

ifstream ifs;                   // input file stream used by lexan
SymTab   symtab;                // global symbol table
Token    token;                 // global token
int      lookahead = 0;         // no look ahead token yet

int      dbg = 1;               // debut is ON

//prototypes
void prog();
void stmt_list();
void stmt();
void var();
void expr();

void match(int t);
void emit(int t);
void error(int t, int expt, const string& str);


int main( )
{
    ifs = get_ifs();           // open an input file stream w/ the program
    init_kws( );                // initialize keywords in the symtab
    match( lookahead );         // get the first input token
    prog( );

    return 0;
}
// Start parsing function
void prog() 
{
   // if (token.tokstr() == "begin") { // look for 'begin' keyword 
      //  emit(lookahead); 
     //   emit('\n');
  //  }
       emit(KW); cout << endl; //printout 'begin' and next line
        match(KW); // look for 'begin'
        stmt_list();
        emit(KW); cout << endl; //printout 'end'
        match(KW); // look for 'end'  
        // Check for additional tokens after 'end' to check for errors
        if (lookahead != DONE) {
            error(lookahead, DONE, "End of input");
        }
}

// List function
void stmt_list() {
    do {
        stmt();
        if (lookahead == ';') {
           match(';'); // Consume the semicolon
       }
    } while (lookahead != KW && lookahead != DONE); // Until 'end' 
   
}

// Each statement function
void stmt() {
    var();
    if (lookahead == '=') {
        match('='); // Expect '='
        expr();
        emit('=');// Emit '=' at the end for postfix
        emit(';'); }// Emit a ';' after each statement
    else {
        error(lookahead, '=', "=");// check for errors
    }
}

// variable function
void var() 
{
        if (token.toktype() == ID && (token.tokvalue() == symtab.lookup("A") ||
        token.tokvalue() == symtab.lookup("B") || token.tokvalue() == symtab.lookup("C"))) 
        {
            emit(lookahead); // Output the variable identifier
            match(ID); // Consume the identifier
        }
        else
        {
            error(ID); // Error if it's not an identifier
        }
}

// Convert midfix to postfix
void expr() {
    if (lookahead == ID || lookahead == UID) {
        emit(lookahead); // Output the first term
        match(lookahead); // Consume the first term
    }
    else {
        error(lookahead, ID, "Variable or constant"); // Error if it's not a variable or constant
    }
    while (lookahead == '+' || lookahead == '-') {
        int op = lookahead;
        match(lookahead); // Consume the operator
        if (lookahead == ID || lookahead == UID) {
            emit(lookahead); // Output the next term
            match(lookahead); // Consume the next term
        }
        else {
            error(lookahead, ID, "Variable or constant"); // Error if it's not a variable or constant
        }
        emit(op); // Emit the operator
    }  
}

// utility methods

void emit( int t )
{
    switch( t )
    {
        case '+': case '-': case '=':
            cout << char( t ) << ' ';
            break;

        case ';':
            cout << ";\n";
            break;

        case '\n':
            cout << "\n";
            break;

        case ID:
        case KW:
        case UID:
            cout << symtab.tokstr( token.tokvalue( ) ) << ' ';
            break;

        default:
            cout << "'token " << t << ", tokvalue "
                 << token.tokvalue( ) << "' ";
            break;
    }
}

void error( int t, int expt, const string &str )
{
    cerr << "\nunexpected token '";
    if( lookahead == DONE )
    {
        cerr << "EOF";
    }
    else
    {
        cerr << token.tokstr( );
    }
    cerr << "' of type " << lookahead;

    switch( expt )
    {
        case 0:         // default value; nothing to do
            break;

        case ID:
            cout << " while looking for an ID";
            break;

        case KW:
            cout << " while looking for KW '" << str << "'";
            break;

        default:
            cout << " while looking for '" << char( expt ) << "'";
            break;
    }

    cerr << "\nsyntax error.\n";

    exit( 1 );
}


void match( int t )
{
    if( lookahead == t )
    {
        token = lexan( );               // get next token
        lookahead = token.toktype( );   // lookahed contains the tok type
    }
    else
    {
        error( t );
    }
}

