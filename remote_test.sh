#!/usr/bin/env bash

#TARGET_PATH="/home/panther/Desktop/dev/px-software"
TARGET_PATH="/home/panther/projects/px-software"
SERVER_ADDRESS="panther@127.0.0.1"

#ssh ${SERVER_ADDRESS} -p 2222 "rm -rf $TARGET_PATH;"

ssh ${SERVER_ADDRESS} -p 2222 "mkdir -p $TARGET_PATH"

rsync -av -e "ssh -p 2222" --exclude ".git" --exclude "cmake-build-debug" --exclude "docs/html" --exclude ".idea" "." "$SERVER_ADDRESS:$TARGET_PATH/"

CMD='export GUILE_LOAD_PATH=$GUILE_LOAD_PATH:.'
CMD="$CMD;
     cd $TARGET_PATH;
     mkdir -p build && cd build ;
     cmake .. &&
        make
        && echo '#####################################################################################'
        && echo 'DONE'
     "

ssh ${SERVER_ADDRESS} -p 2222 ${CMD}
