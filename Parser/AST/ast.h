#ifndef AST_H
#define AST_H

#include <sstream>
#include <vector>
#include "../Lexer/Lexer.h"
#include "../../Object/Array/darray.h"

class IntNode;
class FloatNode;
class StrNode;
class ArrayNode;
class IdNode;
class BinOpNode;
class UnOpNode;
class FullVarDeclNode;
class EmptyVarDeclNode;
class ReasignVarNode;
class IfStmtNode;
class WhileStmtNode;
class BlockOfCodeNode;
// class ParamNode;
// class FuncCallNode;

// Interface for AstPrinter
struct PrintVisitor {
	virtual std::stringstream visit(IntNode* node, int deep) = 0;
	virtual std::stringstream visit(FloatNode* node, int deep) = 0;
	virtual std::stringstream visit(StrNode* node, int deep) = 0;
	virtual std::stringstream visit(ArrayNode* node, int deep) = 0;
	virtual std::stringstream visit(IdNode* node, int deep) = 0;
	virtual std::stringstream visit(UnOpNode* node, int deep) = 0;
	virtual std::stringstream visit(BinOpNode* node, int deep) = 0;
	virtual std::stringstream visit(EmptyVarDeclNode* node, int deep) = 0;
	virtual std::stringstream visit(FullVarDeclNode* node, int deep) = 0;
	virtual std::stringstream visit(ReasignVarNode* node, int deep) = 0;
	virtual std::stringstream visit(BlockOfCodeNode* node, int deep) = 0;
	virtual std::stringstream visit(IfStmtNode* node, int deep) = 0;
	virtual std::stringstream visit(WhileStmtNode* node, int deep) = 0;
	// virtual std::stringstream visit(ParamNode* node, int deep) = 0;
	// virtual std::stringstream visit(FuncCallNode* node, int deep) = 0;
};

// Interface for Interpreter
struct Visitor {
	virtual void visit(IntNode* node) = 0;
	virtual void visit(FloatNode* node) = 0;
	virtual void visit(StrNode* node) = 0;
	virtual void visit(ArrayNode* node) = 0;
	virtual void visit(IdNode* node) = 0;
	virtual void visit(UnOpNode* node) = 0;
	virtual void visit(BinOpNode* node) = 0;
	virtual void visit(EmptyVarDeclNode* node) = 0;
	virtual void visit(FullVarDeclNode* node) = 0;
	virtual void visit(ReasignVarNode* node) = 0;
	virtual void visit(BlockOfCodeNode* node) = 0;
	virtual void visit(IfStmtNode* node) = 0;
	virtual void visit(WhileStmtNode* node) = 0;
	// virtual void visit(ParamNode* node) = 0;
	// virtual void visit(FuncCallNode* node) = 0;
};

struct AST {
	virtual std::stringstream handler(PrintVisitor* print_visitor, int deep) = 0;
	virtual void handler(Visitor* visitor) = 0;
};

// Node for int
class IntNode: public AST {
public:
	int value;
	size_t line, column;
	TokenType type;

public:
	IntNode(Token* token): value(std::stoi(token->value)), line(token->line), column(token->column), type(token->type) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override{ visitor->visit(this); }
};

// Node for float
class FloatNode: public AST {
public:
	float value;
	size_t line, column;
	TokenType type;

public:
	FloatNode(Token* token) : value(std::stof(token->value)), line(token->line), column(token->column), type(token->type) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for string
class StrNode: public AST{
public:
	std::string value;
	size_t line, column;
	TokenType type;

public:
	StrNode(Token* token) : value(token->value), line(token->line), column(token->column), type(token->type) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for array
class ArrayNode: public AST{
public:
	DArray array;

public:
	ArrayNode(std::vector<Token*> array): array(array){}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for identifiers
class IdNode: public AST {
public:
	Token* identifier;

public:
	IdNode(Token* token) : identifier(token) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Unary operations
class UnOpNode: public AST {
public:
	Token* operation;
	AST* right;

public:
	UnOpNode(Token* operation, AST* expression) : right(expression), operation(operation) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Binary operations
class BinOpNode: public AST {
public:
	AST* left;
	AST* right;
	Token* operation;

public:
	BinOpNode(AST* left, Token* operation, AST* right): left(left), operation(operation), right(right) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Variable declaration statement
class EmptyVarDeclNode: public AST {
public:
	Token* key_word;
	Token* type_assign_op;
	Token* var_type;
	IdNode* identifier;

public:
	EmptyVarDeclNode(Token* key_word, IdNode* identifier, Token* type_assign_op, Token* var_type): key_word(key_word), identifier(identifier), type_assign_op(type_assign_op), var_type(var_type) {} // -> key_word id: type;
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Variable declaration statement
class FullVarDeclNode: public AST {
public:
	EmptyVarDeclNode* declaration;
	Token* assign;
	AST* expr;

public:
	FullVarDeclNode(EmptyVarDeclNode* declaration, Token* assign, AST* expr) : declaration(declaration), assign(assign), expr(expr) {} // -> key_word id: type = expr;
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for Variable reasigment statement
class ReasignVarNode: public AST {
public:
	IdNode* identifier;
	Token* assign;
	AST* expr;

public:
	ReasignVarNode(IdNode* identifier, Token* assign, AST* expr) : identifier(identifier), assign(assign), expr(expr) {} // -> id = expr; || id [+ - * /]= expr;
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for list of statements
class BlockOfCodeNode: public AST {
public:
	std::vector<AST*> list;

public:
	BlockOfCodeNode(std::vector<AST*> list): list(std::move(list)) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for if statement
class IfStmtNode: public AST {
public:
	AST* condition;
	BlockOfCodeNode* code_to_execute;

public:
	IfStmtNode(AST* condition, BlockOfCodeNode* code_to_execute): condition(condition), code_to_execute(code_to_execute) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};

// Node for while statement
class WhileStmtNode: public AST {
public:
	AST* condition;
	BlockOfCodeNode* code_to_execute;

public:
	WhileStmtNode(AST* condition, BlockOfCodeNode* code_to_execute): condition(condition), code_to_execute(code_to_execute) {}
	std::stringstream handler(PrintVisitor* print_visitor, int deep) override { return print_visitor->visit(this, deep); }
	void handler(Visitor* visitor) override { visitor->visit(this); }
};
#endif // !AST_H