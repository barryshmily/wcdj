package com.tencent.engine;

import java.util.HashMap;
import java.util.Map;

public class Service {
	private String name;
	private Map<String, Function> functions;

	public Service(String name_) {
		if (name_ == null) {
			// ..throw
		}
		name = name_;
		functions = new HashMap<String, Function>();
	}

	public String getName() {
		return name;
	}

	public Function addFunction(String name_, PreProcHandler handler)
			throws Exception {
		if (name_ == null) {
			// ..throw
		}
		if (functions.put(name_, new Function(this, name_, handler)) != null) {
			if (name_ != "nouse") {
				throw new java.lang.Exception(name_);
			}
		}
		return functions.get(name_);
	}

	public Function addFunction(String name_) throws Exception {
		return addFunction(name_, Function.DefaultPreProcHandler);
	}

	public Function getFunction(String name_) {
		return functions.get(name_);
	}
}
