//
// MATLAB Compiler: 4.18.1 (R2013a)
// Date: Fri Mar 14 12:45:54 2014
// Arguments: "-B" "macro_default" "-W" "cpplib:libfinalflow" "-T" "link:lib"
// "-d" "D:\Github\eleMat\libfinalflow\src" "-w"
// "enable:specified_file_mismatch" "-w" "enable:repeated_file" "-w"
// "enable:switch_ignored" "-w" "enable:missing_lib_sentinel" "-w"
// "enable:demo_license" "-v" "D:\Github\eleMat\Newflow.m" 
//

#ifndef __libfinalflow_h
#define __libfinalflow_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_libfinalflow
#define PUBLIC_libfinalflow_C_API __global
#else
#define PUBLIC_libfinalflow_C_API /* No import statement needed. */
#endif

#define LIB_libfinalflow_C_API PUBLIC_libfinalflow_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libfinalflow
#define PUBLIC_libfinalflow_C_API __declspec(dllexport)
#else
#define PUBLIC_libfinalflow_C_API __declspec(dllimport)
#endif

#define LIB_libfinalflow_C_API PUBLIC_libfinalflow_C_API


#else

#define LIB_libfinalflow_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libfinalflow_C_API 
#define LIB_libfinalflow_C_API /* No special import/export declaration */
#endif

extern LIB_libfinalflow_C_API 
bool MW_CALL_CONV libfinalflowInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libfinalflow_C_API 
bool MW_CALL_CONV libfinalflowInitialize(void);

extern LIB_libfinalflow_C_API 
void MW_CALL_CONV libfinalflowTerminate(void);



extern LIB_libfinalflow_C_API 
void MW_CALL_CONV libfinalflowPrintStackTrace(void);

extern LIB_libfinalflow_C_API 
bool MW_CALL_CONV mlxNewflow(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libfinalflow
#define PUBLIC_libfinalflow_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libfinalflow_CPP_API __declspec(dllimport)
#endif

#define LIB_libfinalflow_CPP_API PUBLIC_libfinalflow_CPP_API

#else

#if !defined(LIB_libfinalflow_CPP_API)
#if defined(LIB_libfinalflow_C_API)
#define LIB_libfinalflow_CPP_API LIB_libfinalflow_C_API
#else
#define LIB_libfinalflow_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libfinalflow_CPP_API void MW_CALL_CONV Newflow(int nargout, mwArray& busOut, const mwArray& busAdd, const mwArray& genAdd);

#endif
#endif
