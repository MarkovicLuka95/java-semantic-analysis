%option noyywrap
%option nounput
%option noinput

%{
#include <iostream>
#include <string>
#include <cstdlib>
#include "type.hpp"

using namespace std;

#include "parser.tab.hpp"
%}

%%
"class"                     {return class_token;}
"public"                    {
    yylval.s = new string(yytext);
    return public_token;
}
"private"                   {
    yylval.s = new string(yytext);
    return private_token;
}
"int"                       {
    yylval.s = new string(yytext);
    return int_token;
}
"double"                    {
    yylval.s = new string(yytext);
    return double_token;
}
"void"                      {
    yylval.s = new string(yytext);
    return void_token;
}
"string"                    {
    yylval.s = new string(yytext);
    return string_token;
}
[A-Za-z_][A-Za-z0-9_]*      {
    yylval.s = new string(yytext);
    return id_token;
}
[0-9]+                      {
    yylval.number = atoi(yytext);
    return intNum_token;
}
[0-9]+\.[0-9]+              {
    yylval.d = atof(yytext);
    return doubleNum_token;
}
\"(\\.|[^\"])*\"            {
    yylval.s = new string(yytext);
    return text_token;
}
\/\/[^\n]*                  { }
[\.,;{}=\(\)+*\-]           {return *yytext;}
[\n\t ]                     { }
.                           {
    cerr << "Lexical error: unknown token '" << *yytext << "'" << endl;
    exit(EXIT_FAILURE);
}

%%
