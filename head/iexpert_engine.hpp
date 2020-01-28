/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iexpert_engine.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 14:28:54 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 14:28:55 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEXPERT_ENGINE_HPP
#define IEXPERT_ENGINE_HPP

#include "expert_program.hpp"
#include "xper6tem.h"

namespace xper6tem
{
    class IExpertEngine
    {
        public:
        virtual int search_for_elem(char elem) = 0;
    };
}

#endif