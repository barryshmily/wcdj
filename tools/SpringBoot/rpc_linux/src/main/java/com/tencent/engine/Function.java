package com.tencent.engine;

import java.util.List;

public class Function {
	public enum FunctionType
	{
		SYNC,
		ASYNC
	}
	
	public static final PreProcHandler DefaultPreProcHandler = new PreProcHandler() {
        public int preProcIn(Object str) {
        	return 0;
        }
        public int preProcOut(Object str) {
        	return 0;
        }
    };
		
	Service owner;
	private String name;
	private PreProcHandler preProcHandler;
	private FunctionType type;
	private int asyncDelayLevel;
	private int maxCallTime;
	private String returnField;
	private List<String> mergeKeys;
	
	public Function(Service service, String name_, PreProcHandler handler)
	{
		owner = service;
		name = name_;
		setPreProcHandler(handler);
		type = FunctionType.SYNC;
	}
	
	public Function(Service service, String name_)
	{
		this(service, name_, DefaultPreProcHandler);
	}
	
	
	Service getOwner()
	{
		return owner;
	}
	
	public String getName()
	{
		return name;
	}
	
	public FunctionType getType()
	{
		return type;
	}
	public Function setType(FunctionType type_)
	{
		if (type_ == null) {
			//..throw
		}
		type = type_;
		return this;
	}

	public int getAsyncDelayLevel() {
		return asyncDelayLevel;
	}

	public Function setAsyncDelayLevel(int asyncDelayLevel) {
		this.asyncDelayLevel = asyncDelayLevel;
		return this;
	}

	public Function setPreProcHandler(PreProcHandler preProcHandler) {
		this.preProcHandler = preProcHandler;
		return this;
	}
	
	public PreProcHandler getPreProcHandler()
	{
		return preProcHandler;
	}

	public int getMaxCallTime() {
		return maxCallTime;
	}

	public Function setMaxCallTime(int maxCallTime) {
		this.maxCallTime = maxCallTime;
		return this;
	}

	public String getReturnField() {
		return returnField;
	}

	public Function setReturnField(String returnField) {
		this.returnField = returnField;
		return this;
	}

	public List<String> getMergeKeys() {
		return mergeKeys;
	}

	public Function setMergeKeys(List<String> mergeKeys) {
		this.mergeKeys = mergeKeys;
		return this;
	}
}
