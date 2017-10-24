package com.tencent.rpc;
import com.tencent.midas.network.CommandCustomHeader;
import com.tencent.midas.network.exception.RemotingCommandException;


public class TestResponseHeader implements CommandCustomHeader {
	
    private Integer count;
    private String messageTitle;

    public Integer getCount() {
        return count;
    }

    public void setCount(Integer count) {
        this.count = count;
    }


    public String getMessageTitle() {
        return messageTitle;
    }

    public void setMessageTitle(String messageTitle) {
        this.messageTitle = messageTitle;
    }

	@Override
	public void checkFields() throws RemotingCommandException {
		
	}


}