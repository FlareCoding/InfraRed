#pragma once
#ifdef IFR_BUILD
    #define IFRAPI __declspec(dllexport)
#else
    #define IFRAPI __declspec(dllimport)
#endif