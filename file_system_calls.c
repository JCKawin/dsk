/*
 * File system operations using system calls (not stdio).
 * Demonstrates: creat/open, write, lseek, read, stat, unlink, mkdir, rename.
 *
 * Compile: gcc file_system_calls.c -o file_system_calls
 * Run:     ./file_system_calls
 *
 * Creates ./demo_dir/notes.txt, writes, reads back, prints metadata,
 * renames, then cleans up.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define DIR_NAME  "demo_dir"
#define FILE_NAME "demo_dir/notes.txt"
#define NEW_NAME  "demo_dir/notes_renamed.txt"

static void die(const char *what) {
    perror(what);
    exit(1);
}

int main(void) {
    /* mkdir: ignore EEXIST so the program can be re-run */
    if (mkdir(DIR_NAME, 0755) == -1 && errno != EEXIST)
        die("mkdir");
    printf("mkdir(%s) ok\n", DIR_NAME);

    /* open with O_CREAT|O_TRUNC|O_RDWR  (creat-style create) */
    int fd = open(FILE_NAME, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1) die("open");
    printf("open(%s) -> fd %d\n", FILE_NAME, fd);

    const char *text = "Operating Systems lab\nFile system calls: open read write lseek stat\n";
    ssize_t n = write(fd, text, strlen(text));
    if (n == -1) die("write");
    printf("write: %zd bytes\n", n);

    const char *extra = "Appended line via lseek(SEEK_END)\n";
    if (lseek(fd, 0, SEEK_END) == -1) die("lseek end");
    if (write(fd, extra, strlen(extra)) == -1) die("write append");

    /* read from the start */
    if (lseek(fd, 0, SEEK_SET) == -1) die("lseek set");
    char buf[512];
    memset(buf, 0, sizeof(buf));
    ssize_t total = 0, r;
    while ((r = read(fd, buf + total, sizeof(buf) - 1 - total)) > 0)
        total += r;
    if (r == -1) die("read");
    printf("read %zd bytes:\n----\n%s----\n", total, buf);

    /* fstat / stat */
    struct stat st;
    if (fstat(fd, &st) == -1) die("fstat");
    printf("stat: size=%lld mode=%o inode=%llu\n",
           (long long)st.st_size,
           st.st_mode & 0777,
           (unsigned long long)st.st_ino);

    if (close(fd) == -1) die("close");
    printf("close ok\n");

    if (rename(FILE_NAME, NEW_NAME) == -1) die("rename");
    printf("rename -> %s\n", NEW_NAME);

    if (unlink(NEW_NAME) == -1) die("unlink");
    printf("unlink(%s) ok\n", NEW_NAME);

    if (rmdir(DIR_NAME) == -1) die("rmdir");
    printf("rmdir(%s) ok\n", DIR_NAME);

    printf("All file-system system calls completed.\n");
    return 0;
}
