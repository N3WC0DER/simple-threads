#!/usr/bin/env bash
DIR="$(dirname "$(realpath $BASH_SOURCE[0])")"
cd ${DIR}

echo "Build examples..."

if cmake --build .
then
  echo
else
  cmake ../src/
  cmake --build .
fi

mv ./example-simple-threads ./bin/
