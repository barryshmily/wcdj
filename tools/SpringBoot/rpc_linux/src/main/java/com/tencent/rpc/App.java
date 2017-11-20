package com.tencent.rpc;

import java.util.List;
import java.util.concurrent.Executors;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import ch.qos.logback.classic.LoggerContext;
import ch.qos.logback.classic.joran.JoranConfigurator;
import ch.qos.logback.core.joran.spi.JoranException;

import com.alibaba.rocketmq.client.consumer.DefaultMQPushConsumer;
import com.alibaba.rocketmq.client.consumer.listener.ConsumeConcurrentlyContext;
import com.alibaba.rocketmq.client.consumer.listener.ConsumeConcurrentlyStatus;
import com.alibaba.rocketmq.client.consumer.listener.MessageListenerConcurrently;
import com.alibaba.rocketmq.client.exception.MQClientException;
import com.alibaba.rocketmq.common.message.MessageExt;
// private packages
import com.tencent.midas.network.RemotingClient;
import com.tencent.midas.network.RemotingServer;
import com.tencent.midas.network.netty.NettyClientConfig;
import com.tencent.midas.network.netty.NettyRemotingClient;
import com.tencent.midas.network.netty.NettyRemotingServer;
import com.tencent.midas.network.netty.NettyServerConfig;

// --------------- import TME package beg ---------------
import com.tencent.demo.Demo;
import com.tencent.engine.TransMachine;
import com.tencent.engine.TransMachineManager;

//--------------- import TME package end ---------------

public class App {

	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	// static function, server-end
	public static RemotingServer createRemotingServer(int port)
			throws InterruptedException {

		NettyServerConfig config = new NettyServerConfig();
		config.setListenPort(port);
		RemotingServer remotingServer = new NettyRemotingServer(config);
		remotingServer.registerProcessor("ServiceTest", new ServiceTestImpl(),
				Executors.newCachedThreadPool());

		// anonymous class
		// remotingServer.registerProcessor("ServiceTest2", new
		// NettyRequestProcessor() {
		//
		// // request process
		// @Override
		// public RemotingCommand processRequest(ChannelHandlerContext ctx,
		// RemotingCommand request) {
		// request.setRemark("This is answer2." +
		// ctx.channel().remoteAddress());
		// return request;
		// }
		//
		// @Override
		// public boolean rejectRequest() {
		// return false;
		// }
		//
		// }, Executors.newCachedThreadPool());

		remotingServer.start();
		return remotingServer;
	}

	// static function, client-end
	public static RemotingClient createRemotingClient() {

		NettyClientConfig config = new NettyClientConfig();
		RemotingClient client = new NettyRemotingClient(config);
		client.start();
		return client;
	}

	/**
	 *
	 * @author gerryyang
	 * @version 0.0.1
	 */
	public static void main(String[] args) {

		String path = System.getProperty("java.class.path");
		System.out.println("classpath: " + path);

		// TODO configure
		String logbackFile = "../conf/logback.xml";
		LoggerContext lc = (LoggerContext) LoggerFactory.getILoggerFactory();
		JoranConfigurator configurator = new JoranConfigurator();
		configurator.setContext(lc);
		lc.reset();
		try {
			configurator.doConfigure(logbackFile);
		} catch (JoranException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("load logback config[" + logbackFile + "] ok");

		// --------------- INIT TME info beg ---------------
		try {
			// TODO register more routines
			TransMachine tm = Demo.RegistTestRoutine();

			TransMachineManager.instance().addTransMachine(tm);
			// ...

			// initialize TME info to CPP
			tm.initEngine();

		} catch (Exception e) {
			e.printStackTrace();
			log.info("exception: create TransMachine");
		}
		// --------------- INIT TME info end ---------------

		try {

			// RemotingClient client = null;
			//
			RemotingServer server = createRemotingServer(8081);
			RemotingServer server2 = createRemotingServer(8082);

			log.info("server start");

			// client = createRemotingClient();
			//
			// for (int i = 0; i < 1; ++i) {
			// TestRequestHeader requestHeader = new TestRequestHeader();
			// requestHeader.setCount(i);
			// requestHeader.setMessageTitle("HelloMessageTitle");
			//
			// // TODO
			// // parse header to dispatch
			//
			// // client
			// RemotingCommand request =
			// RemotingCommand.createRequestCommand("ServiceTest",
			// requestHeader);
			// RemotingCommand response = client.invokeSync("localhost:8080",
			// request, 1000 * 3000);
			// System.out.println(i + " response[" + response.getRemark() +
			// "]");
			//
			// // write file log
			// log.info(i + "invoke result = " + response.getRemark());
			// }

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();

		} finally {
			// if(client != null)
			// client.shutdown();
			// if(server != null)
			// server.shutdown();
		}

		// ---------------- MQ consumer begin ----------------
		DefaultMQPushConsumer consumer = new DefaultMQPushConsumer(
				"tme_java_consumer");
		consumer.setNamesrvAddr("10.235.25.15:9876");
		consumer.setConsumeMessageBatchMaxSize(30);
		/**
		 * 订阅指定topic下tags分别等于TagA或TagC或TagD
		 */
		try {
			consumer.subscribe("tme_java", "*");
		} catch (MQClientException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		/**
		 * 订阅指定topic下所有消息<br>
		 * 注意：一个consumer对象可以订阅多个topic
		 */
		consumer.registerMessageListener(new MessageListenerConcurrently() {

			/**
			 * 默认msgs里只有一条消息，可以通过设置consumeMessageBatchMaxSize参数来批量接收消息
			 */
			@Override
			public ConsumeConcurrentlyStatus consumeMessage(
					List<MessageExt> msgs, ConsumeConcurrentlyContext context) {
				// System.out.println(Thread.currentThread().getName()
				// + " Receive New Messages: " + msgs.size());

//				long offset = msgs.get(0).getQueueOffset();
//				String maxOffset = msgs.get(0).getProperty(
//						MessageConst.PROPERTY_MAX_OFFSET);
//				long diff = Long.parseLong(maxOffset) - offset;
				
				for (int i = 0; i < msgs.size(); i++) {
					MessageExt msg = msgs.get(i);
					String body = new String(msg.getBody());
					log.info("body[" + body + "]");;
				}

				return ConsumeConcurrentlyStatus.CONSUME_SUCCESS;
				// return ConsumeConcurrentlyStatus.RECONSUME_LATER;
			}


		});

		/**
		 * Consumer对象在使用之前必须要调用start初始化，初始化一次即可<br>
		 */
		try {
			consumer.start();
		} catch (MQClientException e) {
			e.printStackTrace();
		}

		// ---------------- MQ consumer end ----------------

		System.out.println("main over");
	}
}
