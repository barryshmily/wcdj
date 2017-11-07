package com.tencent.engine;

import com.tencent.engine.Function.FunctionType;

public class TmeAction implements Action {
	
	String name;
	private ActionType type;
	Function func0;
	Function func1;
	Function func2;
	
	public StateEdge edge;
	//public StateEdge adjacent_edges;
	
	public final static TmeAction BEGIN = new TmeAction("begin", ActionType.TCC);
	public final static TmeAction COMMIT = new TmeAction("commit", ActionType.TRYBEST);
	public final static TmeAction ROLLBACK = new TmeAction("rollback", ActionType.TRYBEST);
	public final static TmeAction END = new TmeAction("end", ActionType.TRYBEST);
	
	TmeAction(String name_, ActionType type_)
	{
		this(name_, type_, null, null, null, null);
	}
	
	TmeAction(String name_, ActionType type_, Function func0_, Function func1_, Function func2_, StateEdge edge_)
	{
		if (name_ == null || type_ == null) {
			//..throw
		}
		
		name = name_;
		type = type_;
		func0 = func0_;
		func1 = func1_;
		func2 = func2_;
		edge = edge_;
	}
	
	private void checkFunction(Function func)
	{
		if (func.getType() == FunctionType.ASYNC && func.getAsyncDelayLevel() <= 0) {
			//..throw
		}
	}
	
	public String getName()
	{
		return name;
	}
	
	public ActionType getType()
	{
		return type;
	}
	
	public Function getFunc0()
	{
		return func0;
	}
	
	void setFunc0(Function func0_)
	{
		checkFunction(func0_);
		func0 = func0_;
	}
	
	public Function getFunc1()
	{
		return func1;
	}
	
	void setFunc1(Function func1_)
	{
		checkFunction(func1_);
		func1 = func1_;
	}
	
	public Function getFunc2()
	{
		return func2;
	}
	
	void setFunc2(Function func2_)
	{
		checkFunction(func2_);
		func2 = func2_;
	}
	
	public StateEdge getEdge() {
		return edge;
	}
	
	public void setEdge(StateEdge edge_) {
		edge = edge_;
	}
}
