package com.tencent.rpc;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class JavaImpl {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	public static void main(String[] args){
		
		log.info("main JavaImpl");
		
		
	}
	
	public int java_rpc(int input /* TODO params string jason */) {
		log.info("java_rpc hashCode: " + hashCode());
		
		// ...
		
		return 0;// ok
	}

	public static int power(int input, int exponent) {
		int output, i;
		output = 1;
		for (i = 0; i < exponent; i++) {
			output *= input;
		}
		return output;
	}
}

