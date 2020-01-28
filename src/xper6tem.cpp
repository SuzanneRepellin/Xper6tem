/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xper6tem.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srepelli <srepelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:50:50 by srepelli          #+#    #+#             */
/*   Updated: 2020/01/28 14:28:13 by srepelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xper6tem.h"
#include "expert_program.hpp"
#include "expert_engine.hpp"

using namespace std;
using namespace xper6tem;

int     main(int ac, char* av[])
{
   if (ac == 1)
   {
        cout << "usage : ./Xper6tem input_file_name" << endl;
        cout << "input file format :\n";
        cout << "RULE1\nRULE2\nRULE3\n\n=FACTS\n?QUERIES\n";
        return 0;
   }

    ifstream    input_file (av[1], ios::out);
    if (!input_file.is_open())
    {
        return 0;
    }
     
    string  line;
    vector<string> splitted_line;
    vector<string> lines;
    while (getline(input_file, line))
    {
        if (strtrim(line).size() == 0)
            continue;
        if (strtrim(line)[0] == '#')
            continue;
        splitted_line = split(line, '#');
        if (splitted_line.size() > 0 && strtrim(splitted_line[0]).size() > 0)
        {
            lines.push_back(splitted_line[0]);
            if (splitted_line[0][0] == '?' || splitted_line[0][0] == '=')
                cout << remove_doubles(strtrim(splitted_line[0])) << endl;
            else
                cout << splitted_line[0] << endl;
        }
    }
    if (lines.size() == 0) 
    {
        cout << "The file does not contain any rule" << endl;
        return -1;
    }
    ExpertProgram* prog = new ExpertProgram();
    string fact_line;
    string query_line;
    if (lines[lines.size() - 2][0] == '=')
    {
        fact_line = lines[lines.size() - 2];
        query_line = lines[lines.size() - 1];
    }
    else
    {
        fact_line = lines[lines.size() - 1];
        query_line = lines[lines.size() - 2];
    }
    
    if (prog->set_facts(fact_line) != 1)
    {
        return -1;
    }
    if (prog->set_queries(query_line) != 1)
    {
        return -1;
    }
    if (prog->queries.size() == 0)
    {
        cout << "Query line is empty" << endl;
        return 0;
    }
    vector<string> rules_lines(lines.begin(), lines.end() - 2);
    if (prog->set_rules(rules_lines) != 1)
    {
        return -1;
    }
    ExpertEngine *engine = new ExpertEngine(prog);
    FactUpdater *updater = new FactUpdater(prog, engine);
    engine->updater = updater;

    try
    {
        engine->start_solving();
    }
    catch (ParadoxeException* e)
    {
        cout << e->what();
        return -1;
    }
    for (char c : prog->queries)
    {
        t_node* queried = prog->get_entity_by_data(c);
        if (queried->stat == True)
            cout << c << " is true\n"; 
        else if (queried->stat == Unsure)
            cout << c << " is undetermined\n";
        else
            cout << c << " is not true\n"; 
    }
    return 0;
}