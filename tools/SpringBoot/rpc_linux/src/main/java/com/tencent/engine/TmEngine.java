package com.tencent.engine;


import java.util.HashMap;
import java.util.Map;

import com.alibaba.fastjson.JSON;
import com.tencent.exception.InnerException;
import com.tencent.rpc.App;

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
		App.logInstance().info("begin to call cpp");
		TransMachine2Jason tm2j = new TransMachine2Jason();
		tm2j.convertActions(transMachine);
		String stm2j = JSON.toJSONString(tm2j);
		App.logInstance().info("TransMachine2Jason: " + stm2j);
		
		int ret = transMachine.cBegin(uuid, transName, req, rsp);
//		processResult(uuid, status);
		
		
		
		
	}

	
}
