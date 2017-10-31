package com.tencent.operators;

import java.util.List;

public interface Operator 
{
	OperatorType getType();
	List<String> getPath();
	List<Object> getValue();
}
