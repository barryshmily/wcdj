package com.tencent.engine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.rocketmq.client.exception.MQBrokerException;
import com.alibaba.rocketmq.client.exception.MQClientException;
import com.alibaba.rocketmq.client.producer.DefaultMQProducer;
import com.alibaba.rocketmq.client.producer.SendResult;
import com.alibaba.rocketmq.client.producer.SendStatus;
import com.alibaba.rocketmq.common.message.Message;
import com.alibaba.rocketmq.remoting.exception.RemotingException;

public class MQProducer {

	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	private static final DefaultMQProducer producer = new DefaultMQProducer(
			"tme_java_producer");// product group name

	private static void startProducer() {
		try {
			producer.setNamesrvAddr("10.235.25.15:9876"); // test environment
			producer.start();
		} catch (MQClientException e) {
			log.info(CommUtil.getStackTrace(e));
		}
	}

	static {
		startProducer();
	}

	public int rpcProcess(String uuid, String transName, String service,
			String function, String req, String rsp) {

		log.info("tid[" + Thread.currentThread().getId()
				+ "] java_rpc hashCode[" + hashCode() + "] uuid[" + uuid
				+ "] transName[" + transName + "] service:[" + service
				+ "] function[" + function + "] req[" + req + "]");

		// Do RPC according to different service and function
		int ret = 1;

		// TODO enclose to inner
		if (service.equals("mq_local_service_")
				&& function.equals("mq_local_func_")) {

			Message msg = new Message("tme_java",// topic
					transName,// tag
					uuid,// key
					req.getBytes());// body

			// MQ的消费间隔由用户传递的MQ_PARAM_APP_DELAY_LEVEL参数控制，其取值为[1,18]
			// 分别对应的时间间隔为1s 5s 10s 30s 1m 2m 3m 4m 5m 6m 7m 8m 9m 10m 20m 30m 1h
			// 2h
			msg.setDelayTimeLevel(3);

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
				log.info(CommUtil.getStackTrace(e));

			} finally {
				if (ret != 0) {
					// TODO error
					log.info("finally, send to mq error");
				}
			}

		} else {
			rsp = "service[" + service + "] function[" + function + "] invalid";
			ret = 1;
		}

		log.info("ret[" + ret + "] rsp[" + rsp + "]");
		return ret;
	}
}
