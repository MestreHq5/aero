#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *find_airline(const char *line, const char *key_airline) {
    char *result = strstr(line, key_airline);
    if (result != NULL) {
        return result + strlen(key_airline);
    }
    return NULL;
}

int main(void) {
    const char *line = "This is a line without the AIRLINE: key";
    const char *key_airline = "AIRLINE: ";

    char *airline = find_airline(line, key_airline);
    if (airline != NULL) {
        printf("%s\n", airline);
    } else {
        printf("Key not found.\n");
    }

    return 0;
}
