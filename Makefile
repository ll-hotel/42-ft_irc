NAME = ft_irc

SOURCES := Epoll.cpp  main.cpp  Server.cpp  SocketAddr.cpp  TcpSocket.cpp  TcpStream.cpp

SOURCE_DIR := src
BUILD_DIR := build

SOURCES := $(addprefix $(SOURCE_DIR)/, $(SOURCES))
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPENDS := $(patsubst %.o, %.d, $(OBJECTS))

CXX := c++
CPPFLAGS := -MMD -MP
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c++98
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
