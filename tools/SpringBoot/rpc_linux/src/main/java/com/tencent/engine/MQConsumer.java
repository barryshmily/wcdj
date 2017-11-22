package com.tencent.engine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.rocketmq.client.consumer.DefaultMQPushConsumer;
import com.alibaba.rocketmq.client.exception.MQClientException;

public class MQConsumer {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");
	
	private static MQConsumer instance = new MQConsumer();
	public static MQConsumer instance() {
		return instance;
	} 

	public void InitMQConsumer() {
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
			log.info("exception[" + CommUtil.getStackTrace(e) + "]");
		}
		/**
		 * 订阅指定topic下所有消息<br>
		 * 注意：一个consumer对象可以订阅多个topic
		 */
		consumer.registerMessageListener(new MessageListenerConcurrentlyImpl());

		/**
		 * Consumer对象在使用之前必须要调用start初始化，初始化一次即可<br>
		 */
		try {
			consumer.start();
		} catch (MQClientException e) {
			log.info("exception[" + CommUtil.getStackTrace(e) + "]");
		}
	}
}
