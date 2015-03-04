//============================================================================
//                                  I B E X                                   
// File        : ibex_CellDoubleHeap.cpp
// Author      : Gilles Chabert, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Sep 12, 2014
//============================================================================

#include "ibex_CellDoubleHeap.h"

using namespace std;

namespace ibex {

CellDoubleHeap::CellDoubleHeap(int goal_var, CellCostFunc::criterion crit2,  int critpr) :
		 DoubleHeap<Cell>(*CellCostFunc::get_cost(CellCostFunc::LB, goal_var), *CellCostFunc::get_cost(crit2, goal_var), critpr), crit2(crit2), goal_var(goal_var) {

}

CellDoubleHeap::~CellDoubleHeap() {
	delete &heap1->costf;
	delete &heap2->costf;
}

void CellDoubleHeap::contract(double new_loup) {

	CellCostFunc& cost2=((CellCostFunc&) heap2->costf);

	DoubleHeap<Cell>::contract(new_loup, cost2.depends_on_loup);

	SharedHeap<Cell> *copy2 = new SharedHeap<Cell>(cost2, 1);

	// it is necessary to update the loup.
	cost2.set_loup(new_loup);

	copy2->root = heap2->root;
	copy2->nb_nodes =heap2->nb_nodes;
	heap2->root = NULL;
	heap2->nb_nodes =0;

	// why again??
	cost2.set_loup(new_loup);

	copy2->sort(cost2.depends_on_loup ); //update the order if necessary

	heap2->nb_nodes = copy2->nb_nodes;
	heap2->root = copy2->root;
	copy2->root = NULL; // avoid to delete heap2 with copy2
	delete copy2;

	assert(nb_nodes==heap2->size());
	assert(nb_nodes==heap1->size());

}

} // namespace ibex
