
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
            class DeclAssignCountVisitor
                : public RecursiveASTVisitor<DeclAssignCountVisitor> {
                friend class RecursiveASTVisitor<DeclAssignCountVisitor>;

            public:
                explicit DeclAssignCountVisitor(ASTContext *Context, Decl *d)
                    : root(d), count(0), Context(Context) {
                    if (dyn_cast<VarDecl>(d)) {
                        this->count++;
                    }
                }
                size_t getAssignOpCount(void) { return this->count; }
                bool VisitBinaryOperator(const BinaryOperator *binop) {
                    if (binop->isAssignmentOp()) {
                        this->count++;
                    }
                    return true;
                }

            private:
                Decl *root;
                size_t count;
                ASTContext *Context;
            };
            class StmtAssignCountVisitor
                : public RecursiveASTVisitor<StmtAssignCountVisitor> {
                friend class RecursiveASTVisitor<DeclAssignCountVisitor>;

            public:
                explicit StmtAssignCountVisitor(ASTContext *Context, Stmt *s)
                    : root(s), count(0), Context(Context) {}
                size_t getAssignOpCount(void) { return this->count; }

                bool VisitBinaryOperator(const BinaryOperator *binop) {
                    if (binop->isAssignmentOp()) {
                        this->count++;
                    }
                    return true;
                }

            private:
                Stmt *root;
                size_t count;
                ASTContext *Context;
            };

            Rule11cCheck(StringRef Name, ClangTidyContext *Context)
                : ClangTidyCheck(Name, Context),
                  debug_enabled(Options.get("debug", "false")) {
                if (this->debug_enabled == "true") {
                    this->debug = true;
                }
            }

            void Rule11cCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(
                    binaryOperator(isAssignmentOperator()).bind("binary_operator"),
                    this);
            }
            void Rule11cCheck::check(const MatchFinder::MatchResult &Result) {
                ASTContext *Context = Result.Context;
                SourceManager &SM = *Result.SourceManager;
                ASTNodeKind stop_kinds[] = {
                    ASTNodeKind::getFromNodeKind<CompoundStmt>(),
                    ASTNodeKind::getFromNodeKind<DeclStmt>(),
                    ASTNodeKind::getFromNodeKind<DoStmt>(),
                    ASTNodeKind::getFromNodeKind<IfStmt>(),
                    ASTNodeKind::getFromNodeKind<ReturnStmt>(),
                    ASTNodeKind::getFromNodeKind<SwitchCase>(),
                    ASTNodeKind::getFromNodeKind<SwitchStmt>(),
                    ASTNodeKind::getFromNodeKind<WhileStmt>(),
                    ASTNodeKind::getFromNodeKind<ForStmt>(),
                };
                std::deque<DynTypedNode> ascend_q;
                std::string init;
                llvm::raw_string_ostream rso(init);

                if (auto MatchedDecl =
                        Result.Nodes.getNodeAs<BinaryOperator>("binary_operator")) {

                    ParentMapContext &PMC = Context->getParentMapContext();
                    ascend_q.push_back(
                        DynTypedNode::create<BinaryOperator>(*MatchedDecl));

                    while (not ascend_q.empty()) {
                        DynTypedNode node = ascend_q.front();
                        node.dump(rso, *Context);
                        rso << "\n";
                        ascend_q.pop_front();
                        DynTypedNodeList Parents = PMC.getParents(node);
                        for (auto it = Parents.begin(); it != Parents.end(); it++) {
                            // Check if this is a stop parent
                            bool is_stop = false;
                            for (auto &ank : stop_kinds) {
                                if (it->getNodeKind().isSame(ank)) {
                                    is_stop = true;
                                }
                            }
                            if (is_stop) {
                                // Explore children exhaustively and count number of
                                // assigns
                                Stmt *s_parent = const_cast<Stmt *>(node.get<Stmt>());
                                // std::cout << "Number of assignments under this
                                // parent: " << assignCount(parent) << std::endl;
                                if (s_parent) {
                                    StmtAssignCountVisitor s_visitor(Context, s_parent);
                                    s_visitor.TraverseStmt(s_parent);
                                    if (s_visitor.getAssignOpCount() > 1) {
                                        diag(s_parent->getBeginLoc(),
                                             "Expression contains more than one "
                                             "assignment.");
                                    }

                                } else {
                                    Decl *d_parent =
                                        const_cast<Decl *>(node.get<Decl>());
                                    DeclAssignCountVisitor d_visitor(Context, d_parent);
                                    d_visitor.TraverseDecl(d_parent);
                                    if (d_visitor.getAssignOpCount() > 1) {
                                        diag(d_parent->getLocation(),
                                             "Expression contains more than one "
                                             "assignment.");
                                    }
                                }

                            } else {
                                ascend_q.push_back(*it);
                            }
                        }
                    }
                }
                // std::cout << rso.str() << std::endl; // Use this for debugging
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang
