#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

void assert_read(int fildes, void *buf, size_t nbyte) {
	ssize_t out = read(fildes, buf, nbyte);
	if (out == -1)
		err(2, "read");
	if (out < nbyte)
		errx(1, "short read (%zd/%zu)", out, nbyte);
}

char *readnetstring(int fd, char *buf, size_t buflen, size_t *len) {
	assert_read(fd, buf, 3);

	int off = 3;
	if (memchr(buf, ':', 3) == 0) {
		assert_read(fd, buf+3, 102);
		off = 105;
	}

	char *end;
	*len = strtol(buf, &end, 10);
	if (end == 0 || *end != ':')
		errx(1, "netstring length not interpretable");

	size_t datalen = *len+(end-buf)+2;
	if (datalen > buflen)
		errx(1, "netstring length exceeds limit (%zu/%zu)", datalen, buflen);

	assert_read(fd, buf+off, datalen-off);
	if (end[*len+1] != ',')
		errx(1, "netstring terminator missing");
	end[*len+1] = 0;

	return end+1;
}

void pusharray(char **ary, int maxlen, int *len, char *new) {
	if (*len >= maxlen)
		errx(1, "too many arguments");
	ary[(*len)++] = new;
}

int main(int argc, char **argv) {
	char stackbuf[65536];

	char *abuf[1024];
	int amax = sizeof abuf / sizeof *abuf;
	int alen = 0;

	for (argv++; *argv; argv++)
		pusharray(abuf, amax, &alen, *argv);

	size_t len;
	char *str = readnetstring(0, stackbuf, sizeof stackbuf, &len);
	for (size_t x = 0; x < len; x += strlen(str+x)+1)
		pusharray(abuf, amax, &alen, str+x);

	execvp(abuf[0], abuf);
	err(2, "execvp");
}
