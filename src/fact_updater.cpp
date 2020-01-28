/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fact_updater.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:51:09 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 14:47:51 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fact_updater.hpp"

using namespace std;

namespace xper6tem
{
    FactUpdater::FactUpdater(ExpertProgram *_prog, IExpertEngine *_engine)
    {
        this->prog = _prog;
        this->engine = _engine;
    }

    FactUpdater::~FactUpdater() {}

    void FactUpdater::update_parent_stat(t_node *tree)
    {
        return;
        if (tree->stat == Unsure || tree->type == Entity)
            return;
        else if (tree->left && tree->right && tree->right->confirmed && tree->left->confirmed && !((tree->left->stat | tree->right->stat) & 0xC))
        {
            switch (tree->data)
            {
            case '+':
                tree->stat = (status)(tree->left->stat & tree->right->stat);
                tree->confirmed = tree->left->confirmed && tree->right->confirmed;
            case '|':
                tree->stat = (status)(tree->left->stat | tree->right->stat);
                tree->confirmed = (tree->right->confirmed && tree->left->confirmed) || (tree->left->confirmed && tree->left->stat == True) || (tree->right->confirmed && tree->right->stat == True);
            case '^':
                tree->stat = (status)(tree->left->stat ^ tree->right->stat);
                tree->confirmed = tree->left->confirmed && tree->right->confirmed;
            }
        }
        else
        {
            if (tree->left && tree->right && ((tree->left->stat | tree->right->stat) & 0xC))
            {
                update_parent_stat(tree->left);
                update_parent_stat(tree->right);
            }
        }
    }

    int FactUpdater::spread_or(t_node *tree)
    {
        int ret = 0;

        if (tree->left->confirmed && tree->right->confirmed && (tree->left->stat | tree->right->stat) == 0x0)
            return -1;

        if ((!tree->right->confirmed && tree->right->stat == False) && (!tree->left->confirmed && tree->left->stat == False) && ((!this->prog->query_is_in_stack(tree->right->data) || !this->prog->query_is_in_stack(tree->left->data))))
        {
            if (!this->prog->query_is_in_stack(tree->right->data))
            {
                this->prog->query_stack.push_back(tree->right->data);
                this->engine->search_for_elem(tree->right->data);
                this->prog->query_stack.pop_back();
            }
            if (!this->prog->query_is_in_stack(tree->left->data) && (!tree->left->confirmed && tree->left->stat == False) && !this->prog->query_is_in_stack(tree->left->data))
            {
                this->prog->query_stack.push_back(tree->left->data);
                this->engine->search_for_elem(tree->left->data);
                this->prog->query_stack.pop_back();
            }
        }
        if ((tree->right->stat == True && tree->right->confirmed) || (tree->left->stat == True && tree->left->confirmed))
            return 0;
        if ((tree->right->stat | tree->left->stat) == 0x0 && !tree->right->confirmed && !tree->left->confirmed)
        {
            tree->left->stat = Unsure;
            tree->left->confirmed = tree->confirmed;
            tree->right->stat = Unsure;
            tree->right->confirmed = tree->confirmed;
            if (tree->left->type != Entity)
                ret |= operation_rule_result(tree->left);
            if (tree->right->type != Entity)
                ret |= operation_rule_result(tree->right);
        }
        else if (((!tree->confirmed && !tree->left->confirmed) || tree->confirmed) && tree->right->confirmed && tree->right->stat == False)
        {
            if (tree->left->stat == False && tree->left->confirmed)
                return -1;
            else if (!tree->left->confirmed || tree->left->stat == Unsure)
            {
                tree->left->confirmed = tree->confirmed;
                tree->left->stat = tree->stat;
                if (tree->left->type != Entity)
                    ret |= operation_rule_result(tree->left);
            }
        }
        else if (((!tree->confirmed && tree->right->confirmed) || tree->confirmed) && tree->left->confirmed && tree->left->stat == False)
        {
            if (tree->right->stat == False && tree->right->confirmed)
                return -1;
            else if (!tree->right->confirmed || tree->right->stat == Unsure)
            {
                tree->right->confirmed = tree->confirmed;
                tree->right->stat = tree->stat;
                if (tree->right->type != Entity)
                    ret |= operation_rule_result(tree->right);
            }
        }

        return ret;
    }

    int FactUpdater::spread_and(t_node *tree)
    {
        int ret = 0;
        if (tree->confirmed && tree->left->confirmed && ((tree->left->stat | tree->stat) & 0x8) == 0x0 && tree->left->stat != tree->stat)
            return -1;
        if (tree->confirmed && tree->right->confirmed && ((tree->right->stat | tree->stat) & 0x8) == 0x0 && tree->right->stat != tree->stat)
            return -1;
        if ((!tree->left->confirmed || tree->left->stat == tree->stat) && (!tree->right->confirmed || tree->right->stat == tree->stat))
        {
            tree->left->stat = tree->stat;
            tree->left->confirmed |= tree->confirmed;
            tree->right->stat = tree->stat;
            tree->right->confirmed |= tree->confirmed;
        }
        if (tree->left->type != Entity)
            ret |= operation_rule_result(tree->left);
        if (tree->right->type != Entity)
            ret |= operation_rule_result(tree->right);
        return ret;
    }

    int FactUpdater::spread_xor(t_node *tree)
    {
        int ret = 0;

        if (tree->confirmed && tree->stat == True)
        {
            if (tree->left->confirmed && tree->left->stat != Unsure && tree->right->confirmed && tree->right->stat != Unsure && tree->right->stat == tree->left->stat)
                return -1;
        }
        else if (tree->confirmed && tree->stat == False)
        {
            if (tree->left->confirmed && tree->right->confirmed && tree->right->stat != tree->left->stat && tree->left->stat != Unsure && tree->right->stat != Unsure)
                return -1;
        }
        if ((tree->right->type == Entity || tree->left->type == Entity) && (((tree->right->stat | tree->left->stat) == 0x8) || (tree->right->stat & tree->left->stat) == 0x0) && ((!this->prog->query_is_in_stack(tree->right->data) || !this->prog->query_is_in_stack(tree->left->data))))
        {
            if (tree->right->type == Entity && !this->prog->query_is_in_stack(tree->right->data))
            {
                this->prog->query_stack.push_back(tree->right->data);
                this->engine->search_for_elem(tree->right->data);
                this->prog->query_stack.pop_back();
            }
            if (tree->left->type == Entity && (((tree->right->stat | tree->left->stat) == 0x8) || (!tree->right->stat && !tree->left->stat)) && !this->prog->query_is_in_stack(tree->left->data))
            {
                this->prog->query_stack.push_back(tree->left->data);
                this->engine->search_for_elem(tree->left->data);
                this->prog->query_stack.pop_back();
            }
        }
        if (tree->right->stat == Unsure && tree->left->stat == Unsure)
            return 0;
        if (!tree->right->confirmed && !tree->left->confirmed && (tree->right->stat | tree->left->stat) == 0x0)
        {
            tree->left->stat = Unsure;
            tree->left->confirmed = tree->confirmed;
            tree->right->stat = Unsure;
            tree->right->confirmed = tree->confirmed;
            if (tree->left->type != Entity)
                ret |= operation_rule_result(tree->left);
            if (tree->right->type != Entity)
                ret |= operation_rule_result(tree->right);
        }
        else if (tree->right->confirmed ^ tree->left->confirmed)
        {
            if ((tree->right->confirmed && tree->right->stat == Unsure) || (tree->left->confirmed && tree->left->stat == Unsure))
                return 0;
            status tmp_stat = (status)(tree->right->confirmed ? !tree->right->stat : !tree->left->stat);
            if (!tree->left->confirmed)
            {
                tree->left->stat = tmp_stat;
                tree->left->confirmed |= tree->confirmed;
                if (tree->left->type != Entity)
                    ret |= operation_rule_result(tree->left);
            }
            else
            {
                tree->right->stat = tmp_stat;
                tree->right->confirmed |= tree->confirmed;
                if (tree->right->type != Entity)
                    ret |= operation_rule_result(tree->right);
            }
        }
        else if (tree->left->stat == True && !tree->right->confirmed)
        {
            tree->right->stat = (status)!tree->left->stat;
            tree->right->confirmed |= tree->left->confirmed & tree->confirmed;
        }
        else if (tree->right->stat == True && !tree->left->confirmed)
        {
            tree->left->stat = (status)!tree->right->stat;
            tree->left->confirmed |= tree->right->confirmed & tree->confirmed;
        }
        else if (tree->right->confirmed && tree->left->confirmed && (tree->right->stat == Unsure || tree->left->stat == Unsure))
        {
            status tmp_stat = (status)(tree->right->stat == Unsure ? !tree->left->stat : !tree->right->stat);
            if (tree->right->stat == Unsure)
            {
                tree->right->stat = tmp_stat;
                if (tree->right->type != Entity)
                    ret |= operation_rule_result(tree->right);
            }
            else
            {
                tree->left->stat = tmp_stat;
                tree->left->confirmed |= tree->confirmed;
                if (tree->left->type != Entity)
                    ret |= operation_rule_result(tree->left);
            }
        }
        return ret;
    }

    int FactUpdater::spread_not(t_node *tree)
    {
        if (tree->confirmed && tree->left->confirmed && tree->stat == tree->left->stat)
            return -1;
        int ret = 0;
        if (tree->stat == Unsure && (tree->confirmed || !tree->left->confirmed))
        {
            tree->left->stat = tree->stat;
            if (tree->left->type != Entity)
                ret = operation_rule_result(tree->left);
        }
        else if (tree->stat == True)
        {
            if (tree->left->stat == True && tree->left->confirmed && !tree->confirmed)
            {
                tree->stat = False;
                tree->confirmed = true;
                return ret;
            }
            else
            {
                tree->left->stat = (status)!tree->stat;
                tree->left->confirmed |= tree->confirmed;
                if (tree->left->type != Entity)
                    ret = operation_rule_result(tree->left);
            }
        }
        return ret;
    }

    int FactUpdater::operation_rule_result(t_node *tree)
    {
        update_parent_stat(tree);
        int ret = 0;
        switch (tree->data)
        {
        case '|':
            return spread_or(tree);
            break;
        case '+':
            return spread_and(tree);
            break;
        case '^':
            return spread_xor(tree);
            break;
        case '!':
            return spread_not(tree);
            break;
        default:
            break;
        }
        return ret;
    }

    int FactUpdater::update_facts(t_node *tree, status stat, bool confirmed)
    {
        int ret = 0;
        if ((tree->stat != Unsure) && (tree->type == Entity) && (tree->stat & stat) == 0x0 && (tree->stat | stat) != 0x0 && confirmed && tree->confirmed)
            return -1;

        ret = (int)((tree->confirmed != confirmed) | (tree->stat != stat));
        tree->confirmed = confirmed;
        tree->stat = stat;
        if (tree->type == Entity && (tree->confirmed || stat == Unsure))
            return ret;
        else
            return operation_rule_result(tree);
    }
} // namespace xper6tem