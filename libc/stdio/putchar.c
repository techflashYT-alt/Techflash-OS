#include <stdio.h>
#include <stdbool.h>
#if defined(__is_libk)
#include <kernel/tty.h>
#endif
int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	terminalWrite(&c, sizeof(c));
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}
