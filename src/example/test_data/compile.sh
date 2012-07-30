#!/bin/bash
gcc  -Wall src/*.c $1 -I./sdk  -I/usr/local/include/ -I/usr/local/ssl/include/ -I/usr/local/include/libxml2/ -L/usr/local/ssl/lib  -lcurl  -lxml2 -lssl -lcrypto -o $2
