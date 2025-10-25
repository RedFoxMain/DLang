#ifndef AST_H
#define AST_H

#include <sstream>
#include <vector>
#include "../Lexer/Lexer.h"

class Int;
class Float;
class Str;
class Id;
class BinOp;
class UnOp;
class FullVarDecl;
class EmptyVarDecl;
class ReasignVar;
class If;
class BlockOfCode;

// Interface for AstPrinter
struct PrintVisitor {
	virtual std::stringstream visit(Int* node, int deep) = 0;
	virtual std::stringstream visit(Float* node, int deep) = 0;
	virtual std::stringstream visit(Str* node, int deep) = 0;
	virtual std::stringstream visit(Id* node, int deep) = 0;
	virtual std::stringstream visit(UnOp* node, int deep) = 0;
	virtual std::stringstream visit(BinOp* node, int deep) = 0;
	virtual std::stringstream visit(EmptyVarDecl* node, int deep) = 0;
	virtual std::stringstream visit(FullVarDecl* node, int deep) = 0;
	virtual std::stringstream visit(ReasignVar* node, int deep) = 0;
	virtual std::stringstream visit(BlockOfCode* node, int deep) = 0;
	virtual std::stringstream visit(If* node, int deep) = 0;
};

// Interface for Interpreter
struct Visitor {
	virtual void visit(Int* node) = 0;
	virtual void visit(Float* node) = 0;
	virtual void visit(Str* node) = 0;
	virtual void visit(Id* node) = 0;
	virtual void visit(UnOp* node) = 0;
	virtual void visit(BinOp* node) = 0;
	virtual void visit(EmptyVarDecl* node) = 0;
	virtual void visit(FullVarDecl* node) = 0;
	virtual void visit(ReasignVar* node) = 0;
	virtual void visit(BlockOfCode* node) = 0;
	virtual void visit(If* node) = 0;
};

struct AST {
	virtual std::stringstream handler(PrintVisitor* print_visitor, int deep) = 0;
	virtual void handler(Visitor* visitor) = 0;
};

// Node for int
class Int: public AST {
public:
	int value;
	size_t line, column;
	TokenType type;

public:
	Int(Token* token): value(std::stoi(token->value)), line(token->line), column(token->column), type(token->type) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override{ visitor->visit(this); }
};

// Node for float
class Float : public AST {
public:
	float value;
	size_t line, column;
	TokenType type;

public:
	Float(Token* token) : value(std::stof(token->value)), line(token->line), column(token->column), type(token->type) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for string
class Str: public AST{
public:
	std::string value;
	size_t line, column;
	TokenType type;

public:
	Str(Token* token) : value(token->value), line(token->line), column(token->column), type(token->type) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for identifiers
class Id: public AST {
public:
	Token* identifier;

public:
	Id(Token* token) : identifier(token) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Unary operations
class UnOp: public AST {
public:
	Token* operation;
	AST* right;

public:
	UnOp(Token* operation, AST* right) : right(right), operation(operation) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Binary operations
class BinOp: public AST {
public:
	AST* left;
	AST* right;
	Token* operation;

public:
	BinOp(AST* left, Token* operation, AST* right): left(left), operation(operation), right(right) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Variable declaration statement
class EmptyVarDecl: public AST {
public:
	Token* key_word;
	Token* type_assign_op;
	Token* var_type;
	Id* identifier;

public:
	EmptyVarDecl(Token* key_word, Id* identifier, Token* type_assign_op, Token* var_type): key_word(key_word), identifier(identifier), type_assign_op(type_assign_op), var_type(var_type) {} // -> key_word id: type;
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Variable declaration statement
class FullVarDecl : public AST {
public:
	EmptyVarDecl* declaration;
	Token* assign;
	AST* expr;

public:
	FullVarDecl(EmptyVarDecl* declaration, Token* assign, AST* expr) : declaration(declaration), assign(assign), expr(expr) {} // -> key_word id: type = expr;
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Variable reasigment statement
class ReasignVar : public AST {
public:
	Id* identifier;
	Token* assign;
	AST* expr;

public:
	ReasignVar(Id* identifier, Token* assign, AST* expr) : identifier(identifier), assign(assign), expr(expr) {} // -> id = expr; || id [+ - * /]= expr;
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for list of statements
class BlockOfCode : public AST {
public:
	std::vector<AST*> list;

public:
	BlockOfCode(std::vector<AST*> list): list(std::move(list)) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for if statement
class If : public AST {
public:
	AST* condition;
	BlockOfCode* code_to_execute;

public:
	If(AST* condition, BlockOfCode* code_to_execute): condition(condition), code_to_execute(code_to_execute) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};
#endif // !AST_H