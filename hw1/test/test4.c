#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(void){
 pid_t childpid;
 int fd[2];
 if(pipe(fd) == -1){ /*setup a pipe*/
  perror("Failed to setup pipeline");
  return 1;
 }
 if((childpid = fork()) == -1){ /*fork a child*/
  perror("Failed to fork a child");
  return 1;
 }
 if(childpid == 0){ /*env is the child*/
  if(dup2(fd[1],STDOUT_FILENO)==-1)
   perror("Failed to redirect stdout of env");
  else if(close(fd[0] == -1)) /*close unused file descriptor*/
   perror("Failed to close extra pipe descriptors on env");
  else{
   execl("/usr/bin/env", "env", NULL); /*execute env*/
   perror("Failed to exec env");
  }
  return 1;
 }
 if(dup2(fd[0],STDIN_FILENO)==-1) /*grep is the parent*/
  perror("Failed to redirect stdin of grep");
 //else if(close(fd[1]==-1))
  //perror("Failed to close extra pipe file descriptors on grep");
 else{
  execl("/bin/grep", "grep", "HOME", NULL); /*execute "grep HOME"*/
  perror("Failed to exec grep");
 }
 return 1;
}
