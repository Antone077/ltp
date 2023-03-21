// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) International Business Machines Corp., 2001
 * Copyright (c) Linux Test Project, 2009-2022
 */

/*\
 * [Description]
 *
 * Verify that setuid(2) returns 0 and effective uid has
 * been set successfully as a normal or super user.
 */

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "tst_test.h"
#include "compat_tst_16.h"
#include "lapi/syscalls.h"

static void verify_setuid(void)
{
	uid_t uid;

	uid = getuid();
	UID16_CHECK(uid, setuid);

	TEST(tst_syscall(__NR_setuid, uid));

	//TST_EXP_PASS(SETUID(uid), "setuid(%d)", uid);
	if(TST_RET < 0){
		tst_brk(TBROK | TTERRNO, "SetUid not supported in this ARCH");
	}else{
		TST_EXP_PASS(SETUID(uid), "setuid(%d)", uid);
	}
}

static struct tst_test test = {
	.test_all = verify_setuid,
};
