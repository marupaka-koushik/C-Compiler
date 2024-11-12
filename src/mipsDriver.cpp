// MIPS Code Generator Driver
// This file demonstrates how to use the MIPS generator with your existing 3AC code

#include "../inc/mipsGenerator.h"
#include "../inc/symbolTable.h"
#include <iostream>
#include <fstream>

using namespace std;

// External reference to the global code generator from parser
extern CodeGenerator codeGen;

// Function to generate MIPS code from 3AC and write to file
void generateMIPSFromTAC(const string& outputFilename) {
    cout << "\n========================================" << endl;
    cout << "MIPS CODE GENERATION" << endl;
    cout << "========================================\n" << endl;
    
    // Create MIPS generator
    MIPSGenerator mipsGen;
    
    // Generate MIPS from 3AC
    cout << "Generating MIPS assembly from " << codeGen.tacCode.size() << " 3AC instructions..." << endl;
    mipsGen.generateMIPS(codeGen.tacCode);
    
    // Write to file
    cout << "Writing MIPS code to: " << outputFilename << endl;
    mipsGen.writeToFile(outputFilename);
    
    cout << "\nMIPS code generation completed successfully!" << endl;
    cout << "========================================\n" << endl;
    
    // Print descriptors for debugging (optional)
    #ifdef DEBUG_MIPS
    mipsGen.printDescriptors();
    #endif
}

// Alternative: Generate MIPS and return as string
string generateMIPSAsString() {
    MIPSGenerator mipsGen;
    mipsGen.generateMIPS(codeGen.tacCode);
    return mipsGen.getOutput();
}

// Example usage (if compiling standalone)
#ifdef STANDALONE_MIPS_DRIVER
int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <output.asm>" << endl;
        return 1;
    }
    
    // This is just an example - normally the codeGen would be populated by the parser
    cout << "Note: In standalone mode, the 3AC code should already be in codeGen.tacCode" << endl;
    
    generateMIPSFromTAC(argv[1]);
    
    return 0;
}
#endif
