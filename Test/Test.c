#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    int fd, ret, len, val = 1, gpio = 22;
    char str[64], type[] = "out";

    fd = open("/sys/class/gpio/export", O_WRONLY);
    printf("export open: %d\n", fd);

    memset(str, 0, 64);
    sprintf(str, "%d", gpio);
    len = strlen(str);
    ret = write(fd, str, len);
    printf("export write: %d\n", ret);

    close(fd);

    memset(str, 0, 64);
    sprintf(str, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(str, O_RDWR);
    printf("direction open: %d\n", fd);

    len = strlen(type);
    ret = write(fd, type, len);
    printf("direction write: %d\n", ret);

    close(fd);

    memset(str, 0, 64);
    sprintf(str, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(str, O_RDWR);
    printf("value open: %d\n", fd);

    memset(str, 0, 64);
    sprintf(str, "%d", val);
    ret = write(fd, str, 1);
    printf("value write: %d | ret: %d\n", val, ret);

    close(fd);

    printf("Hallo");

    return 0;
}

// export PATH=/home/7e12/Android/Sdk/ndk/22.1.7171670:$PATH
// export PATH=/home/7e12/Documents/Projects/EmbeddedAndroid/CrossCompile/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin:$PATH