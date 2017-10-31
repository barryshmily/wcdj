package com.tencent.engine;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class TransMachine 
{
	private String name;
	private Map<String, Action> actions = new HashMap<String, Action>();
	private List<Transition> transitions = new ArrayList<Transition>();
	private Map<String, Service> services = new HashMap<String, Service>();
	
	public TransMachine(String name_)
	{
		if (name_ == null) {
			//..throw
		}
		name = name_;
	}
	
	public String getName()
	{
		return name;
	}
	
	private void addService(Function func) 
	{
		if (func != null) {
			Service service = func.getOwner();
			Service orginService = services.put(service.getName(), service);
			if (orginService != null && orginService != service) {
				//throw ...
			}
		}
	}
	
	public void addAction(Action act)
	{
		if (act == null) {
			//..throw
		}
		if (act.getFunc0() == null) {
			//..throw
		}
		if (actions.put(act.getName(), act) != null) {
			//.. throw
		}
	}
	
	private void checkExistAction(Action act)
	{
		Action ca = actions.get(act.getName());
		if (ca == null || ca != act) {
			//throw ...
		}
	}
	
	public void addTransition(Transition transition)
	{
		if (transition == null) {
			//throw ...
		}
		Action from = transition.getFrom();
		checkExistAction(from);
		Action to = transition.getFrom();
		checkExistAction(to);
		transitions.add(transition);
	}
	
	public Service getService(String name)
	{
		return services.get(name);
	}
	
	Collection<Action> getActions()
	{
		return actions.values();
	}
	
	Collection<Service> getServices()
	{
		return services.values();
	}
	
	List<Transition> getTransitions()
	{
		return transitions;
	}
}