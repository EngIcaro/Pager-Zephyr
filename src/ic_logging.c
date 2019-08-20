#include "ic_logging.h"

static struct {
    u8_t major;
    u8_t minor;
    u16_t build;
} version = {0, 0, 1};

void ic_log_err(char *message)
{
    printk("\033[1;31m");
    printk("[ERR] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__);
    printk("%s", message);
    printk("\n\033[0m");
}
void ic_log_wrn(char *message)
{
    printk("\033[1;33m");
    printk("[WRN] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__);
    printk("%s", message);
    printk("\n\033[0m");
}
void ic_log_inf(char *message)
{
    printk("\033[1;34m");
    printk("[INF] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__);
    printk("%s", message);
    printk("\n\033[0m");
}
void ic_log_dbg(char *message)
{
    printk("[DBG] %s.%s[%d]: ", __FILE__, __FUNCTION__, __LINE__);
    printk("%s", message);
    printk("\n");
}

void ic_log_version()
{
    printk("Firmware version %d.%d.%d\n", version.major, version.minor, version.build);
}

void ic_log_version_inc_build()
{
    version.build++;
}