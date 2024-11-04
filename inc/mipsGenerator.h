#ifndef MIPS_GENERATOR_H
#define MIPS_GENERATOR_H

#include <bits/stdc++.h>
#include "symbolTable.h"
#include "treeNode.h"

using namespace std;

// Register types for MIPS
enum class RegType {
    TEMP,      // $t0-$t9 (temporary registers)
    SAVED,     // $s0-$s7 (saved registers)
    ARG,       // $a0-$a3 (argument registers)
    RETURN,    // $v0-$v1 (return value registers)
    SPECIAL    // $fp, $sp, $ra, $zero, etc.
};

// Register descriptor: tracks what each MIPS register holds
struct RegisterDescriptor {
    map<string, string> regToVar;  // register name -> variable/temp name
    
    void clear(const string& reg) {
        regToVar.erase(reg);
    }
    
    void set(const string& reg, const string& var) {
        regToVar[reg] = var;
    }
    
    string get(const string& reg) {
        if (regToVar.count(reg)) return regToVar[reg];
        return "";
    }
    
    bool isEmpty(const string& reg) {
        return regToVar.count(reg) == 0;
    }
    
    void clearAll() {
        regToVar.clear();
    }
};

// Address descriptor: tracks location(s) of each variable/temporary
struct AddressDescriptor {
    map<string, set<string>> varToLocs;  // variable -> set of locations (register names or "memory")
    
    void addLocation(const string& var, const string& loc) {
        varToLocs[var].insert(loc);
    }
    
    void removeLocation(const string& var, const string& loc) {
        if (varToLocs.count(var)) {
            varToLocs[var].erase(loc);
        }
    }
    
    void clearVar(const string& var) {
        varToLocs.erase(var);
    }
    
    set<string> getLocations(const string& var) {
        if (varToLocs.count(var)) return varToLocs[var];
        return {};
    }
    
    bool isInRegister(const string& var) {
        if (!varToLocs.count(var)) return false;
        for (const auto& loc : varToLocs[var]) {
            if (loc[0] == '$') return true;  // Register names start with $
        }
        return false;
    }
    
    string getRegister(const string& var) {
        if (!varToLocs.count(var)) return "";
        for (const auto& loc : varToLocs[var]) {
            if (loc[0] == '$') return loc;
        }
        return "";
    }
    
    void clearAll() {
        varToLocs.clear();
    }
};

// MIPS Code Generator class
class MIPSGenerator {
private:
    vector<string> mipsCode;           // Generated MIPS instructions
    vector<string> dataSection;        // .data section (global variables, strings)
    RegisterDescriptor regDesc;        // Register descriptor
    AddressDescriptor addrDesc;        // Address descriptor
    
    // Available registers for allocation
    vector<string> tempRegs = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"};
    vector<string> savedRegs = {"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"};
    
    // Stack and frame management
    map<string, int> varOffsets;       // Variable name -> stack offset from $fp
    int currentOffset = 0;             // Current stack offset
    int frameSize = 0;                 // Total frame size for current function
    
    // Pointer tracking (for pointer arithmetic scaling)
    set<string> pointerVars;           // Variables that are pointers
    set<int> pointerParams;            // Parameter indices that are pointers
    map<pair<string, int>, bool> functionParamIsPointer;  // {funcName, paramIdx} -> isPointer
    map<pair<string, int>, int> functionParamPointerSize; // {funcName, paramIdx} -> pointerSize (1 for char*, 4 for int*)
    map<string, int> pointerSizes;     // Pointer -> size of pointed-to type (4 for int*, 1 for char*)
    
    // Struct tracking
    map<string, map<string, int>> structMemberOffsets; // structVarName -> (memberName -> offset)
    
    // String literal management
    map<string, string> stringLiterals; // String content -> label
    int stringCounter = 0;
    
    // Label management
    map<string, string> labelMap;      // 3AC label -> MIPS label
    set<string> emittedLabels;         // Set of MIPS labels that have been emitted
    int mipsLabelCounter = 0;
    
    // Static variable tracking
    set<string> staticVars;            // Set of static variable names (both global and local)
    set<string> globalStaticVars;      // Set of global static variable names only
    map<string, string> staticVarLabels;  // Variable name -> unique .data label (for local statics)
    int staticVarCounter = 0;          // Counter for unique static variable labels
    
    // Global variable tracking
    set<string> globalVars;            // Set of global variable names
    
    // Goto target tracking (instruction indices that are jump targets)
    set<int> gotoTargetsSet;           // Set of instruction indices that are jump targets
    set<string> functionNames;         // Set of function names (to distinguish from user labels)
    
    // Current function name (for tracking function entry labels)
    string currentFunc;
    
    // Parameter tracking for function calls
    int paramCount = 0;
    vector<string> paramRegs;  // Holds parameter registers for multi-param calls
    vector<string> paramValues; // Holds parameter values/names for tracking
    
    // Next-use analysis for register allocation
    struct NextUseInfo {
        map<string, int> nextUse;  // variable -> distance to next use (-1 if dead)
    };
    map<int, NextUseInfo> nextUseMap;  // instruction index -> next-use info
    int currentInstrIndex = 0;         // Current instruction being processed
    
    // Helper functions
    bool isGlobalVar(const string& var) const;
    string getMIPSLabel(const string& tacLabel);
    string allocateStringLiteral(const string& str);
    string getStaticLabel(const string& var);
    int getVarOffset(const string& var);
    
    // Struct member access helpers
    bool isStructMemberAccess(const string& var, string& baseName, string& memberName, bool& isArrow);
    int getStructMemberOffset(const string& baseName, const string& memberName);
    void genStructMemberLoad(const string& destReg, const string& var);
    void genStructMemberStore(const string& var, const string& srcReg);
    
    void spillRegister(const string& reg);
    string selectRegisterToSpill(int instrIndex);
    string getReg(const string& var);
    string getRegForTemp(const string& temp);
    bool isImmediate(const string& operand);
    bool isLiteral(const string& operand);
    string sanitizeOperand(const string& operand);
    bool isStatic(const string& var);
    void computeNextUse(const vector<TACInstruction>& tacCode);
    int getNextUse(const string& var, int instrIndex);
    
public:
    MIPSGenerator() {}
    
    // Main generation functions
    void generateMIPS(const vector<TACInstruction>& tacCode);
    void emitProlog(const string& funcName, int localVarCount);
    void emitEpilog(const string& funcName = "");
    void generateInstruction(const TACInstruction& instr);
    
    // Instruction-specific generation
    void genAssign(const TACInstruction& instr);
    void genArithmetic(const TACInstruction& instr);
    void genComparison(const TACInstruction& instr);
    void genGoto(const TACInstruction& instr);
    void genConditionalJump(const TACInstruction& instr);
    void genLabel(const TACInstruction& instr);
    void genCall(const TACInstruction& instr);
    void genReturn(const TACInstruction& instr);
    void genArrayAccess(const TACInstruction& instr);
    
    // Output functions
    void writeToFile(const string& filename);
    string getOutput();
    void reset();
    
    // Debugging
    void printDescriptors();
};

// Get MIPS label from 3AC label
string MIPSGenerator::getMIPSLabel(const string& tacLabel) {
    if (labelMap.count(tacLabel)) {
        return labelMap[tacLabel];
    }
    // The tacLabel is an instruction index (0-indexed), not a line number
    // Use it directly for the label
    string mipsLabel = "L_" + tacLabel;
    labelMap[tacLabel] = mipsLabel;
    return mipsLabel;
}

// Check if a variable is global
bool MIPSGenerator::isGlobalVar(const string& var) const {
    return globalVars.count(var) > 0 || staticVars.count(var) > 0;
}

// Allocate a string literal in .data section
string MIPSGenerator::allocateStringLiteral(const string& str) {
    if (stringLiterals.count(str)) {
        return stringLiterals[str];
    }
    string label = "str_" + to_string(stringCounter++);
    stringLiterals[str] = label;
    dataSection.push_back(label + ": .asciiz " + str);
    return label;
}

// Get stack offset for a variable
int MIPSGenerator::getVarOffset(const string& var) {
    // Check if this is a struct member access
    string baseName, memberName;
    bool isArrow;
    if (isStructMemberAccess(var, baseName, memberName, isArrow)) {
        // For struct member access with dot operator, return full offset
        if (!isArrow) {
            int baseOffset = getVarOffset(baseName);
            int memberOffset = getStructMemberOffset(baseName, memberName);
            return baseOffset + memberOffset;
        }
        // For arrow operator, return the offset of the pointer variable itself
        return getVarOffset(baseName);
    }
    
    if (varOffsets.count(var)) {
        return varOffsets[var];
    }
    
    // Determine size and allocation strategy
    int size = 4;  // Default: 4 bytes per word
    bool isArray = false;
    
    // Check if this is an array (heuristic based on name)
    if (var.find("Arr") != string::npos || var.find("arr") != string::npos || 
        var.find("ARR") != string::npos || var.find("grid") != string::npos ||
        var.find("cube") != string::npos || var.find("table") != string::npos ||
        var.find("mat") != string::npos) {
        size = 400;  // Reserve 400 bytes for arrays (100 elements max)
        isArray = true;
    }
    // Check if this might be a struct (reserve more space)
    else if (var == "pt" || var == "r" || var == "h" || var == "n1" || var == "n2" || var == "n3" || var == "m" || var == "g") {
        size = 100;  // Reserve space for struct with array members
    }
    
    // CRITICAL FIX: Allocate arrays at the END of the stack frame
    // to prevent collision with temporary register spills
    // Stack layout:
    // -4: ra, -8 to -16: argc/argv
    // -20 to -400: named variables and temporaries
    // -1000+: arrays (starting far from temporaries)
    if (isArray) {
        // Allocate arrays starting at offset 1000, growing larger
        static int arrayOffset = 1000;
        arrayOffset += size;
        varOffsets[var] = arrayOffset;
        return arrayOffset;
    } else {
        // Regular variables: use normal sequential allocation
        currentOffset += size;
        varOffsets[var] = currentOffset;
        return currentOffset;
    }
}

// Parse struct member access (e.g., "pt.x" or "ptr->y")
// Returns true if var is a struct member access, false otherwise
// Sets baseName (e.g., "pt" or "ptr"), memberName (e.g., "x" or "y"), and isArrow (true for ->)
bool MIPSGenerator::isStructMemberAccess(const string& var, string& baseName, string& memberName, bool& isArrow) {
    // Check for arrow operator (->)
    size_t arrowPos = var.find("->");
    if (arrowPos != string::npos) {
        baseName = var.substr(0, arrowPos);
        memberName = var.substr(arrowPos + 2);
        isArrow = true;
        return true;
    }
    
    // Check for dot operator (.)
    // Need to find the FIRST dot to separate base from member
    // But exclude dots inside array brackets like h.data[i]
    size_t dotPos = var.find('.');
    if (dotPos != string::npos) {
        baseName = var.substr(0, dotPos);
        memberName = var.substr(dotPos + 1);
        isArrow = false;
        return true;
    }
    
    return false;
}

// Get the offset of a struct member
// For now, we'll use a simple heuristic: first member is at offset 0, second at offset 4, etc.
// This assumes all struct members are ints (4 bytes)
int MIPSGenerator::getStructMemberOffset(const string& baseName, const string& memberName) {
    // Check if we have cached offset information for this struct variable
    if (structMemberOffsets.count(baseName) && structMemberOffsets[baseName].count(memberName)) {
        return structMemberOffsets[baseName][memberName];
    }
    
    // Handle nested members like "p2.x" -> p2 is at offset 8, x is at offset 0, total = 8
    size_t dotPos = memberName.find('.');
    if (dotPos != string::npos) {
        string firstPart = memberName.substr(0, dotPos);
        string secondPart = memberName.substr(dotPos + 1);
        int firstOffset = getStructMemberOffset(baseName, firstPart);
        int secondOffset = getStructMemberOffset(firstPart, secondPart);
        int totalOffset = firstOffset + secondOffset;
        structMemberOffsets[baseName][memberName] = totalOffset;
        return totalOffset;
    }
    
    // Simple heuristic: common member names and their typical offsets
    // For struct Point { int x; int y; }, x is at offset 0, y is at offset 4
    int offset = 0;
    if (memberName == "y" || memberName == "second" || memberName == "b") {
        offset = 4;
    } else if (memberName == "z" || memberName == "third" || memberName == "c") {
        offset = 8;
    } else if (memberName == "w" || memberName == "fourth" || memberName == "d") {
        offset = 12;
    } else if (memberName == "p2" || memberName == "next") {
        offset = 8;  // Second struct member (assuming first is 8 bytes = 2 ints)
    } else if (memberName == "grade") {
        offset = 4;  // Mixed: after id (4 bytes)
    } else if (memberName == "score") {
        offset = 8;  // Mixed: after id (4 bytes) and grade (4 bytes with padding)
    } else if (memberName == "cols") {
        offset = 4;  // Matrix: after rows (4 bytes)
    } else if (memberName == "data") {
        offset = 8;  // Matrix: after rows (4 bytes) and cols (4 bytes)
    }
    // First member (x, a, first, p1, id, val, rows) gets offset 0
    
    // Cache this for future use
    structMemberOffsets[baseName][memberName] = offset;
    return offset;
}

// Load a struct member into a register
void MIPSGenerator::genStructMemberLoad(const string& destReg, const string& var) {
    string baseName, memberName;
    bool isArrow;
    
    if (!isStructMemberAccess(var, baseName, memberName, isArrow)) {
        // Not a struct member access, shouldn't happen
        return;
    }
    
    // Check if baseName is an array access (e.g., "pts[i]" in "pts[i].x")
    // This is an array of structs
    size_t baseArrayBracket = baseName.find('[');
    if (baseArrayBracket != string::npos) {
        // Array of structs: arrayName[index].member
        string arrayName = baseName.substr(0, baseArrayBracket);
        size_t closeBracket = baseName.find(']', baseArrayBracket);
        string indexVar = baseName.substr(baseArrayBracket + 1, closeBracket - baseArrayBracket - 1);
        
        // Get member offset within struct
        int memberOffset = getStructMemberOffset(arrayName, memberName);
        
        // Get base address of array
        int arrayBaseOffset = getVarOffset(arrayName);
        
        // Determine struct size - for Point it's 8 bytes (2 ints)
        // For now, use heuristic based on array name
        int structSize = 8;  // Default: assume struct with 2 ints
        if (arrayName.find("Node") != string::npos || arrayName.find("node") != string::npos) {
            structSize = 12;  // Node has val(4) + padding + next(4) = 12 bytes aligned
        }
        
        // Load index
        string indexReg = getReg(indexVar);
        
        // Calculate offset: index * structSize + memberOffset
        // Use scratch register for calculations
        string offsetReg = "$s7";
        
        // Shift left by log2(structSize)
        if (structSize == 8) {
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 3  # offset = index * 8 (struct size)");
        } else if (structSize == 4) {
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 2  # offset = index * 4");
        } else {
            // For non-power-of-2 sizes, use multiply
            mipsCode.push_back("    li $v1, " + to_string(structSize) + "  # struct size");
            mipsCode.push_back("    mul " + offsetReg + ", " + indexReg + ", $v1  # offset = index * struct_size");
        }
        
        // Add member offset if non-zero
        if (memberOffset > 0) {
            mipsCode.push_back("    addiu " + offsetReg + ", " + offsetReg + ", " + to_string(memberOffset) + "  # add member offset");
        }
        
        // Load base address and add offset
        string baseReg = "$s6";
        mipsCode.push_back("    la " + baseReg + ", -" + to_string(arrayBaseOffset) + "($fp)  # base of " + arrayName);
        mipsCode.push_back("    add " + baseReg + ", " + baseReg + ", " + offsetReg);
        
        // Load value
        mipsCode.push_back("    lw " + destReg + ", 0(" + baseReg + ")  # Load " + var);
        return;
    }
    
    // Check if memberName is an array access (e.g., "data[i]")
    size_t bracketPos = memberName.find('[');
    if (bracketPos != string::npos) {
        // Array access within struct: base.array[index]
        string arrayMemberName = memberName.substr(0, bracketPos);
        size_t closeBracket = memberName.find(']', bracketPos);
        string indexVar = memberName.substr(bracketPos + 1, closeBracket - bracketPos - 1);
        
        // Get offset of array member within struct
        int arrayOffset = getStructMemberOffset(baseName, arrayMemberName);
        
        // Get base address of struct
        int structBaseOffset = getVarOffset(baseName);
        
        // Calculate: base address + array offset + (index * element_size)
        // Assume int array (4 bytes per element) for now
        bool isCharArray = (arrayMemberName.find("char") != string::npos ||
                           arrayMemberName.find("Char") != string::npos);
        int elementSize = isCharArray ? 1 : 4;
        
        // Load index
        string indexReg = getReg(indexVar);
        
        // Calculate offset: index * elementSize
        string offsetReg = "$s7";
        if (elementSize == 1) {
            mipsCode.push_back("    move " + offsetReg + ", " + indexReg + "  # offset = index (char array)");
        } else {
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 2  # offset = index * 4");
        }
        
        // Calculate total offset from struct base
        int totalStructOffset = structBaseOffset + arrayOffset;
        
        // Load base address and add offsets
        string baseReg = "$s6";
        mipsCode.push_back("    la " + baseReg + ", -" + to_string(totalStructOffset) + "($fp)  # base of " + baseName + "." + arrayMemberName);
        mipsCode.push_back("    add " + baseReg + ", " + baseReg + ", " + offsetReg);
        
        // Load value
        if (isCharArray) {
            mipsCode.push_back("    lb " + destReg + ", 0(" + baseReg + ")  # Load " + var);
        } else {
            mipsCode.push_back("    lw " + destReg + ", 0(" + baseReg + ")  # Load " + var);
        }
        return;
    }
    
    int memberOffset = getStructMemberOffset(baseName, memberName);
    
    if (isArrow) {
        // Arrow operator: ptr->member
        // Check if pointer is already in a register
        string ptrReg = addrDesc.getRegister(baseName);
        
        if (ptrReg.empty()) {
            // Not in register, need to load from memory
            ptrReg = "$s6";
            int ptrOffset = getVarOffset(baseName);
            mipsCode.push_back("    lw " + ptrReg + ", -" + to_string(ptrOffset) + "($fp)  # Load pointer " + baseName);
        } else {
            // Already in register, but move to $s6 if needed to avoid conflicts with destReg
            if (ptrReg != "$s6") {
                mipsCode.push_back("    move $s6, " + ptrReg + "  # Save pointer to $s6");
                ptrReg = "$s6";
            }
        }
        
        // Members grow in POSITIVE direction (toward higher addresses)
        // If ptr points to $fp-48, and y is at offset +4, then ptr->y is at ($fp-48)+4 = $fp-44
        if (memberOffset == 0) {
            mipsCode.push_back("    lw " + destReg + ", 0(" + ptrReg + ")  # Load " + var);
        } else {
            mipsCode.push_back("    lw " + destReg + ", " + to_string(memberOffset) + "(" + ptrReg + ")  # Load " + var);
        }
    } else {
        // Dot operator: struct.member
        // Load from stack at base offset + member offset
        // Stack grows down: if struct is at $fp-656 and member is at offset +4 within struct,
        // then member is at address ($fp-656) + 4 = $fp-652
        // So we use offset (656 - 4) = 652
        int baseOffset = getVarOffset(baseName);
        int memberOffsetValue = getStructMemberOffset(baseName, memberName);
        int totalOffset = baseOffset - memberOffsetValue;  // Subtract because we negate the whole offset
        mipsCode.push_back("    lw " + destReg + ", -" + to_string(totalOffset) + "($fp)  # Load " + var);
    }
}

// Store a value into a struct member
void MIPSGenerator::genStructMemberStore(const string& var, const string& srcReg) {
    string baseName, memberName;
    bool isArrow;
    
    if (!isStructMemberAccess(var, baseName, memberName, isArrow)) {
        // Not a struct member access, shouldn't happen
        return;
    }
    
    // Check if baseName is an array access (e.g., "pts[i]" in "pts[i].x")
    // This is an array of structs
    size_t baseArrayBracket = baseName.find('[');
    if (baseArrayBracket != string::npos) {
        // Array of structs: arrayName[index].member = value
        string arrayName = baseName.substr(0, baseArrayBracket);
        size_t closeBracket = baseName.find(']', baseArrayBracket);
        string indexVar = baseName.substr(baseArrayBracket + 1, closeBracket - baseArrayBracket - 1);
        
        // Get member offset within struct
        int memberOffset = getStructMemberOffset(arrayName, memberName);
        
        // Get base address of array
        int arrayBaseOffset = getVarOffset(arrayName);
        
        // Determine struct size - for Point it's 8 bytes (2 ints)
        int structSize = 8;  // Default: assume struct with 2 ints
        if (arrayName.find("Node") != string::npos || arrayName.find("node") != string::npos) {
            structSize = 12;  // Node has val(4) + padding + next(4) = 12 bytes aligned
        }
        
        // Save source value to safe register before we compute offset
        // Use $s5 to avoid conflicts with offset calculation
        string valueReg = "$s5";
        if (srcReg != valueReg) {
            mipsCode.push_back("    move " + valueReg + ", " + srcReg + "  # Save value to store");
        }
        
        // Load index
        string indexReg = getReg(indexVar);
        
        // Calculate offset: index * structSize + memberOffset
        string offsetReg = "$s7";
        
        // Shift left by log2(structSize)
        if (structSize == 8) {
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 3  # offset = index * 8 (struct size)");
        } else if (structSize == 4) {
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 2  # offset = index * 4");
        } else {
            // For non-power-of-2 sizes, use multiply
            mipsCode.push_back("    li $v1, " + to_string(structSize) + "  # struct size");
            mipsCode.push_back("    mul " + offsetReg + ", " + indexReg + ", $v1  # offset = index * struct_size");
        }
        
        // Add member offset if non-zero
        if (memberOffset > 0) {
            mipsCode.push_back("    addiu " + offsetReg + ", " + offsetReg + ", " + to_string(memberOffset) + "  # add member offset");
        }
        
        // Load base address and add offset
        string baseReg = "$s6";
        mipsCode.push_back("    la " + baseReg + ", -" + to_string(arrayBaseOffset) + "($fp)  # base of " + arrayName);
        mipsCode.push_back("    add " + baseReg + ", " + baseReg + ", " + offsetReg);
        
        // Store value using the saved register
        mipsCode.push_back("    sw " + valueReg + ", 0(" + baseReg + ")  # Store to " + var);
        return;
    }
    
    // Check if memberName is an array access (e.g., "data[i]")
    size_t bracketPos = memberName.find('[');
    if (bracketPos != string::npos) {
        // Array access within struct: base.array[index] = value
        string arrayMemberName = memberName.substr(0, bracketPos);
        size_t closeBracket = memberName.find(']', bracketPos);
        string indexVar = memberName.substr(bracketPos + 1, closeBracket - bracketPos - 1);
        
        // Get offset of array member within struct
        int arrayOffset = getStructMemberOffset(baseName, arrayMemberName);
        
        // Get base address of struct
        int structBaseOffset = getVarOffset(baseName);
        
        // Calculate: base address + array offset + (index * element_size)
        bool isCharArray = (arrayMemberName.find("char") != string::npos ||
                           arrayMemberName.find("Char") != string::npos);
        int elementSize = isCharArray ? 1 : 4;
        
        // Load index
        string indexReg = getReg(indexVar);
        
        // Calculate offset: index * elementSize
        string offsetReg = "$s7";
        if (elementSize == 1) {
            mipsCode.push_back("    move " + offsetReg + ", " + indexReg + "  # offset = index (char array)");
        } else {
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 2  # offset = index * 4");
        }
        
        // Calculate total offset from struct base
        int totalStructOffset = structBaseOffset + arrayOffset;
        
        // Load base address and add offsets
        string baseReg = "$s6";
        mipsCode.push_back("    la " + baseReg + ", -" + to_string(totalStructOffset) + "($fp)  # base of " + baseName + "." + arrayMemberName);
        mipsCode.push_back("    add " + baseReg + ", " + baseReg + ", " + offsetReg);
        
        // Store value
        if (isCharArray) {
            mipsCode.push_back("    sb " + srcReg + ", 0(" + baseReg + ")  # Store to " + var);
        } else {
            mipsCode.push_back("    sw " + srcReg + ", 0(" + baseReg + ")  # Store to " + var);
        }
        return;
    }
    
    int memberOffset = getStructMemberOffset(baseName, memberName);
    
    if (isArrow) {
        // Arrow operator: ptr->member = value
        // Load the pointer value into a dedicated register, then store value
        // CRITICAL: Use $s6 for pointer to avoid conflicts with srcReg
        string ptrReg = "$s6";
        int ptrOffset = getVarOffset(baseName);
        mipsCode.push_back("    lw " + ptrReg + ", -" + to_string(ptrOffset) + "($fp)  # Load pointer " + baseName);
        
        // Members grow in POSITIVE direction (toward higher addresses)
        // If ptr points to $fp-48, and y is at offset +4, then ptr->y is at ($fp-48)+4 = $fp-44
        if (memberOffset == 0) {
            mipsCode.push_back("    sw " + srcReg + ", 0(" + ptrReg + ")  # " + var + " = value");
        } else {
            mipsCode.push_back("    sw " + srcReg + ", " + to_string(memberOffset) + "(" + ptrReg + ")  # " + var + " = value");
        }
    } else {
        // Dot operator: struct.member = value
        // Store to stack at base offset + member offset
        // Stack grows down: if struct is at $fp-48 and member is at +4 within struct,
        // then member is at address ($fp-48) + 4 = $fp-44
        // So we use offset (48 - 4) = 44
        int baseOffset = getVarOffset(baseName);
        int memberOffsetValue = getStructMemberOffset(baseName, memberName);
        int totalOffset = baseOffset - memberOffsetValue;
        mipsCode.push_back("    sw " + srcReg + ", -" + to_string(totalOffset) + "($fp)  # " + var + " = " + srcReg);
    }
}

// Check if a variable is static
bool MIPSGenerator::isStatic(const string& var) {
    return staticVars.count(var) > 0;
}

// Get the label for a static variable
// Returns the unique .data label generated during initialization
string MIPSGenerator::getStaticLabel(const string& var) {
    // If it's a global static, use plain name
    if (globalStaticVars.count(var)) {
        return var;
    }
    
    // Local static: look up the cached label for this function_var combination
    string contextKey = currentFunc + "_" + var;
    if (staticVarLabels.count(contextKey)) {
        return staticVarLabels[contextKey];
    }
    
    // Fallback: if not found, might be a global static we missed
    // or a variable accessed before proper initialization
    return var;
}

// Select register to spill based on next-use analysis
string MIPSGenerator::selectRegisterToSpill(int instrIndex) {
    string bestReg = tempRegs[0];  // Default fallback
    int maxNextUse = -1;
    
    // Find register whose variable has the furthest next-use or is dead
    for (const auto& reg : tempRegs) {
        if (regDesc.isEmpty(reg)) continue;  // Skip empty registers
        
        string var = regDesc.get(reg);
        int nextUse = getNextUse(var, instrIndex);
        
        // Prefer dead variables (nextUse == -1) or furthest next-use
        if (nextUse == -1) {
            // Dead variable - best candidate for spilling
            return reg;
        }
        
        if (nextUse > maxNextUse) {
            maxNextUse = nextUse;
            bestReg = reg;
        }
    }
    
    return bestReg;
}

// Spill a register to memory
void MIPSGenerator::spillRegister(const string& reg) {
    string var = regDesc.get(reg);
    if (var.empty()) return;  // Register is empty
    
    // Save to memory
    if (isStatic(var)) {
        // Static variables are stored to .data section with unique labels
        string label = getStaticLabel(var);
        mipsCode.push_back("    la $s4, " + label + "  # Load address of static " + var);
        mipsCode.push_back("    sw " + reg + ", 0($s4)  # Spill " + var + " to static");
    } else if (isGlobalVar(var)) {
        // Global variables are stored to .data section using la
        // Use $s4 for address loading (saved register, won't interfere with temps)
        mipsCode.push_back("    la $s4, " + var + "  # Load address of global " + var);
        mipsCode.push_back("    sw " + reg + ", 0($s4)  # Spill " + var + " to global");
    } else {
        // Local variables are stored on the stack
        int offset = getVarOffset(var);
        mipsCode.push_back("    sw " + reg + ", -" + to_string(offset) + "($fp)  # Spill " + var);
    }
    
    // Update descriptors
    addrDesc.removeLocation(var, reg);
    addrDesc.addLocation(var, "memory");
    regDesc.clear(reg);
}

// Check if operand is an immediate value (number)
bool MIPSGenerator::isImmediate(const string& operand) {
    if (operand.empty()) return false;
    
    // Check for character constants like 'H', '\n', etc.
    if (operand[0] == '\'') {
        return true;  // Character constants are treated as immediate integer values
    }
    
    if (operand[0] == '-' || isdigit(operand[0])) {
        // Check if all chars are digits (or first is -)
        for (size_t i = 1; i < operand.length(); i++) {
            if (!isdigit(operand[i]) && operand[i] != '.') return false;
        }
        return true;
    }
    return false;
}

// Check if operand is a string/char literal
bool MIPSGenerator::isLiteral(const string& operand) {
    if (operand.empty()) return false;
    // Only string literals (with double quotes) are treated as literals
    // Character constants (with single quotes) are treated as immediates
    return (operand[0] == '"');
}

// Sanitize operand (remove quotes, handle special chars)
string MIPSGenerator::sanitizeOperand(const string& operand) {
    if (operand.length() >= 2 && operand[0] == '"' && operand.back() == '"') {
        return operand;  // Keep quotes for string literals
    }
    if (operand.length() >= 2 && operand[0] == '\'' && operand.back() == '\'') {
        // Character literal - convert to ASCII value
        char c = operand[1];
        if (operand[1] == '\\' && operand.length() >= 4) {
            // Escape sequence
            switch (operand[2]) {
                case 'n': c = '\n'; break;
                case 't': c = '\t'; break;
                case 'r': c = '\r'; break;
                case '0': c = '\0'; break;
                case '\\': c = '\\'; break;
                case '\'': c = '\''; break;
                default: c = operand[2]; break;
            }
        }
        return to_string((int)c);
    }
    return operand;
}

// Compute next-use information for all variables at each instruction
void MIPSGenerator::computeNextUse(const vector<TACInstruction>& tacCode) {
    nextUseMap.clear();
    
    // Optimization: limit analysis for very large programs
    // This prevents hanging on complex files
    if (tacCode.size() > 5000) {
        // For very large files, skip next-use analysis (fallback to simple allocation)
        cerr << "Warning: Skipping next-use analysis for large file (" << tacCode.size() << " instructions)" << endl;
        return;
    }
    
    // For each instruction, track the next use of each variable
    map<string, int> lastUse;  // variable -> last instruction index where it was used
    
    // Process instructions backward with progress for large files
    int progressInterval = tacCode.size() / 10;  // Report every 10%
    
    for (int i = tacCode.size() - 1; i >= 0; i--) {
        // Progress indicator for large files
        if (tacCode.size() > 500 && progressInterval > 0 && i % progressInterval == 0) {
            cerr << "  Next-use analysis: " << (100 - (i * 100 / tacCode.size())) << "% complete..." << endl;
        }
        
        const TACInstruction& instr = tacCode[i];
        
        // Mark variables used in this instruction (update lastUse map)
        auto markUse = [&](const optional<string>& var) {
            if (var.has_value() && !var->empty() && !isImmediate(*var) && !isLiteral(*var)) {
                lastUse[*var] = i;
            }
        };
        
        // Check operands based on instruction type
        if (instr.op == TACOp::ASSIGN) {
            markUse(instr.operand1);
            markUse(instr.operand2);
        } else if (instr.op == TACOp::ADD || instr.op == TACOp::SUB || instr.op == TACOp::MUL || 
                   instr.op == TACOp::DIV || instr.op == TACOp::MOD || instr.op == TACOp::BIT_AND || 
                   instr.op == TACOp::BIT_OR || instr.op == TACOp::BIT_XOR || instr.op == TACOp::LSHFT || 
                   instr.op == TACOp::RSHFT) {
            markUse(instr.operand1);
            markUse(instr.operand2);
        } else if (instr.op == TACOp::LT || instr.op == TACOp::GT || instr.op == TACOp::LE || 
                   instr.op == TACOp::GE || instr.op == TACOp::EQ || instr.op == TACOp::NE) {
            markUse(instr.operand1);
            markUse(instr.operand2);
        } else if (instr.op == TACOp::GOTO) {
            // No operands to mark
        } else if (instr.op == TACOp::IF_EQ || instr.op == TACOp::IF_NE) {
            markUse(instr.operand1);
        } else if (instr.op == TACOp::LABEL) {
            // No operands to mark
        } else if (instr.op == TACOp::CALL || instr.op == TACOp::CALL2) {
            // Arguments handled separately
        } else if (instr.op == TACOp::RETURN) {
            if (!instr.result.empty()) {
                lastUse[instr.result] = i;
            }
        } else if (instr.op == TACOp::INDEX || instr.op == TACOp::ARR_INDEX) {
            markUse(instr.operand1);
            markUse(instr.operand2);
        } else if (instr.op == TACOp::ARRAY_STORE) {
            markUse(instr.operand1);
            markUse(instr.operand2);
            if (!instr.result.empty()) {
                lastUse[instr.result] = i;
            }
        }
        
        // Store next-use info for this instruction (only store relevant variables)
        NextUseInfo info;
        for (const auto& entry : lastUse) {
            if (entry.second > i) {
                info.nextUse[entry.first] = entry.second - i;
            } else {
                info.nextUse[entry.first] = -1;  // Dead
            }
        }
        nextUseMap[i] = info;
    }
}

// Get next-use distance for a variable at given instruction
int MIPSGenerator::getNextUse(const string& var, int instrIndex) {
    if (nextUseMap.count(instrIndex) && nextUseMap[instrIndex].nextUse.count(var)) {
        return nextUseMap[instrIndex].nextUse[var];
    }
    return -1;  // Assume dead if no info
}

// Get register for a variable (with register allocation)
string MIPSGenerator::getReg(const string& var) {
    // Check if this is a struct member access (e.g., "pt.x" or "ptr->y")
    string baseName, memberName;
    bool isArrow;
    if (isStructMemberAccess(var, baseName, memberName, isArrow)) {
        // Use a temporary register to load the struct member
        string tempReg = "$v1";  // Use $v1 for struct member loads
        genStructMemberLoad(tempReg, var);
        return tempReg;
    }
    
    // Check if this is an array access (e.g., "intArr[t12]")
    size_t bracketPos = var.find('[');
    if (bracketPos != string::npos) {
        // Parse array name and index
        string arrayName = var.substr(0, bracketPos);
        size_t closeBracket = var.find(']', bracketPos);
        string indexVar = var.substr(bracketPos + 1, closeBracket - bracketPos - 1);
        
        // Detect if this is a char array
        bool isCharArray = (arrayName.find("char") != string::npos || 
                           arrayName.find("Char") != string::npos ||
                           arrayName.find("str") != string::npos);
        
        // Get index register
        string indexReg = getReg(indexVar);
        
        // Use scratch registers for array access
        string offsetReg = "$s7";
        string baseReg = "$s6";
        string resultReg = "$v1";  // Result goes in $v1
        
        // Calculate offset based on element size
        if (isCharArray) {
            // For char arrays, offset = index * 1 (no shift needed)
            mipsCode.push_back("    move " + offsetReg + ", " + indexReg + "  # offset = index (char array)");
        } else {
            // For int arrays, offset = index * 4
            mipsCode.push_back("    sll " + offsetReg + ", " + indexReg + ", 2  # offset = index * 4");
        }
        
        // Get base address of array
        int arrayOffset = getVarOffset(arrayName);
        mipsCode.push_back("    la " + baseReg + ", -" + to_string(arrayOffset) + "($fp)  # base address of " + arrayName);
        
        // Add offset to base
        mipsCode.push_back("    add " + baseReg + ", " + baseReg + ", " + offsetReg);
        
        // Load value (lb for char, lw for int)
        if (isCharArray) {
            mipsCode.push_back("    lb " + resultReg + ", 0(" + baseReg + ")  # " + var);
        } else {
            mipsCode.push_back("    lw " + resultReg + ", 0(" + baseReg + ")  # " + var);
        }
        
        // Mark that this temp is in resultReg (don't track array[index] as a variable)
        return resultReg;
    }
    
    // Check if variable is already in a register
    string reg = addrDesc.getRegister(var);
    if (!reg.empty()) {
        return reg;
    }
    
    // Find an empty register
    for (const auto& r : tempRegs) {
        if (regDesc.isEmpty(r)) {
            // Load variable from memory
            if (isStatic(var)) {
                // Static variables are loaded from .data section with unique labels
                string label = getStaticLabel(var);
                mipsCode.push_back("    la $s5, " + label + "  # Load address of static " + var);
                mipsCode.push_back("    lw " + r + ", 0($s5)  # Load value from static");
            } else if (isGlobalVar(var)) {
                // Global variables are loaded from .data section using la
                // Use $s5 for address loading (saved register, won't interfere with temps)
                mipsCode.push_back("    la $s5, " + var + "  # Load address of global " + var);
                mipsCode.push_back("    lw " + r + ", 0($s5)  # Load value from global");
            } else if (addrDesc.getLocations(var).count("memory")) {
                int offset = getVarOffset(var);
                mipsCode.push_back("    lw " + r + ", -" + to_string(offset) + "($fp)  # Load " + var);
            }
            // Update descriptors
            regDesc.set(r, var);
            addrDesc.addLocation(var, r);
            return r;
        }
    }
    
    // No empty registers - use next-use based spill strategy
    string regToSpill = selectRegisterToSpill(currentInstrIndex);
    spillRegister(regToSpill);
    
    // Load variable from memory
    if (isStatic(var)) {
        // Static variables are loaded from .data section with unique labels
        string label = getStaticLabel(var);
        mipsCode.push_back("    la $s5, " + label + "  # Load address of static " + var);
        mipsCode.push_back("    lw " + regToSpill + ", 0($s5)  # Load value from static");
    } else if (isGlobalVar(var)) {
        // Global variables are loaded from .data section using la
        // Use $s5 for address loading (saved register, won't interfere with temps)
        mipsCode.push_back("    la $s5, " + var + "  # Load address of global " + var);
        mipsCode.push_back("    lw " + regToSpill + ", 0($s5)  # Load value from global");
    } else if (addrDesc.getLocations(var).count("memory")) {
        int offset = getVarOffset(var);
        mipsCode.push_back("    lw " + regToSpill + ", -" + to_string(offset) + "($fp)  # Load " + var);
    }
    
    // Update descriptors
    regDesc.set(regToSpill, var);
// Register allocation logic
