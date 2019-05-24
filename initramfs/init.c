
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void print_file() {
    int fp;
    fp = open("text.txt", O_RDONLY);
    if(fp < 0)
    {
        write(1, "Error\n", 6);
        return;
    }
    char c[1];
    while(read(fp, &c, 1))
    {
        write(1, &c, sizeof(c));
    }
    close(fp);

}
int main() {
    while(1){
		print_file();
	}
	
    return 0;
}
