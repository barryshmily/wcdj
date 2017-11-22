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
import com.tencent.engine.CommUtil;

public class JavaImpl {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	public static void main(String[] args) {
		log.info("main JavaImpl");
	}

	/**
	 * TODO
	 * 
	 * @param service
	 * @param function
	 * @param req
	 *            JSON
	 * @param rsp
	 * @return
	 */
	public int rpcProcess(String uuid, String transName, String service,
			String function, String req, String rsp) {

		log.info("tid[" + Thread.currentThread().getId()
				+ "] java_rpc hashCode[" + hashCode() + "] uuid[" + uuid
				+ "] transName[" + transName + "] service:[" + service
				+ "] function[" + function + "] req[" + req + "]");

		// Do RPC according to different service and function
		// ...
		
		log.info("service[" + service + "] do function[" + function + "]");
		rsp = "rpcProcess OK";
		int ret = 0;

		log.info("ret[" + ret + "] rsp[" + rsp + "]");

		return ret;
	}

}
