// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// rcpp_hello_world
List rcpp_hello_world();
RcppExport SEXP _RTrollBase_rcpp_hello_world() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_hello_world());
    return rcpp_result_gen;
END_RCPP
}
// readl
SEXP readl(std::string path, bool lines);
RcppExport SEXP _RTrollBase_readl(SEXP pathSEXP, SEXP linesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type path(pathSEXP);
    Rcpp::traits::input_parameter< bool >::type lines(linesSEXP);
    rcpp_result_gen = Rcpp::wrap(readl(path, lines));
    return rcpp_result_gen;
END_RCPP
}
// str_split_cpp
std::vector<std::string> str_split_cpp(const std::string& s);
RcppExport SEXP _RTrollBase_str_split_cpp(SEXP sSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type s(sSEXP);
    rcpp_result_gen = Rcpp::wrap(str_split_cpp(s));
    return rcpp_result_gen;
END_RCPP
}
// recup_troll_str_vec
std::vector<std::string> recup_troll_str_vec(std::string filename);
RcppExport SEXP _RTrollBase_recup_troll_str_vec(SEXP filenameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type filename(filenameSEXP);
    rcpp_result_gen = Rcpp::wrap(recup_troll_str_vec(filename));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_RTrollBase_rcpp_hello_world", (DL_FUNC) &_RTrollBase_rcpp_hello_world, 0},
    {"_RTrollBase_readl", (DL_FUNC) &_RTrollBase_readl, 2},
    {"_RTrollBase_str_split_cpp", (DL_FUNC) &_RTrollBase_str_split_cpp, 1},
    {"_RTrollBase_recup_troll_str_vec", (DL_FUNC) &_RTrollBase_recup_troll_str_vec, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_RTrollBase(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
