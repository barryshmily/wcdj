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

/**
 * @author gerryyang
 *
 */
public class JavaImpl {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	public static void main(String[] args) {
		log.info("main JavaImpl");
	}

	public int rpcProcess(int reconsumeTimes, String uuid, String transName, String service,
			String function, String req, String rsp) {

		log.info("reconsumeTimes[" + reconsumeTimes +"] tid[" + Thread.currentThread().getId()
				+ "] java_rpc hashCode[" + hashCode() + "] uuid[" + uuid
				+ "] transName[" + transName + "] service:[" + service
				+ "] function[" + function + "] req[" + req + "]");

		// Do RPC according to different service and function
		// ...
		int ret = 0;
		if (service.equals("service2")
				&& function.equals("addLilyMoney")) {
			ret = -1;
			log.info("ABNORMAL: addLilyMoney");
			
		} else {
			rsp = "rpcProcess OK";
		}
		
		log.info("ret[" + ret + "] rsp[" + rsp + "]");
		return ret;
	}

}
