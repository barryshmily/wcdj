package com.tencent.engine;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.tencent.exception.InnerException;
import com.tencent.exception.LogicException;
import com.tencent.operators.Operator;
import com.tencent.operators.OperatorType;


class StateEdge {
	public StateEdge(TmeAction from_, TmeAction to_, List<Operator> operators_,
			StateEdge next_) {
		from = from_;
		to = to_;
		operators = operators_;
		next = next_;

	}

	//
	// public StateNode ownerStateNode;

	public TmeAction from;
	public TmeAction to;
	public List<Operator> operators;

	public StateEdge next;

	public StateEdge getNext() {
		return next;
	}

	public void setNext(StateEdge next_) {
		next = next_;
	}

	public TmeAction getFrom() {
		return from;
	}

	public void setFrom(TmeAction from_) {
		from = from_;
	}

	public TmeAction getTo() {
		return to;
	}

	public void setTo(TmeAction to_) {
		from = to_;
	}

	public List<Operator> getOperators() {
		return operators;
	}

	public void setOperators(List<Operator> operators_) {
		operators = operators_;
	}

}

class StateNode {
	public TransMachine ownerTransMachine;
	public String name;

	public Function fun0;
	public Function fun1;
	public Function fun2;

}

class TmeAction2Jason {
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public ActionType getType() {
		return type;
	}

	public void setType(ActionType type) {
		this.type = type;
	}

	public String getFunc0() {
		return func0;
	}

	public void setFunc0(String func0) {
		this.func0 = func0;
	}

	public String getFunc1() {
		return func1;
	}

	public void setFunc1(String func1) {
		this.func1 = func1;
	}

	public String getFunc2() {
		return func2;
	}

	public void setFunc2(String func2) {
		this.func2 = func2;
	}

	public String getNext_name() {
		return next_name;
	}

	public void setNext_name(String next_name) {
		this.next_name = next_name;
	}

	public String name;
	public ActionType type;

	// TODO
	public String func0;
	public String func1;
	public String func2;

	public String next_name;
}

class DestNode {
	public String name;
	//public Operator2Jason oper2j;

	public List<OperatorImpl2Jason> operators = new ArrayList<OperatorImpl2Jason>();

	public void convertOperators(TransMachine transMachine, String from,
			String to) {
		// from -> e1 -> e2 -> ...
		for (StateEdge edge : transMachine.stateEdges) {

			if (edge.getFrom().getName() != from
					|| edge.getTo().getName() != to) {
				continue;
			}

			OperatorImpl2Jason operImpl2j = new OperatorImpl2Jason();
			for (Operator oper : edge.getOperators()) {

				operImpl2j.path = oper.getPath();

				for (Object object : oper.getValue()) {
					operImpl2j.value.add(Objects.toString(object, null));
				}

				operImpl2j.type = oper.getType();
			}
			operators.add(operImpl2j);
		}
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

}

class StateEdge2Jason {
	public String getFrom() {
		return from;
	}

	public void setFrom(String from) {
		this.from = from;
	}

	public DestNode getDestNode() {
		return destNode;
	}

	public void setDestNode(DestNode destNode) {
		this.destNode = destNode;
	}

	public String from;
	DestNode destNode;
}

class OperatorImpl2Jason {
	public OperatorType getType() {
		return type;
	}

	public void setType(OperatorType type) {
		this.type = type;
	}

	public List<String> getPath() {
		return path;
	}

	public void setPath(List<String> path) {
		this.path = path;
	}

	public List<String> getValue() {
		return value;
	}

	public void setValue(List<String> value) {
		this.value = value;
	}

	public List<String> path = new ArrayList<String>();
	public List<String> value = new ArrayList<String>();
	public OperatorType type;
}

class TransMachine2Jason {
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	public List<StateEdge2Jason> getStateEdges() {
		return stateEdges;
	}

	public void setStateEdges(List<StateEdge2Jason> stateEdges) {
		this.stateEdges = stateEdges;
	}

	public Map<String, TmeAction2Jason> actions = new HashMap<String, TmeAction2Jason>();
	public List<StateEdge2Jason> stateEdges = new ArrayList<StateEdge2Jason>();

	public void convertStateEdges(TransMachine transMachine) {
		// from -> e1 -> e2 -> ...
		for (StateEdge edge : transMachine.stateEdges) {

			String from = edge.getFrom().getName();
			String to = edge.getTo().getName();

			DestNode dn = new DestNode();
			dn.name = edge.getTo().getName();
			dn.convertOperators(transMachine, from, to);

			StateEdge2Jason e = new StateEdge2Jason();
			e.from = from;
			e.destNode = dn;
			stateEdges.add(e);
		}
	}

	public void convert2Jason(TransMachine transMachine) {

		// actions
		// for (Map.Entry<String, TmeAction> entry:
		// transMachine.actions.entrySet()) {
		// String key = entry.getKey();
		// TmeAction tac = entry.getValue();
		//
		// App.logInstance().info("key: " + key + " value: " + tac);
		//
		// TmeAction2Jason tac2j = new TmeAction2Jason();
		// tac2j.name = tac.getName();
		// tac2j.type = tac.getType();
		// tac2j.func0 = tac.getFunc0().getName();
		// tac2j.func1 = tac.getFunc1().getName();
		// tac2j.func2 = tac.getFunc2().getName();
		// tac2j.next_name = tac.getEdge().getFrom().getName();
		//
		// actions.put(key, tac2j);
		//
		// }

		convertStateEdges(transMachine);

		Iterator<Map.Entry<String, TmeAction>> entries = transMachine.actions
				.entrySet().iterator();

		while (entries.hasNext()) {

			Map.Entry<String, TmeAction> entry = entries.next();
			String key = entry.getKey();
			TmeAction tac = entry.getValue();

			// App.logInstance().info("key: " + key + " value: " + tac);

			TmeAction2Jason tac2j = new TmeAction2Jason();
			tac2j.name = tac.getName();
			tac2j.type = tac.getType();
			tac2j.func0 = tac.getFunc0().getName();
			tac2j.func1 = tac.getFunc1().getName();
			tac2j.func2 = tac.getFunc2().getName();

			if (actions.put(key, tac2j) != null) {
				log.info("dulpicated key: " + key);
			}

		}
	}

	public Map<String, TmeAction2Jason> getActions() {
		return actions;
	}

	public void setActions(Map<String, TmeAction2Jason> actions_) {
		actions = actions_;
	}

}

public class TransMachine {
	
	private static final Logger log = LoggerFactory.getLogger("RunLogger");

	static {
		System.loadLibrary("engine"); // api.dll (Windows) or libapi.so (Unixes)
	}

	// Native method declaration
	public native int cBegin(String uuid, String transName, String req, String rsp);
	public native int cInit(String transName, String transMachine2jason);

	// ---------------------------

	public boolean isInit = false;
	
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

	public TransMachine(String name_) throws InnerException {
		if (name_ == null) {
			// ..throw
			log.info("TransMachine err");
			throw new InnerException("TransMachine");
		}
		name = name_;
	}

	public String getName() {
		return name;
	}

	private void addService(Function func) {
		if (func != null) {
			Service service = func.getOwner();
			Service orginService = services.put(service.getName(), service);
			if (orginService != null && orginService != service) {
				// throw ...
			}
		}
	}

	public void addAction(TmeAction act) {
		if (act == null) {
			// ..throw
		}
		if (act.getFunc0() == null) {
			// ..throw
		}
		if (actions.put(act.getName(), act) != null) {
			// .. throw
		}
	}

	private void checkExistAction(TmeAction act) {
		TmeAction ca = actions.get(act.getName());
		if (ca == null || ca != act) {
			// throw ...
		}
	}

	public void addTransition(Transition transition) {
		if (transition == null) {
			// throw ...
		}
		TmeAction from = transition.getFrom();
		checkExistAction(from);
		TmeAction to = transition.getTo();
		checkExistAction(to);
		transitions.add(transition);
	}

	public Service getService(String name) {
		return services.get(name);
	}

	public void buildAndCheck() throws Exception {
		// checkActions();

		// add inner nodes
		checkAndAddCommitRollback();

		// build DAG
		connectEdge();

		// check DAG invalid
		calcReachability();

		// other

	}

	public void calcReachability() throws LogicException {

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

		// TmeAction act_begin = getAction("begin");
		// if (in_degree.get(act_begin) != 0) {
		// throw new LogicException("begin in_degree");
		// }

		// TODO topo sort
	}

	public void connectEdge() {
		// save all edges
		for (Transition trans : transitions) {
			stateEdges.add(new StateEdge(trans.getFrom(), trans.getTo(), trans
					.getOperators(), null));
		}

		// connect them
		// from -> e1 -> e2 -> ...
		for (StateEdge edge : stateEdges) {
			edge.next = edge.from.edge;
			edge.from.edge = edge;
		}
	}

	public void checkAndAddCommitRollback() throws Exception {
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
				begin.addFunction("nouse"), begin.addFunction("nouse"),
				begin.addFunction("nouse"));
		addAction(act_begin);

		// node end
		Service end = new Service("end");
		TccAction act_end = new TccAction("end", end.addFunction("nouse"),
				end.addFunction("nouse"), end.addFunction("nouse"));
		addAction(act_end);

		if (add_commit) {
			// node commit
			Service commit = new Service("commit");
			TccAction act_commit = new TccAction("commit",
					commit.addFunction("nouse"), commit.addFunction("nouse"),
					commit.addFunction("nouse"));
			addAction(act_commit);
		}

		if (add_rollback) {
			// node rollback
			Service rollback = new Service("rollback");
			TccAction act_rollback = new TccAction("rollback",
					rollback.addFunction("nouse"),
					rollback.addFunction("nouse"),
					rollback.addFunction("nouse"));
			addAction(act_rollback);
		}

	}

	public void initEngine()
			throws InnerException {
		TmEngine.instance().init(this);
	}
	
	public void onRequest(String uuid, String transName, String req, String rsp)
			throws InnerException {
		TmEngine.instance().begin(this, uuid, transName, req, rsp);
	}

	public TmeAction getAction(String name) {
		return actions.get(name);
	}

	Collection<TmeAction> getActions() {
		return actions.values();
	}

	Collection<Service> getServices() {
		return services.values();
	}

	List<Transition> getTransitions() {
		return transitions;
	}

}
