NAME = Xper6tem

SRCS = xper6tem.cpp tools.cpp expert_engine.cpp expert_program.cpp fact_updater.cpp

SRCPATH = src/

OBJPATH = obj/

OBJ = $(addprefix $(OBJPATH),$(SRCS:.cpp=.o))

DEP = $(addprefix $(HEAD),$(SRCS:.hpp=.d))

HEAD = -I head/

FLAGS = -std=c++11 -Wall -Wextra -Werror

all: $(NAME)

$(NAME):$(OBJPATH) $(OBJ)
	g++ $(FLAGS) $(HEAD) $(OBJ) -o $(NAME)

$(OBJPATH):
	mkdir -p $(OBJPATH)

$(OBJPATH)%.o: $(SRCPATH)%.cpp | $(OBJPATH)
	g++ -MMD $(FLAGS) $(HEAD) -c $< -o $@

-include $(DEP)

clean:
	/bin/rm -rf $(OBJPATH)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
