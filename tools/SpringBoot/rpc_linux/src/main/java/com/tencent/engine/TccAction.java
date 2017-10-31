package com.tencent.engine;

public class TccAction extends TmeAction {
	
	public TccAction(String name)
	{
		super(name, ActionType.TCC);
	}
	
	public TccAction(String name, Function tryFunc, Function confirmFunc, Function cancelFunc)
	{
		super(name, ActionType.TCC, tryFunc, confirmFunc, cancelFunc);
	}
	
	public TccAction setTry(Function tryFunc)
	{
		setFunc0(tryFunc);
		return this;
	}
	public TccAction setConfirm(Function confirmFunc)
	{
		setFunc1(confirmFunc);
		return this;
	}
	public TccAction setCancel(Function cancelFunc)
	{
		setFunc2(cancelFunc);
		return this;
	}

}
