/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fact_updater.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:51:09 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 14:30:50 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACT_UPDATER_HPP
#define FACT_UPDATER_HPP

#include "xper6tem.h"
#include "expert_program.hpp"
#include "iexpert_engine.hpp"

using namespace std;

namespace xper6tem
{
    class FactUpdater
    {
        public:
        ExpertProgram* prog;
        IExpertEngine* engine;
        
        FactUpdater(ExpertProgram* _prog, IExpertEngine* _engine);
        ~FactUpdater();
        
        int update_facts(t_node *tree, status stat, bool confirmed);

        private:
        void    update_parent_stat(t_node *tree);
        int     spread_or(t_node *tree);
        int     spread_and(t_node *tree);
        int     spread_xor(t_node *tree);
        int     spread_not(t_node *tree);
        int     operation_rule_result(t_node *tree);
    };
} // namespace xper6tem

#endif