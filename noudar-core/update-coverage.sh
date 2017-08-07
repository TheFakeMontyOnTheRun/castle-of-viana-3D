GTEST_DIR="lib/googletest/googletest"
GMOCK_DIR="lib/googletest/googlemock"
find . | grep gcno | xargs rm
find . | grep gcda | xargs rm
make clean
make CXX='g++' CXXFLAGS='-Inoudar-core/include -Incurses-version -g -c -std=c++0x -fprofile-arcs -ftest-coverage -I$(GTEST_DIR)/include -I$(GTEST_DIR) -I$(GTEST_DIR)/include/gtest -I$(GTEST_DIR)/include/gtest/internal -I$(GMOCK_DIR)/include -I$(GMOCK_DIR) -I$(GMOCK_DIR)/include/gtest -I$(GMOCK_DIR)/include/gtest/internal' LDFLAGS='-lncurses -lpthread --coverage' unittests

./unittests
#gcov lib/googletest/googlemock/src/gmock_main.gcno
lcov --capture --compat split_crc=on --directory . --output-file lcov-output.info
genhtml lcov-output.info --output-directory coverage
rm lcov-output.info
