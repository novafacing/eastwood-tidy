
//===--- Rule11cCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11cCheck.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule11cCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(binaryOperator(isAssignmentOperator()).bind("binary_operator"), this);
            }
            void Rule11cCheck::check(const MatchFinder::MatchResult &Result) {
                ASTContext *Context = Result.Context;
                SourceManager &SM = *Result.SourceManager;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<BinaryOperator>("binary_operator")) {
                    const Stmt &CurrentStmt = *MatchedDecl;
                    // TODO: Get parent until parent is no longer an Expr. Add the assignment expression to the map indexed by expr. If we have 1 already, error.
                    ParentMapContext &PMC = Context->getParentMapContext();
                    DynTypedNodeList Parents = PMC.getParents(CurrentStmt);
                    DynTypedNodeList LParents = Parents;
                    ASTNodeKind ParenExprKind = ASTNodeKind::getFromNodeKind<ParenExpr>();
                    ASTNodeKind WhileStmtKind = ASTNodeKind::getFromNodeKind<WhileStmt>();
                    ASTNodeKind IfStmtKind = ASTNodeKind::getFromNodeKind<IfStmt>();
                    ASTNodeKind BinaryOperatorKind = ASTNodeKind::getFromNodeKind<BinaryOperator>();
                    ASTNodeKind CaseStmtKind = ASTNodeKind::getFromNodeKind<CaseStmt>();
                    if (not Parents.empty() and BinaryOperatorKind.isSame(Parents[0].getNodeKind()) and Parents[0].get<BinaryOperator>()->isAssignmentOp()) {
                        // = is child of another =; this is forbidden
                        diag(MatchedDecl->getExprLoc(), "Multiple assignments are not permitted in a single expression");
                        return;
                    }
                    std::string paren_expr_str;
                    llvm::raw_string_ostream rso(paren_expr_str);
                    rso << ASTNodeKind::getFromNode(CurrentStmt);
                    //std::cout << "Initial node type: " << rso.str() << std::endl;
                    while (not Parents.empty()) {
                        std::string paren_expr_str;
                        llvm::raw_string_ostream rso(paren_expr_str);
                        rso << ParenExprKind << " VS " << Parents[0].getNodeKind();
                        //std::cout << rso.str() << std::endl;
                        LParents = Parents;
                        Parents = PMC.getParents(Parents[0]);
                        if (not(Parents.size() == 1) or CaseStmtKind.isSame(Parents[0].getNodeKind())) {
                            //std::cout << "Parents of size" << Parents.size() << std::endl;
                            break;
                        } else {
                            //std::cout << "Parents of size 1; ascending" << std::endl;
                            //std::cout << ParenExprKind << " VS " << Parents[0].getNodeKind() << std::endl;
                            if (IfStmtKind.isSame(Parents[0].getNodeKind()) or WhileStmtKind.isSame(Parents[0].getNodeKind())) {
                                for (auto n : this->paren_nodes) {
                                    if (Parents[0] == n) {
                                        diag(MatchedDecl->getExprLoc(), "Multiple assignments are not permitted in a single expression");
                                        return;
                                    }
                                }
                                this->paren_nodes.push_back(Parents[0]);
                            }
                        }
                    }
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang
