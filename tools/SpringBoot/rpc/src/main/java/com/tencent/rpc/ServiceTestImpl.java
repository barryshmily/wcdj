package com.tencent.rpc;

import io.netty.channel.ChannelHandlerContext;

import com.tencent.midas.network.netty.NettyRequestProcessor;
import com.tencent.midas.network.protocol.RemotingCommand;

public class ServiceTestImpl implements NettyRequestProcessor{

// auto generate codes
//	@Override
//	public RemotingCommand processRequest(ChannelHandlerContext arg0,
//			RemotingCommand arg1) throws Exception {
//		// TODO Auto-generated method stub
//		return null;
//	}
//
//	@Override
//	public boolean rejectRequest() {
//		// TODO Auto-generated method stub
//		return false;
//	}

	
	
	// request process
	@Override
	public RemotingCommand processRequest(ChannelHandlerContext ctx, RemotingCommand request) {
		request.setRemark("This is answer." + ctx.channel().remoteAddress());
		return request;
	}

	@Override
	public boolean rejectRequest() {
		return false;
	}

}
