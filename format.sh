find ./src -name *.h -exec clang-format -i {} \;
find ./src -name *.c -exec clang-format -i {} \;
find ./src -name *.cpp -exec clang-format -i {} \;
find ./include -name *.h -exec clang-format -i {} \;
