package com.tencent.rpc;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;
import com.tencent.midas.network.netty.NettyRequestProcessor;
import com.tencent.midas.network.protocol.RemotingCommand;
import com.tencent.rpc.App;

import com.tencent.engine.TransMachineManager;


public class ServiceTestImpl implements NettyRequestProcessor {

	private static final Logger log = LoggerFactory.getLogger("RunLogger");
	
	public static String getStackTrace(Exception e) {
		String eStr = e.toString();
		StackTraceElement[] err = e.getStackTrace();
		for(int i=0; i<err.length; i++) {
			eStr += "\n";
			eStr += err[i];
		}
		return eStr;
	}

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

		// use singleton to prevent thread-safe issue
		App app = new App();
		log.info("hashCode: " + app.hashCode());
		log.info("tid: " + Thread.currentThread().getId());

		// call TME
		String uuid = "12345abc";
		String transName = "routine1";
		String req = "gerry test";
		String rsp = "";
		
		try {
			TransMachineManager.instance().getTransMachine(transName)
					.onRequest(uuid, transName, req, rsp);
		} catch (Exception e) {
			log.info("onRequest exception: " + e.getMessage());
			log.info("onRequest exception(detail): " + getStackTrace(e));
		}

		request.setRemark("This is answer." + ctx.channel().remoteAddress());
		return request;
	}

}
