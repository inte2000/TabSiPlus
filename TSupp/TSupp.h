
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TSUPP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TSUPP_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef TSUPP_EXPORTS
#define TSUPP_API __declspec(dllexport)
#else
#define TSUPP_API __declspec(dllimport)
#endif

// This class is exported from the TSupp.dll
class TSUPP_API CTSupp {
public:
    CTSupp(void);
    // TODO: add your methods here.
};

extern TSUPP_API int nTSupp;

TSUPP_API int fnTSupp(void);

