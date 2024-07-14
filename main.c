#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ft_nm.h"
#include "read_unit.h"
#include "init_file.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: %s [file]\n", argv[0]);
		return (1);
	}
	int fd;
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "Error while opening the file.");
		return (1);
	}

	t_eident	e_ident = {0};
	init_filestart(fd, &e_ident);
	if (e_ident.EI_CLASS == 1) {
		// 32 bits
		t_fileinfo32	fileinfo = {0};
		if (e_ident.EI_DATA == 1)
			init_fileinfo32(fd, &fileinfo, read_word_le, read_dword_le);
		else
			init_fileinfo32(fd, &fileinfo, read_word_be, read_dword_be);

		lseek(fd, fileinfo.e_phoff, SEEK_SET);

		for (int i=0; i < fileinfo.e_phnum; i++)
		{
			t_proginfo32	proginfo = {0};
			if (e_ident.EI_DATA == 1)
				init_proginfo32(fd, &proginfo, read_dword_le);
			else
				init_proginfo32(fd, &proginfo, read_dword_be);
			printf("segment offset: %d\n", proginfo.p_offset);
		}
	} else {
		// 64 bits
		t_fileinfo64	fileinfo = {0};
		if (e_ident.EI_DATA == 1)
			init_fileinfo64(fd, &fileinfo, read_word_le, read_dword_le, read_qword_le);
		else
			init_fileinfo64(fd, &fileinfo, read_word_be, read_dword_be, read_qword_be);

		int section_offset = fileinfo.e_shoff + fileinfo.e_shstrndx * fileinfo.e_shentsize;
		lseek(fd, section_offset, SEEK_SET);
		t_secinfo64		secinfo = {0};
		init_secinfo64(fd, &secinfo, read_dword_le, read_qword_le);
		lseek(fd, secinfo.sh_offset, SEEK_SET);

		char *buffer = malloc(secinfo.sh_size);
		read(fd, buffer, secinfo.sh_size);
		printf("section name: %s\n", buffer);
		printf("section size: %ld\n", secinfo.sh_size);
		free(buffer);

//		for (int i=0; i < fileinfo.e_phnum; i++)
//		{
//			t_proginfo64	proginfo = {0};
//			if (e_ident.EI_DATA == 1)
//				init_proginfo64(fd, &proginfo, read_dword_le, read_qword_le);
//			else
//				init_proginfo64(fd, &proginfo, read_dword_be, read_qword_be);
//			printf("segment offset: %ld\n", proginfo.p_offset);
//		}
	}

	close(fd);
}
