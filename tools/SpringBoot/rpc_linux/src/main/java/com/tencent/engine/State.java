package com.tencent.engine;

public class State {
	enum PreProcType {
		IN, OUT
	}

	enum Status {
		RUNNING, RETRY, ASYNCWAIT, ENDED, ERROR
	}

	private String uuid;
	private TransMachine transMachine;

	public State(String transName) {
		transMachine = TransMachineManager.instance()
				.getTransMachine(transName);
		if (transMachine == null) {
			// throw ...
		}
	}

	public TransMachine getTransMachine() {
		return transMachine;
	}

//	public void begin(String uuid, String transName, Object record) {
//		State state = states.get(uuid);
//		if (state != null) {
//			// throw ...
//		}
//		states.put(uuid, new State(transName));
//
//		int status = nBegin(uuid, transName, recordToString(record));
//		processResult(uuid, status);
//	}

//	public void transition(String uuid, String serviceName,
//			String functionName, Object record) {
//		int status = nTransition(uuid, serviceName, functionName,
//				recordToString(record));
//		processResult(uuid, status);
//	}

//	private Object processResult(String uuid, int status) {
//		State state = states.get(uuid);
//		if (state == null) {
//			// throw ...
//		}
//
//		if (status == Status.RUNNING.ordinal()) {
//			// call services and functions
//
//		} else if (status == Status.RETRY.ordinal()) {
//			// To MQ
//		} else if (status == Status.ASYNCWAIT.ordinal()) {
//			// Db and set Mq Timer
//		} else if (status == Status.ENDED.ordinal()) {
//			return stringToRecord(nGetResultAndClean(uuid));
//		} else { // ERROR
//			states.remove(uuid);
//		}
//		return null;
//	}

	private String recordToString(Object obj) {
		return com.alibaba.fastjson.JSON.toJSONString(obj);
	}

	private Object stringToRecord(String str) {
		return com.alibaba.fastjson.JSON.parse(str);
	}

	private int runPreProc(PreProcType type, String uuid, String service_,
			String function_, Object record) {
		TransMachine transMachine = getTransMachine();
		Service service = transMachine.getService(service_);
		Function function = service.getFunction(function_);
		PreProcHandler preProcHandler = function.getPreProcHandler();
		if (type == PreProcType.IN) {
			return preProcHandler.preProcIn(record);
		} else {
			return preProcHandler.preProcOut(record);
		}
	}

	public int runPreProcIn(PreProcType type, String uuid, String service,
			String function, Object record) {
		return runPreProc(PreProcType.IN, uuid, service, function, record);
	}

	public int runPreProcOut(PreProcType type, String uuid, String service,
			String function, Object record) {
		return runPreProc(PreProcType.OUT, uuid, service, function, record);
	}

	private static native int nBegin(String uuid, String transName,
			String record);

	private static native int nTransition(String uuid, String serviceName,
			String functionName, String record);

	private static native String nGetResultAndClean(String uuid);
}
