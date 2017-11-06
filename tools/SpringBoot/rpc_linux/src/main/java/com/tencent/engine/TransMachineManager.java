package com.tencent.engine;

import java.io.File;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

import com.tencent.exception.*;

public class TransMachineManager {
	
	private final String modulePath = "bin"; // TODO config // for test

	private static TransMachineManager instance = new TransMachineManager();

	private Map<String, TransMachine> transMachines = new HashMap<String, TransMachine>();

	private TransMachineManager() {
	}

	public static TransMachineManager instance() {
		return instance;
	}

	public void addTransMachine(TransMachine transMachine) throws InnerException, LogicException, Exception {
				
		if (transMachine == null) {
			// throw
			throw new InnerException("addTransMachine");
		}
		
		// TODO
		transMachine.buildAndCheck();
		
		if (transMachines.put(transMachine.getName(), transMachine) != null) {
			// throw
			throw new LogicException("addTransMachine");
		}
		
		
	}

	public TransMachine getTransMachine(String name) {
		return transMachines.get(name);
	}
	
//	private void dfsLoadRegistModule(File file) throws Exception {
//		if (file.isDirectory()) {
//			for (File childFile : file.listFiles()) {
//				dfsLoadRegistModule(childFile);
//			}
//		} else if (file.getPath().endsWith(".class")) {
//			String className = file.getPath().toString();
//			className = className.substring(modulePath.length() + 1);
//			className = className.substring(0, className.length() - 6);
//			className = className.replace(File.separatorChar, '.');
//			Class<?> registClass = Class.forName(className);
//			for (Method mod : registClass.getMethods()) {
//				if (java.lang.reflect.Modifier.isStatic(mod.getModifiers())) {
//					if (mod.getAnnotation((Class<T>) RegistTransMachine.class) != null) {
//						Object obj = mod.invoke(null);
//						if (obj instanceof TransMachine) {
//							TransMachineManager.instance().addTransMachine(
//									(TransMachine) obj);
//						}
//					}
//				}
//			}
//		}
//	}

//	private void initTransMachines() throws Exception {
//		
//		
//        
//		Method method = URLClassLoader.class.getDeclaredMethod("addURL",
//				URL.class);
//		boolean accessible = method.isAccessible();
//
//		if (accessible == false) {
//			method.setAccessible(true);
//		}
//		URLClassLoader classLoader = (URLClassLoader) ClassLoader
//				.getSystemClassLoader();
//		method.invoke(classLoader, Paths.get(modulePath).toUri().toURL());
//
//		method.setAccessible(accessible); // finally
//
//		File moduleRoot = new File(modulePath);
//		dfsLoadRegistModule(moduleRoot);
//		
//	}

	// native void Regist(TransMachine transMachine);
}
