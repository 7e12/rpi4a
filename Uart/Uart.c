/**
 * @author  7e12
 * @date    20 May 2021
 * @version v1.0.0
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "Uart.h"

/** @brief Open and initialise the serial port, setting all the right port parameters */
JNIEXPORT jint JNICALL Java_com_example_raspio_Uart_open(JNIEnv *env, jobject obj, jint uart, jint baud) {
    char device[32];
    struct termios option;
    speed_t sys_baud;
    int fd, status;
    
    switch (baud) {
        case      50:	sys_baud =      B50; break;
        case      75:	sys_baud =      B75; break;
        case     110:	sys_baud =     B110; break;
        case     134:	sys_baud =     B134; break;
        case     150:	sys_baud =     B150; break;
        case     200:	sys_baud =     B200; break;
        case     300:	sys_baud =     B300; break;
        case     600:	sys_baud =     B600; break;
        case    1200:	sys_baud =    B1200; break;
        case    1800:	sys_baud =    B1800; break;
        case    2400:	sys_baud =    B2400; break;
        case    4800:	sys_baud =    B4800; break;
        case    9600:	sys_baud =    B9600; break;
        case   19200:	sys_baud =   B19200; break;
        case   38400:	sys_baud =   B38400; break;
        case   57600:	sys_baud =   B57600; break;
        case  115200:	sys_baud =  B115200; break;
        case  230400:	sys_baud =  B230400; break;
        default: return -1;
    }

    sprintf(device, "/dev/ttyS%d", uart);
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (fd == -1)
        return -1;

    fcntl(fd, F_SETFL, O_RDWR);

    // Get and modify current option:
    tcgetattr(fd, &option);
    cfmakeraw(&option);
    cfsetispeed(&option, sys_baud);
    cfsetospeed(&option, sys_baud);

    option.c_cflag |= (CLOCAL | CREAD);
    option.c_cflag &= ~PARENB;
    option.c_cflag &= ~CSTOPB;
    option.c_cflag &= ~CSIZE;
    option.c_cflag |= CS8;
    option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    option.c_oflag &= ~OPOST;

    option.c_cc[VMIN]  =   0;
    option.c_cc[VTIME] = 100; // seconds

    tcsetattr(fd, TCSANOW, &option);
    ioctl(fd, TIOCMGET, &status);

    status |= TIOCM_DTR;
    status |= TIOCM_RTS;

    ioctl(fd, TIOCMSET, &status);
    usleep (10000) ; // 10ms

    return fd;
};

/** @brief Release the serial port */
JNIEXPORT void JNICALL Java_com_example_raspio_Uart_close(JNIEnv *env, jobject obj, jint fd) {
    close (fd);
};

JNIEXPORT jint JNICALL Java_com_example_raspio_Uart_read(JNIEnv *env, jobject obj, jint fd, jbyteArray buf, jint len) {
    if (read(fd, buf, len) == 0)
        return -1 ;

    return 1;
};

/** @brief Send a string to the serial port */
JNIEXPORT void JNICALL Java_com_example_raspio_Uart_write(JNIEnv *env, jobject obj, jint fd, jbyteArray buf, jint len) {
    write(fd, buf, len);
};

/** @brief Return the number of bytes of data available to be read in the serial port */
JNIEXPORT jint JNICALL Java_com_example_raspio_Uart_isDataAvailable(JNIEnv *env, jobject obj, jint fd) {
    int ret;

    if (ioctl(fd, FIONREAD, &ret) == -1)
        return -1 ;

    return ret;
};

/** @brief Flush the uart buffers (both Tx & Rx) */
JNIEXPORT void JNICALL Java_com_example_raspio_Uart_flush(JNIEnv *env, jobject obj, jint fd) {
    tcflush(fd, TCIOFLUSH);
};