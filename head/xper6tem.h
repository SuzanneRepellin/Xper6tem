/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xper6tem.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:50:56 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/03 16:44:56 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XPER6TEM_H
#define XPER6TEM_H

#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <exception>
#include <map>

struct ParadoxeException : public std::exception
{
	const char * what () const throw ()
    {
    	return "The rules contain a paradoxe\n";
    }
};

enum    status
{
    False = 0,
    True = 1,
    Unsure = 8
};

enum    node_type
{
    Equivalence,
    Operator,
    Entity
};

class  t_node
{
    public:
    t_node() {};
    ~t_node() {};
    char            data;
    status          stat;
    node_type       type;
    t_node*         parent;
    t_node*         right;
    t_node*         left;
    std::vector<t_node*> rules;
    bool            confirmed = false;

    t_node  *go_to_right_end()
    {
        t_node* moving = this;
        while (moving->right->left != NULL)
        {
            moving = moving->right;
        }
        return moving;
    }

    t_node  *go_to_left_end()
    {
        t_node* moving = this;
        while (moving->left->left != NULL)
        {
            moving = moving->left;
        }
        return moving;
    }
};

size_t                      charpos(std::vector<char> vec, char c);
size_t                      charpos_nopar(std::vector<char> vec, char c);
std::string                 strtrim(std::string str);
void                        print_bin_tree(t_node* tree);
std::vector<std::string>    split(const std::string &s, char delim);
bool                        find_elem_in_tree(t_node *tree, char elem);
int                         countchar(const std::string s, char c);
std::string                 remove_doubles(const std::string src);

#endif