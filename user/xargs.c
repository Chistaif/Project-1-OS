#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXBUFF 512

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: [command 1] | xargs [command 2]\n");
        exit(1);
    }

    char buf[MAXBUFF];
    int len = 0;

    char c;
    while (read(0, &c, 1) > 0) {
        if (c == '\n') {
            /*
            Xu ly lenh vua duoc nhap xong
            */
            buf[len] = '\0';            // them ky tu ket thuc cho lenh              

            char *command[MAXARG];              // xay dung command de exec chay: [lenh] [argument] ... [stdin nhan dc] NULL
            for (int i = 1; i < argc; i++) {
                command[i - 1] = argv[i];       // bo argv[0] = xargs
            }
            command[argc - 1] = buf;
            command[argc] = 0;

            int pid = fork();    
            if (pid == 0) {
                /*
                process con chay
                */
                exec(command[0], command);

                fprintf(2, "exec failed\n");    // Xu ly loi exec
                exit(1);
            } else {
                /*
                process cha chay
                */
                wait(0);
            }

            len = 0;                            // reset lai buffer
        } else {
            /*
            Tiep tuc cong don vao buffer neu chua nhan du stdin tu pipe
            */
            if (len < MAXBUFF - 1) {
                buf[len++] = c;
            } else {
                fprintf(2, "xargs: line too long\n");
                exit(1);
            }
        }
    }

    exit(0);                                
}