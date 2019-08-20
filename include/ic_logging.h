#ifndef _IC_LOGGING_H_
#define _IC_LOGGING_H_
#include <zephyr.h>

void ic_log_err(char *message);
void ic_log_wrn(char *message);
void ic_log_inf(char *message);
void ic_log_dbg(char *message);
void ic_log_version();
void ic_log_version_inc_build();

#if 0
#define IC_LOG_ERR(x)                                              \
    printk("\033[1;31m");                                          \
    printk("[ERR] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__); \
    printk(x);                                                     \
    printk("\n\033[0m")
#define IC_LOG_WRN(x)                                              \
    printk("\033[1;33m");                                          \
    printk("[WRN] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__); \
    printk(x);                                                     \
    printk("\n\033[0m")
#define IC_LOG_INF(x)                                              \
    printk("\033[1;34m");                                          \
    printk("[INF] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__); \
    printk(x);                                                     \
    printk("\n\033[0m")
#define IC_LOG_DBG(x)                                              \
    printk("[DBG] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__); \
    printk(x);                                                     \
    printk("\n")

#else
#define IC_LOG_ERR(x)
#define IC_LOG_WRN(x)
#define IC_LOG_INF(x)
#define IC_LOG_DBG(x)
#endif

#endif  // !_IC_LOGGING_H_