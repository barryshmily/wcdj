package com.tencent.rpc;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;
import com.tencent.midas.network.netty.NettyRequestProcessor;
import com.tencent.midas.network.protocol.RemotingCommand;

// import TME jar
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
		/*App app = new App();
		log.info("hashCode: " + app.hashCode());*/
		log.info("tid: " + Thread.currentThread().getId());

		// --------------- carry out TME beg ---------------
		String uuid = "59919ac3-edbb-b021-11fd-0232c46d4491";
		String transName = "routine_order";
		String req = "gerry test";
		String rsp = "";
		
		try {
			TransMachineManager.instance().getTransMachine()
					.onRequest(uuid, transName, req, rsp);
		} catch (Exception e) {
			log.info("onRequest exception: " + e.getMessage() + ", detail: " + getStackTrace(e));
		}
		
		// TODO
		// other interface
		
		// --------------- carry out TME end ---------------

		request.setRemark("This is answer." + ctx.channel().remoteAddress());
		return request;
	}

}
