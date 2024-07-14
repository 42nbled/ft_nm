#ifndef FT_NM_H
# define FT_NM_H

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
	t_eident *e_ident;
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
	union {
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
	};

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

#endif
