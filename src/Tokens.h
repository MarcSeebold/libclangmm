#ifndef TOKENS_H_
#define TOKENS_H_
#include <clang-c/Index.h>
#include "SourceRange.h"
#include "Token.h"
#include <unordered_map>
#include <vector>

namespace clang {
  class Tokens : public std::vector<clang::Token> {
  public:
    Tokens(CXTranslationUnit &cx_tu, SourceRange &range);
    ~Tokens();
    void rename(CXCursor &referenced);
    //std::unordered_map<std::string, std::unordered_multimap<unsigned, unsigned> >
  private:
    CXToken *cx_tokens;
    unsigned num_tokens;
    std::vector<CXCursor> cx_cursors;
    CXTranslationUnit& cx_tu;
  };
}  // namespace clang
#endif  // TOKENS_H_
