/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expert_program.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:50:48 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 16:29:45 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expert_program.hpp"

using namespace std;

namespace xper6tem
{
    ExpertProgram::ExpertProgram(void) {}

    ExpertProgram::~ExpertProgram() {}

    int ExpertProgram::set_facts(string fact_line)
    {
        regex re{"^=[A-Z]*"};
        regex err_re{"[^A-Z=]"};
        if (!regex_search(fact_line, re) || regex_search(strtrim(fact_line), err_re))
        {
            cout << "facts line is not correctly formatted" << endl;
            return -1;
        }
        string trimed_line = remove_doubles(strtrim(fact_line));
        this->fact_list = std::vector<char>(trimed_line.begin() + 1, trimed_line.end());
        for (char fact : this->fact_list)
        {
            t_node *node = new t_node();
            node->data = fact;
            node->parent = NULL;
            node->left = NULL;
            node->right = NULL;
            node->type = Entity;
            node->stat = True;
            node->confirmed = true;
            this->entities.push_back(node);
            snapshot[node->data] = True;
        }
        return 1;
    }

    bool ExpertProgram::query_is_in_stack(char data)
    {
        bool ret;
        ret = find(this->query_stack.begin(), this->query_stack.end(), data) != this->query_stack.end();
        return ret;
    }

    int ExpertProgram::set_queries(string queries_line)
    {
        regex re{"^\\?[A-Z]*"};
        regex err_re{"[^A-Z\?]"};
        if (!regex_search(queries_line, re) || regex_search(strtrim(queries_line), err_re))
        {
            cout << "queries line is not correctly formatted" << endl;
            return -1;
        }
        string trimed_line = remove_doubles(strtrim(queries_line));
        this->queries = vector<char>(trimed_line.begin() + 1, trimed_line.end());
        for (char query : this->queries)
        {
            if (this->get_entity_by_data(query) == NULL)
            {
                t_node *node = new t_node();
                node->data = query;
                node->parent = NULL;
                node->left = NULL;
                node->right = NULL;
                node->type = Entity;
                node->stat = False;
                node->confirmed = false;
                this->entities.push_back(node);
                snapshot[node->data] = False;
            }
        }
        return 1;
    }

    int ExpertProgram::set_rules(vector<string> rules_lines)
    {
        regex re{"^\\(|\\)|[A-Z|\\||\\+|\\^|\\!]+[<]?=>[A-Z|\\||\\+|\\^|\\!]+$"};
        regex err_re{"[A-Z][A-Z]"};
        for (string line : rules_lines)
        {
            string trimed_line = strtrim(line);
            if (!regex_search(trimed_line, re) || regex_search(trimed_line, err_re))
            {
                cout << "rules are not correctly formatted:\n"
                    << line << endl;
                return -1;
            }
            else if (countchar(split(trimed_line, '=')[0], '(') != countchar(split(trimed_line, '=')[0], ')') || countchar(split(trimed_line, '=')[1], '(') != countchar(split(trimed_line, '=')[1], ')'))
            {
                cout << "rules are not correctly formatted:\n"
                    << line << endl;
                return -1;
            }
            else
            {
                t_node *new_rule = new t_node;
                new_rule->parent = NULL;
                new_rule->left = NULL;
                new_rule->right = NULL;
                rules.push_back(new_rule);
                vector<char> rule_vec(trimed_line.begin(), trimed_line.end());
                this->fill_rules_tree(new_rule, rule_vec);
            }
        }
        return 1;
    }

    t_node* ExpertProgram::get_entity_by_data(char data)
    {
        for (t_node *elem : this->entities)
        {
            if (elem->data == data)
                return elem;
        }
        return NULL;
    }

    vector<char> ExpertProgram::get_between_par(vector<char> rule)
    {
        size_t par_op = 0;
        size_t par_cl = 0;
        size_t index = 0;

        for (char c : rule)
        {
            if (c == '(')
                par_op++;
            else if (c == ')')
            {
                par_cl++;
                if (par_op == par_cl)
                {
                    vector<char> between_par(find(rule.begin(), rule.end(), '(') + 1,
                                            rule.begin() + index);
                    return between_par;
                }
            }
            index++;
        }
        return rule;
    }

    bool ExpertProgram::everything_is_between_par(vector<char> rule)
    {
        if (rule[0] != '(' || rule[rule.size() - 1] != ')')
            return false;
        size_t index = 0;
        size_t par_op = 0;
        size_t par_cl = 0;
        for (char c : rule)
        {
            index++;
            if (c == '(')
                par_op++;
            else if (c == ')')
            {
                par_cl++;
                if (par_op == par_cl)
                {
                    if (index == rule.size())
                        return true;
                    else
                        return false;
                }
            }
        }
        return false;
    }

    t_node* ExpertProgram::parenthesis_manager(vector<char> rule, t_node *parent)
    {
        vector<char> between_par = get_between_par(rule);
        return fill_tree_branch(between_par, parent);
    }

    vector<char> ExpertProgram::get_sign_out_of_par_left(vector<char> rule, char data)
    {
        vector<char> newvec;
        if (find(rule.begin(), rule.end(), '(') == rule.end())
            return vector<char>(rule.begin(), find(rule.begin(), rule.end(), data));
        else
        {
            int open_pars = 0;
            int closed_pars = 0;
            for (char c : rule)
            {
                if (c == data && open_pars == closed_pars)
                    return newvec;
                else if (c == '(')
                    open_pars++;
                else if (c == ')')
                    closed_pars++;
                newvec.push_back(c);
            }
        }
        return newvec;
    }

    vector<char> ExpertProgram::get_sign_out_of_par_right(vector<char> rule, char data)
    {
        if (find(rule.begin(), rule.end(), '(') == rule.end())
            return vector<char>(find(rule.begin(), rule.end(), data) + 1, rule.end());
        else
        {
            vector<char> newvec;
            int open_pars = 0;
            int closed_pars = 0;
            bool adding = false;
            for (char c : rule)
            {
                if (c == data && open_pars == closed_pars)
                {
                    if (adding)
                        newvec.push_back(c);
                    else
                        adding = true;
                }
                else if (c == '(' && !adding)
                    open_pars++;
                else if (c == ')' && !adding)
                    closed_pars++;
                else if (adding)
                    newvec.push_back(c);
            }
            return newvec;
        }
    }

    t_node* ExpertProgram::fill_tree_branch(vector<char> rule, t_node *parent)
    {
        if (rule.size() == 0)
            return NULL;

        if (rule.size() == 1)
        {
            for (t_node *x : this->entities)
            {
                if (x->data == rule[0])
                {
                    x->parent = parent;
                    return x;
                }
            }
            t_node *new_node = new t_node;
            new_node->parent = parent;
            new_node->data = rule[0];
            new_node->right = NULL;
            new_node->left = NULL;
            new_node->type = Entity;
            new_node->confirmed = false;
            new_node->stat = False;
            this->entities.push_back(new_node);
            snapshot[new_node->data] = False;
            return new_node;
        }
        string s(rule.begin(), rule.end());
        t_node *new_node = new t_node;
        new_node->parent = parent;
        size_t size = rule.size();
        char data;
        if (charpos_nopar(rule, '^') != size)
            data = '^';
        else if (charpos_nopar(rule, '|') != size)
            data = '|';
        else if (charpos_nopar(rule, '+') != size)
            data = '+';
        else if (charpos_nopar(rule, '!') != size)
            data = '!';
        else if (find(rule.begin(), rule.end(), '(') != rule.end())
            return parenthesis_manager(rule, parent);
        else
            return NULL;
        new_node->type = Operator;
        new_node->stat = False;
        new_node->confirmed = false;
        new_node->data = data;
        vector<char> leftvec = get_sign_out_of_par_left(rule, data);
        vector<char> rightvec = get_sign_out_of_par_right(rule, data);
        if (new_node->data == '!')
        {
            new_node->left = fill_tree_branch(rightvec, new_node);
            new_node->right = NULL;
            new_node->stat = (status) !(new_node->left->stat);
            new_node->confirmed = new_node->left->confirmed;
        }
        else
        {
            new_node->left = fill_tree_branch(leftvec, new_node);
            new_node->right = fill_tree_branch(rightvec, new_node);
            if (new_node->left->confirmed && new_node->right->confirmed)
            {
                switch (new_node->data)
                {
                case '+':
                    new_node->stat = (status)(new_node->left->stat & new_node->left->stat);
                    new_node->confirmed = new_node->left->confirmed & new_node->left->confirmed;
                case '|':
                    new_node->stat = (status)(new_node->left->stat | new_node->left->stat);
                    new_node->confirmed = new_node->left->confirmed | new_node->left->confirmed;
                case '^':
                    new_node->stat = (status)(new_node->left->stat ^ new_node->left->stat);
                    new_node->confirmed = false;
                default:
                    new_node->stat = False;
                    new_node->confirmed = false;
                }
            }
        }
        return new_node;
    }

    void ExpertProgram::add_to_entities_linked_rules(t_node *rule)
    {
        for (t_node *entity : this->entities)
        {
            if (find_elem_in_tree(rule->right, entity->data))
                entity->rules.push_back(rule);
            else if (rule->data == '=' and find_elem_in_tree(rule->left, entity->data))
                entity->rules.push_back(rule);
        }
    }

    void ExpertProgram::fill_rules_tree(t_node *node, vector<char> rule)
    {
        t_node *tree_base = node;
        char data;
        size_t left_shift = 0;
        size_t right_shift = 2;

        if (find(rule.begin(), rule.end(), '<') != rule.end())
        {
            data = '=';
            left_shift = 1;
        }
        else if (find(rule.begin(), rule.end(), '=') != rule.end())
            data = '>';
        else
            return;

        vector<char> leftvec(rule.begin(), find(rule.begin(), rule.end(), '=') - left_shift);
        vector<char> rightvec(find(rule.begin(), rule.end(), '=') + right_shift, rule.end());
        tree_base->data = data;
        tree_base->type = Equivalence;
        tree_base->confirmed = False;
        tree_base->stat = False;
        tree_base->left = fill_tree_branch(leftvec, node);
        tree_base->right = fill_tree_branch(rightvec, node);
        add_to_entities_linked_rules(tree_base);
        this->rules.push_back(tree_base);
    }
} // namespace xper6tem