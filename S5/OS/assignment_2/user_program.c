#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/char_driver"

int main() 
{
    int fd;
    char write_buff[256];
    char read_buff[256];
    char choice;

    // Open the device
    fd = open(DEVICE, O_RDWR);
    if (fd < 0) 
    {
        perror("Failed to open the device");
        return -1;
    }

    printf("Enter 'r' to read from the device, 'w' to write to the device: ");
    scanf(" %c", &choice);

    if (choice == 'w') 
    {
        printf("Enter the string to write: ");
        scanf(" %[^\n]", write_buff);
        int i = strlen(write_buff);
        write_buff[i] = '\n';
        // Write to the device
        if (write(fd, write_buff, strlen(write_buff)) < 0) 
        {
            perror("Failed to write to the device");
            close(fd);
            return -1;
        }
        printf("Written to device\n");

    } 
    else if (choice == 'r') 
    {
        // Read from the device
        if (read(fd, read_buff, sizeof(read_buff)) < 0) {
            perror("Failed to read from the device");
            close(fd);
            return -1;
        }
        printf("Read from device: %s\n", read_buff);

    } 
    else 
    {
        printf("Invalid choice\n");
    }

    // Close the device
    close(fd);
    return 0;
}
