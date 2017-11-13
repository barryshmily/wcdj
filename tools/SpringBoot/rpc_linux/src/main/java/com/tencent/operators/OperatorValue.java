package com.tencent.operators;

import java.util.ArrayList;
import java.util.List;

public class OperatorValue
{
//	public String toString() {
//		String s;
//		for (Object o : value) {
//			s += String.valueOf(o);
//		}
//		return s;
//	}
	
	private List<Object> value = new ArrayList<Object>();
	
	public List<Object> getValue() 
	{
		return value;
	}
	
	public OperatorValue(String... args)
	{
		for(String p: args) {
			value.add(p);
		}
	}
	
	public OperatorValue(int... args)
	{
		for(int p: args) {
			value.add((Object)p);
		}
	}
}
