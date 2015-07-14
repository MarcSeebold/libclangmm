#include "SourceLocation.h"

// // // // // // // //
//  SourceLocation   //
// // // // // // // //
clang::SourceLocation::
SourceLocation(CXTranslationUnit &tu,
               const std::string &filename,
               int line_number,
               int line_offset) {
  CXFile file = clang_getFile(tu,
                              filename.c_str());
  location_ = clang_getLocation(tu,
                                file,
                                line_number,
                                line_offset);
  }

clang::SourceLocation::
SourceLocation(Cursor *cursor) {
  location_ = clang_getCursorLocation(cursor->cursor_);
}

clang::SourceLocation::
SourceLocation(clang::SourceRange *range, bool start) {
  location_ = start ? clang_getRangeStart(range->range_) :
    clang_getRangeEnd(range->range_);
}

clang::SourceLocation::
SourceLocation(CXTranslationUnit &tu,
               Token *token) {
  location_ = clang_getTokenLocation(tu,
                                     token->token_);
}

clang::SourceLocation::
SourceLocation(CXTranslationUnit &tu,
               const std::string &filepath,
               int offset) {
  CXFile file = clang_getFile(tu,
                              filepath.c_str());
  location_ = clang_getLocationForOffset(tu,
                                         file,
                                         offset);
}

void clang::SourceLocation::
get_location_info(std::string* path,
                  unsigned *line,
                  unsigned *column,
                  unsigned *offset) {
   CXFile file;
   clang_getExpansionLocation(location_, &file, line, column, offset);
   if (path != NULL && file!=NULL) {
     path->operator=(((clang_getCString((clang_getFileName(file))))));
   }
}

