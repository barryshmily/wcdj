package com.tencent.rpc;
import java.util.concurrent.Executors;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.ChannelHandlerContext;
import ch.qos.logback.classic.LoggerContext;
import ch.qos.logback.classic.joran.JoranConfigurator;

import com.tencent.midas.network.RemotingClient;
import com.tencent.midas.network.RemotingServer;
import com.tencent.midas.network.netty.NettyClientConfig;
import com.tencent.midas.network.netty.NettyRemotingClient;
import com.tencent.midas.network.netty.NettyRemotingServer;
import com.tencent.midas.network.netty.NettyRequestProcessor;
import com.tencent.midas.network.netty.NettyServerConfig;
import com.tencent.midas.network.protocol.RemotingCommand;

public class App {
	private static final Logger log = LoggerFactory.getLogger("num_pack");
	public static RemotingServer createRemotingServer() throws InterruptedException {
		NettyServerConfig config = new NettyServerConfig();
		config.setListenPort(8080);
		RemotingServer remotingServer = new NettyRemotingServer(config);
		remotingServer.registerProcessor("Test", new NettyRequestProcessor() {
			private int i = 0;

			@Override
			public RemotingCommand processRequest(ChannelHandlerContext ctx, RemotingCommand request) {
				request.setRemark("hello, I am respponse " + ctx.channel().remoteAddress());
				return request;
			}

			@Override
			public boolean rejectRequest() {
				return false;
			}

		}, Executors.newCachedThreadPool());
		remotingServer.start();
		return remotingServer;
	}

	public static RemotingClient createRemotingClient() {
		NettyClientConfig config = new NettyClientConfig();
		RemotingClient client = new NettyRemotingClient(config);
		client.start();
		return client;
	}

	public static void main(String[] args) throws Exception {
		String logbackFile = "conf/logback_rpc.xml";
		LoggerContext lc = (LoggerContext) LoggerFactory.getILoggerFactory();
		JoranConfigurator configurator = new JoranConfigurator();
		configurator.setContext(lc);
		lc.reset();
		configurator.doConfigure(logbackFile);
		System.out.println("load logback config file OK, " + logbackFile);
		RemotingServer server = createRemotingServer();
		RemotingClient client = createRemotingClient();
		for (int i = 0; i < 100; i++) {
			TestRequestHeader requestHeader = new TestRequestHeader();
			requestHeader.setCount(i);
			requestHeader.setMessageTitle("HelloMessageTitle");
			RemotingCommand request = RemotingCommand.createRequestCommand("Test", requestHeader);
			RemotingCommand response = client.invokeSync("localhost:8080", request, 1000 * 3000);
			System.out.println("invoke result = " + response.getRemark());
			log.info("invoke result = " + response.getRemark());
		}
		client.shutdown();
		server.shutdown();
	}
}
