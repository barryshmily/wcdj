package com.tencent.operators;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Operator 
{
	private static final Logger log = LoggerFactory.getLogger("RunLogger");
	
//	OperatorType getType();
//	List<String> getPath();
//	List<Object> getValue();
	
	protected OperatorType type;
	protected OperatorPath path;
	protected OperatorValue value;
	
	public OperatorType getType() {
		return type;
	}
	
	public void setType(OperatorType type_) {
		type = type_;
	}
	
	public List<String> getPath() {
		return path.getPath();
	}
	
	public void setPath(OperatorPath path_) {
		path = path_;
	}
	
	public List<Object> getValue() {
		//log.info("Operator getValue");
		return value.getValue();
	}
	
	public void setValue(OperatorValue value_) {
		value = value_;
	}
}
