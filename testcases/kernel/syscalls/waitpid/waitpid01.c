// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) International Business Machines Corp., 2001
 *    07/2001 John George
 * Copyright (c) 2018 Cyril Hrubis <chrubis@suse.cz>
 */

/*
 * Check that when a child kills itself by SIGALRM the waiting parent is
 * correctly notified.
 *
 * Fork a child that raises(SIGALRM), the parent checks that SIGALRM was
 * returned.
 */
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "tst_test.h"

static void run(void)
{
	pid_t pid;
	int status;

	//pid = SAFE_FORK();
	pid = vfork();
	if (!pid) {
		raise(SIGALRM);
		exit(0);
	}

	TST_EXP_PID_SILENT(waitpid(pid, &status, 0));
	if (!TST_PASS)
		return;

	if (TST_RET != pid) {
		tst_res(TFAIL, "waitpid() returned wrong pid %li, expected %i",
			TST_RET, pid);
	} else {
		tst_res(TPASS, "waitpid() returned correct pid %i", pid);
	}

	if (!WIFSIGNALED(status)) {
		tst_res(TFAIL, "WIFSIGNALED() not set in status (%s)",
		        tst_strstatus(status));
		return;
	}

	tst_res(TPASS, "WIFSIGNALED() set in status");

	if (WTERMSIG(status) != SIGALRM) {
		tst_res(TFAIL, "WTERMSIG() != SIGALRM but %s",
		        tst_strsig(WTERMSIG(status)));
		return;
	}

	tst_res(TPASS, "WTERMSIG() == SIGALRM");
}

static struct tst_test test = {
	.forks_child = 1,
	.test_all = run,
};
