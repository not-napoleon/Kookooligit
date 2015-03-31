#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
#include <stdio.h>

#define LOG_TRACE    6
#define LOG_DEBUG    5
#define LOG_INFO     4
#define LOG_WARN     3
#define LOG_ERROR    2
#define LOG_CRITICAL 1
#define LOG_DISABLED 0


#if LOG_LEVEL >= LOG_TRACE
#define TRACE(...) do { fprintf(stderr, "%s\tTRACE\t", __FILE__); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define TRACE(...) do { } while(0)
#endif

#if LOG_LEVEL >= LOG_DEBUG
#define DEBUG(...) do { fprintf(stderr, "%s\tDEBUG\t", __FILE__); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define DEBUG(...) do { } while(0)
#endif

#if LOG_LEVEL >= LOG_INFO
#define INFO(...) do { fprintf(stderr, "%s\tINFO\t", __FILE__); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define INFO(...) do { } while(0)
#endif

#if LOG_LEVEL >= LOG_WARN
#define WARN(...) do { fprintf(stderr, "%s\tWARN\t", __FILE__); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define WARN(...) do { } while(0)
#endif

#if LOG_LEVEL >= LOG_ERROR
#define ERROR(...) do { fprintf(stderr, "%s\tERROR\t", __FILE__); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define ERROR(...) do { } while(0)
#endif

#if LOG_LEVEL >= LOG_CRITICAL
#define CRITICAL(...) do { fprintf(stderr, "%s\tCRITICAL\t", __FILE__); fprintf(stderr, __VA_ARGS__); } while(0)
#else
#define CRITICAL(...) do { } while(0)
#endif

#endif
