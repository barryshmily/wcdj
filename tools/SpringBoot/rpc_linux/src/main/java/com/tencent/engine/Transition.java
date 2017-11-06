package com.tencent.engine;

import java.util.ArrayList;
import java.util.List;

import com.tencent.operators.Operator;

public class Transition {
	private TmeAction from;
	private TmeAction to;
	private List<Operator> operators = new ArrayList<Operator>();
	
	public Transition(TmeAction from_, TmeAction to_, Operator ...operators_) 
	{
		if (getFrom() == null || to == null || operators_ == null) {
			//throw ...
		}
		from = from_;
		to = to_;
		for (Operator op : operators_) {
			operators.add(op);
		}
	}

	public TmeAction getFrom() {
		return from;
	}
	
	public TmeAction getTo() {
		return to;
	}
	
	public List<Operator> getOperators() {
		return operators;
	}
}
