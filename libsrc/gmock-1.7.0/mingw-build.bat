  g++ -isystem ./gtest/include -I./gtest  -isystem ./include -I.  -pthread -c ./gtest/src/gtest-all.cc
  g++ -isystem ./gtest/include -I./gtest  -isystem ./include -I.  -pthread -c ./src/gmock-all.cc
  ar -rv libgmock.a gtest-all.o gmock-all.o
  
  PAUSE