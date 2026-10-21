#include "aps.h"
#ifdef _WIN32
#include <windows.h>
double tempo_agora(void) {
    static LARGE_INTEGER frequencia = {0};
    LARGE_INTEGER contador;
    if (frequencia.QuadPart == 0) QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&contador);
    return (double)contador.QuadPart / (double)frequencia.QuadPart;
}
#else
#include <time.h>
double tempo_agora(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif
