package com.tencent.rpc;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.rocketmq.client.exception.MQBrokerException;
import com.alibaba.rocketmq.client.exception.MQClientException;
import com.alibaba.rocketmq.client.producer.DefaultMQProducer;
import com.alibaba.rocketmq.client.producer.SendResult;
import com.alibaba.rocketmq.client.producer.SendStatus;
import com.alibaba.rocketmq.common.message.Message;
import com.alibaba.rocketmq.remoting.exception.RemotingException;

public class JavaImpl {

	private static final Logger log = LoggerFactory.getLogger("RunLogger");
	

	private static final DefaultMQProducer producer = new DefaultMQProducer("tme_java_producer");// product group name
	private static void startProducer() {
		try {
			producer.setNamesrvAddr("10.235.25.15:9876"); // test environment
			producer.start();
		} catch (MQClientException e1) {
			log.info(getStackTrace(e1));
		}
	}
	
	static {
		startProducer();
	}
	
	public static String getStackTrace(Exception e) {
		String eStr = e.toString();
		StackTraceElement[] err = e.getStackTrace();
		for(int i=0; i<err.length; i++) {
			eStr += "\n";
			eStr += err[i];
		}
		return eStr;
	}

	public static void main(String[] args) {

		log.info("main JavaImpl");

	}

	/**
	 * 
	 * @param service
	 * @param function
	 * @param req JSON
	 * @param rsp
	 * @return
	 */
	public int java_rpc(String service, String function, String req, String rsp) {
		log.info("java_rpc hashCode: " + hashCode() + ", service: " + service
				+ ", function: " + function + ", req: " + req);

		// Do RPC according to different service and function
		int ret = 1;
		
		// TODO enclose
		if (service.equals("mq_local_service_")
				&& function.equals("mq_local_func_")) {

			Message msg = new Message("tme_java",// topic
					"routine1",// tag
					"12345abc",// key
					req.getBytes());// body
			
			try {
				SendResult sendResult = producer.send(msg);
				if (sendResult.getSendStatus() != SendStatus.SEND_OK) {
					log.info("send to MQ err");
				} else {
					log.info("send to MQ ok");
					ret = 0;
				}
				
			} catch (MQClientException | RemotingException | MQBrokerException
					| InterruptedException e) {
				log.info(getStackTrace(e));
				
			} finally {
				log.info("finally");
				if (ret != 0) {
					// TODO error
				}
			}

		} else {
			log.info("service[" + service + "] do function[" + function + "]");
			rsp = "OK";
			ret = 0;
		}

		log.info("rsp: " + rsp);

		return ret;
	}

}
