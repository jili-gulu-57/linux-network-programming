#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<cstdio>
#include<string>

const std::string fifoname="fifo";
mode_t mode=0666;

int size=128;   //变长数组，c99之后支持

#define SIZE 128


#endif