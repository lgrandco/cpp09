NAME = btc

SRC = ../colors.cpp main.cpp BitcoinExchange.cpp
INC = -I..

OBJ = $(SRC:.cpp=.o)
DEPS = $(SRC:.cpp=.d)

CC = cc
CXX = c++

CFLAGS = -Wall -Wextra -Werror -MMD -MP -g3 $(INC)
CXXFLAGS = $(CFLAGS) --std=c++98

RM = rm -f

all : $(NAME)

$(NAME) : ${OBJ} Makefile
	$(CXX) $(CXXFLAGS) -o $(NAME) -g3 $(OBJ)

clean :
	$(RM) $(OBJ) $(DEPS)

fclean : clean
	$(RM) $(NAME)

-include $(DEPS)

re : fclean all

.PHONY : all clean fclean re
