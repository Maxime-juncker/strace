#include "stdio.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/syscall.h>

void exit_msg(char* msg, int code)
{
	perror(msg);
	exit(code);
}

char* get_syscall_name(int call)
{

}

void print_regs(struct user_regs_struct* regs)
{
	printf("+++ printing register +++\n");
	printf("\torig_rax: %llu (syscall id)\n", regs->orig_rax);
	printf("\trax: %llu (return value)\n", regs->rax);
	// printf("  ++ arguments ++\n");
	// printf("\trdi: %llu\n", regs->rdi);
	// printf("\trdx: %llu\n", regs->rdx);
	// printf("\tr10: %llu\n", regs->r10);
	// printf("\tr9: %llu\n", regs->r9);
	//
	//
	//switch(code) {
}

int main(int argc, char* argv[])
{
	(void)argv;
	if (argc == 1)
	{
		exit_msg("missing argument", 1);
	}

	int pid = fork();
	if (pid == -1)
		exit_msg("fork failed", 1);
	if (pid == 0) // child
	{
		char* args[] = { NULL };
		execve(argv[1], args, NULL);
		exit_msg("exec failed", 1);
	}
	else
	{
		long	trace;
		int		status;
		int		syscall_exit = 0;

		// init the trace
		if ((trace = ptrace(PTRACE_SEIZE, pid, NULL, NULL)) == -1)
			exit_msg("seize", 1);
		
		// process need to be stop for calling PTRACE_SYSCALL
		// <!> TODO: try to override sygstop in child process and see what happen <!>
		kill(pid, SIGSTOP);
		waitpid(pid, &status, 0); 
		struct user_regs_struct regs;

		if ((trace = ptrace(PTRACE_SYSCALL, pid, NULL, NULL)) == -1)
			exit_msg("syscall", 1);

		while (1)
		{
			if (waitpid(pid, &status, 0) == -1)
				exit_msg("waitpid", 1);
			if (WIFEXITED(status))
			{
				printf("child process has exited");
				break;
			}

			if ((trace = ptrace(PTRACE_GETREGS, pid, NULL, &regs)) == -1)
				exit_msg("getregs", 1);
			if (syscall_exit == 1)
				print_regs(&regs);
			syscall_exit = !syscall_exit;

			if ((trace = ptrace(PTRACE_SYSCALL, pid, NULL, NULL)) == -1)
				exit_msg("syscall", 1);


		}

		
	}
}
