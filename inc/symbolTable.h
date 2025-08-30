#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include<bits/stdc++.h>
#include "ast.h"
using namespace std;

// Forward declaration
extern set<string> typedefNames;
extern void registerTypedefName(const string& name);

vector<tuple<string, string, string>> symbolTable;
map<string, string> typedefMap;  // Maps typedef name to actual type

void addToSymbolTable(const string& token, const string& tokenType, const string& extraInfo="") {
    symbolTable.emplace_back(token, tokenType, extraInfo);
}

void addConstantsToSymbolTable(ASTNode *a){
        addToSymbolTable(a->valueToString(), ASTNode::nodeTypeToString(a->type));
}

void addStructMembersToSymbolTable(ASTNode *structOrUnionSpecifier) {
if (!structOrUnionSpecifier) return;

string structName;
vector<pair<string, string>> members;

for (ASTNode* child : structOrUnionSpecifier->children) {
    if (!child) continue;

    string nodeType = ASTNode::nodeTypeToString(child->type);

    if (nodeType == "IDENTIFIER") {
        structName = child->valueToString();
    } 
    else if (nodeType == "STRUCT_DECLARATION_LIST") {
        for (ASTNode* structDecl : child->children) {
            if (!structDecl) continue;

            string typeSpecifiers;
            ASTNode* declaratorList = nullptr;

            for (ASTNode* declChild : structDecl->children) {
                if (!declChild) continue;
                
                string declType = ASTNode::nodeTypeToString(declChild->type);

                if (declType == "KEYWORD" || declType == "IDENTIFIER") {
                    if (!typeSpecifiers.empty()) typeSpecifiers += " ";
                    typeSpecifiers += declChild->valueToString();
                } 
                else if (declType == "STRUCT_DECLARATOR_LIST") {
                    declaratorList = declChild;
                }
            }

            if (!typeSpecifiers.empty() && declaratorList) {
                for (ASTNode* declarator : declaratorList->children) {
                    if (!declarator || declarator->children.empty()) continue;

                    string varName;
                    string varType = typeSpecifiers;
                    int pointerCount = 0;
                    vector<string> dimensions;

                    ASTNode* current = declarator;

                    while (current) {
                        string nodeType = ASTNode::nodeTypeToString(current->type);

                        if (nodeType == "DECLARATOR") {
                            if (!current->children.empty()) {
                                current = current->children[0];
                                continue;
                            }
                        } 
                        else if (nodeType == "POINTER") {
                            pointerCount++;
                        } 
                        else if (nodeType == "ARRAY") {
                            dimensions.push_back(current->children[1] ? current->children[1]->valueToString() : "");
                        } 
                        else {
                            varName = current->valueToString();
                        }

                        current = current->children.empty() ? nullptr : current->children[0];
                    }

                    varType.append(pointerCount, '*');

                    for (const string& dim : dimensions) {
                        varType += "[" + dim + "]";
                    }

                    members.emplace_back(varName, varType);
                }
            }
        }
    }
}

if (!structName.empty()) {
    addToSymbolTable(structName, "struct");
}

for (const auto& [varName, varType] : members) {
    addToSymbolTable(varName, varType);
}
}

vector<string> extractInitDeclarators(ASTNode* initDeclaratorList) {
    vector<string> identifiers;
    if (!initDeclaratorList) return identifiers;

    for (ASTNode* initDeclarator : initDeclaratorList->children) {
        ASTNode* declarator = initDeclarator->children[0];
        while (!declarator->children.empty()) 
            declarator = declarator->children[0];
        
        identifiers.push_back(declarator->valueToString());
    }
    return identifiers;
}


void addDeclaratorsToSymbolTable(ASTNode *a, ASTNode* b) {
    string typeSpecifiers;
    for (ASTNode* specifier : a->children) {
        if (specifier) {
            if (!typeSpecifiers.empty()) {
                typeSpecifiers += " ";
            }
            typeSpecifiers += specifier->valueToString();
        }
    }

    for (ASTNode* declarator : b->children) {
        if (!declarator || declarator->children.empty()) continue;

        string varName;
        string varType = typeSpecifiers;
        int pointerCount = 0;
        vector<string> dimensions;

        ASTNode* current = declarator;

        while (current) {
            string nodeType = ASTNode::nodeTypeToString(current->type);
            
            if (nodeType == "DECLARATOR") {
                if (!current->children.empty()) {
                    current = current->children[0];
                    continue;
                }
            } else if (nodeType == "POINTER") {
                pointerCount++;
            } else if (nodeType == "ARRAY") {
                dimensions.push_back(current->children[1] ? current->children[1]->valueToString() : "");
            } else {
                varName = current->valueToString();
            }
            
            current = current->children.empty() ? nullptr : current->children[0];
        }

        varType.append(pointerCount, '*');

        for (const string& dim : dimensions) {
            varType += "[" + dim + "]";
        }

        addToSymbolTable(varName, varType);
    }
}

void addFunctionToSymbolTable(ASTNode* declarationSpecifiersNode, ASTNode* declaratorNode) {
if (!declarationSpecifiersNode || !declaratorNode) return;

ASTNode* current = declaratorNode;
while (current && !current->children.empty()) {
    current = current->children[0];
    if (current->type == NODE_IDENTIFIER) {
        string functionName = current->valueToString();
        addToSymbolTable(functionName, "function");
        return;
    }
}
cerr << "Error: Function name not found in declarator!" << endl;
}

void addStructVariablesToSymbolTable(ASTNode* structSpecifierNode, ASTNode* initDeclaratorList) {
    if (!structSpecifierNode || !initDeclaratorList) return;
    ASTNode* structNameNode = structSpecifierNode->children[1];
    string structName = "struct " + structNameNode->valueToString();

    vector<string> identifiers = extractInitDeclarators(initDeclaratorList);
    
    for(const auto id : identifiers) addToSymbolTable(id, structName);
}


void printSymbolTable() {
    cout << left << setw(30) << "Token" << setw(30) << "TokenType" << setw(40) << "Extra Info" << endl;
    cout << string(100, '-') << endl;

    for (const auto& entry : symbolTable) {
        cout << left << setw(30) << get<0>(entry)
             << setw(30) << get<1>(entry)
             << setw(40) << get<2>(entry)
             << endl;
    }
}

void addClassMembersToSymbolTable(ASTNode* classSpecifierNode) {
    string className = classSpecifierNode->children[0]->valueToString();
    addToSymbolTable(className, "class");
    // if (classSpecifierNode->children.size() <= 2 || classSpecifierNode->children[2] == nullptr) {
    //     return;  // No members
    // }
    // ASTNode* memberDeclarationListNode = classSpecifierNode->children[2];
    // string currentAccess = "private";  // Default access for class
    // for (ASTNode* memberNode : memberDeclarationListNode->children) {
    //     if (memberNode == nullptr || memberNode->type != NODE_DECLARATION) continue;  // Skip non-declarations
    //     if (memberNode->children.size() < 2 || memberNode->children[0] == nullptr || memberNode->children[1] == nullptr) continue;  // Must have type and declarators
    //     ASTNode* typeNode = memberNode->children[0];
    //     string memberType = typeNode->valueToString();
    //     ASTNode* initDeclaratorList = memberNode->children[1];
    //     for (ASTNode* declaratorNode : initDeclaratorList->children) {
    //         if (declaratorNode == nullptr || declaratorNode->children.empty()) continue;
    //         ASTNode* current = declaratorNode->children[0];  // First child is NODE_DECLARATOR
    //         while (current && current->type == NODE_DECLARATOR && !current->children.empty()) {
    //             current = current->children[0];
    //         }
    //         if (current == nullptr || current->valueToString().empty()) continue;
    //         string memberName = current->valueToString();
    //         addToSymbolTable(memberName, memberType + " (" + currentAccess + ")", className);
    //     }
    // }
}

void addClassVariablesToSymbolTable(ASTNode* classSpecifierNode, ASTNode* initDeclaratorList) {

    ASTNode* classNameNode = classSpecifierNode->children[0];
    string className = "class " + classNameNode->valueToString();

    vector<string> identifiers = extractInitDeclarators(initDeclaratorList);
    for(const auto id : identifiers) addToSymbolTable(id, className);

    }

// Handle typedef declarations
void addTypedefToSymbolTable(ASTNode* declarationSpecifiers, ASTNode* initDeclaratorList) {
    if (!declarationSpecifiers || !initDeclaratorList) return;

    // Build the base type from declaration specifiers (excluding typedef keyword)
    string baseType;
    bool hasStructOrClass = false;
    string structOrClassName;
    
    for (ASTNode* specifier : declarationSpecifiers->children) {
        if (!specifier) continue;
        
        string nodeType = ASTNode::nodeTypeToString(specifier->type);
        string value = specifier->valueToString();
        
        // Skip typedef keyword itself
        if (value == "typedef") continue;
        
        if (nodeType == "STRUCT_SPECIFIER") {
            hasStructOrClass = true;
            // Extract struct name if present
            // Check if children[1] is an IDENTIFIER (named struct) or STRUCT_DECLARATION_LIST (anonymous)
            if (specifier->children.size() >= 2 && specifier->children[1]) {
                string child1Type = ASTNode::nodeTypeToString(specifier->children[1]->type);
                if (child1Type == "IDENTIFIER") {
                    structOrClassName = "struct " + specifier->children[1]->valueToString();
                } else {
                    structOrClassName = "struct <anonymous>";
                }
            } else {
                structOrClassName = "struct";
            }
            if (!baseType.empty()) baseType += " ";
            baseType += structOrClassName;
        } else if (nodeType == "CLASS_SPECIFIER") {
            hasStructOrClass = true;
            if (specifier->children.size() >= 1 && specifier->children[0]) {
                structOrClassName = "class " + specifier->children[0]->valueToString();
            } else {
                structOrClassName = "class";
            }
            if (!baseType.empty()) baseType += " ";
            baseType += structOrClassName;
        } else {
            if (!baseType.empty()) baseType += " ";
            baseType += value;
        }
    }

    // Process each declarator to create typedef aliases
    for (ASTNode* initDeclarator : initDeclaratorList->children) {
        if (!initDeclarator || initDeclarator->children.empty()) continue;
        
        ASTNode* declarator = initDeclarator->children[0];
        if (!declarator) continue;

        string typedefName;
        string finalType = baseType;
        int pointerCount = 0;
        vector<string> dimensions;

        ASTNode* current = declarator;

        // Traverse declarator to extract name, pointers, and arrays
        while (current) {
            string nodeType = ASTNode::nodeTypeToString(current->type);
            
            if (nodeType == "DECLARATOR") {
                if (!current->children.empty()) {
                    current = current->children[0];
                    continue;
                }
            } else if (nodeType == "POINTER") {
                pointerCount++;
            } else if (nodeType == "ARRAY") {
                dimensions.push_back(current->children[1] ? current->children[1]->valueToString() : "");
            } else if (nodeType == "IDENTIFIER") {
                typedefName = current->valueToString();
            }
            
            current = current->children.empty() ? nullptr : current->children[0];
        }

        // Build final type with pointers and arrays
        finalType.append(pointerCount, '*');
        for (const string& dim : dimensions) {
            finalType += "[" + dim + "]";
        }

        if (!typedefName.empty()) {
            // Add to typedef map
            typedefMap[typedefName] = finalType;
            // Add to symbol table with extra info
            addToSymbolTable(typedefName, "typedef", "-> " + finalType);
            // Register with lexer for feedback
            registerTypedefName(typedefName);
        }
    }
}

// Modified version to resolve typedef'd types
void addDeclaratorsWithTypedefToSymbolTable(ASTNode* declarationSpecifiers, ASTNode* initDeclaratorList) {
    if (!declarationSpecifiers || !initDeclaratorList) return;

    string typeSpecifiers;
    string typedefLink;
    
    for (ASTNode* specifier : declarationSpecifiers->children) {
        if (!specifier) continue;
        
        string value = specifier->valueToString();
        string nodeType = ASTNode::nodeTypeToString(specifier->type);
        
        // Check if it's a typedef'd type
        if (nodeType == "IDENTIFIER" && typedefMap.find(value) != typedefMap.end()) {
            typeSpecifiers = typedefMap[value];
            typedefLink = " (typedef: " + value + ")";
        } else {
            if (!typeSpecifiers.empty()) typeSpecifiers += " ";
            typeSpecifiers += value;
        }
    }

    // Process each declarator
    for (ASTNode* declarator : initDeclaratorList->children) {
        if (!declarator || declarator->children.empty()) continue;

        string varName;
        string varType = typeSpecifiers;
        int pointerCount = 0;
        vector<string> dimensions;

        ASTNode* current = declarator;

        while (current) {
            string nodeType = ASTNode::nodeTypeToString(current->type);
            
            if (nodeType == "DECLARATOR") {
                if (!current->children.empty()) {
                    current = current->children[0];
                    continue;
                }
            } else if (nodeType == "POINTER") {
                pointerCount++;
            } else if (nodeType == "ARRAY") {
                dimensions.push_back(current->children[1] ? current->children[1]->valueToString() : "");
            } else {
                varName = current->valueToString();
            }
            
            current = current->children.empty() ? nullptr : current->children[0];
        }

        varType.append(pointerCount, '*');

        for (const string& dim : dimensions) {
            varType += "[" + dim + "]";
        }

        addToSymbolTable(varName, varType, typedefLink);
    }
}

#endif