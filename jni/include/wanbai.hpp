// Qx9-10对接，理论通杀
// 原理: ls -l /proc/*/exe 2>/dev/null | grep -E "/data/[a-z]{6} \\(deleted\\)"
// Output:
// lrwxrwxrwx 1 root root 0 2024-01-01 14:00 /proc/7665/exe -> /data/ktbqmo (deleted)

#ifndef __NEKO_H__
#define __NEKO_H__
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/utsname.h> 
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include <malloc.h>
#include <thread>
#include <sys/mman.h>
#include <sys/uio.h>
#include <math.h>
#include <errno.h>
#include <netdb.h>
#include <cstring>
#include <cctype>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale>
#include <string>
#include <dlfcn.h>


class c_driver
{
    private:
        // int has_upper = 0;
        // int has_lower = 0;
        // int has_symbol = 0;
        // int has_digit = 0;
        int fd;
        pid_t pid;
        const char *dev_path {nullptr};

        typedef struct _COPY_MEMORY {
            pid_t pid;
            uintptr_t addr;
            void* buffer;
            size_t size;
        } COPY_MEMORY, *PCOPY_MEMORY;

        typedef struct _MODULE_BASE {
            pid_t pid;
            char* name;
            uintptr_t base;
        } MODULE_BASE, *PMODULE_BASE;

        enum OPERATIONS {
            OP_INIT_KEY = 0x800,
            OP_READ_MEM = 0x801,
            OP_WRITE_MEM = 0x802,
            OP_MODULE_BASE = 0x803,
        };

        char *kernel()
        {
            struct utsname unameData;
            if (uname(&unameData) == 0)
            {
                return strdup(unameData.release);
                // sysname：操作系统名称
                // nodename：计算机网络名称
                // release：操作系统发行版本
                // version：操作系统内核版本
                // machine：硬件架构
                // domainname：域名
            } else {
                printf("\033[31m [-]无法获取内核版本。 \x1b[0m");
                exit(0);
                return NULL;
            }
        }

        // char* get_dev()
        // {
        //     char* command = "for dir in /proc/*/;\
        //     do cmdline_file=\"cmdline\";\
        //     comm_file=\"comm\";\
        //     proclj=\"$dir$cmdline_file\"; \
        //     proclj2=\"$dir$comm_file\"; \
        //     if [[ -f \"$proclj\" && -f \"$proclj2\" ]]; \
        //         then cmdline=$(head -n 1 \"$proclj\"); \
        //         comm=$(head -n 1 \"$proclj2\"); \
        //         if echo \"$cmdline\" | grep -qE '^/data/[a-z]{6}$'; \
        //             then sbwj=$(echo \"$comm\"); \
        //             open_file=\"\"; \
        //             for file in \"$dir\"/fd/*; \
        //                 do link=$(readlink \"$file\"); if [[ \"$link\" == \"/dev/$sbwj (deleted)\" ]]; then open_file=\"$file\"; break; fi; done; if [[ -n \"$open_file\" ]]; then nhjd=$(echo \"$open_file\"); sbid=$(ls -L -l \"$nhjd\" | sed 's/\\([^,]*\\).*/\\1/' | sed 's/.*root //'); echo \"/dev/$sbwj\"; rm -Rf \"/dev/$sbwj\"; mknod \"/dev/$sbwj\" c \"$sbid\" 0; break; fi; fi; fi; done";
        //     char result[512];
        //     FILE* file = popen(command, "r");
        //     if (file == NULL) {
        //         return NULL;
        //     }
        //     fgets(result, sizeof(result), file);
        //     pclose(file);
        //     int len = strlen(result);
        //     if (len > 0 && result[len - 1] == '\n') {
        //         result[len - 1] = '\0';
        //     }
        //     return strdup(result);
        // }

        int open_driver()
        {
            fd = open(dev_path, O_RDWR);
            if (fd > 0)
            {
                char *version = kernel();
                printf("Kernel version: %s\n", version);
                unlink(dev_path);
                free(version);
                return 0;
            } else
                printf("Can not find the driver!\n");
            unlink(dev_path);
            return -1;
        }

    public:
        c_driver(const char *_dev_path, pid_t _pid) : pid(_pid), dev_path(_dev_path)
        {
            open_driver();
            if (fd <= 0 or pid <= 10) {
                puts("fail to startup!");
                exit(0);
            }
        }

        ~c_driver()
        {
            if (fd > 0)
                close(fd);
        }

        bool initKey(const char* key)
        {
            if (ioctl(fd, OP_INIT_KEY, key) != 0) {
                return false;
            }
            return true;
        }

        bool read(uintptr_t addr, void* buffer, size_t size) {
            COPY_MEMORY cm;
            cm.pid = this->pid;
            cm.addr = addr;
            cm.buffer = buffer;
            cm.size = size;

            return ioctl(fd, OP_READ_MEM, &cm) == 0 ? true : false;
        }

        bool write(uintptr_t addr, void* buffer, size_t size)
        {
            COPY_MEMORY cm;
            cm.pid = this->pid;
            cm.addr = addr;
            cm.buffer = buffer;
            cm.size = size;

            return ioctl(fd, OP_WRITE_MEM, &cm) == 0 ? true : false;
        }

        template <typename T>
        T read(uintptr_t addr)
        {
            T res;
            return this->read(addr, &res, sizeof(T)) ? res : 0;
        }

        template <typename T>
        bool write(uintptr_t addr, T value)
        {
            return this->write(addr, &value, sizeof(T));
        }

        uintptr_t getModuleBase(const char* name)
        {
            MODULE_BASE mb;
            mb.pid = this->pid;
            mb.name = strdup(name);

            if (ioctl(fd, OP_MODULE_BASE, &mb) != 0) {
                free(mb.name);
                return 0;
            }

            uintptr_t base = mb.base;
            free(mb.name);
            return base;
        }
};

#endif // NEKO_H
