#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "AST.h"
#include <iostream>

class ASTPrinter: public PrintVisitor {
private:
	// Print Int node
	std::stringstream visit(Int* node, int deep) override{
		std::stringstream stream;
		stream << "IntNode(" << node->value << ")";
		return stream;
	}

	// Print Float node
	std::stringstream visit(Float* node, int deep) override {
		std::stringstream stream;
		stream << "FloatNode(" << node->value << ")";
		return stream;
	}

	// Print String node
	std::stringstream visit(Str* node, int deep) override {
		std::stringstream stream;
		stream << "StrNode(" << node->value.getSelf() << ")";
		return stream;
	}

	// Print Identifiers node
	std::stringstream visit(Id* node, int deep) override {
		std::stringstream stream;
		stream << "IdNode(" << node->identifier->value << ")";
		return stream;
	}

	// Print Unary operation node
	std::stringstream visit(UnOp* node, int deep) override {
		std::stringstream stream;
		stream << "UnOpNode ->\n";
		deep += 3;
		stream << std::string(deep, ' ') << "Op(" << node->operation->value << ")\n" << std::string(deep, ' ');
		stream << "Right(" << node->right->handler(this, deep).str() << ")";
		return stream;
	}

	// Print Binary operation node
	std::stringstream visit(BinOp* node, int deep) override {
		std::stringstream stream;
		stream << "BinOpNode ->\n";
		deep += 3;
		stream << std::string(deep, ' ')
			<< "Left(" << node->left->handler(this, deep).str() << ")\n" << std::string(deep, ' ')
			<< "Op(" << node->operation->value << ")\n" << std::string(deep, ' ')
			<< "Right(" << node->right->handler(this, deep).str() << ")";
		return stream;
	}

	// Print Empty variable declaration node ( key id: type; )
	std::stringstream visit(EmptyVarDecl* node, int deep) override {
		std::stringstream stream;
		stream << "EmptyVarDeclNode ->\n";
		deep += 3;
		stream << std::string(deep, ' ')
			<< "Key(" << node->key_word->value << ")\n" << std::string(deep, ' ')
			<< node->identifier->handler(this, deep).str() << "\n" << std::string(deep, ' ')
			<< "TypeAssign(" << node->type_assign_op->value << ")\n" << std::string(deep, ' ')
			<< "Type(" << node->var_type->value << ")\n";
		deep -= 3;
		return stream;
	}

	// Print Full variable declaration node ( key id: type = expr; )
	std::stringstream visit(FullVarDecl* node, int deep) override {
		std::stringstream stream;
		stream << "FullVarDeclNode ->\n";
		deep += 3;
		stream << std::string(deep, ' ')
			<< node->declaration->handler(this, deep).str() << std::string(deep, ' ')
			<< "Assign(" << node->assign->value << ")\n" << std::string(deep, ' ')
			<< node->expr->handler(this, deep).str() << "\n";
		deep -= 3;
		return stream;
	}

	// Print variable reasigment node ( id = expr; || id [+ - * /]= expr; )
	std::stringstream visit(ReasignVar* node, int deep) override {
		std::stringstream stream;
		stream << "ReasignNode ->\n";
		deep += 3;
		stream << std::string(deep, ' ')
			<< node->identifier->handler(this, deep).str() << "\n" << std::string(deep, ' ')
			<< "Op(" << node->assign->value << ")\n" << std::string(deep, ' ')
			<< node->expr->handler(this, deep).str() << "\n";
		deep -= 3;
		return stream;
	}

	// Print list of statement
	std::stringstream visit(BlockOfCode* node, int deep) override {
		std::stringstream stream;
		stream << "StmtListNode ->\n";
		deep += 3;
		for (size_t it = 0; it < node->list.size(); ++it) {
			if (node->list[it]) { stream << std::string(deep, ' ') << node->list[it]->handler(this, deep).str(); }
		}
		deep -= 3;
		return stream;
	}

	// Print if statement node
	std::stringstream visit(If* node, int deep) override {
		std::stringstream stream;
		stream << "IfNode ->\n";
		deep += 3;
		stream << std::string(deep, ' ') << "Condition - >";
		if (node->condition) {
			deep += 3;
			stream << "\n" << std::string(deep, ' ') << node->condition->handler(this, deep).str() << "\n";
			deep -= 3;
		} else { stream << " null"; }
		if (node->code_to_execute) { stream << "\n" << std::string(deep, ' ') << node->code_to_execute->handler(this, deep).str(); }
		deep -= 3;
		return stream;
	}

public:
	void print(AST* ast) { 
		if (ast) { std::cout << ast->handler(this, 0).str() << std::endl; } 
	}
};
#endif // !AST_PRINTER_H