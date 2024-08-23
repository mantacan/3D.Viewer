TARGET= 3DViewer
NAME=s21_3dviewer
CC=g++
CC_FLAGS= -std=c++17 -Wall -Wextra -Werror -g  -D_POSIX_C_SOURCE=200809L #-fsanitize=address #--track-origins=yes
GCOV_FLAGS= -L. -ls21_3dviewer --coverage
OS= $(shell uname)
CXXFLAGS += -framework OpenGL
LDFLAGS += -framework OpenGL

ifeq ($(OS), Linux)
	CHECK_FLAGS = -lgtest
	OPEN = xdg-open
else
	CHECK_FLAGS = $(shell pkg-config --libs --cflags gtest) -lm
	OPEN = open
endif

ifeq ($(OS), Linux)
	OS_INSTALL = 3dviewer
else
	OS_INSTALL = 3dviewer.app
endif

TEST_COMPILE = $(CC) $(CC_FLAGS) ./lib/tiny_obj_loader.cc ./test/*.cpp libs21_3dviewer.a $(CHECK_FLAGS) -L. -ls21_3dviewer
all: clean s21_3dviewer.a

clean:
	rm -rf *.o, *.g* *.info *.out report *.a *.dSYM
	rm -rf *.o
	rm -rf model/*.o, model/*.g* model/*.info model*.out report *.a *.dSYM
	rm -rf build/build*
	rm -fr docs

re: clean all


install:
	-echo "Installation of $(TARGET).app:"
	-cd .. && mkdir -p build && cd build && cmake ../src/view
	-cd ../build && make
	-echo "Installation completed! You can find app in the ../build/ folder!"
	

uninstall:
	rm -rf ../build
	echo "Uninstall completed!"


s21_3dviewer.o:
	$(CC) $(CC_FLAGS) -c model/*.cpp

s21_3dviewer.a: s21_3dviewer.o
	ar rcs libs21_3dviewer.a *.o
	rm -rf *.o
	ranlib libs21_3dviewer.a
	cp libs21_3dviewer.a s21_3dviewer.a

tests: s21_3dviewer.a
	$(TEST_COMPILE)
	-./a.out  # '-' ignore errors

dvi:
	doxygen


dist: install
	mkdir -p ../dist
ifeq ($(OS), Darwin)
	@cp -R ../build/$(TARGET).app ../dist
else
	@cp -R ../build/$(TARGET) ../dist
endif
	@cp -R ../build/$(TARGET) ../dist
	@cp -R . ../dist/src
	@cd ../dist && tar cvzf $(TARGET).tgz *
	@cd ../dist && rm -rf $(TARGET)
	@rm -rf ../dist/src
	echo "Archive creation completed successfully!"



gcov_report: s21_3dviewer.a tests
	rm -f *.g*
	$(CC) $(CC_FLAGS) test/*.cpp model/*.cpp libs21_3dviewer.a $(CHECK_FLAGS) $(GCOV_FLAGS)
	-./a.out  # '-' ignore errors
	lcov -t a.out -o rep.info -c -d .
	genhtml -o report rep.info
	$(OPEN) ./report/index.html
	rm -rf *.gcda *.gcno
clang:
	find . -type f \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i -style=google {} \;
check:
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./a.out
else
	valgrind ./a.out --leak-check=full
endif
	make clean
	
	clang-format -style=google -n model/*.cpp model/*.h test/*.cpp test/*.h view/*.cpp view/*.h
.PHONY: all clean re s21_3dviewer.a tests gcov_report check

