#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    struct procinfo info;
    
    // Lấy PID của chính tiến trình này, hoặc PID truyền từ tham số dòng lệnh
    int target_pid = getpid(); 
    if (argc == 2) {
        target_pid = atoi(argv[1]);
    }

    // Gọi system call procinfo
    if(procinfo(target_pid, &info) == 0) {
        printf("Process: %s\n", info.name);
        printf("PID: %d, PPID: %d\n", info.pid, info.ppid);
        printf("State: %d\n", info.state); 
        printf("Memory: %d bytes\n", (int)info.sz);
    } else {
        printf("procinfo failed: Process with PID %d not found\n", target_pid);
    }

    exit(0);
}