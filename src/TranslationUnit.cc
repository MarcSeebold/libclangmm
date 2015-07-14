#include "TranslationUnit.h"
#include "SourceLocation.h"
#include "Tokens.h"
#include <fstream>
#include <sstream>

clang::TranslationUnit::
~TranslationUnit() {
  clang_disposeTranslationUnit(tu_);
}

clang::TranslationUnit& clang::TranslationUnit::
operator=(const clang::TranslationUnit &tu) {
  tu_ = tu.tu_;
  return *this;
}

clang::TranslationUnit::
TranslationUnit(Index *index,
                const std::string &filepath,
                const std::vector<std::string> &command_line_args) {
  std::map<std::string, std::string> buffers;
  std::ifstream ifs(filepath, std::ifstream::in);
  std::stringstream ss;
  ss << ifs.rdbuf();
  buffers[filepath]=ss.str();
  parse(index, filepath, command_line_args, buffers);
}

clang::TranslationUnit::
TranslationUnit(Index *index,
                const std::string &filepath) {
  std::vector<std::string> command_line_args;
  std::map<std::string, std::string> buffers;
  std::ifstream ifs(filepath, std::ifstream::in);
  std::stringstream ss;
  ss << ifs.rdbuf();
  buffers[filepath]=ss.str();
  parse(index, filepath, command_line_args, buffers);
}

clang::TranslationUnit::
TranslationUnit(clang::Index *index,
                const std::string &filepath,
                const std::vector<std::string> &command_line_args,
                const std::map<std::string, std::string> &buffers,
                unsigned flags) {
  parse(index, filepath, command_line_args, buffers, flags);
}

void clang::TranslationUnit::parse(Index *index,
                                   const std::string &filepath,
                                   const std::vector<std::string> &command_line_args,
                                   const std::map<std::string, std::string>  &buffers,
                                   unsigned flags) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  std::vector<const char*> args;
  for(auto &a: command_line_args) {
    args.push_back(a.c_str());
  }
  tu_ =
   clang_parseTranslationUnit(index->index_,
                              filepath.c_str(),
                              args.data(),
                              args.size(),
                              files.data(),
                              files.size(),
                              flags);
}

int clang::TranslationUnit::
ReparseTranslationUnit(const std::string &file_path,
                       const std::map<std::string, std::string>  &buffers,
                       unsigned flags) {
  std::vector<CXUnsavedFile> files;
  for (auto &buffer : buffers) {
    CXUnsavedFile file;
    file.Filename = buffer.first.c_str();
    file.Contents = buffer.second.c_str();
    file.Length = buffer.second.size();
    files.push_back(file);
  }
  return clang_reparseTranslationUnit(tu_,
                                      files.size(),
                                      files.data(),
                                      flags);
}

unsigned clang::TranslationUnit::DefaultFlags() {
  return CXTranslationUnit_CacheCompletionResults | CXTranslationUnit_PrecompiledPreamble | CXTranslationUnit_Incomplete | CXTranslationUnit_IncludeBriefCommentsInCodeCompletion;
}

void clang::TranslationUnit::update_diagnostics() {
  diagnostics.clear();
  for(unsigned c=0;c<clang_getNumDiagnostics(tu_);c++) {
    CXDiagnostic clang_diagnostic=clang_getDiagnostic(tu_, c);
    diagnostics.emplace_back(clang::Diagnostic(tu_, clang_diagnostic));
    clang_disposeDiagnostic(clang_diagnostic);
  }
}
