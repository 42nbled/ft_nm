#ifndef FT_NM_H
# define FT_NM_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define	PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_TLS		7
#define	PT_NUM		8
#define PT_LOOS		0x60000000
#define PT_GNU_EH_FRAME	0x6474e550
#define PT_GNU_STACK	0x6474e551
#define PT_GNU_RELRO	0x6474e552
#define PT_GNU_PROPERTY	0x6474e553
#define PT_LOSUNW	0x6ffffffa
#define PT_SUNWBSS	0x6ffffffa
#define PT_SUNWSTACK	0x6ffffffb
#define PT_HISUNW	0x6fffffff
#define PT_HIOS		0x6fffffff
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

#define PF_X		(1 << 0)
#define PF_W		(1 << 1)
#define PF_R		(1 << 2)
#define PF_MASKOS	0x0ff00000
#define PF_MASKPROC	0xf0000000

# define	STB_LOCAL	0
# define	STB_GLOBAL	1
# define	STB_WEAK	2
# define	STB_LOOS	10
# define	STB_HIOS	12
# define	STB_LOPROC	13
# define	STB_HIPROC	15

# define	STT_NOTYPE	0
# define	STT_OBJECT	1
# define	STT_FUNC	2
# define	STT_SECTION	3
# define	STT_FILE	4
# define	STT_COMMON	5
# define	STT_TLS		6
# define	STT_LOOS	10
# define	STT_HIOS	12
# define	STT_LOPROC	13
# define	STT_HIPROC	15

// Section types
#define SHT_SYMTAB      2
#define SHT_DYNSYM      11
#define SHT_NOBITS      8
#define SHT_PROGBITS    1

// Section flags
#define SHF_WRITE       (1 << 0)
#define SHF_ALLOC       (1 << 1)
#define SHF_EXECINSTR   (1 << 4)

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define ELF64_ST_BIND(i)   ((i)>>4)
#define ELF64_ST_TYPE(i)   ((i)&0xf)
#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))




typedef union s_eident
{
	unsigned char   e_ident[16];
	struct
	{
		char	EI_MAG0;
		char	EI_MAG1;
		char	EI_MAG2;
		char	EI_MAG3;
		char	EI_CLASS;
		char	EI_DATA;
		char	EI_VERSION;
		char	EI_OSABI;
		char	EI_ABIVERSION;
		char	EI_PAD[7];
	};
} t_eident;

typedef struct s_fileinfo32
{
	short	 e_type;
	short	 e_machine;
	int		 e_version;
	int		 e_entry;
	int		 e_phoff;
	int		 e_shoff;
	int		 e_flags;
	short	 e_ehsize;
	short	 e_phentsize;
	short	 e_phnum;
	short	 e_shentsize;
	short	 e_shnum;
	short	 e_shstrndx;
}	t_fileinfo32;

typedef struct s_fileinfo64
{
	short	e_type;
	short	e_machine;
	int		e_version;
	long	e_entry;
	long	e_phoff;
	long	e_shoff;
	int		e_flags;
	short	e_ehsize;
	short	e_phentsize;
	short	e_phnum;
	short	e_shentsize;
	short	e_shnum;
	short	e_shstrndx;
}	t_fileinfo64;

typedef struct s_proginfo32 {
	int		p_type;
	int		p_offset;
	int		p_vaddr;
	int		p_paddr;
	int		p_filesz;
	int		p_memsz;
	int		p_flags;
	int		p_align;
} t_proginfo32;

typedef struct s_proginfo64 {
	int		p_type;
	int		p_flags;
	long	p_offset;
	long	p_vaddr;
	long	p_paddr;
	long	p_filesz;
	long	p_memsz;
	long	p_align;
}	t_proginfo64;

typedef struct s_secinfo32 {
	int		sh_name;
	int		sh_type;
	int		sh_flags;
	int		sh_addr;
	int		sh_offset;
	int		sh_size;
	int		sh_link;
	int		sh_info;
	int		sh_addralign;
	int		sh_entsize;
}	t_secinfo32;

typedef struct t_secinfo64 {
	int		sh_name;
	int		sh_type;
	long	sh_flags;
	long	sh_addr;
	long	sh_offset;
	long	sh_size;
	int		sh_link;
	int		sh_info;
	long	sh_addralign;
	long	sh_entsize;
}	t_secinfo64;

typedef struct s_Elf32_Sym {
	int				st_name;
	int				st_value;
	int				st_size;
	unsigned char	st_info;
	unsigned char	st_other;
	short			st_shndx;
} t_Elf32_Sym;

typedef struct s_Elf64_Sym {
	int				st_name;
	unsigned char	st_info;
	unsigned char	st_other;
	short			st_shndx;
	long long		st_value;
	long long		st_size;
} t_Elf64_Sym;

typedef struct s_name_table {
	char	*name;
	char	value[17];
	char	type;
	int		i_value;
	int		stt_type;
} t_name_table;

struct s_pair {
	t_Elf64_Sym	*symtable;
	char		*strtable;
};

typedef struct s_lst {
	struct s_lst	*next;
	t_name_table	*data;
} t_lst;

typedef struct s_files {
	struct s_files	*next;
	char			*name;
} t_files;

typedef struct s_flags {
	unsigned int	a : 1;
	unsigned int	g : 1;
	unsigned int	u : 1;
	unsigned int	r : 1;
	unsigned int	p : 1;
} t_flags;

// argc_check.c
t_files 	*arg_check(t_flags *flags, int argc, char **argv);

// get_symbol_type.c
char		get_symbol_type(char *secname, int bind, int type, unsigned long addr, short st_shndx);

// parse_table_32.c
int			run_32(int fd, t_fileinfo32 fileh, t_lst *root);

// parse_table_64.c
int			run_64(int fd, t_fileinfo64 fileh, t_lst *root);

// print_nm.c
void		print_nm32(t_lst name_tables, t_flags flags, char *executable_name);
void		print_nm64(t_lst name_tables, t_flags flags, char *executable_name);

// sort.c
int			diff_str(t_lst *a, t_lst *b);
int			reverse_diff_str(t_lst *a, t_lst *b);

// lst.c
t_lst		*lst_last(t_lst *node);
int			lst_append(t_lst *root, t_name_table *table);
void		lst_clear(t_lst *root);
int			lst_size(t_lst *node);
void		ft_swap(t_lst **a, t_lst **b);
void		ft_lst_swap(t_lst **header, t_lst *a, t_lst *b);
void		ft_lst_sort(t_lst **begin_list, int (*cmp)());

#endif
