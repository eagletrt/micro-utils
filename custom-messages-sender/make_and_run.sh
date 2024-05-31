rm -rf build
cmake -S . -B build
cd build
make -j4
./custom_message_sender
