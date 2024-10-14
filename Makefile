
NAME=ircserv

CCXX=c++

INCLDIR = INCL
OBJDIR = obj
SRCDIR = SRCS

CXXFLAGS=-Wall -Wextra -Werror -std=c++98 -g3

PATHS= $(OBJDIR)/$(SRCDIR)

SRCS				=	main.cpp parsing.cpp Server.cpp User.cpp utils.cpp Channel.cpp\
			/invite.cpp	\
			/join.cpp	\
			/kick.cpp	\
			/mode.cpp	\
			/nick.cpp	\
			/names.cpp	\
			/notice.cpp	\
			/part.cpp	\
			/pass.cpp	\
			/privmsg.cpp	\
			/quit.cpp	\
			/topic.cpp	\
			/CommandUser.cpp

SRC			= $(addprefix $(SRCDIR)/,$(SRCS))

INCFLAGS=-I./$(INCLDIR)

RM=rm -rf

OBJ=$(SRC:%.cpp=$(OBJDIR)/%.o)

deps	:= $(patsubst %.o,%.d,$(OBJ))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CCXX) $(CXXFLAGS) $(INCFLAGS) -c $< $(DEPFLAGS) -o $@

$(NAME): $(OBJ)
	$(CCXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR) :
	mkdir -p $(PATHS)

all: $(NAME)

clean:
	$(RM) $(OBJ) $(deps)
fclean: clean
	$(RM) $(NAME) $(OBJDIR)

re: fclean all

.PHONY: clean fclean