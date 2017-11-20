package com.tencent.engine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.serializer.SerializerFeature;
import com.tencent.exception.InnerException;


class TmEngine {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");
	
	private static TmEngine instance = new TmEngine();


	public static TmEngine instance() {
		return instance;
	}

	public void init(TransMachine transMachine) throws InnerException {
		log.info("call cpp to init");
		
		if (!transMachine.isInit) {
			transMachine.isInit = true;
			
			/*
			TransMachine2Jason tm2j = new TransMachine2Jason();
			tm2j.convert2Jason(transMachine);
			String stm2j = JSON.toJSONString(tm2j, SerializerFeature.WriteMapNullValue, SerializerFeature.PrettyFormat);
			log.info("TransMachine2Jason: " + stm2j);*/
			
			String stm2j = "";
			transMachine.cInit(transMachine.getName(), stm2j);
		} 
	}
	
	public void begin(TransMachine transMachine, String uuid, String transName, String req, String rsp) throws InnerException {

		log.info("begin to call cpp");		
		int ret = transMachine.cBegin(uuid, transName, req, rsp);
		
	    log.info("ret:" + ret);
	}
	
}
