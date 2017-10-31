package com.tencent.operators;

import java.util.List;

public class TmeOperator implements Operator
{
	protected OperatorType type;
	protected OperatorPath path;
	protected OperatorValue value;
	
	public TmeOperator(OperatorType type_, OperatorPath path_, OperatorValue value_)
	{
		if (type_ == null) {
			//throw ... 
		}
		if (path_ == null || (path_.getPath().size() > 3 || path_.getPath().size() < 2)) {
			//throw ...
		}
		if (value_ == null) {
			//throw ...
		}
		type = type_;
		path = path_;
		value = value_;
	}
	
	public TmeOperator(OperatorType type_, OperatorValue value_)
	{
		if (type_ == null) {
			//throw ... 
		}
		if (value_ == null) {
			// throw ...
		}
		type = type_;
		value = value_;
	}
	
	public TmeOperator(OperatorType type_)
	{
		if (type_ != OperatorType.ANY) {
			//throw ...
		}
		type = type_;
	}

	@Override
	public OperatorType getType() {
		return type;
		}

	@Override
	public List<String> getPath() {
		return path.getPath();
	}

	@Override
	public List<Object> getValue() {
		return value.getValue();
	}
}
