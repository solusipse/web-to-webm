/* This program replaces old binary with new,
 * downloaded from update serwer.
*/

#include <stdio.h>

#ifdef _WIN32
    #define SYSTEM 0
    #include <windows.h>
#elif __APPLE__
    #define SYSTEM 1
#else
    #define SYSTEM 2
#endif

void windows_procedure(char *argv);
void unix_procedure(char *argv);

int main (int argc, char *argv[]) {
    printf("Starting update procedure.\n");
    if (SYSTEM == 0)
        windows_procedure(argv[1]);
    else
        unix_procedure(argv[1]);
    /*
    int r;
    char oldname[] ="oldname.txt";
    char newname[] ="newname.txt";
    r = rename( oldname , newname );
    if ( r == 0 )
        puts ("File successfully renamed");
    else
        perror("Error renaming file");
    */
    return 0;
}

void windows_procedure(char *argv) {
    printf("Detected WINDOWS system, waiting for main process to die.\n");
    printf("PID: %s\n", argv);
    DWORD dwExitCode = 259;
    while(1) {
        HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, atoi(argv));
        if(GetExitCodeProcess(h, &dwExitCode))
            CloseHandle(h);
        else
            break;

        Sleep(1000);
    }
    printf("Process is dead now. Starting update.");
}

void unix_procedure(char *argv) {

}
