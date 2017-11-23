package com.tencent.engine;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.rocketmq.client.consumer.listener.ConsumeConcurrentlyContext;
import com.alibaba.rocketmq.client.consumer.listener.ConsumeConcurrentlyStatus;
import com.alibaba.rocketmq.client.consumer.listener.MessageListenerConcurrently;
import com.alibaba.rocketmq.common.message.MessageExt;
import com.tencent.engine.CommUtil;
import com.tencent.engine.ResultDao;
import com.tencent.engine.TransMachine;

public class MessageListenerConcurrentlyImpl implements
		MessageListenerConcurrently {

	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	/**
	 * 默认msgs里只有一条消息，可以通过设置consumeMessageBatchMaxSize参数来批量接收消息
	 */
	public ConsumeConcurrentlyStatus consumeMessage(List<MessageExt> msgs,
			ConsumeConcurrentlyContext context) {

		log.info(Thread.currentThread().getName()
				+ " receive new messages size[" + msgs.size() + "]");

		int ret = 1;
		int reconsumeTimes;
		for (int i = 0; i < msgs.size(); i++) {

			MessageExt msg = msgs.get(i);
			String body = new String(msg.getBody());
			reconsumeTimes = msg.getReconsumeTimes();
			log.info("msg " + i + " [" + body + "] ReconsumeTimes["
					+ reconsumeTimes + "]");

			ret = processMQRequest(body, reconsumeTimes);
			log.info("processMQRequest ret[" + ret + "]");
		}

		// TODO return code check on E_INTERNAL
		ConsumeConcurrentlyStatus ccs = ConsumeConcurrentlyStatus.CONSUME_SUCCESS;
		//ConsumeConcurrentlyStatus ccs = (ret == 0 ? ConsumeConcurrentlyStatus.CONSUME_SUCCESS : ConsumeConcurrentlyStatus.RECONSUME_LATER);

		log.info("return MQ ConsumeConcurrentlyStatus[" + ccs + "]");
		return ccs;
	}

	// TODO
	private int processMQRequest(String request, int reconsumeTimes) {

		Map<String, String> map1 = CommUtil.str2Map(request);
		String app_header = map1.get("app_header");
		String mq_header = map1.get("mq_header");
		String content = map1.get("content");

		log.info("app_header[" + app_header + "] mq_header[" + mq_header
				+ "] content[" + content + "]");

		String app_header_decode = "";
		try {
			app_header_decode = URLDecoder.decode(app_header, "ASCII");
		} catch (UnsupportedEncodingException e) {
			log.info("exception[" + CommUtil.getStackTrace(e) + "]");
		}

		Map<String, String> map2 = CommUtil.str2Map(app_header_decode);
		String msg_type = map2.get("msg_type");
		String trans_name = map2.get("trans_name");
		String uuid = map2.get("uuid");

		log.info("msg_type[" + msg_type + "] trans_name[" + trans_name
				+ "] uuid[" + uuid + "]");

		if (uuid == null) {
			uuid = "";
		}
		if (trans_name == null) {
			trans_name = "";
		}
		if (content == null) {
			content = "";
		}

		ResultDao res = null;
		try {
			res = TransMachine.instance()
					.onMQRequest(reconsumeTimes, uuid, trans_name, content);

		} catch (Exception e) {
			log.info("onRequest exception[" + e.getMessage() + "] detail["
					+ CommUtil.getStackTrace(e) + "]");
		}

		if (res.getOk() == 0) {
			log.info("response uuid[" + uuid + "] transname[" + trans_name
					+ "] ok, rsp[" + res.getMsg() + "]");
		} else {
			log.info("response uuid[" + uuid + "] transname[" + trans_name
					+ "] err, rsp[" + res.getMsg() + "]");
		}

		return res.getOk();
	}
}
