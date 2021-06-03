#!/bin/sh

while getopts wlm flag
do
    case "${flag}" in
        w) os="win32";;
        l) os="linux";;
        m) os="darwin";;
    esac
done

input=$2
if [ ${input#*.} = "c" ]
then
    gnu="gcc"
elif [ ${input#*.} = "cpp" ]
then
    gnu="g++"
else
    echo "C/C++ input not found."
    exit
fi

export PATH=/home/7e12/Documents/Projects/EmbeddedAndroid/CrossCompile/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin:$PATH

#javac ${input%.*}.java -h .

if [ $os = "win32" ]
then
    directory=""
    output="${input%.*}.dll"
    option=""
elif [ $os = "linux" ]
then
    directory=$(dirname $(dirname $(readlink -f $(which javac))))
    output="lib${input%.*}.so"
    option="-fPIC"
elif [ $os = "darwin" ]
then
    directory=$(/usr/libexec/java_home)
    output="lib${input%.*}.jnilib"
    option=""
fi

echo "arm-linux-gnueabihf-$gnu -I"$directory/include" -I"$directory/include/$os/" -o $output -shared $option $input"

arm-linux-gnueabihf-$gnu -I"$directory/include" -I"$directory/include/$os/" -o $output -shared $option $input

echo "JNI library generated!"
exit
