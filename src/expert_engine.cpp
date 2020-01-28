/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expert_engine.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 17:13:05 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 14:17:30 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expert_engine.hpp"

using namespace std;

namespace xper6tem
{
    ExpertEngine::ExpertEngine(ExpertProgram *_prog)
    {
        this->prog = _prog;
    }

    ExpertEngine::~ExpertEngine() {}

    status ExpertEngine::ft_or(int bin_result, t_node *tree)
    {
        if ((bin_result & 0x1) == 0x1)
        {
            tree->confirmed = ((tree->right->stat == True) && tree->right->confirmed) || ((tree->left->stat == True) && tree->left->confirmed);
            return True;
        }
        else if (bin_result == 0x0)
        {
            tree->confirmed = tree->right->confirmed && tree->left->confirmed;
            return False;
        }
        else if (bin_result == 0x8)
            return Unsure;
        else
        {
            tree->confirmed = false;
            return False;
        }
    }

    status ExpertEngine::ft_and(int bin_result, t_node *tree)
    {
        if ((tree->right->stat == True) && (tree->left->stat == True))
        {
            tree->confirmed = tree->right->confirmed && tree->left->confirmed;
            return True;
        }
        else if (bin_result == 0x0)
        {
            tree->confirmed = (tree->left->stat == False && tree->left->confirmed) || (tree->right->stat == False && tree->right->confirmed);
            return False;
        }
        else if (bin_result == 0x9 || bin_result == 0x8)
            return Unsure;
        else
        {
            tree->confirmed = false;
            return False;
        }
    }

    status ExpertEngine::ft_xor(status left, status right, t_node *tree)
    {
        if ((left ^ right) == 0x1)
        {
            tree->confirmed = tree->right->confirmed && tree->left->confirmed;
            return True;
        }
        else if (left == 0x1 && right == 0x1)
        {
            tree->confirmed = tree->right->confirmed && tree->left->confirmed;
            return False;
        }
        else if (((left | right) & 0x4) == 0x0 && ((left | right) & 0x8))
            return Unsure;
        else
        {
            tree->confirmed = false;
            return False;
        }
    }

    status ExpertEngine::ft_not(int bin_result, t_node *tree)
    {
        if (bin_result == 0x1)
        {
            tree->confirmed = tree->left->confirmed;
            return False;
        }
        else if (bin_result == 0x0)
        {
            tree->confirmed = tree->left->confirmed;
            return True;
        }
        else if (bin_result == 0x8)
            return Unsure;
        else
        {
            tree->confirmed = false;
            return True;
        }
    }

    status ExpertEngine::get_operator_result(t_node *tree)
    {
        switch (tree->data)
        {
        case '|':
            return ft_or(tree->left->stat | tree->right->stat, tree);
            break;
        case '+':
            return ft_and(tree->left->stat | tree->right->stat, tree);
            break;
        case '^':
            return ft_xor(tree->left->stat, tree->right->stat, tree);
            break;
        case '!':
            return ft_not(tree->left->stat, tree);
            break;
        default:
            break;
        }
        return False;
    }

    status ExpertEngine::tree_browse(t_node *tree)
    {
        status left;
        status right;

        if (tree->stat != Unsure && tree->confirmed)
            return tree->stat;
        else if (tree->type == Equivalence)
        {
            tree->stat = tree->left->stat;
            tree->confirmed = tree->left->confirmed;
            return (tree->stat);
        }
        else if (tree->type == Entity)
        {
            if (tree->stat == Unsure || !tree->confirmed)
            {
                if (find(this->prog->query_stack.begin(), this->prog->query_stack.end(), tree->data) == this->prog->query_stack.end())
                {
                    try
                    {
                        this->prog->query_stack.push_back(tree->data);
                        search_for_elem(tree->data);
                        this->prog->query_stack.pop_back();
                    }
                    catch (ParadoxeException *e)
                    {
                        throw e;
                    }
                }
                if ((tree->stat & 0x9) == 0x0 && !tree->confirmed)
                    tree->stat = False;
            }
            return tree->stat;
        }
        else
        {
            try
            {
                if (tree->data != '!')
                    tree_browse(tree->right);
                tree_browse(tree->left);
                tree->stat = get_operator_result(tree);
            }
            catch (ParadoxeException *e)
            {
                throw e;
            }
        }
        return tree->stat;
    }

    int ExpertEngine::search_for_elem(char elem)
    {
        t_node *node = this->prog->get_entity_by_data(elem);
        for (t_node *x : node->rules)
        {
            status new_stat;
            try
            {
                new_stat = tree_browse(x->left);
                x->stat = new_stat;
            }
            catch (ParadoxeException *e)
            {
                throw e;
            }
            x->confirmed = x->left->confirmed;
            if (x->stat == True)
            {
                if (this->updater->update_facts(x->right, x->stat, x->confirmed) == -1)
                    throw new ParadoxeException();

                if (x->data == '=')
                {
                    if (this->updater->update_facts(x->left, x->stat, x->confirmed) == -1)
                        throw new ParadoxeException();
                }
            }
            else if (x->data == '=')
            {
                new_stat = tree_browse(x->right);
                x->stat = new_stat;
                x->confirmed |= x->right->confirmed;
                if (x->stat == True)
                {
                    if (this->updater->update_facts(x->left, x->stat, x->confirmed) == -1)
                        throw new ParadoxeException();
                }
            }
        }
        return 1;
    }

    int ExpertEngine::start_solving(void)
    {
        bool witness = false;
        status stat_wit;
        bool conf_wit;
        for (char elem : this->prog->queries)
        {
            if (find(this->prog->query_stack.begin(), this->prog->query_stack.end(), elem) == this->prog->query_stack.end())
            {
                try
                {
                    stat_wit = this->prog->get_entity_by_data(elem)->stat;
                    conf_wit = this->prog->get_entity_by_data(elem)->confirmed;
                    this->prog->query_stack.push_back(elem);
                    search_for_elem(elem);
                    witness |= stat_wit != this->prog->get_entity_by_data(elem)->stat;
                    witness |= conf_wit != this->prog->get_entity_by_data(elem)->confirmed;
                    this->prog->query_stack.pop_back();
                }
                catch (ParadoxeException *e)
                {
                    throw e;
                }
            }
        }
        for (t_node *node : this->prog->entities)
        {
            if (this->prog->snapshot[node->data] != node->stat)
            {
                witness = true;
                this->prog->snapshot[node->data] = node->stat;
            }
        }
        if (witness)
            return start_solving();
        return 1;
    }
} // namespace xper6tem