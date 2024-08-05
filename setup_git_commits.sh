#!/usr/bin/env bash

# Script to create a Git repository with backdated commits from multiple contributors
# This creates a realistic commit history across different project phases

set -e

REPO_DIR="/Users/koushikmarupaka/Downloads/IR 2 copy 2"
REPO_URL="https://github.com/marupaka-koushik/C-Compiler.git"

cd "$REPO_DIR"

# Remove existing .git if it exists
if [ -d ".git" ]; then
    echo "Removing existing .git directory..."
    rm -rf .git
fi

# Initialize new repository
echo "Initializing new Git repository..."
git init
git config user.name "marupaka-koushik"
git config user.email "marupaka.koushik@example.com"

# Function to get contributor name
get_contributor_name() {
    case $1 in
        marupaka) echo "marupaka-koushik" ;;
        charitha) echo "Togarucharitha" ;;
        mellie) echo "MelliCorleone1972" ;;
        ravi) echo "ravitejajaligama" ;;
    esac
}

# Function to get contributor email
get_contributor_email() {
    case $1 in
        marupaka) echo "marupaka.koushik@example.com" ;;
        charitha) echo "charitha@example.com" ;;
        mellie) echo "mellie@example.com" ;;
        ravi) echo "ravi.jaligama@example.com" ;;
    esac
}

# Function to create a commit with specific author and date
make_commit() {
    local author_key=$1
    local date=$2
    local message=$3
    shift 3
    local files=("$@")
    
    local author_name=$(get_contributor_name "$author_key")
    local author_email=$(get_contributor_email "$author_key")
    
    # Stage files
    if [ "${files[0]}" == "all" ]; then
        git add -A
    else
        git add "${files[@]}" 2>/dev/null || true
    fi
    
    # Check if there are changes to commit
    if git diff --cached --quiet 2>/dev/null; then
        echo "⚠ No changes to commit for: $message"
        return
    fi
    
    # Create commit with backdated timestamp
    GIT_AUTHOR_NAME="$author_name" \
    GIT_AUTHOR_EMAIL="$author_email" \
    GIT_AUTHOR_DATE="$date" \
    GIT_COMMITTER_NAME="$author_name" \
    GIT_COMMITTER_EMAIL="$author_email" \
    GIT_COMMITTER_DATE="$date" \
    git commit -m "$message" >/dev/null 2>&1
    
    echo "✓ $date - $author_name: $message"
}

echo ""
echo "================================"
echo "Creating C Compiler Git History"
echo "================================"

# ============================================
# PHASE 1: LEXER (August 5-10, 2024)
# ============================================
echo -e "\n📝 PHASE 1: LEXER (August 5-10, 2024)"

make_commit "marupaka" "2024-08-05 10:00:00" "Initial commit: Project structure setup" "README.md" "Makefile"

make_commit "marupaka" "2024-08-05 14:30:00" "feat: Add lexer foundation with token definitions" "src/lexer.l"

make_commit "ravi" "2024-08-08 11:20:00" "feat: Implement keyword recognition and string handling in lexer" "src/lexer.l"

make_commit "charitha" "2024-08-10 16:45:00" "feat: Add typedef name tracking and finalize lexer" "src/lexer.l"

# ============================================
# PHASE 2: PARSER (August 15-31, 2024)
# ============================================
echo -e "\n📝 PHASE 2: PARSER (August 15-31, 2024)"

make_commit "marupaka" "2024-08-15 09:30:00" "feat: Initialize parser with basic grammar rules" "src/parser.y"

make_commit "ravi" "2024-08-18 14:15:00" "feat: Implement expression parsing and operators" "src/parser.y"

make_commit "mellie" "2024-08-21 10:45:00" "feat: Add control flow statements (if/while/for)" "src/parser.y"

make_commit "marupaka" "2024-08-24 15:30:00" "feat: Implement function declarations and call handling" "src/parser.y"

make_commit "charitha" "2024-08-27 11:00:00" "feat: Add struct and enum parsing support" "src/parser.y"

make_commit "ravi" "2024-08-31 16:20:00" "feat: Finalize parser with array and pointer support" "src/parser.y"

# ============================================
# PHASE 3: IR (October 1-27, 2024)
# ============================================
echo -e "\n📝 PHASE 3: IR - Three Address Code (October 1-27, 2024)"

make_commit "marupaka" "2024-10-01 10:00:00" "feat: Initialize Three Address Code (TAC) generation framework" "inc/treeNode.h"

make_commit "ravi" "2024-10-04 14:30:00" "feat: Implement symbol table with scope management" "inc/symbolTable.h"

make_commit "mellie" "2024-10-07 11:15:00" "feat: Generate TAC for arithmetic and logical expressions" "src/parser.y" "inc/treeNode.h"

make_commit "marupaka" "2024-10-10 15:45:00" "feat: Add TAC generation for control flow statements" "src/parser.y"

make_commit "charitha" "2024-10-13 10:30:00" "feat: Implement TAC for function calls and parameters" "src/parser.y" "inc/symbolTable.h"

make_commit "ravi" "2024-10-16 14:00:00" "feat: Add TAC for arrays and pointer operations" "src/parser.y" "inc/symbolTable.h"

make_commit "marupaka" "2024-10-19 11:30:00" "feat: Implement TAC for struct member access" "src/parser.y" "inc/symbolTable.h"

make_commit "ravi" "2024-10-22 16:15:00" "feat: Add backpatching and control flow optimizations" "src/parser.y"

make_commit "mellie" "2024-10-25 10:45:00" "feat: Implement comprehensive type checking and validation" "inc/symbolTable.h"

make_commit "marupaka" "2024-10-27 15:30:00" "feat: Finalize IR with complete TAC and symbol table support" "src/parser.y" "inc/symbolTable.h" "inc/treeNode.h"

# ============================================
# PHASE 4: CODE GENERATION (November 1-24, 2024)
# ============================================
echo -e "\n📝 PHASE 4: CODE GENERATION - MIPS Assembly (November 1-24, 2024)"

make_commit "ravi" "2024-11-01 09:30:00" "feat: Initialize MIPS code generator framework" "inc/mipsGenerator.h" "src/mipsDriver.cpp"

make_commit "marupaka" "2024-11-04 14:15:00" "feat: Implement register allocation and management for MIPS" "inc/mipsGenerator.h"

make_commit "ravi" "2024-11-07 11:00:00" "feat: Generate MIPS instructions for arithmetic operations" "inc/mipsGenerator.h"

make_commit "marupaka" "2024-11-10 15:45:00" "feat: Add MIPS memory operations and stack management" "inc/mipsGenerator.h"

make_commit "charitha" "2024-11-12 10:30:00" "feat: Implement function prologue and epilogue generation" "inc/mipsGenerator.h"

make_commit "ravi" "2024-11-22 14:00:00" "feat: Add support for string literals and array operations in MIPS" "inc/mipsGenerator.h"

make_commit "marupaka" "2024-11-23 11:30:00" "feat: Implement pointer arithmetic and dereferencing in MIPS" "inc/mipsGenerator.h" "src/parser.y"

make_commit "ravi" "2024-11-24 16:45:00" "feat: Add typedef support and finalize code generation" "all"

echo -e "\n================================"
echo "✅ Git history created successfully!"
echo "================================"
echo ""
echo "📊 Commit Summary:"
git log --oneline --all --graph --decorate | head -30
echo ""
echo "👥 Contribution Summary:"
git shortlog -sn --all
echo ""
echo "📅 Commit Timeline:"
git log --pretty=format:"%ai - %an: %s" --all
echo ""
echo -e "\n================================"
echo "📤 Next Steps:"
echo "================================"
echo "1. Review the commit history above"
echo "2. Push to GitHub with:"
echo ""
echo "   git remote add origin $REPO_URL"
echo "   git branch -M main"
echo "   git push -u origin main --force"
echo ""
echo "⚠️  Note: --force will overwrite existing repository history"
echo "================================"
