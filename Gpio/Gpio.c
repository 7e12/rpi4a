/**
 * @author  7e12
 * @date    20 May 2021
 * @version v1.0.0
**/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "Gpio.h"

/** @brief Export the desired pin by writing to /sys/class/gpio/export */
JNIEXPORT jint JNICALL Java_com_example_raspio_Gpio_open(JNIEnv *env, jobject obj, jint gpio) {
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/export");
        return -1;
    }

    char str[4];
    sprintf(str, "%d", gpio);
    int len = strlen(str);
    if (write(fd, str, len) != len) {
        perror("Error writing to /sys/class/gpio/export");
        return -1;
    }

    close(fd);
    return 1;
};

/** @brief Unexport the desired pin by writing to /sys/class/gpio/unexport */
JNIEXPORT void JNICALL Java_com_example_raspio_Gpio_close(JNIEnv *env, jobject obj, jint gpio) {
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/unexport");
        exit(1);
    }

    char str[4];
    sprintf(str, "%d", gpio);
    int len = strlen(str);
    if (write(fd, str, len) != len) {
        perror("Error writing to /sys/class/gpio/unexport");
        exit(1);
    }

    close(fd);
};

/** @brief Set the desired pin to be an input/output by writing "in"/"out" to /sys/class/gpio/gpio<id>/direction */
JNIEXPORT void JNICALL Java_com_example_raspio_Gpio_setDirection(JNIEnv *env, jobject obj, jint gpio, jint dir) {
    char str[36];
    sprintf(str, "/sys/class/gpio/gpio%d/direction", gpio);
    int fd = open(str, O_RDWR);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio<id>/direction");
        exit(1);
    }

    char dir_str[4];
    if (dir) {
        sprintf(dir_str, "in");
    } else {
        sprintf(dir_str, "out");
    }

    int len = strlen(dir_str);
    if (write(fd, dir_str, len) != len) {
        perror("Error writing to /sys/class/gpio/gpio<id>/direction");
        exit(1);
    }

    close(fd);
};

/** @brief Get the desired pin's value by reading from /sys/class/gpio/gpio<id>/value */
JNIEXPORT jint JNICALL Java_com_example_raspio_Gpio_getValue(JNIEnv *env, jobject obj, jint gpio) {
    char str[36];
    sprintf(str, "/sys/class/gpio/gpio%d/value", gpio);
    int fd = open(str, O_RDWR);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio<id>/value");
        return -1;
    }

    char ret_str[4];
    if (read(fd, ret_str, 4) == 0) {
        perror("Error reading from /sys/class/gpio/gpio<id>/value");
        return -1;
    }

    close(fd);
    return atoi(ret_str);
};

/** @brief Set the desired pin's value by writing to /sys/class/gpio/gpio<id>/value */
JNIEXPORT void JNICALL Java_com_example_raspio_Gpio_setValue(JNIEnv *env, jobject obj, jint gpio, jint val) {
    char str[36];
    sprintf(str, "/sys/class/gpio/gpio%d/value", gpio);
    int fd = open(str, O_RDWR);
    if (fd == -1) {
        perror("Unable to open /sys/class/gpio/gpio<id>/value");
        exit(1);
    }

    if (1 != val && 0 != val) {
        perror("Value not available");
        exit(1);
    }

    char wr_str[4];
    sprintf(wr_str, "%d", val);
    if (write(fd, wr_str, 1) != 1) {
        perror("Error writing to /sys/class/gpio/gpio<id>/value");
        exit(1);
    }

    close(fd);
};
