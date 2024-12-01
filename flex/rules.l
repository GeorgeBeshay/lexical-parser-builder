%{
#include <stdio.h>
#include <stdlib.h>
%}

letter   [a-zA-Z]
digit    [0-9]
id       {letter}({letter}|{digit})*
digits   {digit}+
num      {digit}+(\.{digits}((E){digit}+)?)?

relop    (==|!=|>=|<=|>|<)
assign   =
addop    (\+|-)
mulop    (\*|\/)

%%

boolean|int|float         { printf("Keyword: %s\n", yytext); }
if|else|while             { printf("Keyword: %s\n", yytext); }
[;,(){}]                  { printf("Punctuation: %s\n", yytext); }
{id}                      { printf("Identifier: %s\n", yytext); }
{num}                     { printf("Number: %s\n", yytext); }
{relop}                   { printf("Relational Operator: %s\n", yytext); }
{assign}                  { printf("Assignment Operator: %s\n", yytext); }
{addop}                   { printf("Addition/Subtraction Operator: %s\n", yytext); }
{mulop}                   { printf("Multiplication/Division Operator: %s\n", yytext); }

.                          { /* Ignore unrecognized characters */ }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}