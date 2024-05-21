
NAME	= ircserv
BONUS	= ircserv_bonus

SRCS	= mandatory/main.cpp mandatory/Server.cpp mandatory/Client.cpp mandatory/authentication.cpp mandatory/Channel.cpp\
		 mandatory/invite.cpp mandatory/mode.cpp mandatory/Commands.cpp mandatory/join.cpp mandatory/topic.cpp mandatory/kick.cpp mandatory/privmsg.cpp
		
SRCS_B	= bonus/main.cpp bonus/Server.cpp bonus/Client.cpp bonus/authentication.cpp bonus/Channel.cpp\
		 bonus/invite.cpp bonus/mode.cpp bonus/bot.cpp bonus/Commands.cpp bonus/join.cpp bonus/topic.cpp bonus/kick.cpp bonus/privmsg.cpp

OBJS	= $(SRCS:.cpp=.o)

OBJS_B	=$(SRCS_B:.cpp=.o)

CXX	 = c++

CXXFLAGS = -Wall -Werror -Wextra  -std=c++98

HEADER = mandatory/Server.hpp mandatory/Client.hpp mandatory/Channel.hpp mandatory/responses.hpp

HEADER_B = bonus/Server.hpp bonus/Client.hpp bonus/Channel.hpp bonus/responses.hpp

all: $(NAME)

bonus: $(BONUS)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(BONUS): $(OBJS_B)
	@$(CXX) $(CXXFLAGS) $(OBJS_B) -o $@

mandatory/%.o: mandatory/%.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

bonus/%.o: bonus/%.cpp $(HEADER_B)
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	
clean:
	@rm -f $(OBJS) $(OBJS_B)

fclean: clean
	@rm -f $(NAME) $(BONUS)

re:	fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re bonus re_bonus