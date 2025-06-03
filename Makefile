NAME = ircserv

SOURCES := main.cpp
SOURCES += Epoll.cpp
SOURCES += Server.cpp
SOURCES += SocketAddr.cpp
SOURCES += TcpSocket.cpp
SOURCES += TcpStream.cpp
SOURCES += User.cpp
SOURCES += Channel.cpp
SOURCES += Command.cpp
SOURCES += command_pass.cpp
SOURCES += command_nick.cpp
SOURCES += command_user.cpp
SOURCES += command_join.cpp
SOURCES += command_names.cpp
SOURCES += command_topic.cpp
SOURCES += command_part.cpp
SOURCES += command_privmsg.cpp
SOURCES += command_kick.cpp
SOURCES += command_quit.cpp
SOURCES += command_invite.cpp
SOURCES += command_mode.cpp
SOURCES += ft_split.cpp
SOURCES += patternMatch.cpp
SOURCES += numeric_replies.cpp
SOURCES += utils.cpp

SOURCE_DIR := src
BUILD_DIR := build

SOURCES := $(addprefix $(SOURCE_DIR)/, $(SOURCES))
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst %.o, %.d, $(OBJECTS))

CXX := c++
CPPFLAGS := -MMD -MP
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c++98 -g -O2
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
run: $(NAME)
	./$(NAME) 8080 bocal


.PHONY: valgrind
valgrind: $(NAME)
ifeq ("$(port)", "")
	eval port="8080"
endif
	valgrind -q --track-fds=all ./$(NAME) $(port) bocal

-include $(DEPENDS)
