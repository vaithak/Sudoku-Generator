#!/bin/bash

echo "Compiling the code..."

str=$(g++ sudokuGen.cpp -o sudokuGen)
if [ -z $str ]; then
    echo "Succesfully compiled :)"

    unamestr=$(uname)
    if [[ "$unamestr" == 'Linux' ]]; then
      platform='linux'
    elif [[ "$unamestr" == 'Darwin' ]]; then
      platform='macos'
    fi

    str="alias 'sudokuGen'='$(pwd)/sudokuGen'"

    if [[ "$platform" == 'linux' ]]; then
      if !(grep -q sudokuGen ~/.bashrc) ; then
        echo $str >> $HOME/.bashrc
      fi
    elif [[ "$platform" == 'macos' ]]; then
      if !(grep -q sudokuGen ~/.bash_profile) ; then
        echo $str >> $HOME/.bash_profile
      fi
    fi
fi
