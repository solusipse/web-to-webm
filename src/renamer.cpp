/* rename example */
#include <stdio.h>

int main ()
{
    int result;
    char oldname[] ="oldname.txt";
    char newname[] ="newname.txt";
    result= rename( oldname , newname );
    if ( result == 0 )
        puts ( "File successfully renamed" );
    else
        perror( "Error renaming file" );
    return 0;
}