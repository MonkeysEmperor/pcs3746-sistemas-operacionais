#include "print_ppid.h"
#include <unistd.h>

int main()
{
   while (1) {
      print_ppid();
   }
   return 0;
}