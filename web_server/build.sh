#!/bin/bash

# cleanup
#rm echo_client
#rm echo_server

# build client
gcc echo_client.c -o echo_client

# build_server
gcc echo_server.c -o echo_server
