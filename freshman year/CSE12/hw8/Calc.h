#ifndef CALC_H
#define CALC_H

/* DO NOT CHANGE:  This file is used in evaluation */

#include <iostream>
#include <cstdio>
#include <string.h>
#include "Stack.h"
#include "SymTab.h"
using namespace std;

#define OPERATOR 0
#define VALUE 1
#define VARIABLE 2

class Operator {
        friend class Calculator;
        friend class Word;
        friend ostream & operator << (ostream &, const Operator &);

        char ascii;             /* ascii character of operator */
        long index;             /* index in parallel arrays */
        long priority;          /* priority of operator */

        Operator (char);
};

class Variable {
        friend class Calculator;
        friend class Word;
        friend Variable * assign (Variable *, long);
        friend ostream & operator << (ostream &, const Variable &);

        char name[80];          /* name of variable */
        long value;             /* value of interest */

        Variable (char * nm, long val = 0) : value (val) {
                strcpy (name, nm);
        }
        Variable (void) : value (0) {
                strcpy (name, "");
        }
public:
        Variable (const Variable & variable) : value (variable.value) {
                strcpy (name, variable.name);
        }
        operator const char * (void) const {
               return name;
        }
        long operator == (const Variable & vvv) const {
                return ! strcmp (name, vvv.name);
        }
        long operator > (const Variable & vvv) const {
                return (strcmp (name, vvv.name) > 0) ? 1 : 0;
        }
};

/* declare the word to place on calulator stacks */
class Word {
        friend class Calculator;
        friend ostream & operator << (ostream &, const Word &);

        union {
                Operator * op;
                long value;
                Variable * var;
        };
        long type;

        char isoperator (void) const {
                return (type == OPERATOR);
        }
        char isvalue (void) const {
                return (type == VALUE);
        }
        char isvariable (void) const {
                return (type == VARIABLE);
        }
        Word (char character) : op (new Operator(character)), type (OPERATOR) {}
        Word (long val) : value (val), type (VALUE) {}
        Word (char * name, long val = 0) :
                var (new Variable (name, val)), type (VARIABLE) {}
public:
        ~Word (void) {
                if (type == OPERATOR)
                        delete op;
                else if (type == VARIABLE)
                        delete var;
        }
};

class Calculator {
        friend ostream & operator << (ostream &, const Calculator &);

        SymTab<Variable> symtab;
        Stack<Word> stack1, stack2;
public:
        long Eval (void);
        long InToPost (void);
        ostream & Write_Postfix (ostream &) const;
};

#endif
