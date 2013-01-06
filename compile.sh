#!/bin/bash
gcc  src/snda_ecs_sdk/*.c src/example/snda_ecs_console.c -I./src/sdk  -I/usr/local/include/ -I/usr/local/ssl/include/ -I/usr/local/include/libxml2/ -L/usr/local/ssl/lib  -lcurl  -lxml2 -lssl -lcrypto -o snda_ecs_console
