package com.tencent.rpc;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;

import com.tencent.midas.network.netty.NettyRequestProcessor;
import com.tencent.midas.network.protocol.RemotingCommand;

public class ServiceTestImpl implements NettyRequestProcessor {

	// static logger object
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	// auto generate codes
	// @Override
	// public RemotingCommand processRequest(ChannelHandlerContext arg0,
	// RemotingCommand arg1) throws Exception {
	// // TODO Auto-generated method stub
	// return null;
	// }
	//
	// @Override
	// public boolean rejectRequest() {
	// // TODO Auto-generated method stub
	// return false;
	// }

	@Override
	public boolean rejectRequest() {
		// this check before handle request
		// System.out.println("reject check");
		return false;
	}

	// request process
	@Override
	public RemotingCommand processRequest(ChannelHandlerContext ctx,
			RemotingCommand request) {
		log.info("server handle request:" + request);

		request.setRemark("This is answer." + ctx.channel().remoteAddress());
		return request;
	}

}
