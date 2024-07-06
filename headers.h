#ifndef HEADERS_H_
#define HEADERS_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <termios.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/utsname.h"
#include "sys/wait.h"
#include "prompt.h"

extern int fg_present;

int is_digit(char c);
void ctrlc_handle(int sig);
void seg_handle(int sig);
void ctrlz_handle(int sig);
void take_input();
void ctrlD_handle(int sig);
void prompt(int* prev_prom, char* prev_prom_msg);
void in_out_redirection(char** command, int bg, int cmd_len,int* pipe_fd,int* pipe_fd2,  int active_pipe);
int warp(char** command, int bg, int cmd_len,int* pipe_fd,int* pipe_fd2,  int active_pipe);
int peek(char** command, int bg, int cmd_len,int* pipe_fd,int* pipe_fd2,  int active_pipe);
void verify(char* input);
char* gethome_len();
int rawmode();
void neonate(char* time_halt);
void lstrip(char *command);
void rstrip(char *command);
void save_to_pastevents(char* input);
void pastevents(char** command, int bg, int cmd_len,int* pipe_fd,int* pipe_fd2,  int active_pipe);
void execvp_func(char** command, int bg, int cmd_count,int pipe_count, int* prev_prom,char* prev_prom_msg,char* bg_prom_msg, int* i,int* pipe_fd,int* pipe_fd2,  int active_pipe, int IO_flag);
void proclore(char** command, int bg, int cmd_count,int* pipe_fd,int* pipe_fd2,  int active_pipe);
void seek(char *req_dir_fil, char *target_dir, int is_direct, int is_file, int is_task,int* pipe_fd,int* pipe_fd2,  int active_pipe);
void seek_flags(char** command, int bg,int cmd_count, int* pipe_fd,int* pipe_fd2,  int active_pipe);
void iMan(char** command, int bg,int cmd_count,int* pipe_fd,int* pipe_fd2,  int active_pipe);
void activities();
void fg(char** command,int bg,int cmd_count,int* pipe_fd,int* pipe_fd2, int* i, int active_pipe);
void bg_func(char** command,int bg,int cmd_count,int* pipe_fd,int* pipe_fd2,  int active_pipe);
void send_signal(char** command, int bg, int cmd_len, int* pipe_fd,int* pipe_fd2,  int active_pipe);
#endif