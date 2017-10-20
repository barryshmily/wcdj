package com.tencent.http;
import com.tencent.midas.HttpServer.HttpServer;
import com.tencent.midas.HttpServer.HttpServerConfig;
import com.tencent.midas.HttpServer.action.Action;
import com.tencent.midas.HttpServer.handler.Request;
import com.tencent.midas.HttpServer.handler.Response;

/**

 *
 */
public class ExampleAction implements Action{

	@Override
	public void doAction(Request request, Response response) {
		String a = request.getParam("a");
		response.setContent("response a: " + a);
	}
	public static void main(String[] args) {
		HttpServerConfig httpServerConfig = new HttpServerConfig();
		httpServerConfig.setPort(8090);
		HttpServer httpServer = new HttpServer(httpServerConfig);
		httpServer.setAction("/example1", ExampleAction.class);
		try {
			httpServer.start();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}