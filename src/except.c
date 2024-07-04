/*!
  @file except.c
  @brief The implementation of the lib.

  @author Erick Carrillo.
  @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All rights reserved.
  @license This project is released under the MIT License
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>


#include "../include/except.h"

#define T Except_T

/* General purpuse exceptions */

/* Logical exceptions */
T ExceptLogicalError        = INIT_EXCEPT_T("Logical Error");
T ExceptInvalidArgument     = INIT_EXCEPT_T("Invalid Argument");
T ExceptDomainError         = INIT_EXCEPT_T("Domain Error");
T ExceptLengthError         = INIT_EXCEPT_T("Length Error");
T ExceptOutOfRange          = INIT_EXCEPT_T("Out Of Range");

/* System error exceptions */
T ExceptRuntimeError        = INIT_EXCEPT_T("Runtime Error");
T ExceptRangeError          = INIT_EXCEPT_T("Range Error");
T ExceptOverflowError       = INIT_EXCEPT_T("Overflow Error");
T ExceptSystemError         = INIT_EXCEPT_T("System Error");
T ExceptFileSystemError     = INIT_EXCEPT_T("File System Error");
T ExceptThreadSystemError   = INIT_EXCEPT_T("Thread System Error");
T ExceptSocketSystemError   = INIT_EXCEPT_T("Socket System Error");
T ExceptReadSystemError     = INIT_EXCEPT_T("Read System Error");
T ExceptWriteSystemError    = INIT_EXCEPT_T("Write System Error");

/* Memory exceptions */
T ExceptBadAlloc            = INIT_EXCEPT_T("Bad Alloc");
T ExceptBadArrayNewLength   = INIT_EXCEPT_T("Bad Array New Length");
T ExceptBadPtr              = INIT_EXCEPT_T("Bad Ptr");

/* Signal exceptions */
T ExceptSigAbrt = INIT_EXCEPT_T("Abnormal termination signal");
T ExceptFpe = INIT_EXCEPT_T("Floating-point signal");
T ExceptSigIll = INIT_EXCEPT_T("Illegal instruction signal");
T ExceptSigSegv = INIT_EXCEPT_T("Segmentation fault");
T ExceptSigTerm = INIT_EXCEPT_T("Termination signal");
T ExceptSigSys = INIT_EXCEPT_T("Bad system call");

T *Except_handled_stack = NULL;
Except_Frame *Except_frame_stack = NULL;

int Except_backtrace_size = 0, Except_backtrace_raise_sig = 0;
void *Except_backtrace_array[MAX_BACKTRACE_LINES] = {0};
char **Except_backtrace_strings = NULL;

Except_raise_info_T Except_raise_info = {0};

#define ABORT_SIZE_MSG 128
#define ABORT(msg, ...) {						\
		char err_msg[ABORT_SIZE_MSG];				\
		sprintf(err_msg, msg __VA_OPT__(, __VA_ARGS__));	\
		fprintf(stderr, "[ERROR]:\t%s\t at %s:%i", err_msg, __FILE__, __LINE__); \
		fprintf(stderr, "aborting...\n");			\
		abort();						\
	}

static void Except_handle_signal(int signal)
{
	Except_backtrace_raise_sig = 1;
	switch (signal) {
	case SIGABRT:
		RAISE(ExceptSigAbrt, "The program encountered an unexpected condition and was terminated");
		break;
	case SIGFPE:
		RAISE(ExceptFpe, "A floating-point error occurred, such as division by "
		      "zero or an operation resulting in overflow");
		break;
	case SIGILL:
		RAISE(ExceptSigIll, "The processor executed an illegal or undefined instruction");
		break;
	case SIGSEGV:
		RAISE(ExceptSigSegv, "The program attempted to access a restricted area of memory, "
		      "such as by dereferencing a null pointer");
		break;
	case SIGTERM:
		RAISE(ExceptSigTerm, "Request for the termination of a process, often sent by the kill command");
		break;
	case SIGSYS:
		RAISE(ExceptSigSys, "The process attempted to invoke an undefined or invalid system call");
		break;
	default:
		ABORT("Invalid signal %i", signal);
	}
}

void Except_abort_already_handled(T *e, const char *file, const char *func, int line)
{
	fprintf(stderr, "[ERROR]:\t Exception '%s' is already handled \t at %s:%i, in %s()\n", e->reason,
		file, line, func);
	fprintf(stderr, "aborting...\n");
	
#ifdef NDEBUG	
	abort();
#else
	fprintf(stdout, "Testing: cancel the abortion and exiting success");
	exit(EXIT_SUCCESS);
#endif
}

void Except_init_signal_exceptions(void)
{
	struct sigaction sa;
	sa.sa_handler = Except_handle_signal;
	sa.sa_flags = 0;

	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGFPE, &sa, NULL);
	sigaction(SIGILL, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGSYS, &sa, NULL);
}

void Except_uninit_signal_exceptions(void)
{
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;

	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGFPE, &sa, NULL);
	sigaction(SIGILL, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGSYS, &sa, NULL);
}

void Except_link_handled(T *e)
{
	if (e == NULL)
		ABORT("Passed exception null");

	e->prev = Except_handled_stack;
	Except_handled_stack = e;
}

void Except_unlink_handlers(void)
{
	while (Except_handled_stack) {
		Except_handled_stack->flag = Except_nhandled;

		T *prev;
		prev = Except_handled_stack->prev;
		Except_handled_stack->prev = NULL;
		
		Except_handled_stack = prev;
	}
}

void Except_unlink_frame_stack(const Except_Frame *actual_frame)
{
	if (actual_frame == NULL)
		ABORT("Passed null Except frame");
	
	while (actual_frame != Except_frame_stack)
		Except_frame_stack = Except_frame_stack->prev;
	Except_frame_stack = Except_frame_stack->prev;
}

static int addr2line(const char *bin_name, const char *offset, char *src_name,
		      char *fun_name, int *line)
{
	char buffer[MAX_BACKTRACE_NAME_SIZE];
	sprintf(buffer, "addr2line -f -e %s %s", bin_name, offset);
	
	FILE *fp = popen(buffer, "r");
	if (fp == NULL)
		return -1;
	
	char output[MAX_BACKTRACE_NAME_SIZE];
	if (fgets(fun_name, MAX_BACKTRACE_NAME_SIZE, fp) == NULL)
		return -1;

	
	fun_name[strlen(fun_name) - 1] = '\0';
	if (fgets(output, MAX_BACKTRACE_NAME_SIZE, fp) == NULL)
		return -1;
	
	char *start = output;
	char *end = strchr(output, ':');
	int length = end - start;
	src_name[length] = '\0';
	memcpy(src_name, output, length);
	
	start = strchr(output, ':') + 1;
	*line = atoi(start);
	
	pclose(fp);

	return 0;
}

static void parse_backtrace_entry(const char *entry, char *offset, char *bin_name)
{
	char *start_name = strchr(entry, '(') + 1;
	char *end_name = strchr(start_name, ')');
	size_t length = end_name - start_name;
	memcpy(offset, start_name, length);
	offset[length] = '\0';
	
	start_name = (char *) entry;
	end_name = strchr(start_name, '(');
	length = end_name - start_name;
	memcpy(bin_name, start_name, length);
	bin_name[length] = '\0';
}

static int Except_print_traceback(void)
{
	if (Except_backtrace_strings == NULL)
		return -1;
	
	char offset[MAX_BACKTRACE_NAME_SIZE];
	char bin_name[MAX_BACKTRACE_NAME_SIZE];
	char src_name[MAX_BACKTRACE_NAME_SIZE];
	char fun_name[MAX_BACKTRACE_NAME_SIZE];
	int line;
	
	for (int i = 1 + Except_backtrace_raise_sig; i < Except_backtrace_size - 3; i++) {
		parse_backtrace_entry(Except_backtrace_strings[i], offset, bin_name);
		int ret = addr2line(bin_name, offset, src_name, fun_name, &line);
		if (i < Except_backtrace_size - 4)
			line--;
		if (strcmp(src_name, "??") != 0)
			fprintf(stderr, "\tFile \"%s\", line %i, called from %s()\n", src_name, line, fun_name);
		else if (ret == 0)
			fprintf(stderr, "\tBinary \"%s\", offset %s, called from %s()\n", bin_name, offset,
				fun_name);
		else
			fprintf(stderr, "\tBinary \"%s\", offset %s, called from somewhere()\n", bin_name,
				offset);
	}
	
	return 0;
}

static int gdb_check(void)
{
	char buf[4096];

	const int status_fd = open("/proc/self/status", O_RDONLY);
	if (status_fd == -1)
		return 0;

	const ssize_t num_read = read(status_fd, buf, sizeof(buf) - 1);
	close(status_fd);

	if (num_read <= 0)
		return 0;

	buf[num_read] = '\0';
	const char tracerPidString[] = "TracerPid:";
	const char *tracer_pid_ptr= strstr(buf, tracerPidString);
	if (!tracer_pid_ptr)
		return 0;

	for (const char* characterPtr = tracer_pid_ptr + sizeof(tracerPidString) - 1;
	     characterPtr <= buf + num_read; ++characterPtr) {
		if (isspace(*characterPtr))
			continue;
		else
			return isdigit(*characterPtr) != 0 && *characterPtr != '0';
	}
	
	return 0;
}

void Except_raise(T *e, int debuggable, const char *file, const char *func, int line, const char *msg)
{
	if (e == NULL)
		ABORT("Passed exception null");

	/* Capture the exception information */
	memset(&Except_raise_info, 0, sizeof(Except_raise_info));
	Except_raise_info.reason = e->reason;
	Except_raise_info.file = file;
	Except_raise_info.func = func;
	Except_raise_info.msg = msg;
	Except_raise_info.line = line;

	if (e->flag == Except_nhandled) {
		if (Except_frame_stack != NULL && Except_frame_stack->else_status == Except_handled) {
			free(Except_backtrace_strings);
			longjmp(Except_frame_stack->else_env, Except_raised);
		}

		
		/* Before aborting the program execute all finally blocks from the except frames,
		   Warning this will destroy the stack memory */
		while (Except_frame_stack) {
			int flag = setjmp(Except_frame_stack->except_raised_env);

			/* Stop the loop */
			if (Except_frame_stack == NULL)
				break;

			/* Re set the jump for the next except frame */
			if (flag != 0)
				continue;
			
			Except_Frame *prev = Except_frame_stack;
			Except_frame_stack = Except_frame_stack->prev;
			
			if (prev->finally_status == Except_handled)
				longjmp(prev->finally_env, Except_running_finally);
		}
		
		fprintf(stderr, "Traceback...\n");
		if (Except_backtrace_raise_sig == 0)
			fprintf(stderr, "\tFile \"%s\", line %i, raised in %s()\n", Except_raise_info.file,
				Except_raise_info.line, Except_raise_info.func);
		if (Except_print_traceback() != 0)
			fprintf(stderr, "\tImpossible to fetch traceback information...");
		
		fprintf(stderr, "UncaughtException:");
		
		if (Except_raise_info.reason)
			fprintf(stderr, "\t \"%s\"%s", Except_raise_info.reason, (Except_raise_info.msg)
				? "," : "\n");
		if (Except_raise_info.msg) fprintf(stderr, "\t \"%s\"\n", Except_raise_info.msg);
		
		free(Except_backtrace_strings);

		/* Checks if gdb is active and this raise is debugable */
		int using_gdb = gdb_check();
		if (using_gdb && debuggable) fprintf(stderr, "\nDumping...\n");
		else fprintf(stderr, "\nAborting...\n");
		fflush(stderr);

#ifdef NDEBUG
		// Try to dump the process into the debugger
		if (using_gdb && debuggable) return;
		else abort();
#else
		fprintf(stdout, "Testing: cancel the abortion and exiting success\n");
		exit(EXIT_SUCCESS);
#endif
	}

	free(Except_backtrace_strings);
	
	longjmp(e->env, Except_raised);
}

#undef T
