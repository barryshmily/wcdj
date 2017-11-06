package com.tencent.exception;

public class InnerException extends Exception{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public InnerException(String message) {
		super(message);
	}
	
    public InnerException(String message, Throwable cause) {
        super(message, cause);
    }

}
