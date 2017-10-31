package com.tencent.engine;

public interface PreProcHandler {
	int preProcIn(Object record);
	int preProcOut(Object record);
}
