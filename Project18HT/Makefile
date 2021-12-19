# Псеводонимы для основных команд:
CXX = g++
CXXFLAGS = -o
MD = mkdir -p
AR = ar rc
RM = rm -rf
# Список основных единиц компиляции:
SRC = ./src/main ./src/server/ChatEngine ./src/server/ChatStateTable
# Единицы компиляции, выносимые в статическую библиотеку:
LIBSOURCES = ChatUser ChatUsersList ChatMessage ChatMessagesList sha1

# Имена и пути генерируемых файлов:
TARGET = chat# Исполняемый файл
LIB = ChatLib# Файл статической библиотеки
LIB_SRC_PATH = ./src/common/
BINDIR = ./bins/
OBJDIR = ./objs/
LIBDIR = ./libs/
PREFIX = /usr/local/bin# Путь установки программы

all: ./src/main.cpp lib
	$(MD) $(BINDIR)
	$(CXX) $(CXXFLAGS) $(BINDIR)$(TARGET) $(addsuffix .cpp, $(SRC)) -L. $(LIBDIR)lib$(LIB).a

lib: ChatUser ChatUsersList ChatMessage ChatMessagesList sha1
	$(MD) $(LIBDIR)
	$(AR) $(LIBDIR)lib$(LIB).a $(addprefix $(OBJDIR), $(addsuffix .o, $(LIBSOURCES)))

ChatUser: $(LIB_SRC_PATH)ChatUser.cpp
	$(MD) $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(OBJDIR)ChatUser.o $(LIB_SRC_PATH)ChatUser.cpp -c

ChatUsersList: $(LIB_SRC_PATH)ChatUsersList.cpp
	$(CXX) $(CXXFLAGS) $(OBJDIR)ChatUsersList.o $(LIB_SRC_PATH)ChatUsersList.cpp -c

ChatMessage: $(LIB_SRC_PATH)ChatMessage.cpp
	$(CXX) $(CXXFLAGS) $(OBJDIR)ChatMessage.o $(LIB_SRC_PATH)ChatMessage.cpp -c

ChatMessagesList: $(LIB_SRC_PATH)ChatMessagesList.cpp
	$(CXX) $(CXXFLAGS) $(OBJDIR)ChatMessagesList.o $(LIB_SRC_PATH)ChatMessagesList.cpp -c

sha1: $(LIB_SRC_PATH)sha1.cpp
	$(CXX) $(CXXFLAGS) $(OBJDIR)sha1.o $(LIB_SRC_PATH)sha1.cpp -c

clean:
	$(RM) $(OBJDIR) $(LIBDIR) $(BINDIR)

install:
	install $(BINDIR)$(TARGET) $(PREFIX)

uninstall:
	$(RM) $(PREFIX)/$(TARGET)
