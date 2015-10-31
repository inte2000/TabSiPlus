// Export information about TabSiPlus.dll

#ifdef TABSIPLUSDLL_EXPORTS
#define TABSIPLUSDLL_API __declspec(dllexport)
#else
#define TABSIPLUSDLL_API __declspec(dllimport)
#endif

#ifdef TABSIPLUSDLL_EXPORTS
#    ifdef _DEBUG
#        define TABSIPLUSDLL_DEBUG_API __declspec(dllexport)
#    else
#        define TABSIPLUSDLL_DEBUG_API
#    endif
#else
#    ifdef TABSIHOST_EXE
#        define TABSIPLUSDLL_DEBUG_API
#    else
#        define TABSIPLUSDLL_DEBUG_API __declspec(dllimport)
#    endif
#endif

extern "C"
{
    TABSIPLUSDLL_API BOOL WINAPI Initialize();
}

////////////////////////////////////////////////////////////////////////////////

#define    WM_TABSIPLUS_EX                    WM_APP    + 500

////////////////////////////////////////////////////////////////////////////////
