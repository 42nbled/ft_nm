#include <unistd.h>
#include "read_unit.h"

char	read_byte(int fd)
{
	char	c;
	read(fd, &c, 1);
	return (c);
}

short	read_word_le(int fd)
{
	u_word buffer;
	read(fd, &(buffer.data), 2);
	return buffer.value;
}

int	read_dword_le(int fd)
{
	u_dword buffer;
	read(fd, &(buffer.data), 4);
	return buffer.value;
}

long	read_qword_le(int fd)
{
	u_qword buffer;
	read(fd, &(buffer.data), 8);
	return buffer.value;
}

short	read_word_be(int fd)
{
	u_word le;

	read(fd, &(le.data), 2);
	u_word be;
	be.data[0] = le.data[1];
	be.data[1] = le.data[0];
	return be.value;
}

int	read_dword_be(int fd)
{
	u_dword le;
	read(fd, &(le.data), 4);
	u_dword be;
	be.data[0] = le.data[3];
	be.data[1] = le.data[2];
	be.data[2] = le.data[1];
	be.data[3] = le.data[0];
	return be.value;
}

long	read_qword_be(int fd)
{
	u_qword le;
	read(fd, &(le.data), 8);
	u_qword be;
	be.data[0] = le.data[7];
	be.data[1] = le.data[6];
	be.data[2] = le.data[5];
	be.data[3] = le.data[4];
	be.data[4] = le.data[3];
	be.data[5] = le.data[2];
	be.data[6] = le.data[1];
	be.data[7] = le.data[0];
	return be.value;
}
