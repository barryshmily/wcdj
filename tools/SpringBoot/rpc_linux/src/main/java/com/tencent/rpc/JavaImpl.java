package com.tencent.rpc;

public class JavaImpl {

	public static void main(String[] args){
		
		System.out.println("main JavaImpl");
		
		
	}
	
	public int square(int input){
		System.out.println("hashCode: " + hashCode());
		int output = input * input;
		return output;
	}

	public static int power(int input, int exponent){
		int output, i;
		output = 1;
		for (i = 0; i < exponent; i++) {
			output *= input;
		}
		return output;
	}
}

