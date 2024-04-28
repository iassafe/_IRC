# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/01 18:16:43 by khanhayf          #+#    #+#              #
#    Updated: 2024/04/26 16:13:19 by khanhayf         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= ircserv

SRCS	= main.cpp Server.cpp Client.cpp authentication.cpp Channel.cpp \
		invite.cpp mode.cpp bot.cpp Commands.cpp join.cpp topic.cpp kick.cpp privmsg.cpp

OBJS	= $(SRCS:.cpp=.o)

CXX	 = c++

CXXFLAGS = -Wall -Werror -Wextra  -std=c++98

%.o: %.cpp Server.hpp Client.hpp Channel.hpp responses.hpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	
all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@


clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re