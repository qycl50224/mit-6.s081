#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
 fmtname(char *path)
 {
	 static char buf[DIRSIZ+1];
	 char *p;
	 
	 // Find first character after last slash.
	 for(p=path+strlen(path); p >= path && *p != '/'; p--)
	   ;
	 p++;
	 
	 // Return blank-padded name.
	 if(strlen(p) >= DIRSIZ)
	   return p;
	 memmove(buf, p, strlen(p));
	 memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	 return buf;
}

void
find(char *path, char *f_name)
{
	char buf[512], *p;
	static char f_buf[DIRSIZ+1];
	int fd;
	struct dirent de;
	struct stat st;
	
	memmove(f_buf, f_name, strlen(f_name));
	memset(f_buf+strlen(f_name), ' ', DIRSIZ-strlen(f_name));
	// get fd of path
	if ((fd = open(path, 0)) < 0){
		//fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	// get all info about this dir
	// store info about fd in st, which is a stat
	if (fstat(fd, &st) < 0){
		//fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}
    // recursive find all dirs	
	// consider . and ..   both of them ino is 1
	switch(st.type){
	case T_FILE:
		//printf("fild is :%s   cmp == %d\n", fmtname(path), strcmp(fmtname(path), f_buf));
		//printf("%d %d\n", strlen(fmtname(path)), strlen(f_buf));

		if (strcmp(fmtname(path), f_buf) == 0) {
			printf("%s\n", path);
		}
		break;
	case T_DIR:
		if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
		  //printf("ls: path too long\n");
		  break;
		}
		strcpy(buf, path);
		p = buf+strlen(buf);
		*p++ = '/';
		while(read(fd, &de, sizeof(de)) == sizeof(de)){
		  if(de.inum == 0)
			continue;
		  memmove(p, de.name, DIRSIZ);
		  p[DIRSIZ] = 0;
		  // get path from buf and store info in st
		  if(stat(buf, &st) < 0){
			//printf("ls: cannot stat %s\n", buf);
			continue;
		  }
		  // avoid . and ..
		  if (*fmtname(buf) == '.')  continue;
		  //printf("-----%s\n", buf);
		  find(buf, f_name);
		  //printf("=====%s\n", p);
		  //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
		}
		break;
	}
	close(fd);
}

int
main(int argc, char* argv[])
{
	if (argc <= 2){
		fprintf(2, "no argument");
	} else if (argc == 3) {
		find(argv[1], argv[2]);
		exit(0);
	} else {
		fprintf(2, "too much arguments");
		exit(1);
	}
	return 0;
}

