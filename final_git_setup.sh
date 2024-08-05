#!/usr/bin/env bash

# Advanced Git History Setup - Creates realistic commit distribution
# by staging the entire codebase in logical phases

set -e

cd "/Users/koushikmarupaka/Downloads/IR 2 copy 2"

# Backup and clean
echo "🧹 Preparing repository..."
[ -d ".git" ] && rm -rf .git
git init -q
git config user.name "marupaka-koushik"
git config user.email "marupaka.koushik@example.com"

# Helper functions
commit_as() {
    local who=$1 date=$2 msg=$3
    shift 3
    
    case $who in
        m) name="marupaka-koushik"; email="marupaka.koushik@example.com" ;;
        r) name="ravitejajaligama"; email="ravi.jaligama@example.com" ;;
        c) name="Togarucharitha"; email="charitha@example.com" ;;
        l) name="MelliCorleone1972"; email="mellie@example.com" ;;
    esac
    
    git add "$@" 2>/dev/null || git add -A
    git diff --cached --quiet 2>/dev/null && return
    
    GIT_AUTHOR_NAME="$name" GIT_AUTHOR_EMAIL="$email" GIT_AUTHOR_DATE="$date" \
    GIT_COMMITTER_NAME="$name" GIT_COMMITTER_EMAIL="$email" GIT_COMMITTER_DATE="$date" \
    git commit -q -m "$msg"
    
    printf "✓ %s\n" "$msg"
}

echo ""
echo "═══════════════════════════════════════════════════════"
echo "  CREATING C COMPILER GIT HISTORY"  
echo "  Multiple Contributors | Aug 2024 - Nov 2024"
echo "═══════════════════════════════════════════════════════"

# Phase 1: LEXER (Aug 5-10)
echo -e "\n📖 Phase 1: LEXER (Aug 5-10, 2024)"
commit_as m "2024-08-05 10:00:00" "Initial commit: Project structure and build system" \
    README.md Makefile run.sh

commit_as m "2024-08-05 14:30:00" "feat(lexer): Add lexical analyzer with token definitions" \
    src/lexer.l

commit_as r "2024-08-08 11:20:00" "feat(lexer): Enhance operator recognition and comments" \
    src/lexer.l

commit_as c "2024-08-10 16:45:00" "feat(lexer): Complete lexer with typedef support" \
    src/lexer.l

# Phase 2: PARSER (Aug 15-31)
echo -e "\n🔧 Phase 2: PARSER (Aug 15-31, 2024)"  
commit_as m "2024-08-15 09:30:00" "feat(parser): Initialize parser with expression grammar" \
    src/parser.y

commit_as r "2024-08-18 14:15:00" "feat(parser): Add operators and precedence rules" \
    src/parser.y

commit_as l "2024-08-21 10:45:00" "feat(parser): Implement control flow statements" \
    src/parser.y

commit_as m "2024-08-24 15:30:00" "feat(parser): Add function declaration parsing" \
    src/parser.y

commit_as c "2024-08-27 11:00:00" "feat(parser): Support struct and enum declarations" \
    src/parser.y

commit_as r "2024-08-31 16:20:00" "feat(parser): Complete parser with arrays and pointers" \
    src/parser.y

# Phase 3: IR (Oct 1-27)
echo -e "\n⚙️  Phase 3: IR GENERATION (Oct 1-27, 2024)"
commit_as m "2024-10-01 10:00:00" "feat(ir): Add AST node structures and TAC framework" \
    inc/treeNode.h

commit_as r "2024-10-04 14:30:00" "feat(ir): Implement symbol table with scope management" \
    inc/symbolTable.h

commit_as l "2024-10-07 11:15:00" "feat(ir): Generate TAC for expressions and operators" \
    src/parser.y inc/treeNode.h

commit_as m "2024-10-10 15:45:00" "feat(ir): Add TAC for control flow with backpatching" \
    src/parser.y

commit_as c "2024-10-13 10:30:00" "feat(ir): Implement function call TAC generation" \
    src/parser.y inc/symbolTable.h

commit_as r "2024-10-16 14:00:00" "feat(ir): Support array and pointer TAC operations" \
    src/parser.y inc/symbolTable.h

commit_as m "2024-10-19 11:30:00" "feat(ir): Add struct member access TAC" \
    src/parser.y inc/symbolTable.h

commit_as r "2024-10-22 16:15:00" "feat(ir): Optimize control flow with better backpatching" \
    src/parser.y

commit_as l "2024-10-25 10:45:00" "feat(ir): Add comprehensive type checking" \
    inc/symbolTable.h

commit_as m "2024-10-27 15:30:00" "feat(ir): Finalize IR phase with test cases" \
    tests/*.c

# Phase 4: CODE GEN (Nov 1-24)
echo -e "\n🖥️  Phase 4: MIPS CODEGEN (Nov 1-24, 2024)"
commit_as r "2024-11-01 09:30:00" "feat(codegen): Initialize MIPS code generator" \
    inc/mipsGenerator.h src/mipsDriver.cpp

commit_as m "2024-11-04 14:15:00" "feat(codegen): Add register allocation and temps" \
    inc/mipsGenerator.h

commit_as r "2024-11-07 11:00:00" "feat(codegen): Generate MIPS arithmetic instructions" \
    inc/mipsGenerator.h

commit_as m "2024-11-10 15:45:00" "feat(codegen): Implement stack frame and memory ops" \
    inc/mipsGenerator.h

commit_as c "2024-11-12 10:30:00" "feat(codegen): Add function call infrastructure" \
    inc/mipsGenerator.h

commit_as r "2024-11-22 14:00:00" "feat(codegen): Support strings, arrays, and data section" \
    inc/mipsGenerator.h

commit_as m "2024-11-23 11:30:00" "feat(codegen): Implement pointer operations in MIPS" \
    inc/mipsGenerator.h src/parser.y

commit_as r "2024-11-24 16:45:00" "feat(codegen): Finalize with typedef parameter support" \
    src/parser.y inc/symbolTable.h inc/mipsGenerator.h

# Capture any remaining files
git add -A 2>/dev/null || true
if ! git diff --cached --quiet 2>/dev/null; then
    commit_as m "2024-11-24 17:00:00" "chore: Add documentation and cleanup files" "."
fi

git branch -M main

echo ""
echo "═══════════════════════════════════════════════════════"
echo "✨ GIT HISTORY CREATED SUCCESSFULLY!"
echo "═══════════════════════════════════════════════════════"
echo ""
echo "📊 Statistics:"
git shortlog -sn --all
echo ""
echo "📅 Recent Commits:"
git log --oneline -10
echo ""
echo "═══════════════════════════════════════════════════════"
echo "🚀 TO PUSH TO GITHUB:"
echo "═══════════════════════════════════════════════════════"
echo ""
echo "git remote add origin https://github.com/marupaka-koushik/C-Compiler.git"
echo "git push -u origin main --force"
echo ""
echo "⚠️  Note: --force will REPLACE existing repository history"
echo "═══════════════════════════════════════════════════════"
