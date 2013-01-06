#!/bin/bash
gcc snda_ecs_console.c -I./include -L./lib  -L/usr/local/ssl/lib  -lsnda_ecs_sdk -lcurl  -lxml2 -lssl -lcrypto -o snda_ecs_console

