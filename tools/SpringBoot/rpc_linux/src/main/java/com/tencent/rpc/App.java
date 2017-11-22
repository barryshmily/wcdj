package com.tencent.rpc;

import java.util.concurrent.Executors;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import ch.qos.logback.classic.LoggerContext;
import ch.qos.logback.classic.joran.JoranConfigurator;
import ch.qos.logback.core.joran.spi.JoranException;

import com.tencent.midas.HttpServer.HttpServer;
import com.tencent.midas.HttpServer.HttpServerConfig;
import com.tencent.midas.HttpServer.action.Action;
import com.tencent.midas.HttpServer.handler.Request;
import com.tencent.midas.HttpServer.handler.Response;
import com.tencent.midas.network.RemotingServer;
import com.tencent.midas.network.netty.NettyRemotingServer;
import com.tencent.midas.network.netty.NettyServerConfig;

// --------------- import TME package beg ---------------
import com.tencent.demo.Demo;
import com.tencent.engine.CommUtil;
import com.tencent.engine.MQConsumer;
import com.tencent.engine.ResultDao;
import com.tencent.engine.TransMachine;
import com.tencent.engine.TransMachineManager;
//--------------- import TME package end ---------------

/**
* TCP
* @author gerryyang
* @version 0.0.1
*/
class App2 {

	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	public static RemotingServer createRemotingServer(int port)
			throws InterruptedException {

		NettyServerConfig config = new NettyServerConfig();
		config.setListenPort(port);
		RemotingServer remotingServer = new NettyRemotingServer(config);
		remotingServer.registerProcessor("ServiceTest", new ServiceTestImpl(),
				Executors.newCachedThreadPool());

		remotingServer.start();
		return remotingServer;
	}

	public static void main(String[] args) {

		String path = System.getProperty("java.class.path");
		System.out.println("classpath: " + path);

		String logbackFile = "../conf/logback.xml";
		LoggerContext lc = (LoggerContext) LoggerFactory.getILoggerFactory();
		JoranConfigurator configurator = new JoranConfigurator();
		configurator.setContext(lc);
		lc.reset();
		try {
			configurator.doConfigure(logbackFile);
		} catch (JoranException e) {
			e.printStackTrace();
		}
		System.out.println("load logback config[" + logbackFile + "] ok");

		try {
			RemotingServer server = createRemotingServer(8081);
			//RemotingServer server2 = createRemotingServer(8082);

			log.info("server start");

		} catch (InterruptedException e) {
			e.printStackTrace();

		} catch (Exception e) {
			e.printStackTrace();

		} finally {
			// daemon should not shutdown
			// if(client != null)
			// client.shutdown();
			// if(server != null)
			// server.shutdown();
		}

		// --------------- INIT TME info beg ---------------
		try {
			// usage 1, write java codes to register
			TransMachine tm = Demo.RegistTestRoutine();
			TransMachineManager.instance().addTransMachine(tm);
			//tm.initEngine();

			// usage 2, use XML to register
			TransMachine.instance().initEngine();

		} catch (Exception e) {
			log.info("exception[" + CommUtil.getStackTrace(e) + "]");
		}
		// --------------- INIT TME info end ---------------

		System.out.println("main over");
	}
}

/**
* HTTP
* @author gerryyang
* @version 0.0.1
*/
public class App implements Action {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	/**
	 * curl -v "http://10.56.58.55:8081/example1?transname=routine_order&req=%7b%22k1%22%3a%22v1%22%2c%22k2%22%3a%22v2%22%7d"
	 */
	@Override
	public void doAction(Request request, Response response) {
		
		// --------------- carry out TME beg ---------------
		log.info("tid[" + Thread.currentThread().getId() + "]");
		
		String uuid = request.getParam("uuid");
		if (uuid == null || uuid.isEmpty()) {
			uuid = "59919ac3-edbb-b021-11fd-0232c46d4491";
		}
		String transName = request.getParam("transname");
		if (transName == null || transName.isEmpty()) {
			transName = "";
		}
		String req = request.getParam("req");
		if (req == null || req.isEmpty()) {
			req = "";
		}
		
		ResultDao res = null;
		try {
			// usage 1
			//TransMachineManager.instance().getTransMachine(transName).onRequest(uuid, transName, req);
			
			// usage 2
			res = TransMachine.instance().onRequest(uuid, transName, req);
			
		} catch (Exception e) {
			log.info("onRequest exception[" + e.getMessage() + "] detail[" + CommUtil.getStackTrace(e) + "]");
		}
		
		if (res.getOk() == 0) {
			response.setContent("response uuid[" + uuid + "] transname[" + transName + "] ok, rsp[" + res.getMsg() + "]");
		} else {
			response.setContent("response uuid[" + uuid + "] transname[" + transName + "] err, rsp[" + res.getMsg() + "]");
		}
		// --------------- carry out TME end ---------------
	}
	
	@SuppressWarnings("static-access")
	public static void main(String[] args) {
		
		String path = System.getProperty("java.class.path");
		System.out.println("classpath: " + path);

		String logbackFile = "../conf/logback.xml";
		LoggerContext lc = (LoggerContext) LoggerFactory.getILoggerFactory();
		JoranConfigurator configurator = new JoranConfigurator();
		configurator.setContext(lc);
		lc.reset();
		try {
			configurator.doConfigure(logbackFile);
		} catch (JoranException e) {
			e.printStackTrace();
		}
		System.out.println("load logback config[" + logbackFile + "] ok");
		
		HttpServerConfig httpServerConfig = new HttpServerConfig();
		httpServerConfig.setPort(8081);
		HttpServer httpServer = new HttpServer(httpServerConfig);
		httpServer.setAction("/example1", App.class);
		
		// --------------- INIT TME info beg ---------------
		try {
			// usage 1, write java codes to register
			//TransMachine tm = Demo.RegistTestRoutine();
			//TransMachineManager.instance().addTransMachine(tm);
			//tm.initEngine();

			// usage 2, use XML to register
			TransMachine.instance().initEngine();

		} catch (Exception e) {
			log.info("exception[" + CommUtil.getStackTrace(e) + "]");
		}
		// --------------- INIT TME info end ---------------

		System.out.println("TME init over");
		
		try {
			httpServer.start();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}