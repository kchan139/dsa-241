# chmod +x run.sh
rm main
g++ -Iinclude -Isrc -std=c++17 main.cpp -o main

if [ $? -eq 0 ]; then
    clear && ./main
fi