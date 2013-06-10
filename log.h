#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED
#include <stdio.h>

/*
 * For now, logging is a boolean, but by creating level macros now, I can turn
 * it into leveled logging later without having to tweak the log message senders
 */

#ifdef LOGGING_ENABLED
#define TRACE(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#define DEBUG(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#define INFO(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#define WARN(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#define ERROR(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#define CRITICAL(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#define LOG(...) do { printf("%s\t", __FILE__); printf(__VA_ARGS__); } while(0)
#else
#define TRACE(...) do { } while(0)
#define DEBUG(...) do { } while(0)
#define INFO(...) do { } while(0)
#define WARN(...) do { } while(0)
#define ERROR(...) do { } while(0)
#define CRITICAL(...) do { } while(0)
#define LOG(...) do { } while(0)
#endif

#endif
