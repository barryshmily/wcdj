package com.tencent.engine;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import com.tencent.exception.InnerException;
import com.tencent.exception.LogicException;
import com.tencent.operators.Operator;
import com.tencent.rpc.App;


class StateEdge
{
	public StateEdge(TmeAction from_, TmeAction to_, List<Operator> operators_, StateEdge next_) {
		from = from_;
		to = to_;
		operators = operators_;
		next = next_;
		
	}
//	
//	public StateNode ownerStateNode;
	
	public TmeAction from;
	public TmeAction to;
	public List<Operator> operators;
	
	public StateEdge next;
	
	
}

class StateNode
{
	public TransMachine ownerTransMachine;
	public String name;
	
	public Function fun0;
	public Function fun1;
	public Function fun2;
	
}

public class TransMachine 
{
	
	static {
		System.loadLibrary("engine"); // api.dll (Windows) or libapi.so (Unixes)
	}

	// Native method declaration
	public native int cBegin(String uuid, String transName, String req, String rsp);
	
	// ---------------------------
	public int iname = 1;
	public String name;
	public Map<String, TmeAction> actions = new HashMap<String, TmeAction>();
	
	public Map<String, Service> services = new HashMap<String, Service>();
	
	// static info
	// register edges info
	public List<Transition> transitions = new ArrayList<Transition>();
	// direct edges
	public List<StateEdge> stateEdges = new ArrayList<StateEdge>();
	// reach table
	public List<List<String>> reach_table = new ArrayList<List<String>>();
	
		
	public TransMachine(String name_) throws InnerException
	{
		if (name_ == null) {
			//..throw
			App.logInstance().info("TransMachine err");
			throw new InnerException("TransMachine");
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
	
	public void addAction(TmeAction act)
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
	
	private void checkExistAction(TmeAction act)
	{
		TmeAction ca = actions.get(act.getName());
		if (ca == null || ca != act) {
			//throw ...
		}
	}
	
	public void addTransition(Transition transition)
	{
		if (transition == null) {
			//throw ...
		}
		TmeAction from = transition.getFrom();
		checkExistAction(from);
		TmeAction to = transition.getTo();
		checkExistAction(to);
		transitions.add(transition);
	}
	
	public Service getService(String name)
	{
		return services.get(name);
	}
	
	public void buildAndCheck() throws Exception
	{
		// checkActions();
		
		// add inner nodes
		checkAndAddCommitRollback();
		
		// build DAG
		connectEdge();
		
		// check DAG invalid
		calcReachability();
		
		// other
		
	}
	
	public void calcReachability() throws LogicException
	{
		
		Map<TmeAction, Integer> in_degree = new TreeMap<TmeAction, Integer>();
		
		List<TmeAction> actions = new ArrayList<TmeAction>();
		for (TmeAction action : actions) {
			StateEdge edge = action.edge;
			
			int i = 0;
			while (edge.next != null) {
				if (edge.from.getName() != edge.to.getName()) {
					in_degree.put(edge.to, ++i);
				}
				edge = edge.next;
			}
		}
		
//		TmeAction act_begin = getAction("begin");
//		if (in_degree.get(act_begin) != 0) {
//			throw new LogicException("begin in_degree");
//		}
		
		// TODO topo sort
	}
	
	public void connectEdge()
	{
		// save all edges
		for (Transition trans : transitions) {
			stateEdges.add(new StateEdge(trans.getFrom(), trans.getTo(), trans.getOperators(), null));
		}
		
		// connect them
		// from -> e1 -> e2 -> ...
		for (StateEdge edge : stateEdges) {
			edge.next = edge.from.edge;
			edge.from.edge = edge;
		}
	}
	
	public void checkAndAddCommitRollback() throws Exception
	{
		boolean add_commit = false;
		boolean add_rollback = false;
		
		for (Transition trans : transitions) {
			// TODO check
			String from_name = trans.getFrom().getName();
			String to_name = trans.getTo().getName();
			
			if (from_name == "commit" || to_name == "commit") {
				add_commit = true;
				
			} else if (from_name == "rollback" || to_name == "rollback") {
				add_rollback = true;
			}
		}
		
		// node begin
		Service begin = new Service("begin");
		TccAction act_begin = new TccAction("begin", 
				begin.addFunction("nouse"), begin.addFunction("nouse"), begin.addFunction("nouse"));
		addAction(act_begin);
		
		// node end
		Service end = new Service("end");
		TccAction act_end = new TccAction("end", 
				end.addFunction("nouse"), end.addFunction("nouse"), end.addFunction("nouse"));
		addAction(act_end);
		
		if (add_commit) {
			// node commit
			Service commit = new Service("commit");
			TccAction act_commit = new TccAction("commit", 
					commit.addFunction("nouse"), commit.addFunction("nouse"), commit.addFunction("nouse"));
			addAction(act_commit);
		}
		
		if (add_rollback) {
			// node rollback
			Service rollback = new Service("rollback");
			TccAction act_rollback = new TccAction("rollback", 
					rollback.addFunction("nouse"), rollback.addFunction("nouse"), rollback.addFunction("nouse"));
			addAction(act_rollback);
		}
		
	}
	
	public void onRequest(String uuid, String transName, String req, String rsp) throws InnerException
	{
		TmEngine.instance().begin(this, uuid, transName, req, rsp);
	}
	
	
	public TmeAction getAction(String name)
	{
		return actions.get(name);
	}
		
	Collection<TmeAction> getActions()
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

