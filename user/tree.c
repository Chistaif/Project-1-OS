#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h" 
#include "user/user.h"

#define MAXBUFF 512

void tree(char *path, int depth);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(2, "Usage: %s [directory_name]", argv[0]);
        exit(1);
    }

    // neu khong input path thi mac dinh la current path
    char *path = ".";
    if (argc == 2) {
        path = argv[1];
    }

    // Check xem path co ton tai khong 
    struct stat st;
    if (stat(path, &st) < 0) {
        fprintf(2, "tree: cannot stat %s\n", path);
        exit(1);
    }

    // Check xem co phai folder khong
    if (st.type != T_DIR) {
        fprintf(2, "tree: %s is not a directory\n", path);
        exit(1);
    }

    printf("%s/\n", path);
    tree(path, 0);

    exit(0);
}

void tree(char *path, int depth) {
    struct dirent object;       // doc ten
    struct stat st;             // kiem tra file/folder
    char child[MAXBUFF];

    /*
    Mo thu muc 
    */
    int fd = open(path, 0);
    if (fd < 0) {
        fprintf(2, "tree: cannot open %s\n", path);
        return;
    }

    /*
    Doc tung doi tuong trong thu muc
    */
    while (read(fd, &object, sizeof(object)) == sizeof(object)) {
        /*
        Kiem tra file/folder
        */
        if (object.inum == 0) continue;     // bo qua file trong
        
        char fname[DIRSIZ];   // lay ten file/folder
        strncpy(fname, object.name, DIRSIZ);
        if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) continue;  // tranh lap vo tan

        /*
        Build duong dan den file/folder
        */
        if (strlen(path) + strlen(fname) + 2 > MAXBUFF) {
            fprintf(2, "tree: path too long\n");
            continue;
        }
        sprintf(child, "%s/%s", path, fname);

        /*
        Lay thong tin file/folder
        */
        int childFd = open(child, 0);
        if (childFd < 0) {
            fprintf(2, "tree: cannot open %s\n", child);
            continue;
        }
        if (fstat(childFd, &st) < 0) {
            fprintf(2, "tree: cannot stat %s\n", child);
            close(childFd);
            continue;
        }
        close(childFd);

        /*
        In ra ten file/folder theo chuan format
        */
        for (int i = 0; i <= depth; i++) {
            printf("  ");
        }
        if (st.type == T_DIR) {
            printf("%s/\n", fname);
            tree(child, depth + 1);         
        } else {
            printf("%s\n", fname);
        }
    }

    close(fd);
}