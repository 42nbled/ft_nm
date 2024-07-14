#ifndef READ_UNIT_H
# define READ_UNIT_H

typedef struct s_word
{
	union
	{
		char	data[2];
		short	value;
	};
}	u_word;

typedef struct s_dword
{
	union
	{
		char	data[4];
		int		value;
	};
}	u_dword;

typedef struct s_qword
{
	union
	{
		char	data[8];
		long	value;
	};
}	u_qword;


char	read_byte(int fd);

short	read_word_le(int fd);
int		read_dword_le(int fd);
long	read_qword_le(int fd);

short	read_word_be(int fd);
int		read_dword_be(int fd);
long	read_qword_be(int fd);



#endif // READ_UNIT_H
