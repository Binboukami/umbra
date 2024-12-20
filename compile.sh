mkdir -p bin/dev && cd bin/dev
cmake -G Ninja -S ../.. -B . && ninja
