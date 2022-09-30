#!/bin/env bash
while [ -n "$1" ]
do
  case "$1" in 
    --examples) example=true ;;
    --help) echo "To build ONLY the library, just type \033[36m./build.sh\033[0m.\nTo build examples, type \033[36m./build.sh --examples\033[0m (Examples are built with the library)"
      exit 1 ;;
    *) echo "To build ONLY the library, just type \033[36m./build.sh\033[0m.\nTo build examples, type \033[36m./build.sh --examples\033[0m (Examples are built with the library)"
      exit 1 ;;
esac
shift
done

DIR="$(dirname "$(realpath $BASH_SOURCE[0])")"
cd ${DIR}

echo "Build library..."

# Build library
if cmake ../src/
then
  echo
else
  exit 1
fi

if cmake --build .
then
  echo
else
  exit 1
fi

mkdir bin 2>/dev/null
mkdir ../example/bin 2>/dev/null
mkdir ../example/bin/include 2>/dev/null

cp -f ./libsimple-threads.a ../example/bin/ 
mv -f ./libsimple-threads.a ./bin/

mkdir ../example/bin/include 2>/dev/null
cp -r ../src/* ../example/bin/include/
find ../example/bin/include/ -name "*.cpp" -exec rm -rf {} \; 2>/dev/null
find ../example/bin/include/ -name "*.inl" -exec rm -rf {} \; 2>/dev/null
find ../example/bin/include/ -name "*.txt" -exec rm -rf {} \; 2>/dev/null
find ../example/bin/include/ -empty -delete 2>/dev/null

# For examples
if [ "$example" = true ]
then
  ../example/build/build.sh

  cd ${DIR}

  cp -f ../example/build/bin/example-simple-threads ./bin
fi

echo "Complete!"

exit 0
