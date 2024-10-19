#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <bits/stdc++.h>
#include "treeNode.h"

// Forward declarations for functions defined in parser.y
void backTrackExpr(TreeNode* nd);
void backTrackRelExpr(TreeNode* nd);

enum class TACOp
{
    ASSIGN,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    INDEX,
    ARRAY_STORE,
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
            str = "print " + result;
            break;
        case TACOp::INDEX:
            str = result + " = " + *operand1 + "[" + *operand2 + "]";
            break;
        case TACOp::ARRAY_STORE:
            str = result + "[" + *operand1 + "] = " + *operand2;
            break;
        case TACOp::LABEL:
            str = result + ":";
            break;
        case TACOp::GOTO:
            str = "goto " + result;
            break;
        case TACOp::IF_EQ:
            str = "if " + *operand1 + " == " + *operand2 + " goto " + result;
            break;
        case TACOp::IF_NE:
            str = "if " + *operand1 + " != " + *operand2 + " goto " + result;
            break;
        case TACOp::RETURN:
            str = "return " + result;
            break;
        case TACOp::CALL:
            str = result + " = call " + *operand1 + "(" + (operand2 ? *operand2 : "") + ")";
            break;
        case TACOp::CALL2:
            str = "call " + *operand1 + "(" + (operand2 ? *operand2 : "") + ")";
            break;
        case TACOp::TYPECAST:
            str = result + " = " + *operand1 + "(" + *operand2 + ")";
            break;
        case TACOp::oth:
        {
            str = "goto " + result + " " + *operand1 + " " + *operand2;
        }
        default:
            str = "Unknown";
            break;
        }
        return str;
        ;
    }
};

struct CodeGenerator
{
    vector<TACInstruction> tacCode;
    int tempCounter = 0;
    int labelCounter = 0;
    int currentInstrIndex = 0;
    

    string newTemp()
    {
        return "t" + to_string(tempCounter++);
    }

    string newLabel()
    {
        return "L" + to_string(labelCounter++);
    }
    int emit(TACOp op, const string &result,
             const optional<string> &op1 = nullopt,
             const optional<string> &op2 = nullopt, bool isGotoFlag = false)
    {
        tacCode.emplace_back(op, result, op1, op2, isGotoFlag);
        return currentInstrIndex++;
    }
    
    // Insert instruction at a specific position
    void insertAt(int index, TACOp op, const string &result,
                  const optional<string> &op1 = nullopt,
                  const optional<string> &op2 = nullopt, bool isGotoFlag = false)
    {
        if(index < 0 || index > tacCode.size()) {
            cerr << "Invalid insertion index: " << index << endl;
            return;
        }
        tacCode.insert(tacCode.begin() + index, TACInstruction(op, result, op1, op2, isGotoFlag));
        currentInstrIndex++;
        
        // Update all backpatch indices that are >= index
        // Note: This is a limitation - we'd need to track and update all backpatch lists
        // For now, we'll accept this limitation
    }

    void printTAC()
    {
        for (const auto &instr : tacCode)
            cout << instr.toString() << '\n';
    }
};

CodeGenerator codeGen;
vector<string> staticInitCode;  // Store static variable initialization code

// Helper function to emit INDEX operation for array access if needed
inline void emitIndexForNode(TreeNode* node) {
    if (node && node->isArrayIndex && node->children.size() == 2) {
        // This is an array index that needs INDEX emitted when used as rvalue
        // tacResult already contains array notation like "arr[index]"
        // We need to emit: temp = arr[index]
        string temp = codeGen.newTemp();
        string arrName = node->children[0]->tacResult;
        string indexExpr = node->children[1]->tacResult;
        codeGen.emit(TACOp::INDEX, temp, arrName, indexExpr);
        // Update tacResult to the temp for subsequent use
        node->tacResult = temp;
    }
}

// Changed from linked list to vector for O(1) merge instead of O(n)
using BackpatchList = vector<int>;

class Backpatch
{
public:
    static BackpatchList makeList(int index)
    {
        return {index};
    }

    static void backpatch(const BackpatchList &list, const string &label)
    {
        for (int idx : list)
        {
            if (idx >= 0 && idx < codeGen.tacCode.size())
            {
                codeGen.tacCode[idx].result = label;
            }
        }
    }

    static BackpatchList merge(const BackpatchList &list1, const BackpatchList &list2)
    {
        BackpatchList result = list1;
        result.insert(result.end(), list2.begin(), list2.end());
        return result;
    }
};

int expectedReturnType = -1;
extern unordered_set<string> classOrStructOrUnion;

struct Table
{
    vector<pair<string, TreeNode *>> symbolTable;
    Table *parent;
};

stack<Table *> tableStack;
stack<int> offsetStack;
Table *currentTable;
vector<Table *> allTables;

TreeNode *lookupSymbol(string symbol, bool arg = false)
{
    Table *temp = currentTable;
    while (temp != nullptr)
    {
        for (const auto &entry : temp->symbolTable)
        {
            if (entry.first == symbol)
            {
                return entry.second;
            }
        }
        temp = temp->parent;
    }
    if(!arg)
        cerr << "Symbol " << symbol << " not found in table" << endl;
    return nullptr;
}

void enterScope()
{
    Table *newTable = new Table();
    newTable->parent = currentTable;
    currentTable = newTable;
    tableStack.push(newTable);
    offsetStack.push(0);
    allTables.push_back(newTable);
}

void exitScope()
{
    if (currentTable->parent == nullptr)
    {
        cerr << "Cannot exit global scope" << endl;
        return;
    }
    Table *temp = currentTable;
    currentTable = currentTable->parent;
    tableStack.pop();
    offsetStack.pop();
}

void insertSymbol(string symbol, TreeNode *node)
{
    for (const auto &entry : currentTable->symbolTable)
    {
        if (entry.first == symbol)
        {
            cerr << "Symbol " << symbol << " already exists in table" << endl;
            return;
        }
    }
    currentTable->symbolTable.emplace_back(symbol, node);
}

void printAllTables()
{
    int tblId = 0;
    for (auto &tbl : allTables)
    {
        cout << "Tbl " << tblId++ << " (Scope Lvl):\n";
        cout << "-------------------------------------------------------------------------------------------------------\n";
        cout << left << setw(12) << "Id" << setw(12) << "TypeCat" << setw(12) << "TypeSpec"
             << setw(12) << "StorCls" << setw(8) << "Params" << setw(8) << "Const" << setw(8) << "Static"
             << setw(8) << "Volat" << setw(12) << "PtrLvl" << setw(12) << "SymTabSize" << "\n";
        cout << "------------------------------------------------------------------------------------------------------\n";

        for (const auto &entry : tbl->symbolTable)
        {
            TreeNode *node = entry.second;
            cout << left << setw(12) << entry.first
                 << setw(12) << node->typeCategory
                 << setw(12) << node->typeSpecifier
                 << setw(12) << node->storageClass
                 << setw(8) << node->paramCount
                 << setw(8) << (node->isConst ? "Y" : "N")
                 << setw(8) << (node->isStatic ? "Y" : "N")
                 << setw(8) << (node->isVolatile ? "Y" : "N")
                 << setw(12) << node->pointerLevel
                 << setw(12) << node->symbolTable.size()
                 << "\n";
        }

        cout << "------------------------------------------------------------------------------------\n\n";
    }
}

struct DeclaratorInfo
{
    int typeCategory = -1;  // var = 0, func = 1, struct = 2, enum = 3, class = 4
    int storageClass = -1;  // -1: none, -2: typedef, 0: extern, 1: static, 2: auto, 3: register
    int typeSpecifier = -1; // -1: none, void : 0, char : 1, short : 2, 3: int, 4: bool, 5: long, 6: float, 7: double
    int pointerLevel = 0;   // 0: not a pointer, 1+: pointer depth
    bool isConst = false;
    bool isStatic = false;
    bool isVolatile = false;
    bool isUnsigned = false;
    bool hasLong = false;
    bool isValid = false;
    bool isCustomType = false; // true if it's a class, struct, or union
};

string typeCastInfo(int lhs, int rhs)
{
    if (lhs == 1 && rhs == 3)
        return "int_to_char";
    if (lhs == 2 && rhs == 3)
        return "int_to_short";
    if (lhs == 3 && rhs == 4)
        return "long_to_int";
    if (lhs == 1 && rhs == 4)
        return "long_to_char";
    if (lhs == 2 && rhs == 4)
        return "long_to_short";
    if (lhs == 3 && rhs == 6)
        return "float_to_int";
    if (lhs == 6 && rhs == 7)
        return "double_to_float";
    if (lhs == 3 && rhs == 7)
        return "double_to_int";
    if (lhs == 7 && rhs == 6)
        return "float_to_double";
    if (lhs == 6 && rhs == 3)
        return "int_to_float";
    if (lhs == 7 && rhs == 3)
        return "int_to_double";
    if (lhs == 3 && rhs == 1)
        return "char_to_int";
    // Handle pointer type conversions (void* to/from other pointer types)
    // These are no-ops in terms of value - just type compatibility
    if ((lhs == 0 || rhs == 0) && (lhs == 20 || rhs == 20))
        return "ptr_cast";  // void* <-> struct* conversion
    if (lhs == 20 && rhs == 20)
        return "ptr_cast";  // struct* <-> struct* conversion
    return "invalid"; 
}

bool isValidCast(int toType, int fromType)
{
    // Allow all casts between numeric types (1-6, 12-13)
    if (toType >= 1 && toType <= 6 && fromType >= 1 && fromType <= 6)
        return true;
    if ((toType == 12 || toType == 13) && (fromType >= 1 && fromType <= 6))
        return true;
    if ((fromType == 12 || fromType == 13) && (toType >= 1 && toType <= 6))
        return true;
    // Allow casts involving void (0) - for pointer casts like (Node*)malloc()
    if (toType == 0 || fromType == 0)
        return true;
    // Allow casts involving custom types (20) - for struct/union/class pointer casts
    if (toType == 20 || fromType == 20)
        return true;
    // Allow casts with unknown types (-1) - happens with typedefs before resolution
    if (toType == -1 || fromType == -1)
        return true;
    return false;
}

bool checkFormatSpecifiers(string formatString, vector<int> argTypeList)
{
    int argIndex = 0;
    const char *ptr = formatString.c_str();
    while (*ptr)
    {
        if (*ptr == '%')
        {
            ptr++;
            if (*ptr == '\0')
                break;
            if (*ptr == '%')
            { 
                ptr++;
                continue;
            }

            if (argIndex >= argTypeList.size())
            {
                cerr << "Error: Too few arguments\n";
                return false;
            }

            // Skip flags (-+#0 space)
            while (*ptr && (*ptr == '-' || *ptr == '+' || *ptr == '#' || *ptr == '0' || *ptr == ' '))
                ptr++;
            
            // Skip width
            while (*ptr && isdigit(*ptr))
                ptr++;
            
            // Skip precision
            if (*ptr == '.') {
                ptr++;
                while (*ptr && isdigit(*ptr))
                    ptr++;
            }
            
            // Skip length modifiers (h, hh, l, ll, L, z, t)
            if (*ptr == 'h' || *ptr == 'l' || *ptr == 'L' || *ptr == 'z' || *ptr == 't') {
                char lengthMod = *ptr;
                ptr++;
                // Check for double modifiers (hh, ll)
                if ((*ptr == 'h' && lengthMod == 'h') || (*ptr == 'l' && lengthMod == 'l'))
                    ptr++;
            }

            int expectedType1 = -1, expectedType2 = -1;
            switch (*ptr)
            {
            case 'd':
            case 'i':
            case 'u':
            case 'o':
            case 'x':
            case 'X':
                expectedType1 = 3;  // int (accept int for all integer formats)
                break;
            case 'f':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                expectedType1 = 6, expectedType2 = 7;  // float/double
                break; 
            case 's':
                expectedType1 = 1;  // char (char array/string)
                expectedType2 = 8;  // string literal
                break; 
            case 'c':
                expectedType1 = 1;  // char
                break; 
            case 'p':
                expectedType1 = 200;  // pointer (any pointer type)
                break;
            case '%':
                ptr++;
                continue;  // Skip '%%'
            default:
                cerr << "Error: Unknown format specifier '%" << *ptr << "'\n";
                return false;
            }

            // Special handling for pointers (%p can accept any pointer type >= 200)
            if (expectedType1 == 200) {
                if (argTypeList[argIndex] < 200) {
                    cerr << "Error: Type mismatch for '%" << *ptr << "' - expected pointer\n";
                    return false;
                }
            } else if (argTypeList[argIndex] != expectedType1 && argTypeList[argIndex] != expectedType2)
            {
                cerr << "Error: Type mismatch for '%" << *ptr << "'\n";
                return false;
            }
            argIndex++;
        }
        ptr++;
    }

    if (argIndex < argTypeList.size())
    {
        cerr << "Error: Too many arguments\n";
        return false;
    }

    return true;
}

DeclaratorInfo isValidVariableDeclaration(vector<TreeNode *> &nodes, bool isFunction = false)
{
    DeclaratorInfo declInfo;
    unordered_map<string, int> storageClasses = {
        {"typedef", -2}, {"extern", 0}, {"static", 1}, {"auto", 2}, {"register", 3}};
    unordered_map<string, int> baseTypes = {
        {"void", 0}, {"char", 1}, {"short", 2}, {"int", 3}, {"long", 5}, {"float", 6}, {"double", 7}};
    unordered_set<string> typeModifiers = {"signed", "unsigned"};
    unordered_set<string> qualifiers = {"const", "volatile"};

    int storageClassCount = 0, typeSpecifierCount = 0, typeModifierCount = 0, qualifierCount = 0;
    bool hasSignedOrUnsigned = false;

    for (const auto &node : nodes)
    {
        string val = node->valueToString();

        if (node->type == NODE_STORAGE_CLASS_SPECIFIER)
        {
            if (!storageClasses.count(val))
                return {};
            declInfo.storageClass = storageClasses[val];
            if (val == "static")
                declInfo.isStatic = true;
            storageClassCount++;
            if (storageClassCount > 1)
                return {};
        }
        else if (node->type == NODE_TYPE_SPECIFIER)
        {
            // Check if this is a typedef name - look it up and get its type
            extern unordered_set<string> typedefNames;
            if (typedefNames.count(val) > 0) {
                TreeNode* typedefNode = lookupSymbol(val, true);
                if (typedefNode) {
                    if (declInfo.typeSpecifier != -1)
                        return {};
                    declInfo.typeSpecifier = typedefNode->typeSpecifier;
                    declInfo.typeCategory = typedefNode->typeCategory;
                    declInfo.isCustomType = typedefNode->typeCategory == 4;
                    declInfo.pointerLevel = typedefNode->pointerLevel;  // Preserve pointer level
                    typeSpecifierCount++;
                    continue; // Skip further processing for this node
                }
            }
            
            // Check if this is an enum specifier (already has typeSpecifier set)
            if (node->typeSpecifier != -1 && node->typeCategory == 0) {
                // This is an enum, use its typeSpecifier directly
                if (declInfo.typeSpecifier != -1)
                    return {};
                declInfo.typeSpecifier = node->typeSpecifier;
                typeSpecifierCount++;
            }
            // Check if this is a struct/union specifier (already has typeSpecifier set)
            else if (node->typeSpecifier != -1 && node->typeCategory == 4) {
                // This is a struct/union from struct_or_union_specifier rule or typedef lookup
                if (declInfo.typeSpecifier != -1) {
                    cerr << "ERROR: Duplicate type specifier! declInfo.typeSpecifier=" << declInfo.typeSpecifier << ", node->typeSpecifier=" << node->typeSpecifier << endl;
                    return {};
                }
                declInfo.typeSpecifier = node->typeSpecifier;
                declInfo.typeCategory = node->typeCategory;
                declInfo.isCustomType = true;
                typeSpecifierCount++;
                continue; // Skip further processing for this node
            }
            // For struct_or_union_specifier nodes, extract the type name from children
            else if (node->type == NODE_TYPE_SPECIFIER && node->children.size() >= 2) {
                // This might be a struct_or_union_specifier with [struct_or_union, TYPE_NAME]
                string possibleTypeName = node->children[1]->valueToString();
                if (classOrStructOrUnion.count(possibleTypeName)) {
                    val = possibleTypeName;
                }
            }
            
            if (classOrStructOrUnion.count(val))
            {
                if (declInfo.typeSpecifier != -1)
                    return {};
                declInfo.typeCategory = 4;
                declInfo.typeSpecifier = 20;
                declInfo.isCustomType = true;
                typeSpecifierCount++;
            }
            else if (baseTypes.count(val))
            {
                if (declInfo.typeSpecifier != -1)
                    return {};
                declInfo.typeSpecifier = baseTypes[val];
                typeSpecifierCount++;
            }
            else if (typeModifiers.count(val))
            {
                typeModifierCount++;
                if (val == "unsigned")
                    declInfo.isUnsigned = true;
                if (val == "signed")
                    hasSignedOrUnsigned = true;
            }
            else
            {
                // If we already processed this as enum, don't return error
                if (node->typeSpecifier == -1)
                    return {};
            }
        }
        else if (node->type == NODE_TYPE_QUALIFIER)
        {
            if (!qualifiers.count(val))
                return {};
            if (val == "const")
                declInfo.isConst = true;
            if (val == "volatile")
                declInfo.isVolatile = true;
            qualifierCount++;
        }
        else
        {
            return {};
        }
    }

    if (typeSpecifierCount == 0)
        return {};
    if (typeModifierCount > 2)
        return {};

    if (!isFunction && declInfo.typeSpecifier == 0)
        return {};
    declInfo.isValid = true;
    return declInfo;
}

bool isTypeCompatible(int lhstype, int rhstype, string op, bool lhsIsConst = false)
{
    unordered_set<int> integerTypes = {1, 2, 3, 4};
    unordered_set<int> floatingTypes = {6, 7};
    unordered_set<int> numericTypes = integerTypes;
    numericTypes.insert(floatingTypes.begin(), floatingTypes.end());
    unordered_set<int> validTypes = numericTypes;
    validTypes.insert(8);

    bool lhsIsNumeric = numericTypes.count(lhstype);
    bool rhsIsNumeric = numericTypes.count(rhstype);
    bool lhsIsInteger = integerTypes.count(lhstype);
    bool rhsIsInteger = integerTypes.count(rhstype);
    bool lhsIsString = (lhstype == 8);
    bool rhsIsString = (rhstype == 8);
    
    // Allow custom types (structs/unions/classes with typeSpec=20) for assignment
    bool isCustomType = (lhstype == 20 && rhstype == 20);
    
    // Allow void (0) with custom types (20) - for void* parameters accepting any pointer
    bool isVoidAndCustom = ((lhstype == 0 && rhstype == 20) || (lhstype == 20 && rhstype == 0));

    if (lhstype < -1 || rhstype < -1 ||
        ((lhstype > 8 && lhstype != 20) || (rhstype > 8 && rhstype != 20)) ||
        ((lhstype == 0 || rhstype == 0) && !isVoidAndCustom) ||
        (lhstype == 5 || rhstype == 5))
    {
        return false;
    }

    if (lhstype == -1 || rhstype == -1)
    {
        return false;
    }

    if (op == "+" || op == "-" || op == "*" || op == "/")
    {
        return lhsIsNumeric && rhsIsNumeric;
    }
    if (op == "%")
    {
        return lhsIsInteger && rhsIsInteger;
    }

    if (op == "==" || op == "!=")
    {
        return (lhsIsNumeric && rhsIsNumeric) || (lhsIsString && rhsIsString);
    }
    if (op == "<" || op == ">" || op == "<=" || op == ">=")
    {
        return lhsIsNumeric && rhsIsNumeric;
    }

    if (op == "=")
    {
        if (lhsIsConst)
            return false;
        if (lhstype == rhstype)
            return true;
        if (lhsIsNumeric && rhsIsNumeric)
        {

            return true;
        }
        // Allow custom type (struct/union/class pointer) assignment
        if (isCustomType)
            return true;
        // Allow void* with any custom type pointer
        if (isVoidAndCustom)
            return true;
        return false;
    }

    if (op == "+=" || op == "-=" || op == "*=" || op == "/=")
    {
        if (lhsIsConst)
            return false;
        return lhsIsNumeric && rhsIsNumeric;
    }

    if (op == "+=" && lhsIsString && rhsIsString)
    {
        return !lhsIsConst;
    }

    if (op == "^" || op == "&" || op == "|")
    {
        return lhsIsInteger && rhsIsInteger;
    }
    if (op == "^=" || op == "&=" || op == "|=")
    {
        if (lhsIsConst)
            return false;
        return lhsIsInteger && rhsIsInteger;
    }
    if (op == "<<" || op == ">>")
    {
        return lhsIsInteger && rhsIsInteger;
    }
    if (op == "<<=" || op == ">>=")
    {
        if (lhsIsConst)
            return false;
        return lhsIsInteger && rhsIsInteger;
    }

    if (op == "&&" || op == "||")
    {
        return lhsIsNumeric && rhsIsNumeric;
    }

    if (op == "!" || op == "~")
    {
        return (lhstype != 8) && (rhstype == -1 || rhstype == 8);
    }

    if (op == "++" || op == "--")
    {
        if (lhsIsConst)
            return false;
        return lhsIsNumeric && (rhstype == -1 || rhstype == 8);
    }

    return false;
}

bool structInitializerCheck(TreeNode *identifierNode, TreeNode *initializerList)
{
    if (identifierNode->symbolTable.empty())
    {
        cerr << "Error: No struct definition found for type checking" << endl;
        return false;
    }

    size_t expectedSize = identifierNode->symbolTable.size();
    size_t actualSize = initializerList->children.size();

    if (expectedSize != actualSize)
    {
        cerr << "Error: Struct initialization mismatch - expected "
             << expectedSize << " values, got " << actualSize << endl;
        return false;
    }

    for (size_t i = 0; i < expectedSize; i++)
    {
        const auto &memberPair = identifierNode->symbolTable[i];
        TreeNode *memberNode = memberPair.second;
        TreeNode *initNode = initializerList->children[i];

        int expectedType = memberNode->typeSpecifier;
        int actualType = initNode->typeSpecifier;

        bool typesCompatible = isTypeCompatible(expectedType, actualType, "=");

        if (!typesCompatible)
        {
            cerr << "Error: Type mismatch at position " << i + 1
                 << " (member '" << memberPair.first << "'): expected "
                 << expectedType
                 << ", got " << actualType << endl;
            return false;
        }
        else if (expectedType != actualType)
        {
            // Only do typecast if types are different but compatible
            string temp = codeGen.newTemp();
            codeGen.emit(TACOp::TYPECAST, temp, typeCastInfo(expectedType, actualType), initNode->tacResult);
            initNode->tacResult = temp;
        }

        if (memberNode->isConst)
        {
            cerr << "Error: Cannot initialize const member '"
                 << memberPair.first << "' in struct" << endl;
            return false;
        }

        if (memberNode->pointerLevel > 0)
        {
            cerr << "Error: Pointer initialization not supported in struct initializer for '"
                 << memberPair.first << "'" << endl;
            return false;
        }
    }

    return true;
}

// Helper function to extract literal value from initializer expression
string extractLiteralValue(TreeNode* initNode) {
    if (!initNode) return "";
    
    // First check if the node itself is a literal
    if (initNode->type == INTEGER_LITERAL || 
        initNode->type == FLOAT_LITERAL ||
        initNode->type == NODE_CHAR_LITERAL ||
        initNode->type == NODE_STRING_LITERAL ||
        initNode->type == NODE_BOOLEAN_LITERAL) {
        return initNode->valueToString();
    }
    
    // Recursively search through all children to find a literal
    for (auto child : initNode->children) {
        string litValue = extractLiteralValue(child);
        if (!litValue.empty()) {
            return litValue;
        }
    }
    
    // Not a simple literal, return empty string
    return "";
}

void generateStructInitCode(string varName, TreeNode *structDefNode, TreeNode *initializerList)
{
    if (!structDefNode || structDefNode->symbolTable.empty() || !initializerList)
    {
        return;
    }

    size_t memberCount = structDefNode->symbolTable.size();
    size_t initCount = initializerList->children.size();
    
    // Generate member assignments
    for (size_t i = 0; i < min(memberCount, initCount); i++)
    {
        const auto &memberPair = structDefNode->symbolTable[i];
        string memberName = memberPair.first;
        TreeNode *initNode = initializerList->children[i];
        
        if (!initNode->tacResult.empty())
        {
            // Emit: structVar.member = value
            string memberAccess = varName + "." + memberName;
            codeGen.emit(TACOp::ASSIGN, memberAccess, initNode->tacResult, nullopt);
        }
    }
}

vector<int> typeExtract(TreeNode *node)
{
    vector<int> ans;
    for (auto child : node->children)
    {
        ans.push_back(child->typeSpecifier);
    }
    return ans;
}

// Check if scanf arguments are pointers (addresses)
bool checkScanfArguments(string formatString, TreeNode *argList)
{
    vector<int> formatSpecifiers;
    const char *ptr = formatString.c_str();
    
    // Count format specifiers
    while (*ptr)
    {
        if (*ptr == '%')
        {
            ptr++;
            if (*ptr == '\0') break;
            if (*ptr == '%') {
                ptr++;
                continue;  // Skip %%
            }
            
            // Skip width/precision/flags
            while (*ptr && (isdigit(*ptr) || *ptr == '-' || *ptr == '+' || *ptr == ' ' || *ptr == '#' || *ptr == '0' || *ptr == '.'))
                ptr++;
            
            if (*ptr == 'l') ptr++;  // Skip 'l' modifier
            
            if (*ptr && *ptr != '%') {
                formatSpecifiers.push_back(*ptr);
            }
        }
        ptr++;
    }
    
    // Check argument count
    if (formatSpecifiers.size() != argList->children.size()) {
        return false;  // Count mismatch already handled by checkFormatSpecifiers
    }
    
    // Check each argument
    for (size_t i = 0; i < formatSpecifiers.size(); i++)
    {
        TreeNode *arg = argList->children[i];
        char spec = formatSpecifiers[i];
        
        // For %s (string), the argument should be an array (no & needed)
        if (spec == 's') {
            // String arguments are already addresses (array names decay to pointers)
            continue;
        }
        
        // For all other specifiers, the argument must be a pointer (address)
        if (arg->pointerLevel == 0) {
            cerr << "Error: scanf argument " << (i + 1) << " must be an address (use &)" << endl;
            return false;
        }
    }
    
    return true;
}

int inLoop = 0;
bool inFunc = false;
bool insideClass = false;
int accessSpecifier = 0;
stack<bool> inSwitch;
stack<string> switchExpr; // Stack to store switch expression for nested switches
stack<vector<pair<string, string>>> switchCases; // Stack to store (caseValue, caseLabel) for nested switches
stack<string> switchDefaultLabel; // Stack to store default label for nested switches

void GenerateTAC(TreeNode *initList, vector<int> dimensions, int level, string name)
{
    int vecSize = dimensions.size();
    
    // Check if this is a zero-initialization pattern: array = {0}
    // This means: single element initializer with value 0 should zero-fill the entire array
    if (level == 0 && initList->children.size() == 1) {
        TreeNode* firstElem = initList->children[0];
        // Check if it's a zero value (could be integer literal or expression evaluating to 0)
        bool isZeroInit = false;
        if (firstElem->typeSpecifier == 3) {  // int type
            string val = firstElem->tacResult;
            // Check if it's a literal "0" or a temp assigned from 0
            if (val == "0" || firstElem->valueToString() == "0") {
                isZeroInit = true;
            }
        }
        
        if (isZeroInit) {
            // Generate loop to zero-fill entire array
            // Calculate total number of elements
            int totalElements = 1;
            for (int dim : dimensions) {
                if (dim > 0) {
                    totalElements *= dim;
                }
            }
            
            // Generate loop:
            // t_i = 0
            // L_LOOP_START:
            //   goto L_LOOP_BODY if t_i < totalElements
            //   goto L_LOOP_END
            // L_LOOP_BODY:
            //   name[t_i] = 0
            //   t_i = t_i + 1
            //   goto L_LOOP_START
            // L_LOOP_END:
            
            string loopVar = codeGen.newTemp();
            string labelLoopStart = codeGen.newLabel();
            string labelLoopBody = codeGen.newLabel();
            string labelLoopEnd = codeGen.newLabel();
            string tempTotal = codeGen.newTemp();
            string tempZero = codeGen.newTemp();
            string tempOne = codeGen.newTemp();
            
            codeGen.emit(TACOp::ASSIGN, loopVar, "0", nullopt);
            codeGen.emit(TACOp::LABEL, labelLoopStart);
            codeGen.emit(TACOp::ASSIGN, tempTotal, to_string(totalElements), nullopt);
            codeGen.emit(TACOp::LT, labelLoopBody, loopVar, tempTotal, true);
            codeGen.emit(TACOp::GOTO, labelLoopEnd, nullopt, nullopt, false);
            codeGen.emit(TACOp::LABEL, labelLoopBody);
            codeGen.emit(TACOp::ASSIGN, tempZero, "0", nullopt);
            codeGen.emit(TACOp::ARRAY_STORE, name, loopVar, tempZero);
            codeGen.emit(TACOp::ASSIGN, tempOne, "1", nullopt);
            codeGen.emit(TACOp::ADD, loopVar, loopVar, tempOne);
            codeGen.emit(TACOp::GOTO, labelLoopStart, nullopt, nullopt, false);
            codeGen.emit(TACOp::LABEL, labelLoopEnd);
            
            return;  // Done with zero-initialization
        }
    }
    
    // Normal initialization (not zero-fill pattern)
    if (level == vecSize - 1)
    { 
        int i = 0;
        for (TreeNode *child : initList->children)
        {
            string indexedName = name + "[" + to_string(i) + "]";
            codeGen.emit(TACOp::ASSIGN, indexedName, child->tacResult, nullopt);
            i++;
        }
    }
    else
    { 
        int i = 0;
        for (TreeNode *child : initList->children)
        {
            string newName = name + "[" + to_string(i) + "]";
            if (child->type != NODE_INITIALIZER_LIST)
            {
                return;
            }
            GenerateTAC(child, dimensions, level + 1, newName);
            i++;
        }
    }
}

bool checkInitializerLevel(TreeNode *initList, int baseType, vector<int> &dimensions, int level)
{
    int vecSize = dimensions.size();

    if (baseType != 1 && baseType != 3)
    {
        cerr << "Invalid declaration for an array" << endl;
        return false;
    }

    for (int i = 1; i < vecSize; i++)
    {
        if (dimensions[i] == -1)
        { 
            cerr << "Invalid Declaration: dimension " << i << " cannot be unspecified" << endl;
            return false;
        }
    }

    if (level >= vecSize)
    {
        cerr << "Too many nesting levels at level " << level << endl;
        return false;
    }

    if (level == 0 && dimensions[0] == -1)
    {
        dimensions[0] = initList->children.size();
    }

    if (initList->children.size() > dimensions[level])
    {
        cerr << "Dimension mismatch at level " << level << ": expected at most "
             << dimensions[level] << ", got " << initList->children.size() << endl;
        return false;
    }

    if (level == vecSize - 1)
    {
        for (TreeNode *child : initList->children)
        {
            if (child->typeSpecifier != baseType)
            {
                cerr << "Type mismatch at level " << level << ": expected "
                     << baseType << ", got " << child->typeSpecifier << endl;
                return false;
            }
        }
    }
    else
    {
        for (TreeNode *child : initList->children)
        {
            if (child->type != NODE_INITIALIZER_LIST)
            {
                cerr << "Expected nested initializer list at level " << level << endl;
                return false;
            }
            if (!checkInitializerLevel(child, baseType, dimensions, level + 1))
            {
                return false;
            }
        }
    }
    // Note: Code generation is handled separately in addDeclarators
    // Don't generate code during validation
    return true;
}

vector<int> findArrayDimensions(TreeNode *arr)
{
    if (!arr || arr->children.empty())
        return {};

    vector<int> dimensions;
    TreeNode *current = arr;

    while (current)
    {
        if (current->type == ARRAY)
        {
            if (current->children.size() > 1 && current->children[1] &&
                current->children[1]->type == INTEGER_LITERAL)
            {
                dimensions.push_back(stoi(current->children[1]->valueToString()));
            }
            else
            {
                dimensions.push_back(-1);
            }
        }
        current = (!current->children.empty()) ? current->children[0] : nullptr;
    }
    reverse(dimensions.begin(), dimensions.end());
    return dimensions;
}

void addDeclarators(TreeNode *specifier, TreeNode *list)
{
    DeclaratorInfo declInfo = isValidVariableDeclaration(specifier->children, false);
    if (!declInfo.isValid)
        return;
        
    if (declInfo.isValid)
    {
        // Check if this is a typedef declaration
        bool isTypedef = (declInfo.storageClass == -2);
        
        auto helper = specifier;
        for (auto child : specifier->children)
        {
            if (child->type == NODE_TYPE_SPECIFIER)
            {
                helper = child;
                break;
            }
        }
        // Look up typedef or struct type definition to get symbol table (for arrow operator access)
        // This applies to both struct types (typeCategory==4) and pointers to structs (typeCategory==1, typeSpecifier==20)
        if (declInfo.typeCategory == 4 || (declInfo.typeSpecifier == 20 && declInfo.isCustomType)) {
            string typeName;
            
            // The helper might be NODE_TYPE_SPECIFIER wrapping NODE_STRUCT_OR_UNION_SPECIFIER
            // Check if it has children that indicate it's a struct
            if (helper->children.size() >= 2) {
                // struct_or_union_specifier has [struct_or_union, TYPE_NAME, optional members]
                typeName = helper->children[1]->valueToString();
            } else if (helper->children.size() == 1 && helper->children[0]->type == NODE_IDENTIFIER) {
                // Could be a typedef or forward reference
                typeName = helper->children[0]->valueToString();
            } else {
                typeName = helper->valueToString();
            }
            
            if (!typeName.empty()) {
                TreeNode* foundNode = lookupSymbol(typeName, true); // Don't print error here
                if (foundNode) {
                    helper = foundNode;
                    // If this is a typedef, we need to look up what it refers to
                    extern unordered_set<string> typedefNames;
                    if (typedefNames.count(typeName) > 0 && helper->typeCategory == 4) {
                        // This typedef refers to a struct - we already have the right node
                        // The symbolTable should already be set on this node
                    }
                }
            }
        }
        for (auto child : list->children)
        {
            if (child->type != NODE_DECLARATOR) {
                continue;
            }
            
            
            if (child->children.empty()) {
                continue;  // Safety check
            }

            TreeNode *firstChild = child->children[0];
            
            if (!firstChild) {
                continue;  // Safety check for null pointer
            }
            
            // Check if this is a function declarator (has parameters)
            bool isFunctionDeclarator = false;
            std::function<bool(TreeNode*)> hasParameterList = [&](TreeNode* node) -> bool {
                if (!node) return false;
                if (node->type == NODE_PARAMETER_LIST) return true;
                for (auto c : node->children) {
                    if (hasParameterList(c)) return true;
                }
                return false;
            };
            
            if (hasParameterList(firstChild)) {
                // This is a function declaration/prototype
                // Handle pointer declarators to count return type pointer level
                int returnPointerLevel = 0;
                TreeNode* actualDeclarator = firstChild;
                while (actualDeclarator && actualDeclarator->type == NODE_POINTER) {
                    returnPointerLevel++;
                    if (!actualDeclarator->children.empty()) {
                        actualDeclarator = actualDeclarator->children[0];
                    } else {
                        break;
                    }
                }
                
                // Extract function name and parameter list
                TreeNode* funcNode = actualDeclarator;
                while (funcNode && funcNode->type != NODE_IDENTIFIER && !funcNode->children.empty()) {
                    funcNode = funcNode->children[0];
                }
                
                if (funcNode && funcNode->type == NODE_IDENTIFIER) {
                    string funcName = funcNode->valueToString();
                    // Check for duplicate
                    bool isDuplicate = false;
                    for (const auto &entry : currentTable->symbolTable) {
                        if (entry.first == funcName) {
                            isDuplicate = true;
                            break;
                        }
                    }
                    if (!isDuplicate) {
                        funcNode->typeCategory = 3;  // Function
                        funcNode->typeSpecifier = declInfo.typeSpecifier;
                        funcNode->pointerLevel = returnPointerLevel;
                        funcNode->storageClass = declInfo.storageClass;
                        funcNode->isStatic = declInfo.isStatic;
                        funcNode->isConst = declInfo.isConst;
                        
                        // Extract parameter information
                        if (actualDeclarator->children.size() > 1 && 
                            actualDeclarator->children[1]->type == NODE_PARAMETER_LIST) {
                            TreeNode* paramList = actualDeclarator->children[1];
                            funcNode->paramCount = paramList->children.size();
                            
                            for (auto param : paramList->children) {
                                if (param->type == NODE_PARAMETER_DECLARATION && param->children.size() >= 2) {
                                    TreeNode* paramDeclSpec = param->children[0];
                                    TreeNode* paramDeclarator = param->children[1];
                                    
                                    // Get parameter type
                                    DeclaratorInfo paramInfo = isValidVariableDeclaration(paramDeclSpec->children, true);
                                    funcNode->paramTypes.push_back(paramInfo.typeSpecifier);
                                    
                                    // Count pointer levels for parameter
                                    int paramPointerLevel = 0;
                                    TreeNode* temp = paramDeclarator;
                                    while (temp && temp->type == NODE_POINTER) {
                                        paramPointerLevel++;
                                        if (!temp->children.empty()) {
                                            temp = temp->children[0];
                                        } else {
                                            break;
                                        }
                                    }
                                    // Check for array parameter (treated as pointer)
                                    if (temp && temp->type == ARRAY) {
                                        paramPointerLevel++;
                                    }
                                    funcNode->paramPointerLevels.push_back(paramPointerLevel);
                                }
                            }
                        }
                        
                        insertSymbol(funcName, funcNode);
                    }
                }
                continue;  // Skip further processing
            }
            
            string varName;
            TreeNode *identifierNode = firstChild;
            auto setNodeAttributes = [&](TreeNode *node, int typeCategory, int pointerLevel = 0)
            {
                node->typeCategory = typeCategory;
                // Add typedef pointer level to the declarator's pointer level
                node->pointerLevel = pointerLevel + declInfo.pointerLevel;
                node->storageClass = declInfo.storageClass;
                node->typeSpecifier = declInfo.typeSpecifier;
                node->isConst = declInfo.isConst;
                node->isStatic = declInfo.isStatic;
                node->isVolatile = declInfo.isVolatile;
                node->isUnsigned = declInfo.isUnsigned;
                
                // Copy symbol table from helper (for structs)
                // If helper doesn't have symbol table but is a struct/union type,
                // try to look up the type to get the symbol table
                if (helper->symbolTable.size() > 0) {
                    node->symbolTable = helper->symbolTable;
                } else if (declInfo.typeSpecifier == 20 && declInfo.isCustomType) {
                    // This is a struct/union type but helper doesn't have symbol table
                    // Look for the type in the declaration specifiers
                    extern unordered_set<string> typedefNames;
                    extern unordered_set<string> classOrStructOrUnion;
                    for (auto child : specifier->children) {
                        if (child->type == NODE_TYPE_SPECIFIER) {
                            string typeName = child->valueToString();
                            // Look up typedef or struct name
                            if (typedefNames.count(typeName) > 0 || classOrStructOrUnion.count(typeName) > 0) {
                                TreeNode* typeNode = lookupSymbol(typeName, true);
                                if (typeNode && typeNode->symbolTable.size() > 0) {
                                    node->symbolTable = typeNode->symbolTable;
                                    break;
                                }
                            }
                        }
                    }
                }
            };
            auto checkDuplicate = [&](const string &name)
            {
                for (const auto &entry : currentTable->symbolTable)
                {
                    if (entry.first == name)
                    {
                        cerr << "Error: Duplicate declaration of '" << name << "'\n";
                        return true;
                    }
                }
                return false;
            };

            if (firstChild->type == ARRAY)
            {
                vector<int> dimensions = findArrayDimensions(firstChild);
                while (identifierNode && identifierNode->type == ARRAY)
                {
                    if (identifierNode->children.empty())
                        break;
                    identifierNode = identifierNode->children[0];
                }
                varName = identifierNode->valueToString();
                if (checkDuplicate(varName))
                    continue;

                // Register typedef name if this is a typedef declaration
                if (isTypedef) {
                    extern void registerTypedefName(const string&);
                    registerTypedefName(varName);
                    // For typedef of array, insert with array info
                    setNodeAttributes(identifierNode, 2);
                    identifierNode->dimensions = dimensions;
                    insertSymbol(varName, identifierNode);
                    continue;  // Skip further processing for typedef
                }

                int size = child->children.size();
                if (size == 1 || size == 2)
                {
                    // If array size is not specified but initializer is provided, infer size
                    if (size == 2 && dimensions.size() > 0 && dimensions[0] == -1) {
                        TreeNode* initializer = child->children[1];
                        if (initializer->type == NODE_INITIALIZER_LIST) {
                            // Infer array size from initializer count
                            dimensions[0] = initializer->children.size();
                        } else if (initializer->type == NODE_STRING_LITERAL || initializer->typeSpecifier == 8) {
                            // Infer array size from string literal: char arr[] = "string"
                            string strValue = initializer->valueToString();
                            // Remove quotes from string literal
                            if (strValue.length() >= 2 && strValue[0] == '"' && strValue[strValue.length()-1] == '"') {
                                dimensions[0] = strValue.length() - 2 + 1; // -2 for quotes, +1 for null terminator
                            } else {
                                dimensions[0] = strValue.length() + 1;
                            }
                        }
                    }
                    
                    bool validDims = all_of(dimensions.begin(), dimensions.end(), [](int d)
                                            { return d != -1; });
                    if (!validDims)
                    {
                        cerr << "Invalid declaration dimension cannot be empty\n";
                        continue;
                    }
                    if (size == 2 && !checkInitializerLevel(child->children[1], declInfo.typeSpecifier, dimensions, 0))
                    {
                        cerr << "Error\n";
                        continue;
                    }
                    
                    // Generate code for array initialization
                    if (size == 2) {
                        TreeNode* initializer = child->children[1];
                        // Check if it's an initializer list (braced init)
                        if (initializer->type == NODE_INITIALIZER_LIST) {
                            // Use GenerateTAC for multi-dimensional array support
                            GenerateTAC(initializer, dimensions, 0, varName);
                        } else if (initializer->type == NODE_STRING_LITERAL || 
                                   (initializer->typeSpecifier == 8 && dimensions.size() == 1)) {
                            // String literal initialization for char array: char arr[] = "string"
                            // Emit: arr = "string" (the backend should handle copying)
                            codeGen.emit(TACOp::ASSIGN, varName, initializer->tacResult, nullopt);
                        }
                    }
                    
                    setNodeAttributes(identifierNode, 2); 
                    identifierNode->dimensions = dimensions;
                    insertSymbol(varName, identifierNode);
                }
            }
            else if (firstChild->type == NODE_POINTER) 
            {
                int pointerDepth = 0;
                bool isRefToPointer = false;
                while (identifierNode && identifierNode->type == NODE_POINTER)
                {
                    pointerDepth++;
                    if (identifierNode->children.empty())
                        break;
                    identifierNode = identifierNode->children[0];
                }
                
                // Check if we have a reference to pointer (int* &ptr)
                if (identifierNode && identifierNode->type == NODE_REFERENCE)
                {
                    isRefToPointer = true;
                    if (!identifierNode->children.empty())
                    {
                        identifierNode = identifierNode->children[0];
                    }
                }
                
                varName = identifierNode->valueToString();

                if (identifierNode->type == ARRAY) 
                {
                    vector<int> dimensions = findArrayDimensions(identifierNode);
                    varName = identifierNode->children[0]->valueToString();
                    if (checkDuplicate(varName))
                        continue;

                    int size = child->children.size();
                    if (size == 1 || size == 2)
                    {
                        bool validDims = all_of(dimensions.begin(), dimensions.end(), [](int d)
                                                { return d != -1; });
                        if (!validDims)
                        {
                            cerr << "Invalid declaration dimension cannot be empty\n";
                            continue;
                        }
                        if (size == 2 && !checkInitializerLevel(child->children[1], declInfo.typeSpecifier, dimensions, pointerDepth))
                        {
                            cerr << "Error: Invalid initializer for array of pointers '" << varName << "'\n";
                            continue;
                        }
                        setNodeAttributes(identifierNode, 2, pointerDepth);
                        identifierNode->dimensions = dimensions;
                        insertSymbol(varName, identifierNode);
                    }
                }
                else 
                {
                    if (checkDuplicate(varName))
                        continue;
                    
                    // Register typedef name if this is a typedef declaration
                    if (isTypedef) {
                        extern void registerTypedefName(const string&);
                        registerTypedefName(varName);
                        // For typedef, also insert with pointer info into symbol table
                        setNodeAttributes(identifierNode, 1, pointerDepth);
                        insertSymbol(varName, identifierNode);
                        continue;  // Skip further processing for typedef
                    }
                    
                    int size = child->children.size();
                    
                    // Handle reference to pointer (int* &ptr)
                    if (isRefToPointer)
                    {
                        if (size == 1)
                        {
                            cerr << "Error: Reference to pointer '" << varName << "' must be initialized\n";
                            continue;
                        }
                        // Set as reference with pointer level
                        setNodeAttributes(identifierNode, 8, pointerDepth);  // typeCategory 8 = reference
                        identifierNode->isReference = true;
                        insertSymbol(varName, identifierNode);
                        // Treat reference as alias
                        codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                    }
                    else if (size == 1)
                    {
                        setNodeAttributes(identifierNode, 1, pointerDepth);
                        if (declInfo.isStatic) {
                            // Static variables are initialized to zero in .static_init section
                            staticInitCode.push_back(varName + " = 0");
                        }
                        insertSymbol(varName, identifierNode);
                    }
                    else
                    {
                        int lhsPointerlevel = pointerDepth;  // Use calculated pointerDepth instead of node attribute
                        int rhsPointerlevel = child->children[1]->pointerLevel;
                        
                        // Convert boolean expression to value before using
                        backTrackExpr(child->children[1]);
                        
                        // Allow NULL (integer 0) to be assigned to pointers
                        // Check if RHS is 0 (NULL) - could be wrapped in expression nodes
                        bool isNullAssignment = (lhsPointerlevel > 0 && rhsPointerlevel == 0 && 
                                                child->children[1]->typeSpecifier == 3 && 
                                                (child->children[1]->tacResult == "0" || child->children[1]->valueToString() == "0"));
                        
                        // Check if RHS is void* (from malloc/calloc/realloc) - can assign to any pointer type
                        bool isVoidPtrAssignment = (lhsPointerlevel > 0 && rhsPointerlevel > 0 && 
                                                   child->children[1]->typeSpecifier == 0);  // void = 0
                        
                        if (lhsPointerlevel != rhsPointerlevel && !(declInfo.typeSpecifier == 3 && child->children[1]->typeCategory == 2) && !isNullAssignment && !isVoidPtrAssignment)
                        {
                            cerr << "Error: Invalid pointer initialization for '" << varName << "'\n";
                            // Still insert symbol to avoid crashes on subsequent uses
                            setNodeAttributes(identifierNode, 1, pointerDepth);
                            insertSymbol(varName, identifierNode);
                        }
                        else
                        {
                            if (isNullAssignment)
                            {
                                // NULL assignment to pointer
                                setNodeAttributes(identifierNode, 1, pointerDepth);
                                insertSymbol(varName, identifierNode);
                                codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                            }
                            else if (isVoidPtrAssignment)
                            {
                                // void* assignment (malloc/calloc/realloc) to typed pointer
                                setNodeAttributes(identifierNode, 1, pointerDepth);
                                insertSymbol(varName, identifierNode);
                                if (declInfo.isStatic) {
                                    // Static pointers to dynamic memory: store the call result
                                    staticInitCode.push_back(varName + " = " + child->children[1]->tacResult);
                                } else {
                                    emitIndexForNode(child->children[1]);  // Emit INDEX if RHS is array access
                                    codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                                }
                            }
                            else if (declInfo.typeSpecifier == 3 && child->children[1]->typeCategory == 2)
                            {
                                setNodeAttributes(identifierNode, 1, pointerDepth);
                                insertSymbol(varName, identifierNode);
                                // For static variables, emit to static init section
                                if (declInfo.isStatic) {
                                    // Extract literal value for static initialization
                                    string litValue = extractLiteralValue(child->children[1]);
                                    string initValue = litValue.empty() ? child->children[1]->tacResult : litValue;
                                    staticInitCode.push_back(varName + " = " + initValue);
                                } else {
                                    emitIndexForNode(child->children[1]);  // Emit INDEX if RHS is array access
                                    codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                                }
                            }
                            else if (isTypeCompatible(declInfo.typeSpecifier, child->children[1]->typeSpecifier, "="))
                            {
                                if (declInfo.typeSpecifier != child->children[1]->typeSpecifier)
                                {
                                    emitIndexForNode(child->children[1]);  // Emit INDEX before typecast
                                    string temp = codeGen.newTemp();
                                    codeGen.emit(TACOp::TYPECAST, temp, typeCastInfo(declInfo.typeSpecifier, child->children[1]->typeSpecifier), child->children[1]->tacResult);
                                    child->children[1]->tacResult = temp;
                                }
                                setNodeAttributes(identifierNode, 1, pointerDepth);
                                insertSymbol(varName, identifierNode);
                                // For static variables, emit to static init section
                                if (declInfo.isStatic) {
                                    // Extract literal value for static initialization
                                    string litValue = extractLiteralValue(child->children[1]);
                                    string initValue = litValue.empty() ? child->children[1]->tacResult : litValue;
                                    staticInitCode.push_back(varName + " = " + initValue);
                                } else {
                                    emitIndexForNode(child->children[1]);  // Emit INDEX if RHS is array access
                                    codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                                }
                            }
                            else
                            {
                                cerr << "Error: Cannot initialize variable '" << varName 
                                     << "' of type " << declInfo.typeSpecifier 
                                     << " with value of type " << child->children[1]->typeSpecifier << endl;
                            }
                        }
                    }
                }
            }
            else if (firstChild->type == NODE_REFERENCE)
            {
                // Handle C++ reference declarations: int &ref = x;
                // References must be initialized and act as aliases
                identifierNode = firstChild;
                if (!identifierNode->children.empty()) {
                    identifierNode = identifierNode->children[0];
                }
                
                if (!identifierNode || identifierNode->type != NODE_IDENTIFIER) {
                    cerr << "Error: Invalid reference declaration\n";
                    continue;
                }
                
                varName = identifierNode->valueToString();
                
                if (checkDuplicate(varName))
                    continue;
                
                // Register typedef name if this is a typedef declaration
                if (isTypedef) {
                    extern void registerTypedefName(const string&);
                    registerTypedefName(varName);
                    // For typedef of reference, insert with reference info
                    setNodeAttributes(identifierNode, 8);  // typeCategory 8 = reference
                    identifierNode->isReference = true;
                    insertSymbol(varName, identifierNode);
                    continue;  // Skip further processing for typedef
                }
                
                int size = child->children.size();
                if (size == 1)
                {
                    cerr << "Error: Reference '" << varName << "' must be initialized\n";
                    continue;
                }
                else
                {
                    // References must be initialized with an lvalue
                    setNodeAttributes(identifierNode, 8);  // typeCategory 8 = reference
                    identifierNode->isReference = true;
                    insertSymbol(varName, identifierNode);
                    
                    // For 3AC, treat reference as an alias (assign address or value depending on usage)
                    codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                }
            }
            else
            {
                varName = firstChild->valueToString();
                if (checkDuplicate(varName))
                    continue;
                
                // Register typedef name if this is a typedef declaration
                if (isTypedef) {
                    extern void registerTypedefName(const string&);
                    registerTypedefName(varName);
                    // For typedef, set the correct type category and copy symbol table if it's a struct
                    setNodeAttributes(identifierNode, declInfo.typeCategory);
                    if (declInfo.typeCategory == 4 && helper && !helper->symbolTable.empty()) {
                        // This is a typedef for a struct - copy the symbol table
                        identifierNode->symbolTable = helper->symbolTable;
                    }
                    if (declInfo.isStatic) {
                        // Static variables are initialized to zero in .static_init section
                        staticInitCode.push_back(varName + " = 0");
                    }
                    insertSymbol(varName, identifierNode);
                }
                else
                {
                    // Normal variable or function
                    int size = child->children.size();
                    if (size == 1)
                    {
                        setNodeAttributes(identifierNode, declInfo.typeCategory);
                        // If this is a struct/union/class, copy the symbol table
                        if (declInfo.typeCategory == 4 && helper && !helper->symbolTable.empty()) {
                            identifierNode->symbolTable = helper->symbolTable;
                        }
                        if (declInfo.isStatic) {
                            // Static variables are initialized to zero in .static_init section
                            staticInitCode.push_back(varName + " = 0");
                        }
                        insertSymbol(varName, identifierNode);
                    }
                    else
                    {
                        // Use declInfo.pointerLevel which includes typedef pointer level
                        int lhsPointerLevel = declInfo.pointerLevel;
                        int rhsPointerLevel = child->children[1]->pointerLevel;
                        
                        // Convert boolean expression to value before using
                        backTrackExpr(child->children[1]);
                        
                        // Allow NULL (integer 0) to be assigned to pointers
                        // Check if RHS is 0 (NULL) - could be wrapped in expression nodes  
                        bool isNullAssignment = (lhsPointerLevel > 0 && rhsPointerLevel == 0 && 
                                                child->children[1]->typeSpecifier == 3 && 
                                                (child->children[1]->tacResult == "0" || child->children[1]->valueToString() == "0"));
                        
                        if (rhsPointerLevel != lhsPointerLevel && !isNullAssignment)
                        {
                            cerr << "Error: Invalid pointer initialization for '" << varName << "'\n";
                            // Still insert symbol to avoid crashes on subsequent uses
                            setNodeAttributes(identifierNode, 0);
                            insertSymbol(varName, identifierNode);
                        }
                        else
                        {
                            if (size == 2 && declInfo.typeSpecifier == 20 && lhsPointerLevel == 0 && rhsPointerLevel == 0)
                            {
                                // Struct/union/class initialization (not pointer)
                                if (structInitializerCheck(helper, child->children[1]))
                                {
                                    setNodeAttributes(identifierNode, declInfo.typeCategory);
                                    // Copy the symbol table from the struct definition
                                    if (helper && !helper->symbolTable.empty()) {
                                        identifierNode->symbolTable = helper->symbolTable;
                                    }
                                    insertSymbol(varName, identifierNode);
                                    
                                    // Generate initialization code
                                    if (child->children[1]->type == NODE_INITIALIZER_LIST) {
                                        generateStructInitCode(varName, helper, child->children[1]);
                                    }
                                }
                            }
                            else if (size == 2 && isNullAssignment)
                            {
                                // NULL assignment to pointer
                                setNodeAttributes(identifierNode, 0);
                                insertSymbol(varName, identifierNode);
                                codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                            }
                            else if (size == 2 && isTypeCompatible(declInfo.typeSpecifier, child->children[1]->typeSpecifier, "="))
                            {
                                if (declInfo.typeSpecifier != child->children[1]->typeSpecifier)
                                {
                                    emitIndexForNode(child->children[1]);  // Emit INDEX before typecast
                                    string temp = codeGen.newTemp();
                                    codeGen.emit(TACOp::TYPECAST, temp, typeCastInfo(declInfo.typeSpecifier, child->children[1]->typeSpecifier), child->children[1]->tacResult);
                                    child->children[1]->tacResult = temp;
                                }
                                setNodeAttributes(identifierNode, 0);
                                insertSymbol(varName, identifierNode);
                                
                                if (declInfo.isStatic) {
                                    // Static variables are initialized only once in .static_init section
                                    // Extract the literal value for initialization
                                    string litValue = extractLiteralValue(child->children[1]);
                                    string initValue = litValue.empty() ? child->children[1]->tacResult : litValue;
                                    staticInitCode.push_back(varName + " = " + initValue);
                                    // Do NOT emit assignment to TAC - static vars are initialized once, not on every call
                                } else {
                                    emitIndexForNode(child->children[1]);  // Emit INDEX if RHS is array access
                                    codeGen.emit(TACOp::ASSIGN, varName, child->children[1]->tacResult, nullopt);
                                }
                            }
                            else if (size == 2)
                            {
                                cerr << "Error: Cannot initialize variable '" << varName 
                                     << "' of type " << declInfo.typeSpecifier 
                                     << " with value of type " << child->children[1]->typeSpecifier << endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

void printSymbolTables() {
    cout << "\n========================================" << endl;
    cout << "SYMBOL TABLE DUMP" << endl;
    cout << "========================================\n" << endl;
    
    extern unordered_set<string> typedefNames;
    
    cout << "Typedef Names Registered in Lexer:" << endl;
    cout << "-----------------------------------" << endl;
    if (typedefNames.empty()) {
        cout << "  (none)" << endl;
    } else {
        for (const auto& name : typedefNames) {
            cout << "  • " << name << endl;
        }
    }
    cout << endl;
    
    int tableNum = 0;
    for (Table* table : allTables) {
        cout << "Scope " << tableNum++ << ":" << endl;
        cout << "-----------------------------------" << endl;
        
        if (table->symbolTable.empty()) {
            cout << "  (empty scope)" << endl;
        } else {
            for (const auto& entry : table->symbolTable) {
                TreeNode* node = entry.second;
                cout << "  Symbol: " << entry.first << endl;
                cout << "    Type Category: ";
                switch(node->typeCategory) {
                    case 0: cout << "variable"; break;
                    case 1: cout << "pointer"; break;
                    case 2: cout << "array"; break;
                    case 3: cout << "function"; break;
                    case 4: cout << "struct"; break;
                    case 5: cout << "union"; break;
                    case 6: cout << "class"; break;
                    case 7: cout << "label"; break;
                    case 8: cout << "reference"; break;
                    default: cout << "unknown(" << node->typeCategory << ")"; break;
                }
                cout << endl;
                
                cout << "    Type Specifier: ";
                switch(node->typeSpecifier) {
                    case 0: cout << "void"; break;
                    case 1: cout << "char"; break;
                    case 2: cout << "short"; break;
                    case 3: cout << "int"; break;
                    case 4: cout << "bool"; break;
                    case 5: cout << "long"; break;
                    case 6: cout << "float"; break;
                    case 7: cout << "double"; break;
                    case 20: cout << "custom (struct/union/class)"; break;
                    default: cout << "(" << node->typeSpecifier << ")"; break;
                }
                cout << endl;
                
                if (node->pointerLevel > 0) {
                    cout << "    Pointer Level: " << node->pointerLevel << endl;
                }
                
                if (node->typeCategory == 2 && !node->dimensions.empty()) {
                    cout << "    Array Dimensions: [";
                    for (size_t i = 0; i < node->dimensions.size(); i++) {
                        cout << node->dimensions[i];
                        if (i < node->dimensions.size() - 1) cout << "][";
                    }
                    cout << "]" << endl;
                }
                
                cout << "    Storage Class: ";
                switch(node->storageClass) {
                    case -2: cout << "typedef"; break;
                    case -1: cout << "none"; break;
                    case 0: cout << "extern"; break;
                    case 1: cout << "static"; break;
                    case 2: cout << "auto"; break;
                    case 3: cout << "register"; break;
                    default: cout << "(" << node->storageClass << ")"; break;
                }
                cout << endl;
                
                if (node->isConst) cout << "    Const: yes" << endl;
                if (node->isStatic) cout << "    Static: yes" << endl;
                if (node->isVolatile) cout << "    Volatile: yes" << endl;
                if (node->isReference) cout << "    Reference: yes" << endl;
                if (node->isConstant) cout << "    Enum Constant: " << node->constantValue << endl;
                
                cout << endl;
            }
        }
        cout << endl;
    }
    cout << "========================================\n" << endl;
}


#endif// Function TAC generation
// Struct access TAC
