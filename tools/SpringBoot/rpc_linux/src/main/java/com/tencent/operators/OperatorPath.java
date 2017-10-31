package com.tencent.operators;

import java.util.ArrayList;
import java.util.List;

public class OperatorPath 
{
	private List<String> path = new ArrayList<String>();
	
	public List<String> getPath() 
	{
		return path;
	}
	
	public OperatorPath(String... args)
	{
		for(String p: args) {
			path.add(p);
		}
	}
}
