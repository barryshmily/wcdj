package com.tencent.engine;


import java.util.HashMap;
import java.util.Map;

import com.tencent.exception.InnerException;
import com.tencent.rpc.App;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

class TmEngine {
	
	
	private static TmEngine instance = new TmEngine();


	private Map<String, State> states = new HashMap<String, State>();

	private TmEngine() {

	}

	public void init() throws Exception {
		//initTransMachines();
	}

	public static TmEngine instance() {
		return instance;
	}

	public void begin(TransMachine transMachine, String uuid, String transName, String req, String rsp) throws InnerException {
		
//		State state = states.get(uuid);
//		if (state != null) {
//			// throw ...
//			throw new InnerException("dulplicated uuid");
//		}
//		states.put(uuid, new State(transName));

		// TODO call c engine
		App.logInstance().info("call c");
		
		int ret = transMachine.cBegin(uuid, transName, req, rsp);
//		processResult(uuid, status);
		
		
		
		
	}

	
}
