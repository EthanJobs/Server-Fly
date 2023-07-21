#include <networkServer.h>
#include <processMes.h>
#include <coreMd5.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

int main() {
    server_start(processMes);
    
    return 0;
}
