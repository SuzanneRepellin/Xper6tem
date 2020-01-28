/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expert_program.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:50:48 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 14:23:31 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPERT_PROGRAM_HPP
#define EXPERT_PROGRAM_HPP

#include "xper6tem.h"

using namespace std;

namespace xper6tem
{
    class ExpertProgram
    {
        public:
        ExpertProgram(void);
        ~ExpertProgram();
        class facts         *facts;
        vector<char>        queries;
        deque<char>         query_stack;
        vector<t_node*>     rules;
        vector<t_node*>     entities;
        vector<char>        fact_list;
        map<char, status>   snapshot;

        int     set_facts(string fact_line);
        bool    query_is_in_stack(char data);
        int     set_queries(string queries_line);
        int     set_rules(vector<string> rules_lines);
        t_node  *get_entity_by_data(char data);

        private:
        vector<char>    get_between_par(vector<char> rule);
        bool            everything_is_between_par(vector<char> rule);
        t_node          *parenthesis_manager(vector<char> rule, t_node* parent, t_node *new_node);
        vector<char>    get_sign_out_of_par_left(vector<char> rule, char data);
        vector<char>    get_sign_out_of_par_right(vector<char> rule, char data);
        t_node          *fill_tree_branch(vector<char> rule, t_node* parent);
        void            add_to_entities_linked_rules(t_node* rule);
        void            fill_rules_tree(t_node* node, vector<char> rule);
    };
}

#endif