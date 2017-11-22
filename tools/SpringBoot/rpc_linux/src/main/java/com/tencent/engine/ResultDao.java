package com.tencent.engine;


/**
 * @author gerryyang
 *
 */
public class ResultDao {
	
	public final static int FAILURE = 0;
	public final static int SUCCESS = 1;
	
	/** ok 状态0表示成功，状态1表示失败 . */
	private int ret = 0;
	private String info = "";

	public ResultDao() {
	}

	public ResultDao(int ok, String msg) {
		this.ret = ok;
		this.info = msg;
	}

	public int getOk() {
		return ret;
	}

	public boolean getSuccess() {
		if (ret == 0) {
			return true;
		} else {
			return false;
		}
	}

	public void setOk(int ok) {
		this.ret = ok;
	}

	public void setSuccess(int Success) {
		this.ret = Success;
	}
	
	public String getMsg() {
		return info;
	}
    
	public void setMsg(String msg) {
		this.info = msg;
	}

}
