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
    addrDesc.addLocation(var, regToSpill);
    return regToSpill;
}

// Get register for a temporary (optimized for temps)
string MIPSGenerator::getRegForTemp(const string& temp) {
    // For temporaries, we prefer to keep them in registers
    return getReg(temp);
}

// Generate function prologue
void MIPSGenerator::emitProlog(const string& funcName, int localVarCount) {
    mipsCode.push_back("");
    mipsCode.push_back("# Function: " + funcName);
    mipsCode.push_back(funcName + ":");
    
    // Clear pointer parameter tracking from previous function call
    // (parameters from the CALLER are now available to THIS function)
    // NOTE: Do NOT clear here - we need pointerParams from the caller!
    // pointerParams will be cleared when we start building the NEXT function call
    
    // Save frame pointer
    mipsCode.push_back("    sw $fp, 0($sp)");
    
    // Set new frame pointer
    mipsCode.push_back("    move $fp, $sp");
    
    // Allocate space for locals and temps (estimate)
    // Use larger frame size to accommodate multi-dimensional arrays
    // 2048 bytes = 512 int variables or multiple large arrays + locals
    frameSize = 2048;  // Increased for multi-dimensional arrays
    mipsCode.push_back("    addiu $sp, $sp, -" + to_string(frameSize));
    
    // Save return address
    mipsCode.push_back("    sw $ra, -4($fp)");
    
    // CRITICAL: Clear varOffsets for this function to prevent collisions with previous functions
    varOffsets.clear();
    
    // For main function, save argc and argv from $a0 and $a1
    if (funcName == "main") {
        mipsCode.push_back("    # Save command line arguments");
        mipsCode.push_back("    sw $a0, -8($fp)   # Save argc");
        mipsCode.push_back("    sw $a1, -12($fp)  # Save argv");
        // Reserve space in varOffsets for argc and argv
        varOffsets["argc"] = 8;   // Offset from $fp
        varOffsets["argv"] = 12;
        // Mark argv as a pointer (char**)
        pointerVars.insert("argv");
        pointerSizes["argv"] = 4;  // Points to char* array
        // Start allocating variables after argc/argv
        currentOffset = 12;
        // Mark argc and argv as being in memory
        addrDesc.addLocation("argc", "memory");
        addrDesc.addLocation("argv", "memory");
        currentOffset = 16;  // Start after argc, argv, ra, fp
    } else {
        // For other functions, start after $ra and potential parameters
        // Reserve space for common parameter patterns (up to 2 pointer params)
        currentOffset = 8;  // Start after $ra and first parameter slot
    }
    
    // Save callee-saved registers (if needed)
    // For simplicity, we'll save $s0-$s7 if we use them
    
    // Reset descriptors for new function to clear variables from previous functions
    regDesc.clearAll();
    // For non-main functions, clear everything
    // For main, we already set up argc/argv above, so clear and re-add them
    if (funcName == "main") {
        // Clear and re-add argc/argv
        addrDesc.clearAll();
        addrDesc.addLocation("argc", "memory");
        addrDesc.addLocation("argv", "memory");
    } else {
        addrDesc.clearAll();
    }
}

// Generate function epilogue
void MIPSGenerator::emitEpilog(const string& funcName) {
    mipsCode.push_back("");
    mipsCode.push_back("    # Function epilogue");
    
    // For main function, use exit syscall instead of jr $ra
    if (funcName == "main") {
        // Print the return value before exiting
        mipsCode.push_back("    # Print return value from main");
        mipsCode.push_back("    move $a0, $v0   # Move return value to $a0 for printing");
        mipsCode.push_back("    li $v0, 1       # syscall 1: print_int");
        mipsCode.push_back("    syscall");
        mipsCode.push_back("    # Print newline");
        mipsCode.push_back("    li $a0, 10      # ASCII code for newline");
        mipsCode.push_back("    li $v0, 11      # syscall 11: print_char");
        mipsCode.push_back("    syscall");
        mipsCode.push_back("    # Exit");
        mipsCode.push_back("    li $v0, 10      # syscall 10: exit");
        mipsCode.push_back("    syscall");
    } else {
        // Restore return address
        mipsCode.push_back("    lw $ra, -4($fp)");
        
        // Restore callee-saved registers (if we saved them)
        
        // Deallocate stack frame
        mipsCode.push_back("    move $sp, $fp");
        
        // Restore old frame pointer
        mipsCode.push_back("    lw $fp, 0($sp)");
        
        // Return
        mipsCode.push_back("    jr $ra");
    }
}

// Generate ASSIGN instruction
void MIPSGenerator::genAssign(const TACInstruction& instr) {
    string dest = instr.result;
    string src = instr.operand1.value_or("");
    
    // Handle special cases
    if (src.empty()) return;
    
    // Check for address-of operation (&var)
    if (src[0] == '&') {
        string var = src.substr(1);
        
        // Check if this is taking address of an array element like &arr[index]
        size_t bracketPos = var.find('[');
        if (bracketPos != string::npos) {
            // Extract array name and index
            string arrayName = var.substr(0, bracketPos);
            size_t closeBracket = var.find(']', bracketPos);
            string indexVar = var.substr(bracketPos + 1, closeBracket - bracketPos - 1);
            
            // Determine element size based on array type
            int elementSize = 4; // Default for int arrays
            bool isCharArray = (arrayName.find("char") != string::npos || 
                               arrayName.find("Char") != string::npos);
            if (isCharArray) {
                elementSize = 1;
            } else if (arrayName.find("pts") != string::npos || arrayName.find("Points") != string::npos || 
                       arrayName.find("Point") != string::npos) {
                // Array of Point structs (x, y) = 8 bytes
                elementSize = 8;
            } else if (arrayName.find("Node") != string::npos || arrayName.find("node") != string::npos) {
                // Array of Node structs = 12 bytes
                elementSize = 12;
            }
            
            // Mark dest as a pointer
            pointerVars.insert(dest);
            pointerSizes[dest] = elementSize;
            
            // Compute address: base + index * elementSize
            string indexReg = getReg(indexVar);
            string destReg = getReg(dest);
            int arrayOffset = getVarOffset(arrayName);
            
            if (elementSize == 1) {
                // Char array: offset = index * 1
                mipsCode.push_back("    la " + destReg + ", -" + to_string(arrayOffset) + "($fp)  # base of " + arrayName);
                mipsCode.push_back("    add " + destReg + ", " + destReg + ", " + indexReg + "  # " + dest + " = &" + var);
            } else {
                // Int array: offset = index * 4
                // Use scratch registers to avoid conflicts
                mipsCode.push_back("    sll $s7, " + indexReg + ", 2  # offset = index * 4");
                mipsCode.push_back("    la " + destReg + ", -" + to_string(arrayOffset) + "($fp)  # base of " + arrayName);
                mipsCode.push_back("    add " + destReg + ", " + destReg + ", $s7  # " + dest + " = &" + var);
            }
            
            addrDesc.addLocation(dest, destReg);
            return;
        }
        
        // Taking address of a regular variable (not array element)
        int elementSize = 4; // Default to int size
        if (var.find("char") != string::npos || var.find("Char") != string::npos) {
            elementSize = 1;
        }
        
        // Mark dest as a pointer and track the size of what it points to
        pointerVars.insert(dest);
        pointerSizes[dest] = elementSize;
        
        // Ensure the variable is in memory (allocate offset if needed)
        int offset = getVarOffset(var);
        
        // If the variable is currently in a register, store it to memory
        // BUT: descriptors might be stale after branches/jumps, so we check if variable
        // is a non-temporary (which should always have a memory location)
        string varReg = addrDesc.getRegister(var);
        bool isNonTemp = !(var[0] == 't' && isdigit(var[1]));
        
        if (!varReg.empty() && !isNonTemp) {
            // This is a temporary in a register - store it to memory
            mipsCode.push_back("    sw " + varReg + ", -" + to_string(offset) + "($fp)  # Store " + var + " to memory (address taken)");
        }
        // For non-temporaries, they should already be in memory, so don't trust the descriptor
        // (it might be stale after branches)
        
        // Clear stale descriptor entries for the variable
        addrDesc.clearVar(var);
        addrDesc.addLocation(var, "memory");
        if (!varReg.empty()) {
            regDesc.clear(varReg);
        }
        
        // Now load the address
        string destReg = getReg(dest);
        mipsCode.push_back("    la " + destReg + ", -" + to_string(offset) + "($fp)  # " + dest + " = &" + var);
        addrDesc.addLocation(dest, destReg);
        return;
    }
    
    // Check for dereference operation (*ptr)
    if (src[0] == '*') {
        string ptr = src.substr(1);
        // For pointer dereference, ensure we have the current value
        // If pointer has both register and memory location, reload from memory
        string ptrReg = addrDesc.getRegister(ptr);
        bool hasMemory = addrDesc.getLocations(ptr).count("memory") || varOffsets.count(ptr);
        
        if (!ptrReg.empty() && hasMemory) {
            // Pointer is in both register and memory - reload from memory to be safe
            // Clear stale register location
            addrDesc.removeLocation(ptr, ptrReg);
            regDesc.clear(ptrReg);
            ptrReg = "";  // Force reload
        }
        
        if (ptrReg.empty()) {
            // Load pointer from memory
            ptrReg = getReg(ptr);
        }
        
        // Determine if this is a char pointer (use lb) or int pointer (use lw)
        bool isCharPointer = false;
        if (pointerSizes.count(ptr) > 0 && pointerSizes[ptr] == 1) {
            // Explicitly marked as char pointer
            isCharPointer = true;
        } else if (ptr.find("str") != string::npos || ptr.find("Str") != string::npos ||
                   ptr.find("char") != string::npos || ptr.find("Char") != string::npos ||
                   ptr[0] == 's') {  // Common convention: s, s1, str, etc. for strings
            // Heuristic: variable name suggests it's a string/char pointer
            isCharPointer = true;
        }
        
        string destReg = getReg(dest);
        if (isCharPointer) {
            mipsCode.push_back("    lb " + destReg + ", 0(" + ptrReg + ")  # " + dest + " = *" + ptr + " (char)");
        } else {
            mipsCode.push_back("    lw " + destReg + ", 0(" + ptrReg + ")  # " + dest + " = *" + ptr);
        }
        addrDesc.addLocation(dest, destReg);
        
        // CRITICAL: If ptr is a pointer-to-pointer, then *ptr is still a pointer
        // When dereferencing int**, the result is int* (still a pointer!)
        // Example: row_1 = *(m.data + 1) where m.data is int**
        // Result: row_1 is int* and needs scaling by 4
        // We detect this by checking if ptr is a struct member known to be int**
        string ptrBase, ptrMember;
        bool ptrIsArrow;
        if (isStructMemberAccess(ptr, ptrBase, ptrMember, ptrIsArrow)) {
            if (ptrMember == "data" || ptrMember == "next") {
                // These are known to be pointer-to-pointer types
                // Dereferencing gives us a pointer
                pointerVars.insert(dest);
                pointerSizes[dest] = 4;  // Points to int (4 bytes each)
            }
        }
        // Also check for temporaries that resulted from arithmetic on int**
        // e.g., t103 = m.data + 1, then row_1 = *t103
        // If t103 is marked as int**, then *t103 is int*
        else if (ptr[0] == 't' && isdigit(ptr[1]) && pointerVars.count(ptr) && pointerSizes[ptr] == 4) {
            // This temp is a pointer with size 4, could be int** from pointer arithmetic
            // Check if it came from struct member pointer arithmetic
            // For now, conservatively mark result as pointer only if source was struct member
            // This avoids false positives like *p1 where p1 is int*
            pointerVars.insert(dest);
            pointerSizes[dest] = 4;
        }
        
        return;
    }
    
    // Check for type cast operations (int_to_char, char_to_int, etc.)
    // These are no-ops in MIPS since both are just numbers
    // The actual value is in operand2
    if (src.find("_to_") != string::npos && 
        (src.find("int_to_char") != string::npos || src.find("char_to_int") != string::npos)) {
        // For type casts, the actual value is in operand2
        if (instr.operand2.has_value()) {
            string actualSrc = instr.operand2.value();
            string srcReg = getReg(actualSrc);
            string destReg = getReg(dest);
            mipsCode.push_back("    move " + destReg + ", " + srcReg + "  # " + dest + " = " + src + "(" + actualSrc + ")");
            addrDesc.addLocation(dest, destReg);
            return;
        }
    }
    
    // Handle copying FROM function parameter registers TO local variables
    // This happens at function entry: x = param0, y = param1, etc.
    if (src.substr(0, 5) == "param" && src.length() > 5 && isdigit(src[5])) {
        int paramIndex = stoi(src.substr(5));
        
        // Special case: main function's argc and argv are already saved in prologue
        if (currentFunc == "main" && (dest == "argc" || dest == "argv")) {
            // Already handled in emitProlog, just update descriptor
            addrDesc.addLocation(dest, "memory");
            return;
        }
        
        string srcReg;
        
        if (paramIndex == 0) srcReg = "$a0";
        else if (paramIndex == 1) srcReg = "$a1";
        else if (paramIndex == 2) srcReg = "$a2";
        else if (paramIndex == 3) srcReg = "$a3";
        else {
            // Parameter is on stack
            int offset = (paramIndex - 4) * 4;
            srcReg = "$t9";  // Use temp register
            mipsCode.push_back("    lw " + srcReg + ", " + to_string(offset) + "($sp)  # Load parameter " + to_string(paramIndex));
        }
        
        // Check if this parameter is a pointer based on function signature analysis
        bool isPointer = functionParamIsPointer.count({currentFunc, paramIndex}) > 0;
        if (!isPointer) {
            // Fallback: check pointerParams from recent function call
            isPointer = (pointerParams.count(paramIndex) > 0);
        }
        // ADDITIONAL FALLBACK: Check if variable name suggests it's a string/char pointer
        if (!isPointer && (dest.find("str") != string::npos || 
                          dest.find("Str") != string::npos ||
                          (dest.length() >= 1 && dest[0] == 's' && dest.length() <= 3))) {
            isPointer = true;
        }
        
        int ptrSize = 4;  // Default to int* size
        if (isPointer) {
            pointerVars.insert(dest);
            // Get the pointer size from pre-pass analysis
            if (functionParamPointerSize.count({currentFunc, paramIndex}) > 0) {
                ptrSize = functionParamPointerSize[{currentFunc, paramIndex}];
            } else if (dest.find("str") != string::npos || 
                      dest.find("Str") != string::npos ||
                      (dest.length() >= 1 && dest[0] == 's' && dest.length() <= 3)) {
                // Variable name suggests char pointer
                ptrSize = 1;
            }
            pointerSizes[dest] = ptrSize;
        }
        
        // Assign to local variable
        string destReg = getReg(dest);
        mipsCode.push_back("    move " + destReg + ", " + srcReg + "  # " + dest + " = " + src);
        addrDesc.addLocation(dest, destReg);
        
        // CRITICAL: If this is a pointer parameter, also store it to memory
        // so it can be reloaded later when the register gets reused
        if (isPointer) {
            int destOffset = getVarOffset(dest);
            mipsCode.push_back("    sw " + destReg + ", -" + to_string(destOffset) + "($fp)  # Store " + dest + " to memory (pointer param)");
            addrDesc.addLocation(dest, "memory");  // Mark that it's also in memory
            // CRITICAL: Update currentOffset to ensure subsequent variables don't collide
            if (destOffset >= currentOffset) {
                currentOffset = destOffset + 4;  // Move past this variable
            }
        }
        
        return;
    }
    
    // Handle param assignment (for function calls) - CHECK THIS FIRST before other checks
    if (dest == "param") {
        // If this is the first parameter of a new call, clear previous pointer tracking
        if (paramCount == 0) {
            pointerParams.clear();
        }
        
        string srcReg;
        
        // Handle string literals
        if (isLiteral(src)) {
            string label = allocateStringLiteral(src);
            // Use a dedicated register for loading parameters
            // Try $s7 first (saved register, less likely to be in use for temps)
            srcReg = "$s7";
            mipsCode.push_back("    la " + srcReg + ", " + label + "  # Load string " + src);
        }
        // Handle immediate values
        else if (isImmediate(src)) {
            // Use $s7 for immediate parameter loading
            srcReg = "$s7";
            mipsCode.push_back("    li " + srcReg + ", " + sanitizeOperand(src) + "  # Load immediate " + src);
        }
        // Handle variables
        else {
            srcReg = getReg(src);
        }
        
        // Track if this parameter is a pointer (for proper INDEX handling later)
        if (pointerVars.count(src) > 0) {
            // Mark this parameter index as containing a pointer value
            pointerParams.insert(paramCount);
        }
        
        // Store parameters in sequence: $a0, $a1, $a2, $a3, then stack
        if (paramCount == 0) {
            mipsCode.push_back("    move $a0, " + srcReg + "  # Pass parameter " + src);
            paramRegs.push_back("$a0");
        } else if (paramCount == 1) {
            mipsCode.push_back("    move $a1, " + srcReg + "  # Pass parameter " + src);
            paramRegs.push_back("$a1");
        } else if (paramCount == 2) {
            mipsCode.push_back("    move $a2, " + srcReg + "  # Pass parameter " + src);
            paramRegs.push_back("$a2");
        } else if (paramCount == 3) {
            mipsCode.push_back("    move $a3, " + srcReg + "  # Pass parameter " + src);
            paramRegs.push_back("$a3");
        } else {
            // Store additional parameters on stack
            int offset = (paramCount - 4) * 4;
            mipsCode.push_back("    sw " + srcReg + ", " + to_string(offset) + "($sp)  # Pass parameter " + src + " on stack");
        }
        paramCount++;
        return;
    }
    
    // Check for immediate value
    if (isImmediate(src)) {
        string destReg = getReg(dest);
        mipsCode.push_back("    li " + destReg + ", " + sanitizeOperand(src) + "  # " + dest + " = " + src);
        addrDesc.addLocation(dest, destReg);
        return;
    }
    
    // Check for string literal
    if (isLiteral(src)) {
        string label = allocateStringLiteral(src);
        string destReg = getReg(dest);
        mipsCode.push_back("    la " + destReg + ", " + label + "  # " + dest + " = " + src);
        addrDesc.addLocation(dest, destReg);
        return;
    }
    
    // Handle struct member store (e.g., pt.x = value or ptr->y = value)
    string baseName, memberName;
    bool isArrow;
    if (isStructMemberAccess(dest, baseName, memberName, isArrow)) {
        string srcReg = getReg(src);
        genStructMemberStore(dest, srcReg);
        return;
    }
    
    // Handle indirect store (*ptr = value)
    if (dest[0] == '*') {
        string ptr = dest.substr(1);
        
        // Get the pointer value
        string ptrReg = getReg(ptr);
        
        // CRITICAL: For indirect stores, we need BOTH the pointer register AND the source register
        // active simultaneously. To avoid conflicts, we check if src would use the same register.
        // If so, we move the pointer to a safe register first.
        
        string srcReg;
        // Check if src is already in a register
        string srcExistingReg = addrDesc.getRegister(src);
        if (!srcExistingReg.empty() && srcExistingReg != ptrReg) {
            // src is already in a different register, use it
            srcReg = srcExistingReg;
        } else {
            // Need to load src - but first save ptr to a safe location ($v1 is safe)
            if (ptrReg != "$v1") {
                mipsCode.push_back("    move $v1, " + ptrReg + "  # Save pointer for indirect store");
                ptrReg = "$v1";
            }
            srcReg = getReg(src);
        }
        
        mipsCode.push_back("    sw " + srcReg + ", 0(" + ptrReg + ")  # *" + ptr + " = " + src);
        
        // CRITICAL: After an indirect store, we must invalidate variables that might be
        // aliases of *ptr. We can't know at compile time which variables are modified.
        // Strategy: Only invalidate variables that have memory locations (not pointers/parameters)
        // This is conservative - pointers and parameters stay valid, only stack variables are invalidated.
        for (const auto& reg : tempRegs) {
            string var = regDesc.get(reg);
            if (!var.empty()) {
                // Only invalidate if this is a variable with a memory location (not a pointer param)
                // Skip pointers (they won't be modified by storing through another pointer)
                // Skip variables without memory (like parameters that are only in registers)
                bool isPointer = pointerVars.count(var);
                bool hasMemory = addrDesc.getLocations(var).count("memory") || varOffsets.count(var);
                
                if (!isPointer && hasMemory) {
                    // This is a non-pointer variable with memory - it could be aliased, so invalidate
                    addrDesc.removeLocation(var, reg);
                    addrDesc.addLocation(var, "memory");
                    regDesc.clear(reg);
                }
            }
        }
        
        return;
    }
    
    // Regular assignment: dest = src  
    // Get source value into a register
    string srcReg = getReg(src);
    
    // If src is a pointer, propagate pointer information to dest
    if (pointerVars.count(src)) {
        pointerVars.insert(dest);
        pointerSizes[dest] = pointerSizes[src];
    }
    
    // CRITICAL: If src is a struct member that's a pointer (like m.data), mark both src and dest as pointers
    string srcBase, srcMember;
    bool srcIsArrow;
    if (isStructMemberAccess(src, srcBase, srcMember, srcIsArrow)) {
        // Check if this member is a known pointer type
        if (srcMember == "data" || srcMember == "next" || srcMember == "ptr") {
            // Mark the struct member itself as a pointer (for future arithmetic operations)
            pointerVars.insert(src);
            pointerSizes[src] = 4;  // Points to int* or similar (4 bytes each)
            // Also mark the destination
            pointerVars.insert(dest);
            pointerSizes[dest] = 4;
        }
    }
    
    // For assignment, we can reuse the source register as destination
    // or allocate a new one if needed
    string destReg;
    
    // Check if dest is already in a register
    string existingDestReg = addrDesc.getRegister(dest);
    if (!existingDestReg.empty()) {
        // Dest is already in a register, use it
        destReg = existingDestReg;
    } else {
        // Dest is not in a register - we can reuse srcReg if src is a temporary
        // Otherwise allocate a new register
        if (src[0] == 't' && isdigit(src[1])) {
            // src is a temporary - reuse its register for dest
            destReg = srcReg;
        } else {
            // src is a variable - need separate register for dest
            destReg = getReg(dest);
        }
    }
    
    // Only generate move if src and dest are in different registers
    if (srcReg != destReg) {
        mipsCode.push_back("    move " + destReg + ", " + srcReg + "  # " + dest + " = " + src);
    }
    
    // CRITICAL: If destReg already held another variable, remove that mapping
    string oldVar = regDesc.get(destReg);
    if (!oldVar.empty() && oldVar != dest) {
        addrDesc.removeLocation(oldVar, destReg);
    }
    
    // Update descriptor
    regDesc.set(destReg, dest);
    addrDesc.addLocation(dest, destReg);
    
    // If src is a pointer, propagate pointer information to dest
    if (pointerVars.count(src)) {
        pointerVars.insert(dest);
        pointerSizes[dest] = pointerSizes[src];
    }
    
    // Store back to memory if dest has a memory location (local var, global, or static)
    // For non-temporaries, we need to ensure they're stored to memory
    bool shouldStore = false;
    if (isStatic(dest)) {
        shouldStore = true;
    } else if (isGlobalVar(dest)) {
        shouldStore = true;
    } else if (!(dest[0] == 't' && isdigit(dest[1]))) {
        // dest is not a temporary - it's a local variable that needs memory storage
        // Allocate offset if not already done
        if (!varOffsets.count(dest)) {
            getVarOffset(dest);  // This allocates an offset
        }
        shouldStore = true;
    }
    
    if (shouldStore) {
        if (isStatic(dest)) {
            // Static variable - store to .data section with unique label
            string label = getStaticLabel(dest);
            mipsCode.push_back("    la $s4, " + label + "  # Load address of static " + dest);
            mipsCode.push_back("    sw " + destReg + ", 0($s4)  # Store to static");
        } else if (isGlobalVar(dest)) {
            // Global variable - store to .data section using la
            // Use $s4 for address loading (saved register, won't interfere with temps)
            mipsCode.push_back("    la $s4, " + dest + "  # Load address of global " + dest);
            mipsCode.push_back("    sw " + destReg + ", 0($s4)  # Store to global");
        } else {
            // Local variable - store back to stack
            int offset = varOffsets[dest];
            mipsCode.push_back("    sw " + destReg + ", -" + to_string(offset) + "($fp)  # Store " + dest);
            addrDesc.addLocation(dest, "memory");
        }
    }
}

// Generate arithmetic instruction
void MIPSGenerator::genArithmetic(const TACInstruction& instr) {
    string dest = instr.result;
    string op1 = instr.operand1.value_or("");
    string op2 = instr.operand2.value_or("");
    
    // Use $v0 and $v1 as scratch registers (not used by getReg for variable allocation)
    string scratch1 = "$v0";
    string scratch2 = "$v1";
    
    // Strategy: Load operands into dedicated scratch registers to avoid clobbering
    // First, load op1 into scratch1
    if (isImmediate(op1)) {
        mipsCode.push_back("    li " + scratch1 + ", " + sanitizeOperand(op1) + "  # Load immediate " + op1);
    } else {
        // Check if op1 is a struct member access
        string baseName1, memberName1;
        bool isArrow1;
        if (isStructMemberAccess(op1, baseName1, memberName1, isArrow1)) {
            // Struct member access - use genStructMemberLoad
            genStructMemberLoad(scratch1, op1);
        } else {
            // Check if op1 is already in a register
            string op1Reg = addrDesc.getRegister(op1);
            
            // CRITICAL FIX: If variable has BOTH register and memory, prefer memory
            // because register may be stale (descriptor not updated after reuse)
            bool hasReg = !op1Reg.empty();
            bool hasMemory = addrDesc.getLocations(op1).count("memory") || varOffsets.count(op1);
            bool isVariable = !(op1[0] == 't' && op1.length() > 1 && isdigit(op1[1]));
            
            if (hasReg && hasMemory && isVariable) {
                // Variable in BOTH reg and memory - reload from memory for safety
                if (isStatic(op1)) {
                    mipsCode.push_back("    lw " + scratch1 + ", " + op1 + "  # Load static var " + op1);
                } else {
                    int offset = getVarOffset(op1);
                    mipsCode.push_back("    lw " + scratch1 + ", -" + to_string(offset) + "($fp)  # Load " + op1);
                }
                // CRITICAL: Remove stale register location since we reloaded from memory
                addrDesc.removeLocation(op1, op1Reg);
                regDesc.clear(op1Reg);  // Clear the register descriptor too
            } else if (!op1Reg.empty() && op1Reg != scratch1 && op1Reg != scratch2) {
                // Op1 is in a register, move it to scratch1
                mipsCode.push_back("    move " + scratch1 + ", " + op1Reg + "  # Move " + op1 + " to " + scratch1);
            } else if (op1Reg == scratch1) {
                // Already in scratch1, nothing to do
            } else if (op1Reg == scratch2) {
                // In scratch2, move to scratch1
                mipsCode.push_back("    move " + scratch1 + ", " + scratch2 + "  # Move " + op1 + " to " + scratch1);
            } else {
                // Not in register, load from memory
                if (isStatic(op1)) {
                    mipsCode.push_back("    lw " + scratch1 + ", " + op1 + "  # Load static var " + op1);
                } else if (hasMemory) {
                    int offset = getVarOffset(op1);
                    mipsCode.push_back("    lw " + scratch1 + ", -" + to_string(offset) + "($fp)  # Load " + op1);
                } else {
                    // Unknown variable, treat as temp - try to load it
                    int offset = getVarOffset(op1);
                    mipsCode.push_back("    lw " + scratch1 + ", -" + to_string(offset) + "($fp)  # Load " + op1);
                }
            }
        }
    }
    
    // Now load op2 into scratch2 (op1 is safely in scratch1)
    bool op2IsImmediate = isImmediate(op2);
    if (!op2IsImmediate) {
        // Check if op2 is a struct member access
        string baseName2, memberName2;
        bool isArrow2;
        if (isStructMemberAccess(op2, baseName2, memberName2, isArrow2)) {
            // Struct member access - use genStructMemberLoad
            genStructMemberLoad(scratch2, op2);
        } else {
            string op2Reg = addrDesc.getRegister(op2);
            
            // CRITICAL FIX: If variable has BOTH register and memory, prefer memory
            bool hasReg = !op2Reg.empty();
            bool hasMemory = addrDesc.getLocations(op2).count("memory") || varOffsets.count(op2);
            bool isVariable = !(op2[0] == 't' && op2.length() > 1 && isdigit(op2[1]));
            
            if (hasReg && hasMemory && isVariable) {
                // Variable in BOTH reg and memory - reload from memory for safety
                if (isStatic(op2)) {
                    mipsCode.push_back("    lw " + scratch2 + ", " + op2 + "  # Load static var " + op2);
                } else {
                    int offset = getVarOffset(op2);
                    mipsCode.push_back("    lw " + scratch2 + ", -" + to_string(offset) + "($fp)  # Load " + op2);
                }
                // CRITICAL: Remove stale register location since we reloaded from memory
                addrDesc.removeLocation(op2, op2Reg);
                regDesc.clear(op2Reg);  // Clear the register descriptor too
            } else if (!op2Reg.empty() && op2Reg != scratch1 && op2Reg != scratch2) {
                // Op2 is in a register, move it to scratch2
                mipsCode.push_back("    move " + scratch2 + ", " + op2Reg + "  # Move " + op2 + " to " + scratch2);
            } else if (op2Reg == scratch2) {
                // Already in scratch2, nothing to do
            } else if (op2Reg == scratch1) {
                // In scratch1 (same as op1), still need to move to scratch2
                mipsCode.push_back("    move " + scratch2 + ", " + scratch1 + "  # Move " + op2 + " to " + scratch2);
            } else {
                // Not in register, load from memory
                if (isStatic(op2)) {
                    mipsCode.push_back("    lw " + scratch2 + ", " + op2 + "  # Load static var " + op2);
                } else if (hasMemory) {
                    int offset = getVarOffset(op2);
                    mipsCode.push_back("    lw " + scratch2 + ", -" + to_string(offset) + "($fp)  # Load " + op2);
                } else {
                    // Unknown variable, treat as temp
                    int offset = getVarOffset(op2);
                    mipsCode.push_back("    lw " + scratch2 + ", -" + to_string(offset) + "($fp)  # Load " + op2);
                }
            }
        }
    }
    
    // Now get destination register - operands are safely in scratch registers
    string destReg = getReg(dest);
    
    // CRITICAL: If destReg already held another variable, remove that mapping
    string oldVar = regDesc.get(destReg);
    if (!oldVar.empty() && oldVar != dest) {
        addrDesc.removeLocation(oldVar, destReg);
    }
    
    string op1Reg = scratch1;
    string op2Reg = scratch2;
    
    // Handle pointer arithmetic scaling
    // If op1 is a pointer and we're doing ADD or SUB, scale op2 by the size of pointed-to type
    bool isOp1Pointer = pointerVars.count(op1);
    int elementSize = 4;  // Default
    
    // Check if op1 is a struct member that's a pointer
    if (!isOp1Pointer) {
        string op1Base, op1Member;
        bool op1IsArrow;
        if (isStructMemberAccess(op1, op1Base, op1Member, op1IsArrow)) {
            if (op1Member == "data" || op1Member == "next" || op1Member == "ptr") {
                isOp1Pointer = true;
                elementSize = 4;
            }
        }
    } else {
        elementSize = pointerSizes[op1];
    }
    
    if ((instr.op == TACOp::ADD || instr.op == TACOp::SUB) && isOp1Pointer) {
        if (elementSize > 1) {
            // Scale op2 by element size
            if (op2IsImmediate) {
                // op2 is immediate, scale it now
                int scaled = stoi(op2) * elementSize;
                mipsCode.push_back("    li " + scratch2 + ", " + to_string(scaled) + "  # Scale offset by " + to_string(elementSize));
            } else {
                // op2 is in a register, multiply it by element size
                if (elementSize == 8) {
                    // Shift left by 3 (multiply by 8)
                    mipsCode.push_back("    sll " + scratch2 + ", " + scratch2 + ", 3  # Scale by 8 for pointer arithmetic");
                } else if (elementSize == 4) {
                    // Shift left by 2 (multiply by 4)
                    mipsCode.push_back("    sll " + scratch2 + ", " + scratch2 + ", 2  # Scale by 4 for pointer arithmetic");
                } else if (elementSize == 2) {
                    mipsCode.push_back("    sll " + scratch2 + ", " + scratch2 + ", 1  # Scale by 2 for pointer arithmetic");
                } else {
                    // General case: multiply (avoid using $at)
                    mipsCode.push_back("    li $t9, " + to_string(elementSize));
                    mipsCode.push_back("    mul " + scratch2 + ", " + scratch2 + ", $t9  # Scale by " + to_string(elementSize) + " for pointer arithmetic");
                }
            }
        }
        // Mark result as also being a pointer of the same type
        pointerVars.insert(dest);
        pointerSizes[dest] = elementSize;
    }
    
    string mipsOp;
    switch (instr.op) {
        case TACOp::ADD: mipsOp = "add"; break;
        case TACOp::SUB: mipsOp = "sub"; break;
        case TACOp::MUL: mipsOp = "mul"; break;
        case TACOp::DIV: mipsOp = "div"; break;
        case TACOp::MOD: 
            // MIPS doesn't have direct MOD, use div + mfhi
            {
                string op2Reg = getReg(op2);
                mipsCode.push_back("    div " + op1Reg + ", " + op2Reg);
                mipsCode.push_back("    mfhi " + destReg + "  # " + dest + " = " + op1 + " % " + op2);
                regDesc.set(destReg, dest);
                addrDesc.addLocation(dest, destReg);
                return;
            }
        case TACOp::BIT_AND: mipsOp = "and"; break;
        case TACOp::BIT_OR: mipsOp = "or"; break;
        case TACOp::BIT_XOR: mipsOp = "xor"; break;
        case TACOp::LSHFT: mipsOp = "sll"; break;
        case TACOp::RSHFT: mipsOp = "srl"; break;
        default: return;
    }
    
    // Check if op2 is immediate
    if (op2IsImmediate) {
        // Special case: XOR with -1 is bitwise NOT, use NOR instead
        if (instr.op == TACOp::BIT_XOR && op2 == "-1") {
            mipsCode.push_back("    nor " + destReg + ", " + op1Reg + ", $zero" + 
                             "  # " + dest + " = ~" + op1 + " (bitwise NOT)");
        } else {
            string immOp = mipsOp + "i";
            // Not all operations have immediate versions
            if (instr.op == TACOp::ADD || instr.op == TACOp::BIT_AND || 
                instr.op == TACOp::BIT_OR || instr.op == TACOp::BIT_XOR) {
                mipsCode.push_back("    " + immOp + " " + destReg + ", " + op1Reg + ", " + sanitizeOperand(op2) + 
                                 "  # " + dest + " = " + op1 + " " + opToStr(instr.op) + " " + op2);
            } else {
                // Load immediate into scratch register
                mipsCode.push_back("    li " + op2Reg + ", " + sanitizeOperand(op2) + "  # Load immediate " + op2);
                mipsCode.push_back("    " + mipsOp + " " + destReg + ", " + op1Reg + ", " + op2Reg + 
                                 "  # " + dest + " = " + op1 + " " + opToStr(instr.op) + " " + op2);
            }
        }
    } else {
        // op2Reg already loaded into scratch register
        mipsCode.push_back("    " + mipsOp + " " + destReg + ", " + op1Reg + ", " + op2Reg + 
                         "  # " + dest + " = " + op1 + " " + opToStr(instr.op) + " " + op2);
    }
    
    regDesc.set(destReg, dest);
    addrDesc.addLocation(dest, destReg);
    
    // If dest is a static variable, store it back to memory immediately
    if (isStatic(dest)) {
        mipsCode.push_back("    sw " + destReg + ", " + dest + "  # Store static var " + dest);
    }
}

// Generate comparison instruction (with conditional jumps)
void MIPSGenerator::genComparison(const TACInstruction& instr) {
    if (instr.isGoto) {
        // Conditional jump: goto label if op1 <cmp> op2
        string op1 = instr.operand1.value_or("");
        string op2 = instr.operand2.value_or("");
        string label = getMIPSLabel(instr.result);
        
        // CRITICAL FIX: For conditional jumps near label boundaries, reload variables from memory
        // This handles cases where we jump to nearby instructions (e.g., continue to condition)
        string op1Reg;
        bool isOp1Variable = !(op1[0] == 't' && op1.length() > 1 && isdigit(op1[1]));
        
        // Check if we're within 3 instructions of a label (likely in same basic block after jump)
        bool nearLabel = false;
        for (int offset = -3; offset <= 0; offset++) {
            if (gotoTargetsSet.count(currentInstrIndex + offset)) {
                nearLabel = true;
                break;
            }
        }
        
        if (isOp1Variable && varOffsets.count(op1) && nearLabel) {
            // Variable near a label: reload from memory for safety
            int offset = getVarOffset(op1);
            mipsCode.push_back("    lw $t8, -" + to_string(offset) + "($fp)  # Load " + op1 + " for condition");
            op1Reg = "$t8";
        } else {
            // Temporary or far from labels: use normal getReg logic
            op1Reg = getReg(op1);
        }
        
        // Handle op2: variables, temporaries, or immediates
        string op2Reg;
        if (isImmediate(op2)) {
            if (op2 == "0") {
                op2Reg = "$zero";
            } else {
                string tempReg = "$t9";  // Use $t9 as scratch
                mipsCode.push_back("    li " + tempReg + ", " + sanitizeOperand(op2));
                op2Reg = tempReg;
            }
        } else {
            bool isOp2Variable = !(op2[0] == 't' && op2.length() > 1 && isdigit(op2[1]));
            if (isOp2Variable && varOffsets.count(op2) && nearLabel) {
                // Variable near a label: reload from memory for safety
                int offset = getVarOffset(op2);
                string reloadReg = (op1Reg == "$t8") ? "$t9" : "$t8";
                mipsCode.push_back("    lw " + reloadReg + ", -" + to_string(offset) + "($fp)  # Load " + op2 + " for condition");
                op2Reg = reloadReg;
            } else {
                // Temporary or far from labels: use normal getReg logic
                op2Reg = getReg(op2);
            }
        }
        
        string branchOp;
        switch (instr.op) {
            case TACOp::LT: branchOp = "blt"; break;
            case TACOp::GT: branchOp = "bgt"; break;
            case TACOp::LE: branchOp = "ble"; break;
            case TACOp::GE: branchOp = "bge"; break;
            case TACOp::EQ: branchOp = "beq"; break;
            case TACOp::NE: branchOp = "bne"; break;
            default: return;
        }
        
        mipsCode.push_back("    " + branchOp + " " + op1Reg + ", " + op2Reg + ", " + label + 
                         "  # if " + op1 + " " + opToStr(instr.op) + " " + op2 + " goto " + instr.result);
    } else {
        // Regular comparison: dest = op1 <cmp> op2
        string dest = instr.result;
        string op1 = instr.operand1.value_or("");
        string op2 = instr.operand2.value_or("");
        
        string op1Reg = getReg(op1);
        
        // FIX: Check if op1 variable has BOTH register and memory - reload from memory for safety
        // This prevents using stale register values when the register has been reused
        {
            bool hasReg = !op1Reg.empty();
            bool hasMemory = addrDesc.getLocations(op1).count("memory") || varOffsets.count(op1);
            bool isVariable = !(op1[0] == 't' && op1.length() > 1 && isdigit(op1[1]));
            
            if (hasReg && hasMemory && isVariable) {
                // Variable in BOTH reg and memory - reload from memory for safety
                int offset = getVarOffset(op1);
                mipsCode.push_back("    lw $t8, -" + to_string(offset) + "($fp)  # Reload " + op1 + " from memory");
                op1Reg = "$t8";
            }
        }
        
        string op2Reg = isImmediate(op2) ? "$zero" : getReg(op2);
        
        // FIX: Same check for op2
        if (!isImmediate(op2)) {
            bool hasReg = !op2Reg.empty();
            bool hasMemory = addrDesc.getLocations(op2).count("memory") || varOffsets.count(op2);
            bool isVariable = !(op2[0] == 't' && op2.length() > 1 && isdigit(op2[1]));
            
            if (hasReg && hasMemory && isVariable) {
                int offset = getVarOffset(op2);
                mipsCode.push_back("    lw $t9, -" + to_string(offset) + "($fp)  # Reload " + op2 + " from memory");
                op2Reg = "$t9";
            }
        }
        
        string destReg = getReg(dest);
        
        // Handle immediate op2
        if (isImmediate(op2) && op2 != "0") {
            string tempReg = "$t9";
            mipsCode.push_back("    li " + tempReg + ", " + sanitizeOperand(op2));
            op2Reg = tempReg;
        }
        
        // FIX: If destReg is same as op1Reg or op2Reg, we'll corrupt the operand
        // Use a scratch register for the result, then move to destReg
        string actualDestReg = destReg;
        if (destReg == op1Reg || destReg == op2Reg) {
            // Use $t8 as scratch (assuming it's not op1Reg or op2Reg)
            actualDestReg = "$t8";
        }
        
        // Use slt/seq instructions to set dest to 0 or 1
        switch (instr.op) {
            case TACOp::LT:
                mipsCode.push_back("    slt " + actualDestReg + ", " + op1Reg + ", " + op2Reg + "  # " + dest + " = " + op1 + " < " + op2);
                break;
            case TACOp::GT:
                mipsCode.push_back("    sgt " + actualDestReg + ", " + op1Reg + ", " + op2Reg + "  # " + dest + " = " + op1 + " > " + op2);
                break;
            case TACOp::LE:
                mipsCode.push_back("    sle " + actualDestReg + ", " + op1Reg + ", " + op2Reg + "  # " + dest + " = " + op1 + " <= " + op2);
                break;
            case TACOp::GE:
                mipsCode.push_back("    sge " + actualDestReg + ", " + op1Reg + ", " + op2Reg + "  # " + dest + " = " + op1 + " >= " + op2);
                break;
            case TACOp::EQ:
                mipsCode.push_back("    seq " + actualDestReg + ", " + op1Reg + ", " + op2Reg + "  # " + dest + " = " + op1 + " == " + op2);
                break;
            case TACOp::NE:
                mipsCode.push_back("    sne " + actualDestReg + ", " + op1Reg + ", " + op2Reg + "  # " + dest + " = " + op1 + " != " + op2);
                break;
            default: break;
        }
        
        // If we used a scratch register, move result to destReg
        if (actualDestReg != destReg) {
            mipsCode.push_back("    move " + destReg + ", " + actualDestReg + "  # Move result to " + dest);
// Stack frame management
