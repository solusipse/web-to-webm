/* This program replaces old binary with new,
 * downloaded from update serwer.
*/

#include <stdio.h>



int main() {
    int r;
    char oldname[] ="oldname.txt";
    char newname[] ="newname.txt";
    r = rename( oldname , newname );
    if ( r == 0 )
        puts ("File successfully renamed");
    else
        perror("Error renaming file");
    return 0;
}
