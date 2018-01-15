#!/bin/bash
location=$(git rev-parse --git-dir)

MY_PATH="`dirname \"$0\"`"              # relative
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"  # absolutized and normalized
if [ -z "$MY_PATH" ] ; then
  # error; for some reason, the path is not accessible
  # to the script (e.g. permissions re-evaled after suid)
  exit 1  # fail
fi

echo "\
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Copy git-hooks so git pull/checkout
triggers git submodule update and 
if it can checkout the targeted branch
without changing the head sha, it does
it. 
.git folder location
$location
script location
$MY_PATH
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

cp -r $MY_PATH/hooks $location
