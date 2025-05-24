NAME = ft_irc

SOURCES := main.cpp
SOURCES += Epoll.cpp
SOURCES += Server.cpp
SOURCES += SocketAddr.cpp
SOURCES += TcpSocket.cpp
SOURCES += TcpStream.cpp
SOURCES += User.cpp
SOURCES += Channel.cpp
SOURCES += Command.cpp
SOURCES += command_join.cpp
SOURCES += command_names.cpp

SOURCE_DIR := src
BUILD_DIR := build

SOURCES := $(addprefix $(SOURCE_DIR)/, $(SOURCES))
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst %.o, %.d, $(OBJECTS))

CXX := c++
CPPFLAGS := -MMD -MP
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c++98 -g
CXXFLAGS += -Iinclude/

.PHONY: all
all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

$(OBJECTS): $(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(BUILD_DIR):
	@mkdir -p $@

.PHONY: clean
clean: |
	rm -rf $(BUILD_DIR)

.PHONY: fclean
fclean: | clean
	rm -f $(NAME)

.PHONY: re
re: | fclean
	@$(MAKE) --no-print-directory

.PHONY: format
format:
	find $(SOURCE_DIR) include -name '*.hpp' -or -name '*.cpp' -exec clang-format -i '{}' ';'

.PHONY: run
run:
	make $(NAME)
	./$(NAME) 8080 bocal

.PHONY: valgrind
valgrind:
	make $(NAME)
	valgrind -q --track-fds=all ./$(NAME) 8080 bocal

