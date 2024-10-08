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
//	t_eident *e_ident;
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
//	union {
//		unsigned char   e_ident[16];
//		struct
//		{
//			char	EI_MAG0;
//			char	EI_MAG1;
//			char	EI_MAG2;
//			char	EI_MAG3;
//			char	EI_CLASS;
//			char	EI_DATA;
//			char	EI_VERSION;
//			char	EI_OSABI;
//			char	EI_ABIVERSION;
//			char	EI_PAD[7];
//		};
//	};
//	t_eident *e_ident;

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
	int		i_value;
	char	type;
} t_name_table;

struct s_pair {
	t_Elf64_Sym	*symtable;
	char		*strtable;
};

typedef struct s_lst {
	struct s_lst	*next;
	t_name_table	*data;
} t_lst;


typedef struct s_flags {
	unsigned int	a : 1;
	unsigned int	g : 1;
	unsigned int	u : 1;
	unsigned int	r : 1;
	unsigned int	p : 1;
} t_flags;

// LST.C FUNCTION
t_lst	*lst_last(t_lst *node);
void	lst_append(t_lst *root, t_name_table *table);
void	lst_clear(t_lst *root);
int		lst_size(t_lst *node);
int		diff_str(t_lst *a, t_lst *b);
int		reverse_diff_str(t_lst *a, t_lst *b);
void	ft_swap(t_lst **a, t_lst **b);
void	ft_lst_swap(t_lst **header, t_lst *a, t_lst *b);
void	ft_lst_sort(t_lst **begin_list, int (*cmp)());

#endif
