#!/bin/bash
gcc test.cpp -I./include -L./lib  -L/usr/local/ssl/lib  -lsnda_ecs_sdk -lcurl  -lxml2 -lssl -lcrypto -o test
