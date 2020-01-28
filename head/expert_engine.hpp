/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expert_engine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 17:13:05 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 13:59:07 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef EXPERT_ENGINE_HPP
#define EXPERT_ENGINE_HPP

#include "xper6tem.h"
#include "expert_program.hpp"
#include "iexpert_engine.hpp"
#include "fact_updater.hpp"  

using namespace std;

namespace xper6tem
{
    class ExpertEngine : public IExpertEngine
    {
    public:
        ExpertProgram *prog;
        FactUpdater *updater;

        ExpertEngine(ExpertProgram *_prog);
        ~ExpertEngine();

    private:
        status ft_or(int bin_result, t_node* tree);
        status ft_and(int bin_result, t_node* tree);
        status ft_xor(status left, status right, t_node* tree);
        status ft_not(int bin_result, t_node* tree);
        status get_operator_result(t_node* tree);
        status tree_browse(t_node *tree);

    public:
        int search_for_elem(char elem);
        int start_solving(void);
    };
}

#endif // namespace xper6tem
