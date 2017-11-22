package com.tencent.engine;

import java.util.HashMap;
import java.util.Map;

import com.tencent.exception.*;

public class TransMachineManager {

	private static TransMachineManager instance = new TransMachineManager();

	private Map<String, TransMachine> transMachines = new HashMap<String, TransMachine>();

	public static TransMachineManager instance() {
		return instance;
	}

	public void addTransMachine(TransMachine transMachine) throws InnerException, LogicException, Exception {
				
		if (transMachine == null) {
			throw new InnerException("addTransMachine");
		}
		
		transMachine.buildAndCheck();// TODO
		
		if (transMachines.put(transMachine.getName(), transMachine) != null) {
			throw new LogicException("addTransMachine");
		}
	}

	public TransMachine getTransMachine(String name) {
		return transMachines.get(name);
	}
	
}
