#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <bits/stdc++.h>
#include "treeNode.h"
enum class TACOp
{
    ASSIGN,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    INDEX,
    ARR_INDEX,
    LABEL,
    GOTO,
    IF_EQ,
    IF_NE,
    LSHFT,
    RSHFT,
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE,
    BIT_AND,
    BIT_OR,
    BIT_XOR,
    AND,
    OR,
    XOR,
    PRINT,
    RETURN,
    CALL,
    TYPECAST,
    CALL2,
    oth
};

inline string opToStr(TACOp op)
{
    switch (op)
    {
    case TACOp::ADD:
        return "+";
    case TACOp::SUB:
        return "-";
    case TACOp::MUL:
        return "*";
    case TACOp::DIV:
        return "/";
    case TACOp::MOD:
        return "%";
    case TACOp::LSHFT:
        return "<<";
    case TACOp::RSHFT:
        return ">>";
    case TACOp::LT:
        return "<";
    case TACOp::GT:
        return ">";
    case TACOp::LE:
        return "<=";
    case TACOp::GE:
        return ">=";
    case TACOp::EQ:
        return "==";
    case TACOp::NE:
        return "!=";
    case TACOp::BIT_AND:
        return "&";
    case TACOp::BIT_OR:
        return "|";
    case TACOp::BIT_XOR:
        return "^";
    case TACOp::AND:
        return "&&";
    case TACOp::OR:
        return "||";
    case TACOp::XOR:
        return "^";
    case TACOp::ASSIGN:
        return "=";
    case TACOp::GOTO:
        return "goto";
    case TACOp::IF_EQ:
        return "if ==";
    case TACOp::IF_NE:
        return "if !=";
    case TACOp::TYPECAST:
        return "cast";
    case TACOp::CALL:
        return "call";
    case TACOp::PRINT:
        return "print";
    case TACOp::INDEX:
        return "index";
    case TACOp::ARR_INDEX:
        return "arr_index";
    case TACOp::RETURN:
        return "return";
    case TACOp::LABEL:
        return "label";
    case TACOp::oth:
        return "";
    default:
        return "unknown";
    }
}

struct TACInstruction
{
    TACOp op;
    string result;
    optional<string> operand1;
    optional<string> operand2;
    bool isGoto = false;
    TACInstruction(TACOp operation, const string &res,
                   const optional<string> &op1 = nullopt,
                   const optional<string> &op2 = nullopt,
                   bool isGotoFlag = false)
        : op(operation), result(res), operand1(op1), operand2(op2),
          isGoto(isGotoFlag) {}

    string toString() const
    {
        string str;
        if (isGoto)
        {
            if (op == TACOp::oth)
                str = "goto " + result;
            else
                str = "goto " + result + " if " + *operand1 + " " + opToStr(op) + " " + *operand2;
            return str;
        }
        switch (op)
        {
        case TACOp::ASSIGN:
            str = result + " = " + (operand1 ? *operand1 : "");
            break;
        case TACOp::ADD:
            str = result + " = " + *operand1 + " + " + *operand2;
            break;
        case TACOp::SUB:
            str = result + " = " + *operand1 + " - " + *operand2;
            break;
        case TACOp::MUL:
            str = result + " = " + *operand1 + " * " + *operand2;
            break;
        case TACOp::DIV:
            str = result + " = " + *operand1 + " / " + *operand2;
            break;
        case TACOp::MOD:
            str = result + " = " + *operand1 + " % " + *operand2;
            break;
        case TACOp::LSHFT:
            str = result + " = " + *operand1 + " << " + *operand2;
            break;
        case TACOp::RSHFT:
            str = result + " = " + *operand1 + " >> " + *operand2;
            break;
        case TACOp::LT:
            str = result + " = " + *operand1 + " < " + *operand2;
            break;
        case TACOp::GT:
            str = result + " = " + *operand1 + " > " + *operand2;
            break;
        case TACOp::LE:
            str = result + " = " + *operand1 + " <= " + *operand2;
            break;
        case TACOp::GE:
            str = result + " = " + *operand1 + " >= " + *operand2;
            break;
        case TACOp::EQ:
            str = result + " = " + *operand1 + " == " + *operand2;
            break;
        case TACOp::NE:
            str = result + " = " + *operand1 + " != " + *operand2;
            break;
        case TACOp::BIT_AND:
            str = result + " = " + *operand1 + " & " + *operand2;
            break;
        case TACOp::BIT_OR:
            str = result + " = " + *operand1 + " | " + *operand2;
            break;
        case TACOp::BIT_XOR:
            str = result + " = " + *operand1 + " ^ " + *operand2;
            break;
        case TACOp::AND:
            str = result + " = " + *operand1 + " && " + *operand2;
            break;
        case TACOp::OR:
            str = result + " = " + *operand1 + " || " + *operand2;
            break;
        case TACOp::XOR:
            str = result + " = " + *operand1 + " ^ " + *operand2;
            break;
        case TACOp::PRINT:
