package com.tencent.engine;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.ThreadLocalRandom;

public class CommUtil {

	public static String getStackTrace(Exception e) {
		String eStr = e.toString();
		StackTraceElement[] err = e.getStackTrace();
		for (int i = 0; i < err.length; i++) {
			eStr += "\n";
			eStr += err[i];
		}
		return eStr;
	}

	public static Map<String, String> str2Map(String str) {
		Map<String, String> map = new LinkedHashMap<String, String>();
		for (String keyValue : str.split("&")) {
			String[] pairs = keyValue.split("=", 2);
			map.put(pairs[0], pairs.length == 1 ? "" : pairs[1]);
		}
		return map;
	}

	public static String getCurrentDate() {		
		Date dNow = new Date();
		SimpleDateFormat ft = new SimpleDateFormat(
				"yyyyMMddhhmmss");

		return ft.format(dNow);
	}
	
	public static String getUUID() {
		long m = 1000;
		long n = 9999;
		long rand = ThreadLocalRandom.current().nextLong(m, n);
		Date dNow = new Date();
		SimpleDateFormat ft = new SimpleDateFormat(
				"yyyyMMddhhmmss");

		return ft.format(dNow) + "-" + rand;
	}
	
	

}
