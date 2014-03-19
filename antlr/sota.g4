grammar sota;

tokens { INDENT, DEDENT }
@lexer::header {
    import com.yuvalshavit.antlr4.DenterHelper;
}
@lexer::members {
    private final DenterHelper denter = new DenterHelper(NL, sotaParser.INDENT, sotaParser.DEDENT) {
        @Override
        public Token pullToken() {
            return sotaLexer.super.nextToken();
        }
    };

    @Override
    public Token nextToken() {
        return denter.nextToken();
    }

    public Token rawNextToken() {
        return super.nextToken();
    }
}

prog
    :   global NL* EOF
    ;

global
    :   (namespace | stmts)
    ;

namespace
    :   ID INDENT namespace DEDENT
    |   ID flow
    ;

block
    :   flow
    |   inline
    ;

flow
    :   INDENT stmts DEDENT
    ;

inline
    :   '{' stmt (';' stmt)* '}'
    ;

stmts
    :   stmt ( (NL | ';') stmt)* NL*
    ;

stmt
    :   expr
    |   if_block
    ;

if_block
    :   'if' expr block ('elsif' expr block)* ('else' block)?
    ;

exprs
    :   expr ( (NL | ',') expr)*
    ;

expr
    :   func
    |   call
    |   expr ('*' | '/') expr
    |   expr ('+' | '-') expr
    |   expr '..' expr
    |   <assoc=right> expr 'if' expr 'else' expr
    |   <assoc=right> expr '?' expr ':' expr
    |   <assoc=right> expr '=' expr
    |   INT
    |   ID
    |   '(' expr ')'
    ;

func
    :   '(' params ')' flow
    |   '(' params ')' '->' inline
    ;

call
    :   ID '(' params ')'
    ;

params
    :   exprs?
    ;

INT
    :   [1-0] [0-9]*
    ;

ID  :   [a-zA-Z0-9]+
    ;

WS  :   [ \t]+ -> skip
    ;

NL  :   '\r'? '\n' ' '*
    ;
