package com.tencent.engine;

import java.util.ArrayList;
import java.util.List;

import com.tencent.operators.Operator;

public class Transition {
	private Action from;
	private Action to;
	private List<Operator> operators = new ArrayList<Operator>();
	
	public Transition(Action from_, Action to_, Operator ...operators_) 
	{
		if (getFrom() == null || to == null || operators_ == null) {
			//throw ...
		}
		from = from_;
		to = to_;
		for(Operator op : operators_) {
			operators.add(op);
		}
	}

	public Action getFrom() {
		return from;
	}
	
	public Action getTo() {
		return to;
	}
	
	public List<Operator> getOperators() {
		return operators;
	}
}
